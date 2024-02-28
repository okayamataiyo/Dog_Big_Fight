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

    BoxCollider* pCollision = new BoxCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f));
    AddCollider(pCollision);
}

void WoodBox::Update()
{
    for (int i = 0u; i <= 1; i++)
    {

        RayCastData woodBoxData;
        WoodBox* pWoodBox = (WoodBox*)FindObject("WoodBox");
        int hWoodBoxModel = pWoodBox->GetModelHandle();
        woodBoxData.start = transform_.position_;
        woodBoxData.start.y = 0;
        woodBoxData.dir = XMFLOAT3(0, -1, 0);
        Model::RayCast(hWoodBoxModel + i, &woodBoxData);
        rayWoodBoxDist_ = woodBoxData.dist;
        ImGui::Text("rayWoodBoxDist_=%f", rayWoodBoxDist_);
        if (woodBoxData.hit == true)
        {
            transform_.position_.y = -woodBoxData.dist + 0.6;
        }
        RayCastData stageData;
        Stage* pStage = (Stage*)FindObject("Stage");      //�X�e�[�W�I�u�W�F�N�g
        int hStageModel = pStage->GetModelHandle();         //���f���ԍ����擾
        stageData.start = transform_.position_;             //���C�̔��ˈʒu
        stageData.start.y = 0;
        stageData.dir = XMFLOAT3(0, -1, 0);               //���C�̕���
        Model::RayCast(hStageModel, &stageData);                //���C�𔭎�
        rayStageDist_ = stageData.dist;
        if (stageData.hit == true && !(woodBoxData.hit == true))
        {
            transform_.position_.y = -stageData.dist + 0.6;
        }
    }
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

void WoodBox::OnCollision(GameObject* _pTarget)
{

}
