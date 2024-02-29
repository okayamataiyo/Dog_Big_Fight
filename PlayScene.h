#pragma once
//�C���N���[�h
#include "Engine/GameObject.h"
#include "Engine/Camera.h"
#include "objectManager.h"
#include <vector>
class Player;
class Sky;
class Floor;
/// <summary>
/// �V�ׂ�V�[��
/// </summary>
class PlayScene : public GameObject
{
private:
    //�����o�ϐ�
    Player* pPlayer_[2];
    Sky* pSky_;
    ObjectManager* pObjectManager;
    XMFLOAT3 camVec_[2];
    XMFLOAT3 rotate_;
    Transform floorPos_[3];
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
    std::vector<int> GetwoodBoxs() { return pObjectManager->GetwoodBoxs(); }
};

