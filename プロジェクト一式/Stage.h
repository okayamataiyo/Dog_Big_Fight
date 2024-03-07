#pragma once
//�C���N���[�h
#include <string>
#include "Engine/GameObject.h"
#include "Engine/Direct3D.h"

using std::string;
//Stage���Ǘ�����N���X
class Stage : public GameObject
{
    int hModel_;    //���f���ԍ�
public:
    //�R���X�g���N�^
    //����:parent �e�I�u�W�F�N�g(SceneManager)
    Stage(GameObject* _parent);

    //�f�X�g���N�^
    ~Stage();

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