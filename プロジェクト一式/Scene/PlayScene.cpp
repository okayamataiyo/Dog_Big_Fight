#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"
#include "../Engine/ImGui/imgui.h"
#include "PlayScene.h"
#include "../Player/AttackPlayer.h"
#include "../Player/CollectPlayer.h"
#include "../Object/Floor.h"
#include "../Object/WoodBox.h"
#include "../Object/FrameBox.h"
#include "../Object/Bone.h"
#include "../StageObject/Stage.h"
#include "../StageObject/Sky.h"

PlayScene::PlayScene(GameObject* _pParent)
	:GameObject(_pParent, "PlayScene"), boneCount_(0), isCreateBone_(false), woodBoxCount_(0)
	, attackPlayerPosition_{}, attackPlayerDirection_{},frontPosition_(10.0f), blockOrCollect_(0)
	,pObjectManager_(nullptr), pSky_(nullptr)
{

}

void PlayScene::Initialize()
{
	pObjectManager_ = new ObjectManager(this);
	pSky_ = Instantiate<Sky>(this);
	XMFLOAT3 firstPPos = { -3,0,0 };
	XMFLOAT3 secondsPPos = { 3,0,0 };
	Instantiate<Stage>(this);
	floorPosition_[0].position_ = { 30.0f,0.8f,3.0f };
	floorPosition_[1].position_ = { 6.0f,0.5f,20.0f };
	floorPosition_[2].position_ = { -45.0f, 0.3f,-45.0f };
	XMFLOAT3 scale		   = { 3.0f,1.0f,3.0f };
	XMFLOAT3 DefaultData[2] = { XMFLOAT3(0.0f,0.0f,0.0f)	//0Ç≈èâä˙âª
							   ,XMFLOAT3(1.0f,1.0f,1.0f) };	//1Ç≈èâä˙âª
	XMFLOAT3 WoodBox[3]		= {XMFLOAT3(30.0f,-5.0f,40.0f)
							  ,XMFLOAT3(20.0f,-10.0f,40.0f)
							  ,XMFLOAT3(10.0f,-20.0f,40.0f) };
	XMFLOAT3 FrameBox = { XMFLOAT3(5.0f,5.0f,5.0f) };
	//for (int i = 0u; i <= 1; i++)
	//{
	//	pObjectManager_->CreateObject(OBJECTSTATE::FLOOR, floorPosition_[i].position_, XMFLOAT3(0.0f,90.0f,0.0f), XMFLOAT3(4.0f,1.0f,4.0f));
	//}
	pObjectManager_->CreateObject(OBJECTSTATE::FLOOR, floorPosition_[2].position_, XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(10.0f, 1.0f, 10.0f));

	//for (int i = 0u; i <= 2; i++)
	//{
	//	pObjectManager_->CreateObject(OBJECTSTATE::WOODBOX, WoodBox[i], DefaultData[0], XMFLOAT3(0.3f, 0.3f, 0.3f));
	//}
	pAttackPlayer_ = Instantiate<AttackPlayer>(this);
	camVec_[0].x = 0;
	camVec_[0].y = 5;
	camVec_[0].z = -10;
	pCollectPlayer_ = Instantiate<CollectPlayer>(this);
	camVec_[1].x = 0;
	camVec_[1].y = 5;
	camVec_[1].z = -10;
	pObjectManager_->CreateObject(OBJECTSTATE::FRAMEBOX,DefaultData[0], DefaultData[1], FrameBox);
	pAttackPlayer_->SetCollectPlayer(pCollectPlayer_);
	pCollectPlayer_->SetAttackPlayer(pAttackPlayer_);
	pAttackPlayer_->SetPosition(firstPPos);
	pCollectPlayer_->SetPosition(secondsPPos);
}

