//インクルード
#include "../Engine/Model.h"
#include "../Engine/Audio.h"
#include "Bone.h"
#include "../Player/PlayerBase.h"
#include "../Player/CollectPlayer.h"
#include "../Scene/PlayScene.h"
#include "../StageObject/Stage.h"

Bone::Bone(GameObject* _parent)
	:ObjectBase(_parent, boneName), hModel_(-1),pPlayScene_{nullptr}
{
}

Bone::~Bone()
{
}

void Bone::Initialize()
{
	//モデルのロード
	std::string ModelName = (std::string)"Model&Picture/" + boneName + (std::string)".fbx";
	hModel_ = Model::Load(ModelName);
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
	Stage* pStage = (Stage*)FindObject("Stage");    //ステージオブジェクト
	int stageHModel;
	stageHModel = pStage->GetModelHandle();   //モデル番号を取得
	data.start = transform_.position_;  //レイの発射位置
	data.start.y = 0;
	data.dir = XMFLOAT3(0, -1, 0);       //レイの方向
	Model::RayCast(stageHModel, &data);  //レイを発射
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
		this->KillMe();
		pPlayScene_->AddBoneCount(-1);
	}
}
