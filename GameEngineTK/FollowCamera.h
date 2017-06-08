/// <summary>
/// 自機に追従するカメラクラス
/// </summary>
#pragma once

#include <windows.h>
#include <Keyboard.h>
#include "Camera.h"
#include "Player.h"

class FollowCamera : public Camera
{
public:
	// 自機とカメラの距離
	static const float CAMERA_DISTANCE;
	// コンストラクタ
	FollowCamera(int width, int height);
	// 更新
	void Update() override;

	// 追従対象の座標をセット
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos);
	// 追従対象の回転角をセット
	void SetTargetAngle(float targetangle);
	// キーボードをセット
	void SetKeyboard(DirectX::Keyboard* keyboard);
	// プレイヤーをセット
	void SetPlayer(Player* player) { m_player = player; }
protected:
	// 追従対象の座標
	DirectX::SimpleMath::Vector3 m_targetPos;
	// 追従対象の回転角
	float m_targetAngle;
	// キーボード
	DirectX::Keyboard* m_keyboard;
	// キーボードトラッカー
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	// FPSフラグ
	bool isFPS;
	// プレイヤー
	Player* m_player;
};
