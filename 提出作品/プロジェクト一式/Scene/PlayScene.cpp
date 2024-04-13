//インクルード
#include <random>
#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"
#include "../Engine/ImGui/imgui.h"
#include "../Engine/Audio.h"
#include "../Player/AttackPlayer.h"
#include "../Player/CollectPlayer.h"
#include "../ItemObject/WoodBox.h"
#include "../ItemObject/FrameBox.h"
#include "../ItemObject/Bone.h"
#include "../StageObject/StageObjectManager.h"
#include "PlayScene.h"

PlayScene::PlayScene(GameObject* _pParent)
	:GameObject(_pParent, playSceneName), hSound_{-1,-1,-1},random_value_{0}, soundVolume_{0.05f,},soundVolumeLow_{soundVolume_ / 2}, length_{30}, boneCount_{0}, isCreateBone_{false}, woodBoxCount_{0}
	, attackPlayerPosition_{}, attackPlayerDirection_{},frontPosition_{10.0f}, blockOrCollect_{0}
	,pSceneManager_{nullptr}, pAttackPlayer_{nullptr}, pCollectPlayer_{nullptr}, pItemObjectManager_{nullptr}, pStageObjectManager_{nullptr}
{

}

void PlayScene::Initialize()
{
	//サウンドデータのロード
	std::string soundName;
	for (int i = initZeroInt; i < sizeof(soundPlaySceneNames) / sizeof(soundPlaySceneNames[initZeroInt]); i++)
	{
		soundName = soundFolderName + soundPlaySceneNames[i] + soundModifierName;
		hSound_[i] = Audio::Load(soundName);
		assert(hSound_[i] >= initZeroInt);
	}
	pSceneManager_ = (SceneManager*)FindObject(sceneManagerName);
	pItemObjectManager_ = new ItemObjectManager(this);
	pStageObjectManager_ = new StageObjectManager(this);
	pStageObjectManager_->CreateStageObjectOrigin(STAGEOBJECTSTATE::SKY);
	pStageObjectManager_->CreateStageObjectOrigin(STAGEOBJECTSTATE::STAGE);
	for (int i = 0; i <= 2; i++)
	{
		pStageObjectManager_->CreateStageObject(STAGEOBJECTSTATE::STAGEBLOCK, -100.0f, 100.0f, -100.0f, 100.0f);
	}
	floorPosition_[0].position_ = { 30.0f,0.8f,3.0f };
	floorPosition_[1].position_ = { -70.0f,0.5f,50.0f };
	floorPosition_[2].position_ = { -45.0f, 0.3f,-45.0f };
	XMFLOAT3 scale		   = { 3.0f,1.0f,3.0f };
	XMFLOAT3 DefaultData[2] = { XMFLOAT3(0.0f,0.0f,0.0f)	//0で初期化
							   ,XMFLOAT3(1.0f,1.0f,1.0f) };	//1で初期化
	XMFLOAT3 WoodBox[3]		= {XMFLOAT3(30.0f,-5.0f,40.0f)
							  ,XMFLOAT3(20.0f,-10.0f,40.0f)
							  ,XMFLOAT3(10.0f,-20.0f,40.0f) };
	XMFLOAT3 FrameBox = { XMFLOAT3(5.0f,5.0f,5.0f) };
	//for (int i = 0u; i <= 1; i++)
	//{
	//	pObjectManager_->CreateObject(OBJECTSTATE::FLOOR, floorPosition_[i].position_, XMFLOAT3(0.0f,90.0f,0.0f), XMFLOAT3(4.0f,1.0f,4.0f));
	//}
	pItemObjectManager_->CreateObject(ITEMOBJECTSTATE::FLOOR, floorPosition_[2].position_, XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(10.0f, 1.0f, 10.0f));
	pItemObjectManager_->CreateObject(ITEMOBJECTSTATE::FLOOR, floorPosition_[1].position_, XMFLOAT3(0.0f, 90.0f, 0.0f), XMFLOAT3(10.0f, 1.0f, 10.0f));

	//for (int i = 0u; i <= 2; i++)
	//{
	//	pObjectManager_->CreateObject(OBJECTSTATE::WOODBOX, WoodBox[i], DefaultData[0], XMFLOAT3(0.3f, 0.3f, 0.3f));
	//}
	pAttackPlayer_ = Instantiate<AttackPlayer>(this);
	camVec_[attackPlayerNumber].x = 0;
	camVec_[attackPlayerNumber].y = 5;
	camVec_[attackPlayerNumber].z = -10;
	pCollectPlayer_ = Instantiate<CollectPlayer>(this);
	camVec_[collectPlayerNumber].x = 0;
	camVec_[collectPlayerNumber].y = 5;
	camVec_[collectPlayerNumber].z = -10;
	pItemObjectManager_->CreateObject(ITEMOBJECTSTATE::FRAMEBOX,DefaultData[0], DefaultData[1], FrameBox);
	pAttackPlayer_->SetCollectPlayer(pCollectPlayer_);
	pCollectPlayer_->SetAttackPlayer(pAttackPlayer_);
	XMFLOAT3 firstPPos = { -3,0,0 };
	XMFLOAT3 secondsPPos = { 3,0,0 };
	pAttackPlayer_->SetPosition(firstPPos);
	pCollectPlayer_->SetPosition(secondsPPos);

	//乱数生成器の設定
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 2);

	//1から2までのランダムな値の作成
	random_value_ = dis(gen);
}

