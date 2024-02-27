#include "Engine/Model.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Engine/Fbx.h"
#include "Floor.h"
#include "ObjectBase.h"

Floor::Floor(GameObject* _parent)
    :ObjectBase(_parent, "Floor"), hModel_(-1)
{

}

Floor::~Floor()
{

}

void Floor::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("Floor.fbx");
    assert(hModel_ >= 0);
    transform_.scale_ = { 4,1,4 };
}

void Floor::Update()
{
    //transform_.rotate_.y += 1;
}

void Floor::Draw()
{
    for (int i = 0u; i <= 1; i++)
    {
        Model::SetTransform(hModel_, transform_);
        //最初に3Dで描画後、枠づけも描画
        /*for (int j = 0; j <= 4; j += 4)
        {
            Direct3D::SetShader(static_cast<Direct3D::SHADER_TYPE>(j));
            Model::Draw(hModel_[i]);
        }*/
        Model::Draw(hModel_);
    }
}

void Floor::Release()
{
}

