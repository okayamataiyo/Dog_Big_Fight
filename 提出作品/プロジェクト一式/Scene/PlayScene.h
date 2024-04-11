#pragma once
//インクルード
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
/// 遊べるシーン
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

    //サウンドデータ
    int hSound_[3];
    float length_;
    int random_value_;
    XMFLOAT3 camVec_[2];
    Transform floorPosition_[3];
    //▼骨の処理で使うメンバ変数
    int boneCount_;         //今ある骨の数
    bool isCreateBone_;     //骨を作ったかどうか
    //▼木箱の処理で使うメンバ変数
    int woodBoxCount_;      //今ある木箱の数
    XMFLOAT3 attackPlayerPosition_;         //アタックプレイヤーの位置
    XMVECTOR attackPlayerDirection_;   //アタックプレイヤーの向いてる位置
    float frontPosition_;              //前にどのくらい移動させるか
    //▼演出で使うメンバ変数
    int time_;                         //時間を表す
    int blockOrCollect_;               //邪魔する側か収集する側か

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
    /// コンストラクタ
    /// </summary>
    /// <param name="pParent">親オブジェクト</param>
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

