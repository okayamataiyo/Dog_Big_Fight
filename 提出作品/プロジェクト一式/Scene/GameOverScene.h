#pragma once
//インクルード
#include "../Engine/GameObject.h"
#include "../Engine/Camera.h"

class SolidText;
class StageObjectManager;
class SceneManager;

namespace
{
	std::string soundGameOverSceneNmae = "GameOverBGM";
	std::string gameOverSceneName = "GameOverScene";
}

class GameOverScene : public GameObject
{
	int hSound_;
	int inputWait_;
	int inputWaitTime_;
	XMFLOAT3 camPos_;
	SolidText* pText_;
	StageObjectManager* pStageObjectManager_;
	SceneManager* pSceneManager_;
public:
	GameOverScene(GameObject* _pParent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void ShowCursor() { while (::ShowCursor(true) < 0); }
};

