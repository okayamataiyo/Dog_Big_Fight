#pragma once
//�C���N���[�h
#include <string>
#include "../Engine/Direct3D.h"
#include "ObjectBase.h"
#include "../Scene/PlayScene.h"

class AttackPlayer;

//FrameBox���Ǘ�����N���X
class FrameBox : public ObjectBase
{
private:
    int hModel_;            //���f��
    int hSound_;            //�T�E���h�f�[�^
    bool isBreak_;            //�ؔ����󂳂ꂽ��
    GameObject* pParent_;
    PlayScene* pPlayScene_;
    AttackPlayer* pAttackPlayer_;
    std::vector<int> FrameBoxs_;

    //�����C�L���X�g�Ŏg�������o�ϐ�
    float positionY_;            //�ؔ���Y���W�ɑ������l
    float positionPrevY_;        //1�t���[���O��Y���W
    float positionTempY_;        //1���I�ɍ��W��ۑ�
    bool isJump_;           //���ł��邩
    int  isOnFrameBox_;      //�ؔ��̏�ɂ��邩
    float rayFrameBoxDist_;  //�ؔ��̃f�B�X�g
    float rayStageDistDown_;    //�X�e�[�W�̃f�B�X�g
public:
    //�R���X�g���N�^
    //����:parent �e�I�u�W�F�N�g(SceneManager)
    FrameBox(GameObject* _pParent);

    //�f�X�g���N�^
    ~FrameBox();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    void Move();

    void RayCast();

    void OnCollision(GameObject* _pTarget);

    int GetModelHandle() { return hModel_; }

    XMVECTOR GetVecPos() { return XMLoadFloat3(&transform_.position_); }

    void SetFrameBoxBreak() { isBreak_ = true; }
};