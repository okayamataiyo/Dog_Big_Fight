//�C���N���[�h
#include "Engine/Model.h"
#include "Bone.h"
#include "Stage.h"

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
	RayCastData data;
	Stage* pStage = (Stage*)FindObject("Stage");    //�X�e�[�W�I�u�W�F�N�g
	int hStageModel;
	hStageModel = pStage->GetModelHandle();   //���f���ԍ����擾
	data.start = transform_.position_;  //���C�̔��ˈʒu
	data.start.y = 0;
	data.dir = XMFLOAT3(0, -1, 0);       //���C�̕���
	Model::RayCast(hStageModel, &data);  //���C�𔭎�
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
	if (_pTarget->GetObjectName().find("Player") != std::string::npos)
	{
		KillMe();
	}
}