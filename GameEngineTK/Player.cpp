//--------------------------------------------------------------------------------------
// �t�@�C����: Player.cpp
// �쐬��:
// �쐬��:
// ����:
//--------------------------------------------------------------------------------------

#include "Player.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Player::Player(DirectX::Keyboard* keyboard)
{
	m_pKeyboard = keyboard;

	Initialize();
}

Player::~Player()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
void Player::Initialize()
{
	// ���@�p�[�c�̓ǂݍ���
	m_Obj.resize(PARTS_NUM);
	m_Obj[PARTS_UFO].LoadModel(L"Resources/ufo.cmo");
	m_Obj[PARTS_HEAD].LoadModel(L"Resources/head.cmo");
	m_Obj[PARTS_LWING].LoadModel(L"Resources/lwing.cmo");
	m_Obj[PARTS_RWING].LoadModel(L"Resources/rwing.cmo");
	m_Obj[PARTS_LVERNIER].LoadModel(L"Resources/vernier.cmo");
	m_Obj[PARTS_RVERNIER].LoadModel(L"Resources/vernier.cmo");

	// �e�q�֌W�̍\�z�i�q�p�[�c�ɐe��ݒ�j
	m_Obj[PARTS_HEAD].SetObjParent(&m_Obj[PARTS_UFO]);
	m_Obj[PARTS_LWING].SetObjParent(&m_Obj[PARTS_HEAD]);
	m_Obj[PARTS_RWING].SetObjParent(&m_Obj[PARTS_HEAD]);
	m_Obj[PARTS_LVERNIER].SetObjParent(&m_Obj[PARTS_UFO]);
	m_Obj[PARTS_RVERNIER].SetObjParent(&m_Obj[PARTS_UFO]);

	// �q�p�[�c�̐e����̃I�t�Z�b�g�i���W�̂���j���Z�b�g
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

	// �f�t�H���g�ł͔��˂��Ă��Ȃ�
	FireFlag = false;

	{// �e�ۗp�̓����蔻��m�[�h�̐ݒ�
		m_CollisionNodeBullet.Initialize();
		// �e�p�[�c��ݒ�
		m_CollisionNodeBullet.SetParent(&m_Obj[PARTS_RWING]);
		m_CollisionNodeBullet.SetTrans(Vector3(0.5f, 0, 0.1f));
		m_CollisionNodeBullet.SetLocalRadius(0.5f);
	}

	{// �S�̗p�̓����蔻��m�[�h�̐ݒ�
		m_CollisionNodeBody.Initialize();
		// �e�p�[�c��ݒ�
		m_CollisionNodeBody.SetParent(&m_Obj[0]);
		m_CollisionNodeBody.SetTrans(Vector3(0, 0.5f, 0));
		m_CollisionNodeBody.SetLocalRadius(0.8f);
	}

	m_isJump = false;
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void Player::Update()
{
	// �L�[�{�[�h�̏�Ԏ擾
	Keyboard::State keystate = m_pKeyboard->GetState();
	m_KeyboardTracker.Update(keystate);

	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keys::Space))
	{
		// �W�����v�J�n
		StartJump();
	}

	// �������ł����
	if (m_isJump)
	{
		// �d�͂ɂ�����
		m_Velocity.y -= GRAVITY_ACC;

		// ���������x�̌��E����
		if (m_Velocity.y < -JUMP_SPEED_MAX)
		{
			m_Velocity.y = -JUMP_SPEED_MAX;
		}
	}

	{// ���x�ɂ��ړ�
		Vector3 trans = GetTrans();
		trans += m_Velocity;
		SetTrans(trans);
	}

	// �p�[�c�M�~�b�N
	{
		m_cycle += 0.1f;
		float pos_z = -1.0f + sinf(m_cycle);
		m_Obj[PARTS_LWING].SetRotation(
			Vector3(-0.2f, 0.2f, 0) + Vector3(0, 0, pos_z));
	}
	
	// �����񏈗�
	if (keystate.A)
	{
		// ���@�̊p�x����]
		//head_angle += 0.03f;
		float angle = m_Obj[0].GetRotation().y;
		m_Obj[0].SetRotation(Vector3(0, angle + 0.03f, 0));
	}

	// �E���񏈗�
	if (keystate.D)
	{
		// ���@�̊p�x����]
		//head_angle += -0.03f;
		float angle = m_Obj[0].GetRotation().y;
		m_Obj[0].SetRotation(Vector3(0, angle - 0.03f, 0));
	}

	// �O�i����
	if (keystate.W)
	{
		// �ړ��ʂ̃x�N�g��
		Vector3 moveV(0, 0, -0.1f);
		// �ړ��x�N�g�������@�̊p�x����]������
		//moveV = Vector3::TransformNormal(moveV, head_world);
		float angle = m_Obj[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// ���@�̍��W���ړ�
		Vector3 pos = m_Obj[0].GetTranslation();
		m_Obj[0].SetTranslation(pos + moveV);
	}

	// ��ޏ���
	if (keystate.S)
	{
		// �ړ��ʂ̃x�N�g��
		Vector3 moveV(0, 0, +0.1f);
		// �ړ��x�N�g�������@�̊p�x����]������
		//moveV = Vector3::TransformNormal(moveV, head_world);
		float angle = m_Obj[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// ���@�̍��W���ړ�
		Vector3 pos = m_Obj[0].GetTranslation();
		m_Obj[0].SetTranslation(pos + moveV);
	}

	// �X�y�[�X�L�[����������
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keys::Space))
	{
		// ���˒��Ȃ�
		if (FireFlag)
		{
			// �߂�
			ResetBullet();
		}
		else
		{
			// ����
			FireBullet();
		}
	}

	// �e�ۃp�[�c�̑O�i����
	if (FireFlag)
	{
		// �e�ۃp�[�c�̍��W���ړ�
		Vector3 pos = m_Obj[PARTS_RWING].GetTranslation();
		m_Obj[PARTS_RWING].SetTranslation(pos + m_BulletVel);
	}

	Calc();
}

