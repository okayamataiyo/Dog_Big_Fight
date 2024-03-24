#include "../Engine/Model.h"
#include "../Engine/Camera.h"
#include "../Engine/Input.h"
#include "../Engine/Fbx.h"
#include "../Engine/ImGui/imgui.h"
#include "../Engine/Audio.h"
#include "FrameBox.h"
#include "ObjectManager.h"
#include "../Player/AttackPlayer.h"
#include "../StageObject/Stage.h"

FrameBox::FrameBox(GameObject* _pParent)
    :ObjectBase(_pParent, "FrameBox"), hModel_(-1), hSound_{ -1 }, isOnFrameBox_(0),pAttackPlayer_(nullptr)
{
    pParent_ = _pParent;
}

FrameBox::~FrameBox()
{

}

void FrameBox::Initialize()
{
    //サウンドデータのロード
    //hSound_ = Audio::Load("Sound/FrameBoxBreak.wav");
    //assert(hSound_ >= 0);
    //モデルデータのロード
    //hModel_ = Model::Load("FrameBox.fbx");
    //assert(hModel_ >= 0);
    BoxCollider* pCollision = new BoxCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(5.0f, 5.0f, 5.0f));
    //SphereCollider* pCollision = new SphereCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), 1.5);
    AddCollider(pCollision);
    pPlayScene_ = (PlayScene*)FindObject("PlayScene");
    pAttackPlayer_ = (AttackPlayer*)FindObject(attackPlayerName);
}

void FrameBox::Update()
{
    transform_.position_ = pPlayScene_->GetAttackPlayerPosition();
}

void FrameBox::Draw()
{
    //Model::SetTransform(hModel_, transform_);
    //Model::Draw(hModel_);
}

void FrameBox::Release()
{
}

void FrameBox::Move()
{
}

void FrameBox::RayCast()
{
    positionY_ = transform_.position_.y;
    RayCastData FrameBoxDataDown;
    RayCastData stageDataDown;
    float FrameBoxFling = 2.0f;
    int FrameBoxHModelStart = FrameBoxs_.front();
    int FrameBoxHModelEnd = FrameBoxs_.back();
    int FrameBoxHModelNow = GetModelHandle();
    Stage* pStage = (Stage*)FindObject("Stage");      //ステージオブジェクト
    int stageHModel = pStage->GetModelHandle();         //モデル番号を取得
    if (isJump_)
    {
        //放物線に下がる処理
        positionTempY_ = positionY_;
        positionY_ += (positionY_ - positionPrevY_) - 0.007;
        positionPrevY_ = positionTempY_;
        //if (positionY_ <= -rayFrameBoxDist_ + 0.6)
        //{
        //    isJump_ = false;
        //}
        //if (positionY_ <= -rayStageDistDown_ + 0.6)
        if (rayStageDistDown_ <= FrameBoxFling)
        {
            isJump_ = false;
        }
    }

    if (isBreak_)
    {
        Audio::Play(hSound_, 0.3f);
        this->KillMe();
    }

    //for (int i : vector) {}
    //std::vector<int> FrameBoxSize = ;

    for (int i = 0; i < FrameBoxs_.size(); i++)
    {
        //▼木箱の法線(木箱の上に木箱が乗るため)
        FrameBoxDataDown.start = transform_.position_;
        //FrameBoxDataDown.start.y     = 0;
        FrameBoxDataDown.dir = XMFLOAT3(0, -1, 0);
        if (FrameBoxs_.at(i) != FrameBoxHModelNow)
        {
            int nowData = FrameBoxs_.at(i);
            Model::RayCast(nowData, &FrameBoxDataDown);
        }
        rayFrameBoxDist_ = FrameBoxDataDown.dist;
        if (rayFrameBoxDist_ <= FrameBoxFling)
        {
            if (!isJump_)
            {
                //positionY_ = -FrameBoxDataDown.dist + 0.6;
                isOnFrameBox_ = 1;
                positionTempY_ = positionY_;
                positionPrevY_ = positionTempY_;
                positionY_ = positionPrevY_;
            }
        }
        else
        {
            isOnFrameBox_ = 0;
        }
        //▼ステージの法線(地面に張り付き)
        stageDataDown.start = transform_.position_;             //レイの発射位置
        //stageDataDown.start.y = 0;
        stageDataDown.dir = XMFLOAT3(0, -1, 0);               //レイの方向
        Model::RayCast(stageHModel, &stageDataDown);                //レイを発射
        rayStageDistDown_ = stageDataDown.dist;
        if (rayStageDistDown_ <= FrameBoxFling)
        {
            if (!isJump_ && isOnFrameBox_ == 0)
            {
                //positionY_ += stageDataDown.dist + 0.6;  //地面の張り付き
                positionTempY_ = positionY_;
                positionPrevY_ = positionTempY_;
                positionY_ = positionPrevY_;
            }
        }
        if (isOnFrameBox_ == 0 && rayStageDistDown_ >= FrameBoxFling)
        {
            isJump_ = true;
        }
    }
    transform_.position_.y = positionY_;
    ImGui::Text("rayFrameBoxDist_=%f", rayFrameBoxDist_);
    ImGui::Text("rayStageDistDown_=%f", rayStageDistDown_);
    ImGui::Text("isJump_=%s", isJump_ ? "true" : "false");
    ImGui::Text("positionY_=%f", positionY_);
}

void FrameBox::OnCollision(GameObject* _pTarget)
{

}
