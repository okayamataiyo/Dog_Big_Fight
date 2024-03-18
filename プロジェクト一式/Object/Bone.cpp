//�C���N���[�h
#include "../Engine/Model.h"
#include "Bone.h"
#include "../Stage.h"
#include "../Player/PlayerBase.h"

Bone::Bone(GameObject* _parent)
	:ObjectBase(_parent, "Bone"), hModel_(-1)
{
}

Bone::~Bone()
{
}

void Bone::Initialize()
{
	hModel_ = Model::Load("Bone.fbx");
	assert(hModel_ >= 0);

	SphereCollider* pCollision = new SphereCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), 1.0f);
	AddCollider(pCollision);

	transform_.scale_ = { 0.2,0.2,0.2 };
	transform_.position_ = { 10,0,0 };
}

void Bone::Update()
{
	transform_.rotate_.y += 1.0f;
	RayCastData data;
	Stage* pStage = (Stage*)FindObject("Stage");    //�X�e�[�W�I�u�W�F�N�g
	int stageHModel;
	stageHModel = pStage->GetModelHandle();   //���f���ԍ����擾
	data.start = transform_.position_;  //���C�̔��ˈʒu
	data.start.y = 0;
	data.dir = XMFLOAT3(0, -1, 0);       //���C�̕���
	Model::RayCast(stageHModel, &data);  //���C�𔭎�
	rayDist_ = data.dist;

	if (data.hit == true)
	{
		transform_.position_.y = -data.dist + 0.6;
	}
}

void Bone::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Bone::Release()
{
}

void Bone::OnCollision(GameObject* _pTarget)
{
	if (_pTarget->GetObjectName().find("CollectPlayer") != std::string::npos)
	{
		//this->KillMe();
		//((PlayerBase*)_pTarget)->Set
	}
}
