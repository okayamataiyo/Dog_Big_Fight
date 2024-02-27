#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Engine/Direct3D.h"
#include "SelectScene.h"
#include "Sky.h"
SelectScene::SelectScene(GameObject* _pParent)
	:GameObject(_pParent, "SelectScene")
{

}

void SelectScene::Initialize()
{
	Direct3D::SetIsChangeView(1);
	pText_ = Instantiate<Text>(this);
	pText_->SetMode(1);
	buttonStart_ = Instantiate<Button>(this);
	Instantiate<Sky>(this);
}

void SelectScene::Update()
{
	camPos_ = pText_->GetPosition();
	camPos_.y += 2;
	camPos_.z -= 15;
	Camera::SetPosition(camPos_, 0);
	Camera::SetTarget(pText_->GetPosition(), 0);
	XMFLOAT3 pos = Input::GetMousePosition();
	if (buttonStart_->MouseInArea(pos))
	{

	}
	if (Input::IsKeyDown(DIK_G))
	{
		Direct3D::SetIsChangeView(2);
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
}

void SelectScene::Draw()
{

}

void SelectScene::Release()
{
}
