//�C���N���[�h
#include "../Engine/Model.h"
#include "../Engine/Audio.h"
#include "../Engine/Global.h"
#include "../Player/PlayerBase.h"
#include "../Player/CollectPlayer.h"
#include "../Scene/PlayScene.h"
#include "../StageObject/Stage.h"
#include "Bone.h"

Bone::Bone(GameObject* _parent)
	:ItemObjectBase(_parent, boneName), hModel_(-1),pPlayScene_{nullptr}
{
}

Bone::~Bone()
{
}

void Bone::Initialize()
{
	//���f���̃��[�h
	std::string ModelName = modelFolderName + boneName + modelModifierName;
	hModel_ = Model::Load(ModelName);
	assert(hModel_ >= initZeroInt);

	SphereCollider* pCollision = new SphereCollider(initZeroXMFLOAT3, 1.0f);
	AddCollider(pCollision);
	pPlayScene_ = (PlayScene*)FindObject(playSceneName);
	transform_.scale_ = { 0.5,0.5,0.5 };
	transform_.position_ = { 10,0,0 };
}

void Bone::Update()
{
	transform_.rotate_.y += 1.0f;
	RayCastData data;
	Stage* pStage = (Stage*)FindObject(stageName);    //�X�e�[�W�I�u�W�F�N�g
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
	if(_pTarget->GetObjectName() == collectPlayerName)
	{
		this->KillMe();
		pPlayScene_->AddBoneCount(-1);
	}
}
