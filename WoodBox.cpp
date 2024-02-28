#include "Engine/Model.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Engine/Fbx.h"
#include "WoodBox.h"
#include "Stage.h"
#include "ObjectBase.h"
#include "Engine/ImGui/imgui.h"
WoodBox::WoodBox(GameObject* _parent)
    :ObjectBase(_parent, "WoodBox"), hModel_(-1)
{

}

WoodBox::~WoodBox()
{

}

void WoodBox::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("WoodBox.fbx");
    assert(hModel_ >= 0);

    BoxCollider* pCollision = new BoxCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f));
    AddCollider(pCollision);
}

void WoodBox::Update()
{
    //transform_.rotate_.y += 1;
    RayCastData data;
    Stage* pStage = (Stage*)FindObject("Stage");    //ステージオブジェクト
    int hStageModel;
    hStageModel = pStage->GetModelHandle();   //モデル番号を取得
    data.start = transform_.position_;  //レイの発射位置
    data.start.y = 0;
    data.dir = XMFLOAT3(0, -1, 0);       //レイの方向
    Model::RayCast(hStageModel, &data);  //レイを発射
    rayDist_ = data.dist;

    if (data.hit == true)
    {
        transform_.position_.y = -data.dist + 0.6;
    }
}

void WoodBox::Draw()
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

void WoodBox::Release()
{
}

void WoodBox::OnCollision(GameObject* _pTarget)
{

}
