/// <summary>
/// 衝突判定ライブラリ
/// </summary>
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>

// 球
class Sphere
{
public:
	// 中心座標
	DirectX::SimpleMath::Vector3 Center;
	// 半径
	float Radius;
	// コンストラクタ
	Sphere()
	{
		// デフォルトの半径を１メートルに
		Radius = 1.0f;
	}
};

// 線分
class Segment
{
public:
	// 始点座標
	DirectX::SimpleMath::Vector3 Start;
	// 終点座標
	DirectX::SimpleMath::Vector3 End;
};

// 三角形
class Triangle
{
public:
	// 頂点座標
	DirectX::SimpleMath::Vector3 P0;
	DirectX::SimpleMath::Vector3 P1;
	DirectX::SimpleMath::Vector3 P2;
	// 法線ベクトル
	DirectX::SimpleMath::Vector3 Normal;
};

bool CheckSphere2Sphere(const Sphere & sphereA, const Sphere & sphereB);

void ComputeTriangle(const DirectX::SimpleMath::Vector3 & _p0, const DirectX::SimpleMath::Vector3 & _p1, const DirectX::SimpleMath::Vector3 & _p2, Triangle * _triangle);

bool CheckSphere2Triangle(const Sphere & _sphere, const Triangle & _triangle, DirectX::SimpleMath::Vector3 * _inter);

bool CheckSegment2Triangle(const Segment & _segment, const Triangle & _triangle, DirectX::SimpleMath::Vector3 * _inter);
