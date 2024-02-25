#include "Sky.h"
#include "Engine/Model.h"
#include "Engine/Direct3D.h"

Sky::Sky(GameObject* _parent)
	:GameObject(_parent, "Sky"), hModel_(-1)
{

}

Sky::~Sky()
{
}

void Sky::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Sky.fbx");
	assert(hModel_ >= 0);
	//transform_.scale_ = { 0.5,0.5,0.5 };
	transform_.position_.y = -50;
}

void Sky::Update()
{
	transform_.rotate_.y += 0.01;
}

void Sky::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Sky::Release()
{
}
