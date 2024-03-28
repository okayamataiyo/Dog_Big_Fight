#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Direct3D.h"
#include "../Engine/Image.h"
#include "../Engine/Audio.h"
#include "GameOverScene.h"
#include "../StageObject/StageObjectManager.h"
#include "../StageObject/SolidText.h"

GameOverScene::GameOverScene(GameObject* _pParent)
	:GameObject(_pParent, "GameOverScene"),pStageObjectManager_(nullptr),hSound_(-1)
{

}

void GameOverScene::Initialize()
{
	ShowCursor();
	//�T�E���h�f�[�^�̃��[�h
	hSound_ = Audio::Load("Sound/GameOverBGM.wav");
	assert(hSound_ >= 0);
	//�摜�f�[�^�̃��[�h
	pText_ = Instantiate<SolidText>(this);
	pText_->SetMode(0);
	pStageObjectManager_ = new StageObjectManager(this);
	pStageObjectManager_->CreateStageObjectOrigin(STAGEOBJECTSTATE::Sky);
}

void GameOverScene::Update()
{
	Audio::Play(hSound_, 0.1f);
	camPos_ = pText_->GetPosition();
	camPos_.y += 2;
	camPos_.z -= 15;
	Camera::SetPosition(camPos_, 0);
	Camera::SetTarget(pText_->GetPosition(), 0);
	if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown(0)|| Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		Direct3D::SetIsChangeView(2);
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_GAMETITLE);
	}
}

void GameOverScene::Draw()
{
}

void GameOverScene::Release()
{
}
