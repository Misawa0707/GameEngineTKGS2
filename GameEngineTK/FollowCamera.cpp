#include "FollowCamera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// �ÓI�����o�ϐ��̏�����
const float FollowCamera::CAMERA_DISTANCE = 5.0f;

FollowCamera::FollowCamera(int width, int height)
	:Camera(width, height)
{
	m_targetPos = Vector3::Zero;
	m_targetAngle = 0.0f;
	m_keyboard = nullptr;
	isFPS = false;
}

void FollowCamera::Update()
{
	// �L�[�{�[�h�̏�Ԃ��擾
	Keyboard::State keystate = m_keyboard->GetState();
	m_keyboardTracker.Update(keystate);

	// C�L�[�������ꂽ��
	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keyboard::C))
	{
		// �J�����̃t���O��؂�ւ�
		isFPS = !isFPS;
	}

	// �J�������_���W�A�Q�Ɠ_���W
	Vector3 eyepos, refpos;


	if (isFPS)
	{ // FPS�J�����̏���
		Vector3 position;
	  // �Q�Ɠ_���W���v�Z
		position = m_targetPos + Vector3(0, 0.2f, 0);
		// ���@����J�������W�ւ̍���
		Vector3 cameraV(0, 0, -CAMERA_DISTANCE);
		// ���@�̌��ɉ�荞�ވׂ̉�]�s��
		Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
		// �J�����ւ̃x�N�g������]
		cameraV = Vector3::TransformNormal(cameraV, rotmat);
		// ������Ɛi�񂾈ʒu�����_���W
		eyepos = position + cameraV * 0.1f;
		// ������i�񂾈ʒu���Q�Ɠ_���W
		refpos = position + cameraV;
	}
	else
	{ // TPS�J�����̏���
		// �Q�Ɠ_���W���v�Z
		refpos = m_targetPos + Vector3(0, 2, 0);
		// ���@����J�������W�ւ̍���
		Vector3 cameraV(0, 0, CAMERA_DISTANCE);
		// ���@�̌��ɉ�荞�ވׂ̉�]�s��
		Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
		// �J�����ւ̃x�N�g������]
		cameraV = Vector3::TransformNormal(cameraV, rotmat);
		// �J�������W���v�Z
		eyepos = refpos + cameraV;

		// ���_�����݈ʒu�����Ԃ���
		eyepos = m_eyepos + (eyepos - m_eyepos) * 0.05f;
		// �Q�Ɠ_�����݈ʒu�����Ԃ���
		refpos = m_refpos + (refpos - m_refpos) * 0.2f;
	}

		

	SetEyePos(eyepos);
	SetRefPos(refpos);

	// ���N���X�̍X�V
	Camera::Update();
}

void FollowCamera::SetTargetPos(const DirectX::SimpleMath::Vector3 & targetpos)
{
	m_targetPos = targetpos;
}

void FollowCamera::SetTargetAngle(float targetangle)
{
	m_targetAngle = targetangle;
}

void FollowCamera::SetKeyboard(DirectX::Keyboard * keyboard)
{
	m_keyboard = keyboard;
}
