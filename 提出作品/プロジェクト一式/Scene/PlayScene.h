#pragma once
//�C���N���[�h
#include <vector>
#include "../Engine/GameObject.h"
#include "../Engine/Camera.h"
#include "../ItemObject/ItemObjectManager.h"

namespace
{
    std::string soundPlaySceneNames[] =
    {
        "BGM",
        "LastBGM",
        "LastBGM2",
    };

    std::string playSceneName = "PlayScene";
}

class SceneManager;
class AttackPlayer;
class CollectPlayer;
class ItemObjectManager;
class StageObjectManager;

/// <summary>
/// �V�ׂ�V�[��
/// </summary>
class PlayScene : public GameObject
{
private:

    enum class SOUNDSTATE
    {
        BGM = 0,
        LASTBGM,
        LASTBGM2,
    };

    enum class PADIDSTATE
    {
        FIRST = 0,
        SECONDS,
        THIRD,
        FOUR,
    };

    //�T�E���h�f�[�^
    int hSound_[3];
    float length_;
    int random_value_;
    XMFLOAT3 camVec_[2];
    Transform floorPosition_[3];
    //�����̏����Ŏg�������o�ϐ�
    int boneCount_;         //�����鍜�̐�
    bool isCreateBone_;     //������������ǂ���
    //���ؔ��̏����Ŏg�������o�ϐ�
    int woodBoxCount_;      //������ؔ��̐�
    XMFLOAT3 attackPlayerPosition_;         //�A�^�b�N�v���C���[�̈ʒu
    XMVECTOR attackPlayerDirection_;   //�A�^�b�N�v���C���[�̌����Ă�ʒu
    float frontPosition_;              //�O�ɂǂ̂��炢�ړ������邩
    //�����o�Ŏg�������o�ϐ�
    int time_;                         //���Ԃ�\��
    int blockOrCollect_;               //�ז����鑤�����W���鑤��

    XMFLOAT3 Init = { 0.0f,0.0f,0.0f };
    XMVECTOR vPos[2] = {};
    XMFLOAT3 mouse = Init;
    XMFLOAT3 controller[2] = {};
    XMFLOAT3 rDir = { 0.0f,0.0f,1.0f };
    XMVECTOR Dir[2] = {};
    float sigmaRotY[2] = {};
    float sigmaRotX[2] = {};
    XMMATRIX mxRotX[2] = {};
    XMMATRIX mxRotY[2] = {};
    XMMATRIX rot[2] = {};
    XMFLOAT3 playerPos[2] = {};
    XMFLOAT3 floatDir[2] = {};

    const float mouseSens = 400;
    const float controllerSens = 50;

    SceneManager* pSceneManager_;
    AttackPlayer* pAttackPlayer_;
    CollectPlayer* pCollectPlayer_;
    ItemObjectManager* pItemObjectManager_;
    StageObjectManager* pStageObjectManager_;
public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="pParent">�e�I�u�W�F�N�g</param>
    PlayScene(GameObject* _pParent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    std::vector<int> GetWoodBoxs() { return pItemObjectManager_->GetWoodBoxs(); }
    int GetBlockOrCollect() { return blockOrCollect_; }
    XMFLOAT3 GetAttackPlayerPosition() { return attackPlayerPosition_; }
    void AddBoneCount(int _boneCount) { boneCount_ += _boneCount; }
    void AddWoodBoxCount(int _woodBoxCount) { woodBoxCount_ += _woodBoxCount; }

    void HideCursor() { while (::ShowCursor(false) >= 0); }
};

