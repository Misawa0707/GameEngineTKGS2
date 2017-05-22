/// <summary>
/// �R�c�I�u�W�F�N�g�̃N���X
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
	// �ÓI�����o�֐�
	static void InitializeStatic(
		Microsoft::WRL::ComPtr<ID3D11Device>            d3dDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>     d3dContext,
		Camera* camera);
private:
	// �ÓI�����o�ϐ�
	// �f�o�C�X
	static Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
	// �f�o�C�X�R���e�L�X�g
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
	// �J����
	static Camera* m_Camera;
	// �ėp�X�e�[�g�ݒ�
	static std::unique_ptr<DirectX::CommonStates> m_states;
	// �G�t�F�N�g�t�@�N�g��
	static std::unique_ptr<DirectX::EffectFactory> m_factory;

public:
	// �R���X�g���N�^
	Obj3d();

	// �b�l�n���f����ǂݍ���
	void LoadModel(const wchar_t* fileName);
	// �X�V
	void Update();
	// �`��
	void Draw();

	// �����o�֐�
	// setter
	// �X�P�[�����O(XYZ)

	// ��]�p(XYZ)

	// ���s�ړ�(XYZ)

	// �e�̂R�c�I�u�W�F�N�g

	// getter
	// �X�P�[�����O(XYZ)

	// ��]�p(XYZ)

	// ���s�ړ�(XYZ)

	// ���[���h�s��


private:
	// �����o�ϐ�
	// ���f��

	// �X�P�[�����O(XYZ)

	// ��]�p(XYZ)

	// ���s�ړ�(XYZ)

	// ���[���h�s��

	// �e��Obj3d�ւ̃|�C���^
	Obj3d* m_ObjParent;
};