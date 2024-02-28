#pragma once
//�C���N���[�h
#include <string>
#include "Engine/Direct3D.h"
#include "ObjectBase.h"

using std::string;
//WoodBox���Ǘ�����N���X
class WoodBox : public ObjectBase
{
    int hModel_;            //���f���ԍ�
    float rayWoodBoxDist_;  //�ؔ��̃f�B�X�g
    float rayStageDist_;    //�X�e�[�W�̃f�B�X�g
public:
    //�R���X�g���N�^
    //����:parent �e�I�u�W�F�N�g(SceneManager)
    WoodBox(GameObject* _parent);

    //�f�X�g���N�^
    ~WoodBox();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    void OnCollision(GameObject* _pTarget);

    int GetModelHandle() { return hModel_; }

    XMVECTOR GetVecPos() { return XMLoadFloat3(&transform_.position_); }
};