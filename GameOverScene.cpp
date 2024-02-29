#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Engine/Direct3D.h"
#include "GameOverScene.h"
#include "Sky.h"
GameOverScene::GameOverScene(GameObject* _pParent)
	:GameObject(_pParent, "GameOverScene")
{

}

void GameOverScene::Initialize()
{
	pText_ = Instantiate<SolidText>(this);
	pText_->SetMode(0);
	Instantiate<Sky>(this);
}

void GameOverScene::Update()
{
	camPos_ = pText_->GetPosition();
	camPos_.y += 2;
	camPos_.z -= 15;
	Camera::SetPosition(camPos_, 0);
	Camera::SetTarget(pText_->GetPosition(), 0);
	if (Input::IsKeyDown(DIK_B))
	{
		Direct3D::SetIsChangeView(2);
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_SELECT);
	}
}

void GameOverScene::Draw()
{

}

void GameOverScene::Release()
{
}