//-----------------------------------------------------------------------------
// �s��X�V
//-----------------------------------------------------------------------------
void Player::Calc()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Update();
	}

	m_CollisionNodeBullet.Update();
	m_CollisionNodeBody.Update();
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void Player::Draw()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Draw();
	}

	m_CollisionNodeBullet.Draw();
	m_CollisionNodeBody.Draw();
}

void Player::FireBullet()
{
	if (FireFlag) return;

	// ���˂���p�[�c�̃��[���h�s����擾
	Matrix worldm = m_Obj[PARTS_RWING].GetWorld();

	// ���o�����������܂��Ă����ϐ�
	Vector3 scale;			// ���[���h�X�P�[�����O
	Quaternion rotation;	// ���[���h��]
	Vector3 translation;	// ���[���h���W

	// ���[���h�s�񂩂�e�v�f�𒊏o
	worldm.Decompose(scale, rotation, translation);

	// �e�p�[�c���番���A�Ɨ�������
	m_Obj[PARTS_RWING].SetObjParent(nullptr);
	m_Obj[PARTS_RWING].SetScale(scale);
	m_Obj[PARTS_RWING].SetRotationQ(rotation);
	m_Obj[PARTS_RWING].SetTranslation(translation);

	// �e�ۃp�[�c�̑��x��ݒ�
	m_BulletVel = Vector3(0, 0, -0.1f);
	// �p�[�c�̌����ɍ��킹�đ��x�x�N�g������]
	m_BulletVel = Vector3::Transform(m_BulletVel, rotation);

	FireFlag = true;
}

void Player::ResetBullet()
{
	if (!FireFlag) return;

	m_Obj[PARTS_RWING].SetObjParent(&m_Obj[PARTS_HEAD]);
	m_Obj[PARTS_RWING].SetScale(Vector3(1, 1, 1));
	m_Obj[PARTS_RWING].SetRotation(Vector3(0, 0, XMConvertToRadians(+30)));
	m_Obj[PARTS_RWING].SetTranslation(Vector3(+0.2f, 0.2f, 0));

	FireFlag = false;
}

const DirectX::SimpleMath::Vector3& Player::GetTrans()
{
	// �^���N�p�[�c�̍��W��Ԃ�
	return m_Obj[0].GetTranslation();
}

const DirectX::SimpleMath::Vector3 & Player::GetRot()
{
	// �^���N�p�[�c�̉�]��Ԃ�
	return m_Obj[0].GetRotation();
}

void Player::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	// �^���N�p�[�c�̍��W��ݒ�
	m_Obj[0].SetTranslation(trans);
}

void Player::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	// �^���N�p�[�c�̍��W��ݒ�
	m_Obj[0].SetRotation(rot);
}

const DirectX::SimpleMath::Matrix& Player::GetLocalWorld()
{
	// �^���N�p�[�c�̃��[���h�s���Ԃ�
	return m_Obj[0].GetWorld();
}

/// <summary>
/// �W�����v���J�n
/// </summary>
void Player::StartJump()
{
	if (!m_isJump)
	{
		// ������̏�����ݒ�
		m_Velocity.y = JUMP_SPEED_FIRST;
		m_isJump = true;
	}
}

/// <summary>
/// �������J�n
/// </summary>
void Player::StartFall()
{
	if (!m_isJump)
	{
		// ������̏�����ݒ�
		m_Velocity.y = 0;
		m_isJump = true;
	}
}

/// <summary>
/// �������I��
/// </summary>
void Player::StopJump()
{
	m_isJump = false;
	m_Velocity = Vector3::Zero;
}