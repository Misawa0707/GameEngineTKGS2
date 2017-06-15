//--------------------------------------------------------------------------------------
// ファイル名: Player.cpp
// 作成者:
// 作成日:
// 説明:
//--------------------------------------------------------------------------------------

#include "Player.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Player::Player(DirectX::Keyboard* keyboard)
{
	m_pKeyboard = keyboard;

	Initialize();
}

Player::~Player()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
void Player::Initialize()
{
	// 自機パーツの読み込み
	m_Obj.resize(PARTS_NUM);
	m_Obj[PARTS_UFO].LoadModel(L"Resources/ufo.cmo");
	m_Obj[PARTS_HEAD].LoadModel(L"Resources/head.cmo");
	m_Obj[PARTS_LWING].LoadModel(L"Resources/lwing.cmo");
	m_Obj[PARTS_RWING].LoadModel(L"Resources/rwing.cmo");
	m_Obj[PARTS_LVERNIER].LoadModel(L"Resources/vernier.cmo");
	m_Obj[PARTS_RVERNIER].LoadModel(L"Resources/vernier.cmo");

	// 親子関係の構築（子パーツに親を設定）
	m_Obj[PARTS_HEAD].SetObjParent(&m_Obj[PARTS_UFO]);
	m_Obj[PARTS_LWING].SetObjParent(&m_Obj[PARTS_HEAD]);
	m_Obj[PARTS_RWING].SetObjParent(&m_Obj[PARTS_HEAD]);
	m_Obj[PARTS_LVERNIER].SetObjParent(&m_Obj[PARTS_UFO]);
	m_Obj[PARTS_RVERNIER].SetObjParent(&m_Obj[PARTS_UFO]);

	// 子パーツの親からのオフセット（座標のずれ）をセット
	m_Obj[PARTS_HEAD].SetTranslation(Vector3(0, 0.3f, 0));
	m_Obj[PARTS_LWING].SetTranslation(Vector3(-0.2f, 0.2f, 0));
	m_Obj[PARTS_RWING].SetTranslation(Vector3(+0.2f, 0.2f, 0));
	m_Obj[PARTS_LVERNIER].SetTranslation(Vector3(-0.2f, 0.1f, 0.6f));
	m_Obj[PARTS_RVERNIER].SetTranslation(Vector3(+0.2f, 0.1f, 0.6f));

	m_Obj[PARTS_HEAD].SetScale(Vector3(2, 2, 2));
	m_Obj[PARTS_LWING].SetScale(Vector3(0.5f, 0.5f, 0.5f));

	m_Obj[PARTS_LWING].SetRotation(Vector3(0, 0, XMConvertToRadians(-30)));
	m_Obj[PARTS_RWING].SetRotation(Vector3(0, 0, XMConvertToRadians(+30)));

	m_cycle = 0;

	// デフォルトでは発射していない
	FireFlag = false;
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void Player::Update()
{
	// キーボードの状態取得
	Keyboard::State keystate = m_pKeyboard->GetState();
	m_KeyboardTracker.Update(keystate);

	// パーツギミック
	{
		m_cycle += 0.1f;
		float pos_z = -1.0f + sinf(m_cycle);
		m_Obj[PARTS_LWING].SetRotation(
			Vector3(-0.2f, 0.2f, 0) + Vector3(0, 0, pos_z));
	}
	
	// 左旋回処理
	if (keystate.A)
	{
		// 自機の角度を回転
		//head_angle += 0.03f;
		float angle = m_Obj[0].GetRotation().y;
		m_Obj[0].SetRotation(Vector3(0, angle + 0.03f, 0));
	}

	// 右旋回処理
	if (keystate.D)
	{
		// 自機の角度を回転
		//head_angle += -0.03f;
		float angle = m_Obj[0].GetRotation().y;
		m_Obj[0].SetRotation(Vector3(0, angle - 0.03f, 0));
	}

	// 前進処理
	if (keystate.W)
	{
		// 移動量のベクトル
		Vector3 moveV(0, 0, -0.1f);
		// 移動ベクトルを自機の角度分回転させる
		//moveV = Vector3::TransformNormal(moveV, head_world);
		float angle = m_Obj[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// 自機の座標を移動
		Vector3 pos = m_Obj[0].GetTranslation();
		m_Obj[0].SetTranslation(pos + moveV);
	}

	// 後退処理
	if (keystate.S)
	{
		// 移動量のベクトル
		Vector3 moveV(0, 0, +0.1f);
		// 移動ベクトルを自機の角度分回転させる
		//moveV = Vector3::TransformNormal(moveV, head_world);
		float angle = m_Obj[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// 自機の座標を移動
		Vector3 pos = m_Obj[0].GetTranslation();
		m_Obj[0].SetTranslation(pos + moveV);
	}

	// スペースキーを押したら
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keys::Space))
	{
		// 発射中なら
		if (FireFlag)
		{
			// 戻す
			ResetBullet();
		}
		else
		{
			// 発射
			FireBullet();
		}
	}

	// 弾丸パーツの前進処理
	if (FireFlag)
	{
		// 弾丸パーツの座標を移動
		Vector3 pos = m_Obj[PARTS_RWING].GetTranslation();
		m_Obj[PARTS_RWING].SetTranslation(pos + m_BulletVel);
	}

	Calc();
}

//-----------------------------------------------------------------------------
// 行列更新
//-----------------------------------------------------------------------------
void Player::Calc()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Update();
	}
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void Player::Draw()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Draw();
	}
}

