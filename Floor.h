#pragma once
//インクルード
#include <string>
#include "Engine/Direct3D.h"
#include "ObjectBase.h"

using std::string;
//Floorを管理するクラス
class Floor : public ObjectBase
{
    int hModel_;    //モデル番号
    int upOrDown_;  //上がってるか下がってるか
public:
    //コンストラクタ
    //引数:parent 親オブジェクト(SceneManager)
    Floor(GameObject* _parent);

    //デストラクタ
    ~Floor();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    int GetModelHandle() { return hModel_; }
};