#pragma once
//�C���N���[�h
#include <string>
#include "Engine/Direct3D.h"
#include "ObjectBase.h"

using std::string;
//Floor���Ǘ�����N���X
class Floor : public ObjectBase
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

    int GetModelHandle() { return hModel_; }
};