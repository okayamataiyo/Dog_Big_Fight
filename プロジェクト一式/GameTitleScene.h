#pragma once
//インクルード
#include "Engine/GameObject.h"
#include "Engine/Camera.h"
#include "SolidText.h"
class GameTitleScene : public GameObject
{
	SolidText* pText_;
	XMFLOAT3 camPos_;
public:
	GameTitleScene(GameObject* _pParent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

