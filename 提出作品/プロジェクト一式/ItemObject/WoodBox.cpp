//�C���N���[�h
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
    //�T�E���h�f�[�^�̃��[�h
    std::string soundName = soundFolderName + soundWoodBoxName + soundModifierName;
    hSound_ = Audio::Load(soundName);
    assert(hSound_ >= initZeroInt);
    //���f���f�[�^�̃��[�h
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
    Stage* pStage           = (Stage*)FindObject(stageName);      //�X�e�[�W�I�u�W�F�N�g
    int stageHModel         = pStage->GetModelHandle();         //���f���ԍ����擾
    if (isJump_)
    {
        //�������ɉ����鏈��
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
        ////���ؔ��̖@��(�ؔ��̏�ɖؔ�����邽��)
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
        //���X�e�[�W�̖@��(�n�ʂɒ���t��)
        stageDataDown.start = transform_.position_;             //���C�̔��ˈʒu
        //stageDataDown.start.y = 0;
        stageDataDown.dir = XMFLOAT3(0, -1, 0);               //���C�̕���
        Model::RayCast(stageHModel, &stageDataDown);                //���C�𔭎�
        rayStageDistDown_ = stageDataDown.dist;
        if (rayStageDistDown_ <= woodBoxFling)
        {
            if (!isJump_ && isOnWoodBox_ == 0)
            {
                //positionY_ += stageDataDown.dist + 0.6;  //�n�ʂ̒���t��
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
