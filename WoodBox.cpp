#include "Engine/Model.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Engine/Fbx.h"
#include "Engine/ImGui/imgui.h"
#include "WoodBox.h"
#include "Stage.h"
#include "ObjectBase.h"
#include "ObjectManager.h"
WoodBox::WoodBox(GameObject* _pParent)
    :ObjectBase(_pParent, "WoodBox"), hModel_(-1)
{
    pParent_ = _pParent;
}

WoodBox::~WoodBox()
{

}

void WoodBox::Initialize()
{
    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("WoodBox.fbx");
    assert(hModel_ >= 0);
    BoxCollider* pCollision = new BoxCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));
    AddCollider(pCollision);
    pPlayScene_ = (PlayScene*)FindObject("PlayScene");
}

void WoodBox::Update()
{
    woodBoxs_ = pPlayScene_->GetWoodBoxs();
    RayCast();
}

void WoodBox::Draw()
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

void WoodBox::Release()
{
}

void WoodBox::Move()
{
}

void WoodBox::RayCast()
{
    transform_.position_.y  = posY_;
    RayCastData woodBoxData;
    RayCastData stageData;
    float woodBoxFling      = 1.0f;
    int startWoodHModel     = woodBoxs_.front();
    int endWoodHModel       = woodBoxs_.back();
    int nowWoodBoxHModel     = GetModelHandle();
    Stage* pStage           = (Stage*)FindObject("Stage");      //�X�e�[�W�I�u�W�F�N�g
    int hStageModel         = pStage->GetModelHandle();         //���f���ԍ����擾
    if (isJump_ == true)
    {
        //�������ɉ����鏈��
        posYTemp_ = posY_;
        posY_ += (posY_ - posYPrev_) - 0.007;
        posYPrev_ = posYTemp_;
        if (posY_ <= -rayWoodBoxDist_ + 0.6)
        {
            isJump_ = false;
        }
        if (posY_ <= -rayStageDist_ + 0.6)
        {
            isJump_ = false;
        }
    }

    //for (int i : vector) {}
    //std::vector<int> woodBoxSize = ;

    for (int i = 0; i < woodBoxs_.size();i++)
    {
        //���ؔ��̖@��(�ؔ��̏�ɖؔ�����邽��)
        woodBoxData.start       = transform_.position_;
        woodBoxData.start.y     = 0;
        woodBoxData.dir         = XMFLOAT3(0, -1, 0);
        if (!(woodBoxs_.at(i) == nowWoodBoxHModel))
        {
            int nowData = woodBoxs_.at(i);
            Model::RayCast(nowData, &woodBoxData);
        }
        rayWoodBoxDist_         = woodBoxData.dist;
        if (rayWoodBoxDist_ + posY_ <= woodBoxFling)
        {
            if (isJump_ == false)
            {
                posY_ = -woodBoxData.dist + 0.6;
                isOnWoodBox_ = 1;
                posYTemp_ = posY_;
                posYPrev_ = posYTemp_;
            }
        }
        else
        {
            isOnWoodBox_ = 0;
        }
        //���X�e�[�W�̖@��(�n�ʂ̒���t��)
        stageData.start = transform_.position_;             //���C�̔��ˈʒu
        stageData.start.y = 0;
        stageData.dir = XMFLOAT3(0, -1, 0);               //���C�̕���
        Model::RayCast(hStageModel, &stageData);                //���C�𔭎�
        rayStageDist_ = stageData.dist;
        if (rayStageDist_ + posY_ <= woodBoxFling)
        {
            if (isJump_ == false && isOnWoodBox_ == 0)
            {
                posY_ = -stageData.dist + 0.6;
                posYTemp_ = posY_;
                posYPrev_ = posYTemp_;
            }
        }
        else if (isOnWoodBox_ == 0)
        {
            isJump_ = true;
        }
    }
    ImGui::Text("rayWoodBoxDist_=%f", rayWoodBoxDist_);
    ImGui::Text("isJum_p=%s", isJump_ ? "true" : "false");
}

void WoodBox::OnCollision(GameObject* _pTarget)
{

}
