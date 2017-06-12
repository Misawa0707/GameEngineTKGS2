#include "Obj3d.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// 静的メンバ変数の定義
// デバイス
Microsoft::WRL::ComPtr<ID3D11Device>            Obj3d::m_d3dDevice;
// デバイスコンテキスト
Microsoft::WRL::ComPtr<ID3D11DeviceContext>     Obj3d::m_d3dContext;
// カメラ
Camera* Obj3d::m_Camera;
// 汎用ステート設定
std::unique_ptr<DirectX::CommonStates> Obj3d::m_states;
// エフェクトファクトリ
std::unique_ptr<DirectX::EffectFactory> Obj3d::m_factory;

void Obj3d::InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, Camera * camera)
{
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_Camera = camera;

	// 汎用ステート設定を生成
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());
	// エフェクトファクトリ生成
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	// テクスチャのパスを指定
	m_factory->SetDirectory(L"Resources");
}

Obj3d::Obj3d()
{
	// メンバ変数を初期化
	m_scale = Vector3(1, 1, 1);
	m_ObjParent = nullptr;

	// デフォルトではオイラー角を使用
	m_UseQuaternion = false;
}

void Obj3d::LoadModel(const wchar_t * fileName)
{
	// ＣＭＯからモデルを読み込む
	m_model = Model::CreateFromCMO(
		m_d3dDevice.Get(),
		fileName,
		*m_factory
	);
}

void Obj3d::Update()
{
	// ワールド行列の計算
	Matrix scalemat = Matrix::CreateScale(m_scale);
	Matrix rotmat;
	if (m_UseQuaternion)
	{// クォータニオンで回転を計算
		rotmat = Matrix::CreateFromQuaternion(m_rotationQ);
	}
	else
	{// オイラー角で回転を計算
		Matrix rotmatZ = Matrix::CreateRotationZ(m_rotation.z);
		Matrix rotmatX = Matrix::CreateRotationX(m_rotation.x);
		Matrix rotmatY = Matrix::CreateRotationY(m_rotation.y);
		rotmat = rotmatZ * rotmatX * rotmatY;
	}
	Matrix transmat = Matrix::CreateTranslation(m_translation);
	// ワールド行列の合成
	m_world = scalemat * rotmat * transmat;
	// 親の行列を掛ける
	if (m_ObjParent)
	{
		m_world *= m_ObjParent->GetWorld();
	}
}

void Obj3d::Draw()
{
	// 描画
	if (m_model)
	{
		m_model->Draw(m_d3dContext.Get(),
			*m_states,
			m_world,
			m_Camera->GetView(),
			m_Camera->GetProj()
		);
	}
}
