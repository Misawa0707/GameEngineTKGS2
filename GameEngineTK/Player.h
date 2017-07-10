/**
*	@file	Player.h
*
*	@brief	自機
*
*	@date	2015/05/08
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
class Player
{
public:
	// 重力加速度
	const float GRAVITY_ACC = 0.03f;
	// ジャンプ初速
	const float JUMP_SPEED_FIRST = 0.5f;
	// ジャンプ速度制限
	const float JUMP_SPEED_MAX = 0.3f;

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
	Player(DirectX::Keyboard* keyboard);
	// デストラクタ
	~Player();
	// 初期化
	void Initialize();
	// 毎フレーム更新
	void Update();
	// 行列の計算
	void Calc();
	// 描画
	void Draw();
	// 弾丸を発射
	void FireBullet();
	// 弾丸を再装着（リセット）
	void ResetBullet();

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
	void StartJump();
	void StartFall();
	void StopJump();
	// 弾丸用の当たり判定を取得
	const SphereNode& GetCollisionNodeBullet() { return m_CollisionNodeBullet; }
	// 全身用の当たり判定を取得
	const SphereNode& GetCollisionNodeBody() { return m_CollisionNodeBody; }
	// 速度を取得
	const DirectX::SimpleMath::Vector3& GetVelocity() { return m_Velocity; }


protected:
	// メンバ変数
	// ロボットの構成パーツ
	std::vector<Obj3d>	m_Obj;
	// キーボード
	DirectX::Keyboard* m_pKeyboard;
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;

	// サイン用の角度
	float m_cycle;

	// 弾丸の速度ベクトル
	DirectX::SimpleMath::Vector3 m_BulletVel;

	// 発射中フラグ
	bool FireFlag;
	// 弾丸用の当たり判定
	SphereNode m_CollisionNodeBullet;
	// 全身用の当たり判定
	SphereNode m_CollisionNodeBody;
	// 速度
	DirectX::SimpleMath::Vector3 m_Velocity;
	// 落下中フラグ
	bool m_isJump;
};

