#pragma once
//インクルード
#include "../Engine/GameObject.h"
#include "../Engine/Camera.h"

class SolidText;
class StageObjectManager;

class GameOverScene : public GameObject
{
	SolidText* pText_;
	StageObjectManager* pStageObjectManager_;
	XMFLOAT3 camPos_;
public:
	GameOverScene(GameObject* _pParent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

