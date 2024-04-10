//インクルード
#include "../Engine/Model.h"
#include "../Engine/Camera.h"
#include "../Engine/Input.h"
#include "../Engine/Fbx.h"
#include "../Engine/ImGui/imgui.h"
#include "../Engine/Audio.h"
#include "../Engine/Global.h"
#include "../StageObject/Stage.h"
#include "../Player/AttackPlayer.h"
#include "ItemObjectManager.h"
#include "WoodBox.h"

WoodBox::WoodBox(GameObject* _pParent)
    :ItemObjectBase(_pParent, woodBoxName), hModel_{-1}, hSound_{-1}, isOnWoodBox_{0}
    ,pParent_{nullptr},pPlayScene_{nullptr}, pAttackPlayer_{ nullptr },pCollision_{nullptr}
{
    pParent_ = _pParent;
}

WoodBox::~WoodBox()
{ 

}

void WoodBox::Initialize()
{
    //サウンドデータのロード
    std::string soundName = soundFolderName + soundWoodBoxName + soundModifierName;
    hSound_ = Audio::Load(soundName);
    assert(hSound_ >= initZeroInt);
    //モデルデータのロード
    std::string modelName = modelFolderName + woodBoxName + modelModifierName;
    hModel_ = Model::Load(modelName);
    assert(hModel_ >= initZeroInt);
    pCollision_ = new SphereCollider(initZeroXMFLOAT3, 3.0f);
    AddCollider(pCollision_);
    pPlayScene_ = (PlayScene*)FindObject(playSceneName);
    pAttackPlayer_ = (AttackPlayer*)FindObject(attackPlayerName);
}

void WoodBox::Update()
{
    woodBoxs_ = pPlayScene_->GetWoodBoxs();
    RayCast();
}

void WoodBox::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void WoodBox::Release()
{
}

void WoodBox::Move()
{
}

void WoodBox::RayCast()
{
    positionY_ = transform_.position_.y;
    RayCastData woodBoxDataDown;
    RayCastData stageDataDown;
    float woodBoxFling      = 2.0f;
    int woodBoxHModelStart     = woodBoxs_.front();
    int woodBoxHModelEnd       = woodBoxs_.back();
    int woodBoxHModelNow     = GetModelHandle();
    Stage* pStage           = (Stage*)FindObject(stageName);      //ステージオブジェクト
    int stageHModel         = pStage->GetModelHandle();         //モデル番号を取得
    if (isJump_)
    {
        //放物線に下がる処理
        positionTempY_ = positionY_;
        positionY_ += (positionY_ - positionPrevY_) - 0.007;
        positionPrevY_ = positionTempY_;
        isJump_ = !(rayStageDistDown_ <= woodBoxFling);
    }

    if (isBreak_)
    {
        Audio::Play(hSound_,0.3f);
        this->KillMe();
    }

    //for (int i : vector) {}
    //std::vector<int> woodBoxSize = ;

    for (int i = 0; i < woodBoxs_.size();i++)
    {
        ////▼木箱の法線(木箱の上に木箱が乗るため)
        //woodBoxDataDown.start       = transform_.position_;
        ////woodBoxDataDown.start.y     = 0;
        //woodBoxDataDown.dir         = XMFLOAT3(0, -1, 0);
        //if (woodBoxs_.at(i) != woodBoxHModelNow)
        //{
        //    int nowData = woodBoxs_.at(i);
        //    Model::RayCast(nowData, &woodBoxDataDown);
        //}
        //rayWoodBoxDist_         = woodBoxDataDown.dist;
        //if (rayWoodBoxDist_ <= woodBoxFling)
        //{
        //    if (!isJump_)
        //    {
        //        //positionY_ = -woodBoxDataDown.dist + 0.6;
        //        isOnWoodBox_ = 1;
        //        positionTempY_ = positionY_;
        //        positionPrevY_ = positionTempY_;
        //        positionY_ = positionPrevY_;
        //    }
        //}
        //else
        //{
        //    isOnWoodBox_ = 0;
        //}
        //▼ステージの法線(地面に張り付き)
        stageDataDown.start = transform_.position_;             //レイの発射位置
        //stageDataDown.start.y = 0;
        stageDataDown.dir = XMFLOAT3(0, -1, 0);               //レイの方向
        Model::RayCast(stageHModel, &stageDataDown);                //レイを発射
        rayStageDistDown_ = stageDataDown.dist;
        if (rayStageDistDown_ <= woodBoxFling)
        {
            if (!isJump_ && isOnWoodBox_ == 0)
            {
                //positionY_ += stageDataDown.dist + 0.6;  //地面の張り付き
                positionTempY_ = positionY_;
                positionPrevY_ = positionTempY_;
                positionY_ = positionPrevY_;
            }
        }
        if(isOnWoodBox_ == 0 && rayStageDistDown_ >= woodBoxFling)
        {
            isJump_ = true;
        }
    }
    transform_.position_.y = positionY_;
    //ImGui::Text("rayWoodBoxDist_=%f", rayWoodBoxDist_);
    //ImGui::Text("rayStageDistDown_=%f", rayStageDistDown_);
    //ImGui::Text("isJump_=%s", isJump_ ? "true" : "false");
    //ImGui::Text("positionY_=%f", positionY_);
}

void WoodBox::OnCollision(GameObject* _pTarget)
{

}
