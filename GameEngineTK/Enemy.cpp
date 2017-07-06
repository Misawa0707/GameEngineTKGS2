//--------------------------------------------------------------------------------------
// �t�@�C����: Enemy.cpp
// �쐬��:
// �쐬��:
// ����:
//--------------------------------------------------------------------------------------

#include "Enemy.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Enemy::Enemy(DirectX::Keyboard* keyboard)
{
	m_pKeyboard = keyboard;

	Initialize();
}

Enemy::~Enemy()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
void Enemy::Initialize()
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

	// �����ʒu������
	Vector3 pos;

	pos.x = rand() % 10;
	pos.z = rand() % 10;

	SetTrans(pos);

	m_Timer = 0;

	m_DistAngle = 0;

	{// �S�̗p�̓����蔻��m�[�h�̐ݒ�
		m_CollisionNodeBody.Initialize();
		// �e�p�[�c��ݒ�
		m_CollisionNodeBody.SetParent(&m_Obj[0]);
		m_CollisionNodeBody.SetTrans(Vector3(0,0.5f,0));
		m_CollisionNodeBody.SetLocalRadius(0.8f);
	}
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void Enemy::Update()
{
	// �L�[�{�[�h�̏�Ԏ擾
	Keyboard::State keystate = m_pKeyboard->GetState();
	m_KeyboardTracker.Update(keystate);

	// �p�[�c�M�~�b�N
	{
		m_cycle += 0.1f;
		float pos_z = -1.0f + sinf(m_cycle);
		m_Obj[PARTS_LWING].SetRotation(
			Vector3(-0.2f, 0.2f, 0) + Vector3(0, 0, pos_z));
	}

	m_Timer--;
	if (m_Timer < 0)
	{
		m_Timer = 60;

		// �ڕW�p�������_���ɒ��I
		// -0.5�`+0.5�̗���
		float rnd = (float)rand() / RAND_MAX - 0.5f;
		// -90�`+90�̗���
		rnd *= 180.0f;
		rnd = XMConvertToRadians(rnd);

		m_DistAngle += rnd;
	}
	
	{
		// ���@�̊p�x����]
		Vector3 rot = GetRot();

		// ���̊p�x�ƖڕW�p�x�̍�
		float angle = m_DistAngle - rot.y;
		if (angle > XM_PI)
		{
			angle -= XM_2PI;
		}
		if (angle < -XM_PI)
		{
			angle += XM_2PI;
		}

		// ���
		rot.y += angle * 0.01f;

		SetRot(rot);
	}

	// �@�̂������Ă�������ɐi�ޏ���
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

	Calc();
}

//-----------------------------------------------------------------------------
// �s��X�V
//-----------------------------------------------------------------------------
void Enemy::Calc()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Update();
	}

	m_CollisionNodeBody.Update();
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void Enemy::Draw()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Draw();
	}

	m_CollisionNodeBody.Draw();
}

const DirectX::SimpleMath::Vector3& Enemy::GetTrans()
{
	// �^���N�p�[�c�̍��W��Ԃ�
	return m_Obj[0].GetTranslation();
}

const DirectX::SimpleMath::Vector3 & Enemy::GetRot()
{
	// �^���N�p�[�c�̉�]��Ԃ�
	return m_Obj[0].GetRotation();
}

void Enemy::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	// �^���N�p�[�c�̍��W��ݒ�
	m_Obj[0].SetTranslation(trans);
}

void Enemy::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	// �^���N�p�[�c�̍��W��ݒ�
	m_Obj[0].SetRotation(rot);
}

const DirectX::SimpleMath::Matrix& Enemy::GetLocalWorld()
{
	// �^���N�p�[�c�̃��[���h�s���Ԃ�
	return m_Obj[0].GetWorld();
}
