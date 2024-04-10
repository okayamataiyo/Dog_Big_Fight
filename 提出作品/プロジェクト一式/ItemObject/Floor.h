#pragma once
//�C���N���[�h
#include <string>
#include "../Engine/Direct3D.h"
#include "ItemObjectBase.h"

namespace
{
    std::string floorName = "Floor";
}

//Floor���Ǘ�����N���X
class Floor : public ItemObjectBase
{
    int hModel_;    //���f���ԍ�
    int upOrDown_;  //�オ���Ă邩�������Ă邩
public:
    //�R���X�g���N�^
    //����:parent �e�I�u�W�F�N�g(SceneManager)
    Floor(GameObject* _parent);

    //�f�X�g���N�^
    ~Floor();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    int GetModelHandle() override { return hModel_; }
};