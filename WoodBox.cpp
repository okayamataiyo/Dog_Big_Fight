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
    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("WoodBox.fbx");
    assert(hModel_ >= 0);

    BoxCollider* pCollision = new BoxCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));
    AddCollider(pCollision);
}

void WoodBox::Update()
{
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
    transform_.position_.y = posY_;
    RayCastData woodBoxData;
    RayCastData stageData;
    float woodBoxFling = 1.0f;
    WoodBox* pWoodBox = (WoodBox*)FindObject("WoodBox");
    int hWoodBoxModel = pWoodBox->GetModelHandle();
    Stage* pStage = (Stage*)FindObject("Stage");      //�X�e�[�W�I�u�W�F�N�g
    int hStageModel = pStage->GetModelHandle();         //���f���ԍ����擾
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
    int startWoodHModel = 6;
    int endWoodHModel = 0;

    //for (int i : vector) {}

    for (int i = 0; i <= vectorSize.size(); i++)
    {
        //���ؔ��̖@��(�ؔ��̏�ɖؔ�����邽��)
        woodBoxData.start       = transform_.position_;
        woodBoxData.start.y     = 0;
        woodBoxData.dir         = XMFLOAT3(0, -1, 0);
        Model::RayCast(hWoodBoxModel + i, &woodBoxData);
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
