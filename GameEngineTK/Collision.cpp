#include "Collision.h"

using namespace DirectX::SimpleMath;

/// <summary>
/// ���Ƌ��̓����蔻��
/// </summary>
/// <param name="sphereA">��A</param>
/// <param name="sphereB">��B</param>
/// <returns>true: �������Ă���	false: �������Ă��Ȃ�</returns>
bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB)
{
	// A����B�Ɍ������x�N�g��
	Vector3 sub = sphereB.Center - sphereA.Center;
	// A��B�̋����̓����v�Z
	float distanceSquare = sub.x * sub.x + sub.y * sub.y + sub.z * sub.z;

	// ���a�̘a�̓����v�Z
	float radiusSquare = sphereA.Radius + sphereB.Radius;
	radiusSquare = radiusSquare * radiusSquare;

	// �����̓�悪���a�̘a�̓����傫����Γ������Ă��Ȃ�
	if (distanceSquare > radiusSquare)
	{
		return false;
	}

	// �������Ă���
	return true;
}

/// <summary>
/// 3�_����O�p�`���\�z
/// </summary>
/// <param name="_p0">�_�O</param>
/// <param name="_p1">�_�P</param>
/// <param name="_p2">�_�Q</param>
/// <param name="_triangle">�O�p�`�i���ʏo�͗p�j</param>
void ComputeTriangle(const Vector3& _p0, const Vector3& _p1, const Vector3& _p2, Triangle* _triangle)
{
	// ���W���R�s�[
	_triangle->P0 = _p0;
	_triangle->P1 = _p1;
	_triangle->P2 = _p2;

	// �@���x�N�g�����v�Z
	Vector3 P0_P1 = _p1 - _p0;
	Vector3 P1_P2 = _p2 - _p1;

	// 2�x�N�g���ɐ����ȃx�N�g���𓾂�
	_triangle->Normal = P0_P1.Cross(P1_P2);
	_triangle->Normal.Normalize();
}