void Player::FireBullet()
{
	if (FireFlag) return;

	// 発射するパーツのワールド行列を取得
	Matrix worldm = m_Obj[PARTS_RWING].GetWorld();

	// 抽出した情報をしまっておく変数
	Vector3 scale;			// ワールドスケーリング
	Quaternion rotation;	// ワールド回転
	Vector3 translation;	// ワールド座標

	// ワールド行列から各要素を抽出
	worldm.Decompose(scale, rotation, translation);

	// 親パーツから分離、独立させる
	m_Obj[PARTS_RWING].SetObjParent(nullptr);
	m_Obj[PARTS_RWING].SetScale(scale);
	m_Obj[PARTS_RWING].SetRotationQ(rotation);
	m_Obj[PARTS_RWING].SetTranslation(translation);

	// 弾丸パーツの速度を設定
	m_BulletVel = Vector3(0, 0, -0.1f);
	// パーツの向きに合わせて速度ベクトルを回転
	m_BulletVel = Vector3::Transform(m_BulletVel, rotation);

	FireFlag = true;
}

void Player::ResetBullet()
{
	if (!FireFlag) return;

	m_Obj[PARTS_RWING].SetObjParent(&m_Obj[PARTS_HEAD]);
	m_Obj[PARTS_RWING].SetScale(Vector3(1, 1, 1));
	m_Obj[PARTS_RWING].SetRotation(Vector3(0, 0, XMConvertToRadians(+30)));
	m_Obj[PARTS_RWING].SetTranslation(Vector3(+0.2f, 0.2f, 0));

	FireFlag = false;
}

const DirectX::SimpleMath::Vector3& Player::GetTrans()
{
	// タンクパーツの座標を返す
	return m_Obj[0].GetTranslation();
}

const DirectX::SimpleMath::Vector3 & Player::GetRot()
{
	// タンクパーツの回転を返す
	return m_Obj[0].GetRotation();
}

void Player::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	// タンクパーツの座標を設定
	m_Obj[0].SetTranslation(trans);
}

void Player::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	// タンクパーツの座標を設定
	m_Obj[0].SetRotation(rot);
}

const DirectX::SimpleMath::Matrix& Player::GetLocalWorld()
{
	// タンクパーツのワールド行列を返す
	return m_Obj[0].GetWorld();
}
