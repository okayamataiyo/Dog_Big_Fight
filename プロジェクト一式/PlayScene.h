#pragma once
//インクルード
#include "Engine/GameObject.h"
#include "Engine/Camera.h"
#include "ObjectManager.h"
#include <vector>
class AttackPlayer;
class CollectPlayer;
class Sky;
class Floor;
/// <summary>
/// 遊べるシーン
/// </summary>
class PlayScene : public GameObject
{
private:
    //メンバ変数
    AttackPlayer* pAttackPlayer_;
    CollectPlayer* pCollectPlayer_;
    Sky* pSky_;
    ObjectManager* pObjectManager_;
    XMFLOAT3 camVec_[2];
    Transform floorPos_[3];
    //▼骨の処理で使うメンバ変数
    int boneCount_;         //今ある骨の数
    bool isCreateBone_;     //骨を作ったかどうか
    //▼木箱の処理で使うメンバ変数
    int playerFirstCreatWoodBoxNum_;   //プレイヤー1が何個木箱を作ったか
    int playerSecondsCreatWoodBoxNum_; //プレイヤー2が何個木箱を作ったか
    //▼演出で使うメンバ変数
    int time_;                         //時間を表す
    int blockOrCollect_;               //邪魔する側か収集する側か
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
    std::vector<int> GetWoodBoxs() { return pObjectManager_->GetWoodBoxs(); }
    int GetBlockOrCollect() { return blockOrCollect_; }
    void AddBoneCount(int _boneCount) { boneCount_ += _boneCount; }
    void AddPlayerFirstWoodBoxNum(int _woodBoxNum) { playerFirstCreatWoodBoxNum_ += _woodBoxNum; }
    void AddPlayerSecondsWoodBoxNum(int _woodBoxNum) { playerSecondsCreatWoodBoxNum_ += _woodBoxNum; }
};

