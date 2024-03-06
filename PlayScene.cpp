#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Engine/ImGui/imgui.h"
#include "PlayScene.h"
#include "Player.h"
#include "Sky.h"
#include "Floor.h"
#include "WoodBox.h"
#include "Stage.h"
#include "Bone.h"

PlayScene::PlayScene(GameObject* _pParent)
	:GameObject(_pParent, "PlayScene"), pObjectManager_(nullptr),pSky_(nullptr)
	,boneCount_(0),isCreateBone_(false),playerFirstCreatWoodBoxNum_(0),playerSecondsCreatWoodBoxNum_(0),blockOrCollect_(0)
{

}

void PlayScene::Initialize()
{
	pObjectManager_ = new ObjectManager(this);
	pSky_ = Instantiate<Sky>(this);
	pSky_->SetObjectName("SkyFirst");
	XMFLOAT3 firstPPos = { -3,0,0 };
	XMFLOAT3 secondsPPos = { 3,0,0 };
	Instantiate<Stage>(this);
	floorPos_[0].position_ = { 30.0f,0.8f,3.0f };
	floorPos_[1].position_ = { 6.0f,0.5f,20.0f };
	floorPos_[2].position_ = { 25.0f, -0.3f,-20.0f };
	XMFLOAT3 scale		   = { 3.0f,1.0f,3.0f };
	XMFLOAT3 DefaultData[2] = { XMFLOAT3(0.0f,0.0f,0.0f)	//0Ç≈èâä˙âªw
							   ,XMFLOAT3(1.0f,1.0f,1.0f) };	//1Ç≈èâä˙âª
	XMFLOAT3 WoodBox[3];
	WoodBox[0]			   = { 30.0f,-5.0f,40.0f };
	WoodBox[1]			   = { 20.0f,-10.0f,40.0f };
	WoodBox[2]			   = { 10.0f,-20.0f,40.0f };
	//for (int i = 0u; i <= 1; i++)
	//{
	//	pObjectManager_->CreateObject(OBJECTSTATE::FLOOR, floorPos_[i].position_, XMFLOAT3(0.0f,90.0f,0.0f), XMFLOAT3(4.0f,1.0f,4.0f));
	//}
	pObjectManager_->CreateObject(OBJECTSTATE::FLOOR, floorPos_[2].position_, XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(10.0f, 1.0f, 10.0f));

	//for (int i = 0u; i <= 2; i++)
	//{
	//	pObjectManager_->CreateObject(OBJECTSTATE::WOODBOX, WoodBox[i], DefaultData[0], XMFLOAT3(0.3f, 0.3f, 0.3f));
	//}

	for (int i = 0u; i <= 1; i++)
	{
		pPlayer_[i] = Instantiate<Player>(this);
		camVec_[i].x = 0;
		camVec_[i].y = 5;
		camVec_[i].z = -10;
	}
	pPlayer_[0]->SetObjectName("PlayerFirst");
	pPlayer_[1]->SetObjectName("PlayerSeconds");
	pPlayer_[0]->SetPosition(firstPPos);
	pPlayer_[1]->SetPosition(secondsPPos);
}

