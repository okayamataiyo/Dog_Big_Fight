#pragma once
//�C���N���[�h
#include "Engine/GameObject.h"
#include "Engine/Camera.h"
#include "Player.h"
#include "Sky.h"
/// <summary>
/// �V�ׂ�V�[��
/// </summary>
class PlayScene : public GameObject
{
private:
    //�����o�ϐ�
    Player* pPlayer_[2];
    Sky* pSky_;
    //Camera* pCamera_;

    XMFLOAT3 camVec_[2];
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
};

