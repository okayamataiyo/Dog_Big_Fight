#pragma once
//インクルード
#include <string>
#include "../Engine/Direct3D.h"
#include "ObjectBase.h"
#include "../Scene/PlayScene.h"

class AttackPlayer;

//FrameBoxを管理するクラス
class FrameBox : public ObjectBase
{
private:
    int hModel_;            //モデル
    int hSound_;            //サウンドデータ
    bool isBreak_;            //木箱が壊されたか
    GameObject* pParent_;
    PlayScene* pPlayScene_;
    AttackPlayer* pAttackPlayer_;
    std::vector<int> FrameBoxs_;

    //▼レイキャストで使うメンバ変数
    float positionY_;            //木箱のY座標に代入する値
    float positionPrevY_;        //1フレーム前のY座標
    float positionTempY_;        //1時的に座標を保存
    bool isJump_;           //飛んでいるか
    int  isOnFrameBox_;      //木箱の上にいるか
    float rayFrameBoxDist_;  //木箱のディスト
    float rayStageDistDown_;    //ステージのディスト
public:
    //コンストラクタ
    //引数:parent 親オブジェクト(SceneManager)
    FrameBox(GameObject* _pParent);

    //デストラクタ
    ~FrameBox();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    void Move();

    void RayCast();

    void OnCollision(GameObject* _pTarget);

    int GetModelHandle() { return hModel_; }

    XMVECTOR GetVecPos() { return XMLoadFloat3(&transform_.position_); }

    void SetFrameBoxBreak() { isBreak_ = true; }
};