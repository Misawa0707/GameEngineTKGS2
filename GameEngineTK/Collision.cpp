#include "Collision.h"

using namespace DirectX::SimpleMath;

/// <summary>
/// 球と球の当たり判定
/// </summary>
/// <param name="sphereA">球A</param>
/// <param name="sphereB">球B</param>
/// <returns>true: 当たっている	false: 当たっていない</returns>
bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB)
{
	// AからBに向かうベクトル
	Vector3 sub = sphereB.Center - sphereA.Center;
	// AとBの距離の二乗を計算
	float distanceSquare = sub.x * sub.x + sub.y * sub.y + sub.z * sub.z;

	// 半径の和の二乗を計算
	float radiusSquare = sphereA.Radius + sphereB.Radius;
	radiusSquare = radiusSquare * radiusSquare;

	// 距離の二乗が半径の和の二乗より大きければ当たっていない
	if (distanceSquare > radiusSquare)
	{
		return false;
	}

	// 当たっている
	return true;
}

/// <summary>
/// 3点から三角形を構築
/// </summary>
/// <param name="_p0">点０</param>
/// <param name="_p1">点１</param>
/// <param name="_p2">点２</param>
/// <param name="_triangle">三角形（結果出力用）</param>
void ComputeTriangle(const Vector3& _p0, const Vector3& _p1, const Vector3& _p2, Triangle* _triangle)
{
	// 座標をコピー
	_triangle->P0 = _p0;
	_triangle->P1 = _p1;
	_triangle->P2 = _p2;

	// 法線ベクトルを計算
	Vector3 P0_P1 = _p1 - _p0;
	Vector3 P1_P2 = _p2 - _p1;

	// 2ベクトルに垂直なベクトルを得る
	_triangle->Normal = P0_P1.Cross(P1_P2);
	_triangle->Normal.Normalize();
}