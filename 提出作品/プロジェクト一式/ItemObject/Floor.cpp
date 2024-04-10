#include "../Engine/Model.h"
#include "../Engine/Camera.h"
#include "../Engine/Input.h"
#include "../Engine/Fbx.h"
#include "Floor.h"
#include "ItemObjectBase.h"

Floor::Floor(GameObject* _parent)
    :ItemObjectBase(_parent, floorName), hModel_(-1),upOrDown_(0)
{

}

Floor::~Floor()
{

}

void Floor::Initialize()
{
    //モデルデータのロード
    std::string ModelName = (std::string)"Model&Picture/" + floorName + (std::string)".fbx";
    hModel_ = Model::Load(ModelName);
    assert(hModel_ >= 0);
    transform_.scale_ = { 1,1,1 };
}

void Floor::Update()
{
    //transform_.position_.y += 0.01;
    if (upOrDown_ == 0)
    {
        transform_.position_.y += 0.1f;
    }
    if (upOrDown_ == 1)
    {
        transform_.position_.y -= 0.1f;
    }

    if (transform_.position_.y >= 10.0f)
    {
        upOrDown_ = 1;
    }
    if (transform_.position_.y <= 0.0f)
    {
        upOrDown_ = 0;
    }
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

