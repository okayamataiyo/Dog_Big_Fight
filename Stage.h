#pragma once
//インクルード
#include <string>
#include "Engine/GameObject.h"
#include "Engine/Direct3D.h"

using std::string;
//Stageを管理するクラス
class Stage : public GameObject
{
    int hModel_[2];    //モデル番号

    Transform FlyFloor;
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

    int GetModelHandle(int _type) { return hModel_[_type]; }
};