#pragma once
//インクルード
#include "Engine/GameObject.h"
#include "Engine/Camera.h"
#include "objectManager.h"
#include <vector>
class Player;
class Sky;
class Floor;
/// <summary>
/// 遊べるシーン
/// </summary>
class PlayScene : public GameObject
{
private:
    //メンバ変数
    Player* pPlayer_[2];
    Sky* pSky_;
    ObjectManager* pObjectManager;
    XMFLOAT3 camVec_[2];
    XMFLOAT3 rotate_;
    Transform floorPos_[3];
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="pParent">親オブジェクト</param>
    PlayScene(GameObject* _pParent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    std::vector<int> GetwoodBoxs() { return pObjectManager->GetwoodBoxs(); }
};

