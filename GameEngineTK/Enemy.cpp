//--------------------------------------------------------------------------------------
// ファイル名: Enemy.cpp
// 作成者:
// 作成日:
// 説明:
//--------------------------------------------------------------------------------------

#include "Enemy.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Enemy::Enemy(DirectX::Keyboard* keyboard)
{
	m_pKeyboard = keyboard;

	Initialize();
}

Enemy::~Enemy()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
void Enemy::Initialize()
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

	// 初期位置を決定
	Vector3 pos;

	pos.x = rand() % 10;
	pos.z = rand() % 10;

	SetTrans(pos);

	m_Timer = 0;

	m_DistAngle = 0;

	{// 全体用の当たり判定ノードの設定
		m_CollisionNodeBody.Initialize();
		// 親パーツを設定
		m_CollisionNodeBody.SetParent(&m_Obj[0]);
		m_CollisionNodeBody.SetTrans(Vector3(0,0.5f,0));
		m_CollisionNodeBody.SetLocalRadius(0.8f);
	}
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void Enemy::Update()
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

	m_Timer--;
	if (m_Timer < 0)
	{
		m_Timer = 60;

		// 目標角をランダムに抽選
		// -0.5～+0.5の乱数
		float rnd = (float)rand() / RAND_MAX - 0.5f;
		// -90～+90の乱数
		rnd *= 180.0f;
		rnd = XMConvertToRadians(rnd);

		m_DistAngle += rnd;
	}
	
	{
		// 自機の角度を回転
		Vector3 rot = GetRot();

		// 今の角度と目標角度の差
		float angle = m_DistAngle - rot.y;
		if (angle > XM_PI)
		{
			angle -= XM_2PI;
		}
		if (angle < -XM_PI)
		{
			angle += XM_2PI;
		}

		// 補間
		rot.y += angle * 0.01f;

		SetRot(rot);
	}

	// 機体が向いている方向に進む処理
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

	Calc();
}

//-----------------------------------------------------------------------------
// 行列更新
//-----------------------------------------------------------------------------
void Enemy::Calc()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Update();
	}

	m_CollisionNodeBody.Update();
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void Enemy::Draw()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Draw();
	}

	m_CollisionNodeBody.Draw();
}

const DirectX::SimpleMath::Vector3& Enemy::GetTrans()
{
	// タンクパーツの座標を返す
	return m_Obj[0].GetTranslation();
}

const DirectX::SimpleMath::Vector3 & Enemy::GetRot()
{
	// タンクパーツの回転を返す
	return m_Obj[0].GetRotation();
}

void Enemy::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	// タンクパーツの座標を設定
	m_Obj[0].SetTranslation(trans);
}

void Enemy::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	// タンクパーツの座標を設定
	m_Obj[0].SetRotation(rot);
}

const DirectX::SimpleMath::Matrix& Enemy::GetLocalWorld()
{
	// タンクパーツのワールド行列を返す
	return m_Obj[0].GetWorld();
}
