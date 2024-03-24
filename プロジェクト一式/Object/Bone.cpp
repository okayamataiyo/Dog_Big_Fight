//�C���N���[�h
#include "../Engine/Model.h"
#include "../Engine/Audio.h"
#include "Bone.h"
#include "../Stage.h"
#include "../Player/PlayerBase.h"
#include "../Player/CollectPlayer.h"
#include "../Scene/PlayScene.h"

Bone::Bone(GameObject* _parent)
	:ObjectBase(_parent, "Bone"), hModel_(-1),hSound_{-1},pPlayScene_{nullptr}
{
}

Bone::~Bone()
{
}

void Bone::Initialize()
{
	//�T�E���h�̃��[�h
	hSound_ = Audio::Load("Sound/CollectBone.wav");
	assert(hSound_ >= 0);
	//���f���̃��[�h
	hModel_ = Model::Load("Bone.fbx");
	assert(hModel_ >= 0);

	SphereCollider* pCollision = new SphereCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), 1.0f);
	AddCollider(pCollision);
	pPlayScene_ = (PlayScene*)FindObject("PlayScene");
	transform_.scale_ = { 0.5,0.5,0.5 };
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
	//if (_pTarget->GetObjectName().find("CollectPlayer") != std::string::npos)
	if(_pTarget->GetObjectName() == collectPlayerName)
	{
		Audio::Play(hSound_, 0.3f);
		this->KillMe();
		pPlayScene_->AddBoneCount(-1);
	}
}
