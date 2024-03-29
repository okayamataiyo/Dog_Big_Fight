#include "../Engine/Model.h"
#include "../Engine/Camera.h"
#include "../Engine/Input.h"
#include "../Engine/Fbx.h"
#include "../Engine/ImGui/imgui.h"
#include "../Engine/Audio.h"
#include "WoodBox.h"
#include "ObjectManager.h"
#include "../StageObject/Stage.h"
#include "../Player/AttackPlayer.h"

WoodBox::WoodBox(GameObject* _pParent)
    :ObjectBase(_pParent, woodBoxName), hModel_(-1), hSound_{ -1 },isOnWoodBox_(0)
    ,pAttackPlayer_(nullptr)
{
    pParent_ = _pParent;
}

WoodBox::~WoodBox()
{ 

}

void WoodBox::Initialize()
{
    //�T�E���h�f�[�^�̃��[�h
    hSound_ = Audio::Load("Sound/WoodBoxBreak.wav");
    assert(hSound_ >= 0);
    //���f���f�[�^�̃��[�h
    std::string ModelName = (std::string)"Model&Picture/" + woodBoxName + (std::string)".fbx";
    hModel_ = Model::Load(ModelName);
    assert(hModel_ >= 0);
    BoxCollider* pCollision = new BoxCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(5.0f, 5.0f, 5.0f));
    //SphereCollider* pCollision = new SphereCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), 1.5);
    AddCollider(pCollision);
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
    //for (int i = 0u; i <= 1; i++)
    {
        //Direct3D::SetShader(Direct3D::SHADER_NORMALMAP);
        Model::SetTransform(hModel_, transform_);
        //�ŏ���3D�ŕ`���A�g�Â����`��
        /*for (int j = 0; j <= 4; j += 4)
        {
            Direct3D::SetShader(static_cast<Direct3D::SHADER_TYPE>(j));
            Model::Draw(hModel_[i]);
    }   */
        Model::Draw(hModel_);
        //Direct3D::SetShader(Direct3D::SHADER_3D);
    }
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
    Stage* pStage           = (Stage*)FindObject("Stage");      //�X�e�[�W�I�u�W�F�N�g
    int stageHModel         = pStage->GetModelHandle();         //���f���ԍ����擾
    if (isJump_)
    {
        //�������ɉ����鏈��
        positionTempY_ = positionY_;
        positionY_ += (positionY_ - positionPrevY_) - 0.007;
        positionPrevY_ = positionTempY_;
        //if (positionY_ <= -rayWoodBoxDist_ + 0.6)
        //{
        //    isJump_ = false;
        //}
        //if (positionY_ <= -rayStageDistDown_ + 0.6)
        if(rayStageDistDown_ <= woodBoxFling)
        {
            isJump_ = false;
        }
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
