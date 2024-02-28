#pragma once
//インクルード
#include <string>
#include "Engine/Direct3D.h"
#include "ObjectBase.h"

using std::string;
//WoodBoxを管理するクラス
class WoodBox : public ObjectBase
{
    int hModel_;            //モデル番号
    float rayWoodBoxDist_;  //木箱のディスト
    float rayStageDist_;    //ステージのディスト
public:
    //コンストラクタ
    //引数:parent 親オブジェクト(SceneManager)
    WoodBox(GameObject* _parent);

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

    void OnCollision(GameObject* _pTarget);

    int GetModelHandle() { return hModel_; }

    XMVECTOR GetVecPos() { return XMLoadFloat3(&transform_.position_); }
};