void PlayScene::Update()
{
	Audio::Play(hSound_[(int)SOUNDSTATE::BGM], soundVolumeLow_);
	if (pAttackPlayer_->GetScore() >= 100 || pCollectPlayer_->GetScore() >= 100)
	{
		Audio::Stop(hSound_[(int)SOUNDSTATE::BGM]);
		Audio::Play(hSound_[random_value_],soundVolume_);
	}
	attackPlayerPosition_ = pAttackPlayer_->GetPosition();
	attackPlayerDirection_ = XMLoadFloat3(&attackPlayerPosition_) - Camera::VecGetPosition(1);
	attackPlayerDirection_ = XMVectorSetY(attackPlayerDirection_, 0);
	attackPlayerDirection_ = XMVector3Normalize(attackPlayerDirection_);
	attackPlayerPosition_.x = attackPlayerPosition_.x + frontPosition_ * XMVectorGetX(attackPlayerDirection_);
	attackPlayerPosition_.z = attackPlayerPosition_.z + frontPosition_ * XMVectorGetZ(attackPlayerDirection_);
	SetCursorPos(600, 600);
	HideCursor();
	isCreateBone_ = (boneCount_ == 0) ? true : isCreateBone_;
	isCreateBone_ = (boneCount_ == 3) ? false : isCreateBone_;

	if (isCreateBone_)
	{
		for (int i = 0u; i <= 2u; i++)
		{
			pItemObjectManager_->CreateObject(ITEMOBJECTSTATE::BONE, -100, 100, -100, 100);
			boneCount_ += 1;
		}
	}
	if (woodBoxCount_ <= 5)
	{
		if (Input::IsPadButtonDown(XINPUT_GAMEPAD_Y,pAttackPlayer_->GetPadID()) && !pAttackPlayer_->GetIsJump())
		{
			XMFLOAT3 woodBoxRotate = {};
			woodBoxRotate.y = XMConvertToDegrees(pAttackPlayer_->GetAngle());
			pItemObjectManager_->CreateObject(ITEMOBJECTSTATE::WOODBOX, attackPlayerPosition_, woodBoxRotate, XMFLOAT3(0.5f, 0.5f, 0.5f));
			woodBoxCount_ += 1;
		}
	}

	static float RotationX[2] = {};
	static float RotationY[2] = {};
	static float vecLength[2] = {};
	static float prevLen[2] = {};
	static float prevRotX[2] = {};

	mouse = Input::GetMouseMove();
	controller[((int)PADIDSTATE::FIRST)] = Input::GetPadStickR(pCollectPlayer_->GetPadID());
	controller[((int)PADIDSTATE::SECONDS)] = Input::GetPadStickR(pAttackPlayer_->GetPadID());
	RotationX[collectPlayerNumber] = controller[((int)PADIDSTATE::FIRST)].x;
	RotationY[collectPlayerNumber] = -controller[((int)PADIDSTATE::FIRST)].y;
	RotationX[attackPlayerNumber] = controller[((int)PADIDSTATE::SECONDS)].x;
	RotationY[attackPlayerNumber] = -controller[((int)PADIDSTATE::SECONDS)].y;
	if (Input::IsPadButton(XINPUT_GAMEPAD_DPAD_UP,pAttackPlayer_->GetPadID()))
	{
		vecLength[1] -= 1.0f;
	}
	if (Input::IsPadButton(XINPUT_GAMEPAD_DPAD_DOWN,pAttackPlayer_->GetPadID()))
	{
		vecLength[1] += 1.0f;
	}
	if (Input::IsPadButton(XINPUT_GAMEPAD_DPAD_UP, pCollectPlayer_->GetPadID()))
	{
		vecLength[collectPlayerNumber] -= 1.0f;
	}
	if (Input::IsPadButton(XINPUT_GAMEPAD_DPAD_DOWN, pCollectPlayer_->GetPadID()))
	{
		vecLength[collectPlayerNumber] += 1.0f;
	}

	vPos[collectPlayerNumber] = pCollectPlayer_->GetVecPos();
	vPos[attackPlayerNumber] = pAttackPlayer_->GetVecPos();
	playerPos[collectPlayerNumber] = pCollectPlayer_->GetPosition();
	playerPos[attackPlayerNumber] = pAttackPlayer_->GetPosition();

	for (int i = 0u; i <= 1u; i++)
	{

		Dir[i] = XMLoadFloat3(&rDir);

		camVec_[collectPlayerNumber].x += RotationY[collectPlayerNumber] / controllerSens;
		camVec_[collectPlayerNumber].y += RotationX[collectPlayerNumber] / controllerSens;

		camVec_[attackPlayerNumber].x += RotationY[attackPlayerNumber] / controllerSens;
		camVec_[attackPlayerNumber].y += RotationX[attackPlayerNumber] / controllerSens;

		sigmaRotY[i]				= camVec_[i].y;
		sigmaRotX[i]				= -camVec_[i].x;

		if (sigmaRotX[i] > initZeroInt * (3.14 / 180))
		{
			sigmaRotX[i]			= initZeroFloat;
			camVec_[attackPlayerNumber].x -= RotationY[attackPlayerNumber] / controllerSens;
			camVec_[collectPlayerNumber].x -= RotationY[collectPlayerNumber] / controllerSens;
		}
		if (sigmaRotX[i] < -88 * (3.14 / 180))
		{
			sigmaRotX[i] = -87.9 * (3.14 / 180);
			camVec_[attackPlayerNumber].x -= RotationY[attackPlayerNumber] / controllerSens;
			camVec_[collectPlayerNumber].x -= RotationY[collectPlayerNumber] / controllerSens;
		}

		prevRotX[i] = sigmaRotX[i];
		mxRotX[i] = XMMatrixRotationX(sigmaRotX[i]);
		mxRotY[i] = XMMatrixRotationY(sigmaRotY[i]);

		rot[i] = mxRotX[i] * mxRotY[i];

		Dir[i] = XMVector3Transform(Dir[i], rot[i]);
		Dir[i] = XMVector3Normalize(Dir[i]);
		if (pAttackPlayer_->GetIsStun())
		{
			static int lengthPrev = length_;
			if (length_ <= lengthPrev + 5)
			{
				++length_;
			}
		}
		else
		{
			static int lengthPrev = length_;
			if (length_ >= lengthPrev - 5)
			{
				--length_;
			}
		}
		Dir[i] = Dir[i] * (vecLength[i] + length_);
		Dir[i] += XMLoadFloat3(&playerPos[i]);
		XMStoreFloat3(&floatDir[i], Dir[i]);
		Camera::SetPosition(floatDir[i], i);
		prevLen[i] = vecLength[i];
	}
	Camera::SetTarget(pAttackPlayer_->GetPosition(), attackPlayerNumber);
	Camera::SetTarget(pCollectPlayer_->GetPosition(), collectPlayerNumber);
	if (Input::IsKeyDown(DIK_L))
	{
		pSceneManager_->ChangeScene(SCENE_ID_SELECT);
	}

}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
	SAFE_DELETE(pItemObjectManager_);
}
