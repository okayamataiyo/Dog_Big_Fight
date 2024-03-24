#pragma once
//インクルード
#include "../Engine/GameObject.h"
#include "../Engine/Camera.h"

class SolidText;
class Stage;

class GameTitleScene : public GameObject
{
	SolidText* pText_;
	Stage* pStage_;
	XMFLOAT3 camPos_;
public:
	GameTitleScene(GameObject* _pParent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

