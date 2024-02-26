#include "Engine/Model.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Engine/Fbx.h"
#include "Stage.h"

Stage::Stage(GameObject* _parent)
    :GameObject(_parent, "Stage"), hModel_(-1)
{

}

Stage::~Stage()
{

}

void Stage::Initialize()
{
    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("Ground.fbx");
    assert(hModel_ >= 0);
    transform_.position_.y = -5;
    transform_.rotate_.y = 90;
    transform_.scale_ = { 4,1,4 };
}

void Stage::Update()
{

}

void Stage::Draw()
{
    for (int i = 0u; i <= 1; i++)
    {
        Model::SetTransform(hModel_, transform_);
        //�ŏ���3D�ŕ`���A�g�Â����`��
        /*for (int j = 0; j <= 4; j += 4)
        {
            Direct3D::SetShader(static_cast<Direct3D::SHADER_TYPE>(j));
            Model::Draw(hModel_[i]);
        }*/
        Model::Draw(hModel_);
    }
}

void Stage::Release()
{
}