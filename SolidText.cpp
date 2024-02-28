#include "SolidText.h"

SolidText::SolidText(GameObject* _pParent)
	:GameObject(_pParent, "Text"), hModel_{ -1,-1 },textState_(Select)
{
}

void SolidText::Initialize()
{
	hModel_[0] = Model::Load("GameOverText.fbx");
	assert(hModel_[0] >= 0);
	hModel_[1] = Model::Load("SelectText.fbx");
	assert(hModel_[1] >= 0);
	transform_.rotate_.y = 180;
}

void SolidText::Update()
{
}

void SolidText::Draw()
{
	switch (textState_)
	{
	case TEXTSTATE::GameOver: 
		Model::SetTransform(hModel_[0], transform_);
		Model::Draw(hModel_[0]);
		break;
	case TEXTSTATE::Select:
		Model::SetTransform(hModel_[1], transform_);
		Model::Draw(hModel_[1]);
		break;
	}
}

void SolidText::Release()
{
}

void SolidText::SetMode(int _mode)
{
	textState_ = static_cast<TEXTSTATE>(_mode);
}