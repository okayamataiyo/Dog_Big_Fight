#pragma once
//インクルード
#include <string>
#include "../Engine/Direct3D.h"
#include "ObjectBase.h"
#include "../Scene/PlayScene.h"
//WoodBoxを管理するクラス
class WoodBox : public ObjectBase
{
private:
    int hModel_;            //モデル
    GameObject* pParent_;
    PlayScene* pPlayScene_;
    std::vector<int> woodBoxs_;

    //▼レイキャストで使うメンバ変数
    float posY_;            //木箱のY座標に代入する値
    float posYPrev_;        //1フレーム前のY座標
    float posYTemp_;        //1時的に座標を保存
    bool isJump_;           //飛んでいるか
    int  isOnWoodBox_;      //木箱の上にいるか
    float rayWoodBoxDist_;  //木箱のディスト
    float rayStageDist_;    //ステージのディスト
public:
    //コンストラクタ
    //引数:parent 親オブジェクト(SceneManager)
    WoodBox(GameObject* _pParent);

    //デストラクタ
    ~WoodBox();

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
};