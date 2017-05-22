/// <summary>
/// ３Ｄオブジェクトのクラス
/// </summary>
#pragma once

#include <windows.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>

#include "Camera.h"

class Obj3d
{
public:
	// 静的メンバ関数
	static void InitializeStatic(
		Microsoft::WRL::ComPtr<ID3D11Device>            d3dDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>     d3dContext,
		Camera* camera);
private:
	// 静的メンバ変数
	// デバイス
	static Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
	// デバイスコンテキスト
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
	// カメラ
	static Camera* m_Camera;
	// 汎用ステート設定
	static std::unique_ptr<DirectX::CommonStates> m_states;
	// エフェクトファクトリ
	static std::unique_ptr<DirectX::EffectFactory> m_factory;

public:
	// コンストラクタ
	Obj3d();

	// ＣＭＯモデルを読み込む
	void LoadModel(const wchar_t* fileName);
	// 更新
	void Update();
	// 描画
	void Draw();

	// メンバ関数
	// setter
	// スケーリング(XYZ)

	// 回転角(XYZ)

	// 平行移動(XYZ)

	// 親の３Ｄオブジェクト

	// getter
	// スケーリング(XYZ)

	// 回転角(XYZ)

	// 平行移動(XYZ)

	// ワールド行列


private:
	// メンバ変数
	// モデル

	// スケーリング(XYZ)

	// 回転角(XYZ)

	// 平行移動(XYZ)

	// ワールド行列

	// 親のObj3dへのポインタ
	Obj3d* m_ObjParent;
};