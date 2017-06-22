/**
*	@file	Enemy.h
*
*	@brief	自機
*
*
*	@author	藤澤　伸治
*/
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <vector>
#include "Obj3D.h"
#include "CollisionNode.h"


// 自機
class Enemy
{
public:
	// ロボットパーツ
	// 自機パーツ
	enum PARTS
	{
		PARTS_UFO,		// UFO
		PARTS_HEAD,		// 頭
		PARTS_LWING,		// 左翼
		PARTS_RWING,		// 右翼
		PARTS_LVERNIER,	// 推進機
		PARTS_RVERNIER,	// 推進機

		PARTS_NUM
	};

	// コンストラクタ
	Enemy(DirectX::Keyboard* keyboard);
	// デストラクタ
	~Enemy();
	// 初期化
	void Initialize();
	// 毎フレーム更新
	void Update();
	// 行列の計算
	void Calc();
	// 描画
	void Draw();
	// 座標を取得
	const DirectX::SimpleMath::Vector3& GetTrans();
	// 回転を取得
	const DirectX::SimpleMath::Vector3& GetRot();
	// 座標を設定
	void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	// 回転を設定
	void SetRot(const DirectX::SimpleMath::Vector3& rot);
	// ワールド行列を取得
	const DirectX::SimpleMath::Matrix& GetLocalWorld();
	// 全身用の当たり判定を取得
	const SphereNode& GetCollisionNodeBody() { return m_CollisionNodeBody; }

protected:
	// メンバ変数
	// ロボットの構成パーツ
	std::vector<Obj3d>	m_Obj;
	// キーボード
	DirectX::Keyboard* m_pKeyboard;
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;

	// サイン用の角度
	float m_cycle;

	// タイマー
	int m_Timer;
	// 目標角度
	float m_DistAngle;
	// 全身用の当たり判定
	SphereNode m_CollisionNodeBody;
};

