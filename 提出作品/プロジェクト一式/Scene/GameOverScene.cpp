//インクルード
#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Direct3D.h"
#include "../Engine/Image.h"
#include "../Engine/Audio.h"
#include "../Engine/Global.h"
#include "../StageObject/StageObjectManager.h"
#include "../StageObject/SolidText.h"
#include "../Player/AttackPlayer.h"
#include "../Player/CollectPlayer.h"
#include "GameOverScene.h"

GameOverScene::GameOverScene(GameObject* _pParent)
	:GameObject(_pParent, gameOverSceneName), hSound_{ -1 }, soundVolume_{0.1f},inputWait_{ 0 }, inputWaitTime_{ 60 },camPos_{0.0f,0.0f,0.0f}
	,pSolidText_{nullptr}, pStageObjectManager_{nullptr}, pSceneManager_{nullptr}
{

}

void GameOverScene::Initialize()
{
	ShowCursor();
	//サウンドデータのロード
	std::string soundName = soundFolderName + soundGameOverSceneNmae + soundModifierName;
	hSound_ = Audio::Load(soundName);
	assert(hSound_ >= initZeroInt);
	//画像データのロード
	pSolidText_ = Instantiate<SolidText>(this);
	pSolidText_->SetMode(static_cast<int>(TEXTSTATE::GAMEOVER));
	pStageObjectManager_ = new StageObjectManager(this);
	pStageObjectManager_->CreateStageObjectOrigin(STAGEOBJECTSTATE::SKY);
	pSceneManager_ = (SceneManager*)FindObject(sceneManagerName);
	camPos_ = pSolidText_->GetPosition();
	camPos_.y += 2;
	camPos_.z -= 15;
}

void GameOverScene::Update()
{
	Audio::Play(hSound_, soundVolume_);
	Camera::SetPosition(camPos_, attackPlayerNumber);
	Camera::SetTarget(pSolidText_->GetPosition(), attackPlayerNumber);
	Camera::SetPosition(camPos_, collectPlayerNumber);
	Camera::SetTarget(pSolidText_->GetPosition(), collectPlayerNumber);
	++inputWait_;
	if (inputWait_ >= inputWaitTime_)
	{
		if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown((int)MOUSESTATE::LEFTCLICK) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, attackPlayerNumber) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, collectPlayerNumber))
		{
			Direct3D::SetIsChangeView(static_cast<int>(Direct3D::VIEWSTATE::LEFT_BOTHVIEW));
			pSceneManager_->ChangeScene(SCENE_ID_GAMETITLE);
			inputWait_ = initZeroInt;
		}
	}
}

void GameOverScene::Draw()
{
}

void GameOverScene::Release()
{
}