void PlayScene::Update()
{
	time_++;
	if (time_ % 1800 == 0)
	{
		if (time_ % 3600 != 0)
		{
			blockOrCollect_ = 1;
		}
	}
	if (time_ % 3600 == 0)
	{
		blockOrCollect_ = 0;
	}
	if (boneCount_ == 0)
	{
		isCreateBone_ = true;
	}
	if (boneCount_ == 3)
	{
		isCreateBone_ = false;
	}

	if (isCreateBone_ == true)
	{
		for (int i = 0u; i <= 2; i++)
		{
			pObjectManager_->CreateObject(OBJECTSTATE::BONE, -140, 140, -140, 140);
			boneCount_ += 1;
		}
	}
	if (blockOrCollect_ == 1)
	{
		if (playerSecondsCreatWoodBoxNum_ < 5)
		{
			if (Input::IsMouseButtonDown(1))
			{
				pObjectManager_->CreateObject(OBJECTSTATE::WOODBOX, pPlayer_[1]->GetPosition(), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f));
				playerSecondsCreatWoodBoxNum_ += 1;
			}
		}
	}
	if (blockOrCollect_ == 0)
	{
		if (playerFirstCreatWoodBoxNum_ < 5)
		{
			if (Input::IsPadButtonDown(XINPUT_GAMEPAD_Y))
			{
				pObjectManager_->CreateObject(OBJECTSTATE::WOODBOX, pPlayer_[0]->GetPosition(), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f));
				playerFirstCreatWoodBoxNum_ += 1;
			}
		}
	}
	for (int i = 0u; i <= 1; i++)
	{
		XMFLOAT3 Init				= { 0.0f,0.0f,0.0f };
		XMVECTOR vPos[2]			= {};
		XMFLOAT3 mouse				= Init;
		XMFLOAT3 controller			= {};
		XMFLOAT3 rDir				= { 0.0f,0.0f,1.0f };
		XMVECTOR Dir[2]				= {};
		float sigmaRotY[2]			= {};
		float sigmaRotX[2]			= {};
		XMMATRIX mxRotX[2]			= {};
		XMMATRIX mxRotY[2]			= {};
		XMMATRIX rot[2]				= {};
		XMFLOAT3 playerPos[2]		= {};
		XMFLOAT3 floatDir[2]		= {};
		vPos[i]						= pPlayer_[i]->GetVecPos();
		mouse						= Input::GetMouseMove();
		controller					= Input::GetPadStickR();

		const float mouseSens		= 400;
		const float controllerSens	= 50;
		static float RotationX[2]	= {};
		static float RotationY[2]	= {};
		static float vecLength[2]	= {};
		static float prevLen[2]		= {};
		static float prevRotX[2]	= {};

		RotationX[0]				= controller.x;
		RotationY[0]				= -controller.y;
		RotationX[1]				= mouse.x;
		RotationY[1]				= mouse.y;
		vecLength[1]			   -= (mouse.z) / 50;

		Dir[i] = XMLoadFloat3(&rDir);

		//Dir = Dir * (pPlayer_[i]->GetRotate().x + RotationX[i]) * (pPlayer_[i]->GetRotate().y + RotationY[i]);
		//Dir = Dir + (vecLength * 2);
		camVec_[0].x			   += RotationY[0] / controllerSens;
		camVec_[0].y			   += RotationX[0] / controllerSens;
		camVec_[1].x			   += RotationY[1] / mouseSens;
		camVec_[1].y			   += RotationX[1] / mouseSens;

		sigmaRotY[i]				= camVec_[i].y;// +pPlayer_[i]->GetRotate().y;
		sigmaRotX[i]				= -camVec_[i].x;// + EasingX[i]; +pPlayer_[i]->GetRotate().x;

		if (sigmaRotX[i] > 0 * (3.14 / 180))
		{
			sigmaRotX[i]			= 0;
			camVec_[1].x		   -= RotationY[1] / mouseSens;
			camVec_[0].x		   -= RotationY[0] / controllerSens;
		}
		if (sigmaRotX[i] < -88 * (3.14 / 180))
		{
			sigmaRotX[i] = -87.9 * (3.14 / 180);
			camVec_[1].x		   -= RotationY[1] / mouseSens;
			camVec_[0].x		   -= RotationY[0] / controllerSens;
		}

		prevRotX[i] = sigmaRotX[i];
		mxRotX[i] = XMMatrixRotationX(sigmaRotX[i]);
		mxRotY[i] = XMMatrixRotationY(sigmaRotY[i]);

		rot[i] = mxRotX[i] * mxRotY[i];
		playerPos[i] = pPlayer_[i]->GetPosition();

		Dir[i] = XMVector3Transform(Dir[i], rot[i]);
		Dir[i] = XMVector3Normalize(Dir[i]);
		Dir[i] = Dir[i] * (vecLength[i] + 10);
		Dir[i] += XMLoadFloat3(&playerPos[i]);
		//camVec_ = pPlayer_[j]->GetPosition();
		//camVec_.y += 5;
		//camVec_.z += -10;
		//auto a = pPlayer_[j]->GetPosition();

		//pCamera_->SetPosition(Dir, i);
		//pCamera_->SetTarget(pPlayer_[i]->GetPosition(), i)
		XMStoreFloat3(&floatDir[i], Dir[i]);
		Camera::SetPosition(floatDir[i], i);
		Camera::SetTarget(pPlayer_[i]->GetPosition(), i);

		prevLen[i] = vecLength[i];
	}
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
	SAFE_DELETE(pObjectManager_);
}
