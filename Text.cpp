#include "Text.h"

Text::Text(GameObject* _pParent)
	:GameObject(_pParent, "Text"), hModel_{ -1,-1 },textState_(Select)
{
}

void Text::Initialize()
{
	hModel_[0] = Model::Load("GameOverText.fbx");
	assert(hModel_[0] >= 0);
	hModel_[1] = Model::Load("SelectText.fbx");
	assert(hModel_[1] >= 0);
	transform_.rotate_.y = 180;
}

void Text::Update()
{
}

void Text::Draw()
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

void Text::Release()
{
}

void Text::SetMode(int _mode)
{
	textState_ = static_cast<TEXTSTATE>(_mode);
}