void PlayScene::Update()
{
	attackPlayerPosition_ = pAttackPlayer_->GetPosition();
	attackPlayerDirection_ = XMLoadFloat3(&attackPlayerPosition_) - Camera::VecGetPosition(1);
	attackPlayerDirection_ = XMVectorSetY(attackPlayerDirection_, 0);
	attackPlayerDirection_ = XMVector3Normalize(attackPlayerDirection_);
	attackPlayerPosition_.x = attackPlayerPosition_.x + frontPosition_ * XMVectorGetX(attackPlayerDirection_);
	attackPlayerPosition_.z = attackPlayerPosition_.z + frontPosition_ * XMVectorGetZ(attackPlayerDirection_);
	//Input::SetMousePosition(600, 600);
	//SetCursorPos(600, 600);
	if (boneCount_ == 0)
	{
		isCreateBone_ = true;
	}
	if (boneCount_ == 3)
	{
		isCreateBone_ = false;
	}

	if (isCreateBone_)
	{
		for (int i = 0u; i <= 2u; i++)
		{
			pObjectManager_->CreateObject(OBJECTSTATE::BONE, -100, 100, -100, 100);
			boneCount_ += 1;
		}
	}
	if (woodBoxCount_ <= 5)
	{
		if (Input::IsPadButtonDown(XINPUT_GAMEPAD_Y))
		{
			pObjectManager_->CreateObject(OBJECTSTATE::WOODBOX, attackPlayerPosition_, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 0.5f));
			woodBoxCount_ += 1;
		}
	}

	static float RotationX[2] = {};
	static float RotationY[2] = {};
	static float vecLength[2] = {};
	static float prevLen[2] = {};
	static float prevRotX[2] = {};

	mouse = Input::GetMouseMove();
	controller = Input::GetPadStickR();
	RotationX[0] = controller.x;
	RotationY[0] = -controller.y;
	RotationX[1] = mouse.x;
	RotationY[1] = mouse.y;
	vecLength[0] -= (mouse.z) / 50;

	vPos[0] = pCollectPlayer_->GetVecPos();
	vPos[1] = pAttackPlayer_->GetVecPos();
	playerPos[0] = pCollectPlayer_->GetPosition();
	playerPos[1] = pAttackPlayer_->GetPosition();

	for (int i = 0u; i <= 1u; i++)
	{

		Dir[i] = XMLoadFloat3(&rDir);

		//Dir = Dir * (pPlayer_[i]->GetRotate().x + RotationX[i]) * (pPlayer_[i]->GetRotate().y + RotationY[i]);
		//Dir = Dir + (vecLength * 2);

		camVec_[1].x += RotationY[0] / controllerSens;
		camVec_[1].y += RotationX[0] / controllerSens;
		camVec_[0].x += RotationY[1] / mouseSens;
		camVec_[0].y += RotationX[1] / mouseSens;

		sigmaRotY[i]				= camVec_[i].y;// +pPlayer_[i]->GetRotate().y;
		sigmaRotX[i]				= -camVec_[i].x;// + EasingX[i]; +pPlayer_[i]->GetRotate().x;

		if (sigmaRotX[i] > 0 * (3.14 / 180))
		{
			sigmaRotX[i]			= 0;
			camVec_[0].x		   -= RotationY[1] / mouseSens;
			camVec_[1].x		   -= RotationY[0] / controllerSens;
		}
		if (sigmaRotX[i] < -88 * (3.14 / 180))
		{
			sigmaRotX[i] = -87.9 * (3.14 / 180);
			camVec_[0].x		   -= RotationY[1] / mouseSens;
			camVec_[1].x		   -= RotationY[0] / controllerSens;
		}

		prevRotX[i] = sigmaRotX[i];
		mxRotX[i] = XMMatrixRotationX(sigmaRotX[i]);
		mxRotY[i] = XMMatrixRotationY(sigmaRotY[i]);

		rot[i] = mxRotX[i] * mxRotY[i];

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
		prevLen[i] = vecLength[i];
	}
	Camera::SetTarget(pAttackPlayer_->GetPosition(), 1);
	Camera::SetTarget(pCollectPlayer_->GetPosition(), 0);
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
	SAFE_DELETE(pObjectManager_);
}
