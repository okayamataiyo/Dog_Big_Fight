#pragma once
//インクルード
#include <string>
#include "Engine/GameObject.h"
#include "Engine/Direct3D.h"

using std::string;
//Stageを管理するクラス
class Stage : public GameObject
{
    int hModel_;    //モデル番号
public:
    //コンストラクタ
    //引数:parent 親オブジェクト(SceneManager)
    Stage(GameObject* _parent);

    //デストラクタ
    ~Stage();

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