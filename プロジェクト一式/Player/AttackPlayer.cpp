#include "../Engine/SceneManager.h"
#include "../Engine/Input.h"
#include "../Engine/Model.h"
#include "../Engine/Direct3D.h"
#include "../Engine/ImGui/imgui.h"
#include "../Engine/Text.h"
#include "../Engine/Audio.h"
#include "AttackPlayer.h"
#include "CollectPlayer.h"
#include "../Object/Floor.h"
#include "../Object/WoodBox.h"
#include "../StageObject/Stage.h"

AttackPlayer::AttackPlayer(GameObject* _pParent)
    :PlayerBase(_pParent, attackPlayerName), hModel_{ -1 }, hSound_{ -1,-1,-1,-1 }, stageHModel_(0),floorHModel_(0), number_(0), scoreTimeCounter_(0), playerState_(PLAYERSTATE::WAIT), playerStatePrev_(PLAYERSTATE::WAIT), gameState_(GAMESTATE::READY)
    , pParent_(nullptr), pPlayScene_(nullptr), pCollectPlayer_(nullptr), pCollision_(nullptr), pWoodBox_(nullptr), pText_(nullptr),pStage_(nullptr),pFloor_(nullptr)
{
    pParent_ = _pParent;
    timeCounter_ = 0;
    score_ = 0;
    positionPrev_ = { 0.0f,0.0f,0.0f };
    controllerMoveSpeed_ = 0.3f;
    mouseMoveSpeed_ = 0.3f;
    positionY_ = 0.0f;
    isDash_ = false;
    isFling_ = 1.0f;
    vecMove_ = { 0.0f,0.0f,0.0f,0.0f };
    vecLength_ = { 0.0f,0.0f,0.0f,0.0f };
    vecFront_ = { 0.0f,0.0f,0.0f,0.0f };
    vecDot_ = { 0.0f,0.0f,0.0f,0.0f };
    vecCross_ = { 0.0f,0.0f,0.0f,0.0f };
    length_ = 0.0f;
    dot_ = 0.0f;
    angle_ = 0.0f;
    positionTempY_ = 0.0f;
    positionPrevY_ = 0.0f;
    isJump_ = false;
    isOnFloor_ = false;
    woodBoxName_ = "WoodBox";
    woodBoxNumber_ = "WoodBox0";
    dotProduct_ = 0.0f;
    angleDegrees_ = 0.0f;
    stunTimeCounter_ = 0;
    stunLimit_ = 0;
    isStun_ = false;
    isKnockBack_ = false;
    rayStageDistDown_ = 0.0f;
    rayFloorDistDown_ = 0.0f;
    rayFloorDistUp_ = 0.0f;
    rayStageDistFront_ = 0.0f;
    rayStageDistBack_ = 0.0f;
    rayStageDistLeft_ = 0.0f;
    rayStageDistRight_ = 0.0f;
}

AttackPlayer::~AttackPlayer()
{
}

void AttackPlayer::Initialize()
{
    //サウンドデータのロード
    hSound_[0] = Audio::Load("Sound/Stun.wav");
    assert(hSound_[0] >= 0);
    hSound_[1] = Audio::Load("Sound/Walk.wav");
    assert(hSound_[1] >= 0);
    hSound_[2] = Audio::Load("Sound/Jump.wav");
    assert(hSound_[2] >= 0);
    hSound_[3] = Audio::Load("Sound/Run.wav");
    assert(hSound_[3] >= 0);

    //モデルデータのロード
    std::string ModelName = attackPlayerName + (std::string)".fbx";
    hModel_ = Model::Load(ModelName);
    assert(hModel_ >= 0);
    transform_.scale_ = { 0.4,0.4,0.4 };
    positionY_ = transform_.position_.y;
    pCollision_ = new SphereCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), 2.0f);
    AddCollider(pCollision_);
    pPlayScene_ = (PlayScene*)FindObject("PlayScene");
    pStage_ = (Stage*)FindObject("Stage");
    pFloor_ = (Floor*)FindObject("Floor");
    pText_ = new Text;
    pText_->Initialize();
}

void AttackPlayer::Update()
{
    switch (gameState_)
    {
    case GAMESTATE::READY:          UpdateReady();      break;
    case GAMESTATE::PLAY:           UpdatePlay();       break;
    case GAMESTATE::GAMEOVER:       UpdateGameOver();   break;
    }
}

void AttackPlayer::Draw()
{
    pText_->Draw(30, 30, "AttackPlayer:Score=");
    pText_->Draw(360, 30, score_);

    //pText_->Draw(30, 120, "Attack!!!");
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void AttackPlayer::Release()
{

}

void AttackPlayer::UpdateReady()
{
    RayCastData stageDataDown;
    stageHModel_ = pStage_->GetModelHandle();         //モデル番号を取得
    floorHModel_ = pFloor_->GetModelHandle();
    //▼下の法線(地面に張り付き)
    stageDataDown.start = transform_.position_;  //レイの発射位置
    stageDataDown.start.y = 0;
    stageDataDown.dir = XMFLOAT3(0, -1, 0);       //レイの方向
    Model::RayCast(stageHModel_, &stageDataDown); //レイを発射
    rayStageDistDown_ = stageDataDown.dist;
    if (stageDataDown.hit)
    {
        transform_.position_.y = -stageDataDown.dist + 0.6;
    }
    ++timeCounter_;
    if (timeCounter_ >= 60)
    {
        gameState_ = GAMESTATE::PLAY;
        timeCounter_ = 0;
    }
    positionY_ = transform_.position_.y;
}

void AttackPlayer::UpdatePlay()
{
    if (playerStatePrev_ != playerState_)
    {
        switch (playerState_)
        {
        case PLAYERSTATE::WAIT:       Model::SetAnimFrame(hModel_, 0, 0, 1.0f); break;
        case PLAYERSTATE::WALK:       Model::SetAnimFrame(hModel_, 20, 60, 0.5f); break;
        case PLAYERSTATE::RUN:        Model::SetAnimFrame(hModel_, 80, 120, 0.5f); break;
        case PLAYERSTATE::JUMP:       Model::SetAnimFrame(hModel_, 120, 120, 1.0f); break;
        case PLAYERSTATE::STUN:       Model::SetAnimFrame(hModel_, 140, 200, 0.5f); break;
        }
    }
    scoreTimeCounter_++;
    if (scoreTimeCounter_ % 600 == 0)
    {
        score_ += 10;
    }
    playerStatePrev_ = playerState_;
    PlayerRayCast();
    PlayerKnockback();
    transform_.position_.y = positionY_;
    if (isStun_)
    {
        stunTimeCounter_++;
        if (stunTimeCounter_ >= stunLimit_)
        {
            gameState_ = GAMESTATE::PLAY;
            isStun_ = false;
            isKnockBack_ = false;
            stunTimeCounter_ = 0;
        }
    }
    if (!isStun_)
    {
        PlayerMove();
    }
    if (score_ >= 150)
    {
        SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
        pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
        Direct3D::SetIsChangeView(1);
    }
    //ImGui::Text("playerState_=%i", playerState_);
    //ImGui::Text("positionPrevY_=%f", positionPrevY_);
    //ImGui::Text("positionTempY_=%f", positionTempY_);
    ImGui::Text("Transform_.position_.x=%f", transform_.position_.x);
    ImGui::Text("Transform_.position_.y=%f", transform_.position_.y);
    ImGui::Text("Transform_.position_.z=%f", transform_.position_.z);
    /*ImGui::Text("prevPosition_.x=%f", prevPosition_.x);
    ImGui::Text("prevPosition_.y=%f", prevPosition_.y);
    ImGui::Text("prevPosition_.z=%f", prevPosition_.z);*/
    //ImGui::Text("angleDegrees_=%f", angleDegrees_);
    //ImGui::Text("timeCounter_=%i", timeCounter_);
    if (IsMoving() && !isJump_ && !isDash_)
    {
        playerState_ = PLAYERSTATE::WALK;
        Audio::Play(hSound_[1],0.5f);
    }
    if (!IsMoving() && !isJump_)
    {
        playerState_ = PLAYERSTATE::WAIT;
        Audio::Stop(hSound_[1]);
        Audio::Stop(hSound_[3]);
    }
    if (Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER) && !isJump_ && IsMoving())
    {
        playerState_ = PLAYERSTATE::RUN;
        Audio::Stop(hSound_[1]);
        Audio::Play(hSound_[3], 0.2f);
        isDash_ = true;
    }
    else
    {
        isDash_ = false;
    }
    if (isJump_)
    {
        playerState_ = PLAYERSTATE::JUMP;
    }
    if (isStun_)
    {
        playerState_ = PLAYERSTATE::STUN;
    }
}

void AttackPlayer::UpdateGameOver()
{
    if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
    {
        SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
        pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
    }
}

void AttackPlayer::Stun(int _timeLimit)
{
    //transform_.position_.y = positionY_;
    isStun_ = true;
    stunLimit_ = _timeLimit;
}

void AttackPlayer::OnCollision(GameObject* _pTarget)
{
    std::vector<int> woodBoxs = pPlayScene_->GetWoodBoxs();
    woodBoxNumber_ = woodBoxName_ + std::to_string(number_);
    if (_pTarget->GetObjectName() == woodBoxNumber_)
    {
        pWoodBox_ = (WoodBox*)FindObject(woodBoxNumber_);
        XMVECTOR vecPos = XMLoadFloat3(&transform_.position_) - pWoodBox_->GetVecPos();
        vecPos = XMVector3Normalize(vecPos);
        XMVECTOR vecUp = { 0,1,0,0 };
        dotProduct_ = XMVectorGetX(XMVector3Dot(vecPos, vecUp));
        float angleRadians = acosf(dotProduct_);
        angleDegrees_ = XMConvertToDegrees(angleRadians);
        if (angleDegrees_ <= 80)
        {
            PlayerJump();
            pWoodBox_->SetWoodBoxBreak();
            pPlayScene_->AddWoodBoxCount(-1);
        }
    }
    //WoodBoxという名前を持つ全てのオブジェクトを参照
    if (_pTarget->GetObjectName().find("WoodBox") != std::string::npos)
    {
        if (angleDegrees_ > 80)
        {
            transform_.position_ = positionPrev_;
        }
    }
    ++number_;
    if (number_ >= woodBoxs.size())
    {
        number_ = 0;
    }
    //if (_pTarget->GetObjectName() == "PlayerFirst")
    //{
    //    SetGameState(GAMESTATE::FIRSTSTUN);
    //    XMVECTOR vecKnockbackDirection = -(XMLoadFloat3(&transform_.position_) - pPlayer_->GetVecPos());
    //    XMFLOAT3 moveRot = {};
    //    XMStoreFloat3(&moveRot, vecKnockbackDirection);
    //    moveRot.y = 0;
    //    vecKnockbackDirection = XMLoadFloat3(&moveRot);
    //    XMVECTOR vectorMove = XMLoadFloat3(&transform_.position_) + (vecKnockbackDirection / 4);
    //    XMStoreFloat3(&transform_.position_, vectorMove);
    //    pPlayer_->SetVecPos(-vectorMove);
    //    SetGameState(GAMESTATE::SECONDSSTUN);
    //}
    if (_pTarget->GetObjectName() == collectPlayerName)
    {
        Stun(10);
        isKnockBack_ = true;
    }
}

void AttackPlayer::PlayerMove()
{
    if (isDash_ == false)
    {
        mouseMoveSpeed_ = 0.4f;
    }
    else
    {
        mouseMoveSpeed_ = 0.5f;
    }
    if (!(Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER)))
    {
        XMVECTOR vecCam = {};
        vecCam = -(Camera::VecGetPosition(1) - Camera::VecGetTarget(1));
        XMFLOAT3 camRot = {};
        XMStoreFloat3(&camRot, vecCam);
        camRot.y = 0;
        vecCam = XMLoadFloat3(&camRot);
        vecCam = XMVector3Normalize(vecCam);
        vecMove_ = vecCam;
    }
    //向き変更
    vecLength_ = XMVector3Length(vecMove_);
    length_ = XMVectorGetX(vecLength_);
    if (length_ != 0)
    {
        //プレイヤーが入力キーに応じて、その向きに変える(左向きには出来ない)
        vecFront_ = { 0,0,1,0 };
        vecMove_ = XMVector3Normalize(vecMove_);

        vecDot_ = XMVector3Dot(vecFront_, vecMove_);
        dot_ = XMVectorGetX(vecDot_);
        angle_ = acos(dot_);

        //右向きにしか向けなかったものを左向きにする事ができる
        vecCross_ = XMVector3Cross(vecFront_, vecMove_);
        if (XMVectorGetY(vecCross_) < 0)
        {
            angle_ *= -1;
        }
    }
    transform_.rotate_.y = XMConvertToDegrees(angle_);
    if (Input::GetPadStickL().y > 0.3)
    {
        XMVECTOR vecDirection = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(1);
        vecDirection = XMVectorSetY(vecDirection, 0);
        vecDirection = XMVector3Normalize(vecDirection);
        transform_.position_.x = transform_.position_.x + mouseMoveSpeed_ * XMVectorGetX(vecDirection);
        transform_.position_.z = transform_.position_.z + mouseMoveSpeed_ * XMVectorGetZ(vecDirection);
    }
    if (Input::GetPadStickL().y < -0.3)
    {
        XMVECTOR vecDirection = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(1);
        vecDirection = XMVectorSetY(vecDirection, 0);
        vecDirection = XMVector3Normalize(vecDirection);
        transform_.position_.x = transform_.position_.x + mouseMoveSpeed_ * XMVectorGetX(-vecDirection);
        transform_.position_.z = transform_.position_.z + mouseMoveSpeed_ * XMVectorGetZ(-vecDirection);
    }
    if (Input::GetPadStickL().x > 0.3)
    {
        XMMATRIX rotmat = XMMatrixRotationY(3.14 / 2);
        XMVECTOR vecDirection = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(1);
        vecDirection = XMVectorSetY(vecDirection, 0);
        vecDirection = XMVector3Normalize(vecDirection);
        XMVECTOR tempvec = XMVector3Transform(vecDirection, rotmat);
        transform_.position_.x = transform_.position_.x + mouseMoveSpeed_ * XMVectorGetX(tempvec);
        transform_.position_.z = transform_.position_.z + mouseMoveSpeed_ * XMVectorGetZ(tempvec);
    }
    if (Input::GetPadStickL().x < -0.3)
    {
        XMMATRIX rotmat = XMMatrixRotationY(3.14 / 2);
        XMVECTOR vecDirection = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(1);
        vecDirection = XMVectorSetY(vecDirection, 0);
        vecDirection = XMVector3Normalize(vecDirection);
        XMVECTOR tempvec = XMVector3Transform(vecDirection, -rotmat);
        transform_.position_.x = transform_.position_.x + mouseMoveSpeed_ * XMVectorGetX(tempvec);
        transform_.position_.z = transform_.position_.z + mouseMoveSpeed_ * XMVectorGetZ(tempvec);
    }
    if (Input::IsPadButton(XINPUT_GAMEPAD_A) && isJump_ == false)
    {
        PlayerJump();
        Audio::Stop(hSound_[1]);
        Audio::Stop(hSound_[3]);
        Audio::Play(hSound_[2],0.5f);
    }
    if (transform_.position_.z <= -99.0f || transform_.position_.z >= 99.0f)
    {
        transform_.position_.z = positionPrev_.z;
    }
    if (transform_.position_.x <= -99.0f || transform_.position_.x >= 99.0f)
    {
        transform_.position_.x = positionPrev_.x;
    }
}

void AttackPlayer::PlayerJump()
{
    //ジャンプの処理
    isJump_ = true;
    positionPrevY_ = positionY_;
    positionY_ = positionY_ + 0.3;
}

void AttackPlayer::PlayerKnockback()
{
    if (isKnockBack_ == true)
    {
        XMVECTOR vecKnockbackDirection = (XMLoadFloat3(&transform_.position_) - pCollectPlayer_->GetVecPos());
        vecKnockbackDirection = XMVector3Normalize(vecKnockbackDirection);
        float knockbackSpeed = 0.3f;
        SetKnockback(vecKnockbackDirection, knockbackSpeed);
        Stun(30);
    }
}

void AttackPlayer::PlayerRayCast()
{
    RayCastData floorDataUp;
    RayCastData floorDataDown;
    RayCastData stageDataDownDown;
    RayCastData stageDataDownFront;
    RayCastData stageDataDownBack;
    RayCastData stageDataDownLeft;
    RayCastData stageDataDownRight;                             //プレイヤーが地面からどのくらい離れていたら浮いている判定にするか
    stageHModel_ = pStage_->GetModelHandle();         //モデル番号を取得
    floorHModel_ = pFloor_->GetModelHandle();
    if (isJump_ == true)
    {
        //放物線に下がる処理
        positionTempY_ = positionY_;
        positionY_ += (positionY_ - positionPrevY_) - 0.007;
        positionPrevY_ = positionTempY_;
        isJump_ = (positionY_ <= -rayFloorDistDown_ + 0.6f) ? false : isJump_;
        isJump_ = (positionY_ <= -rayStageDistDown_ + 0.6f) ? false : isJump_;
    }

    for (int i = 0; i <= 2; i++)
    {
        //▼上の法線(すり抜け床のため)
        floorDataUp.start = transform_.position_;           //レイの発射位置
        floorDataUp.dir = XMFLOAT3(0, 1, 0);                //レイの方向
        Model::RayCast(floorHModel_ + i, &floorDataUp);         //レイを発射
        rayFloorDistUp_ = floorDataUp.dist;
        //ImGui::Text("rayUpDist_=%f", rayUpDist_);

        //▼下の法線(すり抜け床)
        floorDataDown.start = transform_.position_;    //レイの発射位置
        floorDataDown.start.y = 0.0f;
        floorDataDown.dir = XMFLOAT3(0, -1, 0);        //レイの方向
        if (floorDataUp.dist == 99999)
        {
            Model::RayCast(floorHModel_ + i, &floorDataDown);  //レイを発射
        }
        rayFloorDistDown_ = floorDataDown.dist;
        //ImGui::Text("rayFloorDist_=%f", rayFloorDist_);
        if (rayFloorDistDown_ + positionY_ <= isFling_)
        {
            if (isJump_ == false)
            {
                positionY_ = -floorDataDown.dist + 0.6f;
                isOnFloor_ = 1;
                positionTempY_ = positionY_;
                positionPrevY_ = positionTempY_;
            }
        }
        else
        {
            isOnFloor_ = 0;
        }

    }
    //▼下の法線(地面に張り付き)
    stageDataDownDown.start = transform_.position_;  //レイの発射位置
    stageDataDownDown.start.y = 0;
    stageDataDownDown.dir = XMFLOAT3(0, -1, 0);       //レイの方向
    Model::RayCast(stageHModel_, &stageDataDownDown); //レイを発射
    rayStageDistDown_ = stageDataDownDown.dist;
    //プレイヤーが浮いていないとき
    //ImGui::Text("rayGravityDist_=%f", rayGravityDist_);
    if (rayStageDistDown_ + positionY_ <= isFling_)
    {
        //ジャンプしてない＆すり抜け床の上にいない
        if (isJump_ == false && isOnFloor_ == 0)
        {
            //地面に張り付き
            positionY_ = -stageDataDownDown.dist + 0.6;
            positionTempY_ = positionY_;
            positionPrevY_ = positionTempY_;
        }
    }
    else if (isOnFloor_ == 0)
    {
        isJump_ = true;
    }
    //▼前の法線(壁の当たり判定)
    stageDataDownFront.start = transform_.position_;       //レイの発射位置
    stageDataDownFront.dir = XMFLOAT3(0, 1, 1);            //レイの方向
    Model::RayCast(stageHModel_, &stageDataDownFront);  //レイを発射
    rayStageDistFront_ = stageDataDownFront.dist;
    //ImGui::Text("rayStageDistFront_=%f", rayStageDistFront_);
    if (rayStageDistFront_ <= 1.5f)
    {
        //transform_.position_.z = (float)((int)prevPosition_.z) - 0.00001f;
        transform_.position_.z = positionPrev_.z;
    }
    //▼後ろの法線(壁の当たり判定)
    stageDataDownBack.start = transform_.position_;       //レイの発射位置
    stageDataDownBack.dir = XMFLOAT3(0, 1, -1);           //レイの方向
    Model::RayCast(stageHModel_, &stageDataDownBack);  //レイを発射
    rayStageDistBack_ = stageDataDownBack.dist;
    //ImGui::Text("rayStageDistBack_=%f", rayStageDistBack_);
    if (rayStageDistBack_ <= 1.5f)
    {
        //transform_.position_.z = (float)((int)prevPosition_.z) + 0.00001f;
        transform_.position_.z = positionPrev_.z;
    }
    //▼左の法線(壁の当たり判定)
    stageDataDownLeft.start = transform_.position_;       //レイの発射位置
    stageDataDownLeft.dir = XMFLOAT3(-1, 1, 0);           //レイの方向
    Model::RayCast(stageHModel_, &stageDataDownLeft);  //レイを発射
    rayStageDistLeft_ = stageDataDownLeft.dist;
    //ImGui::Text("rayStageDistLeft_=%f", rayStageDistLeft_);
    if (rayStageDistLeft_ <= 1.5f)
    {
        //transform_.position_.x = (float)((int)prevPosition_.x) + 0.00001f;
        transform_.position_.x = positionPrev_.x;
    }
    //▼右の法線(壁の当たり判定)
    stageDataDownRight.start = transform_.position_;       //レイの発射位置
    stageDataDownRight.dir = XMFLOAT3(1, 1, 0);           //レイの方向
    Model::RayCast(stageHModel_, &stageDataDownRight);  //レイを発射
    rayStageDistRight_ = stageDataDownRight.dist;
    //ImGui::Text("rayStageDistRight_=%f", rayStageDistRight_);
    if (rayStageDistRight_ <= 1.5f)
    {
        //transform_.position_.x = (float)((int)prevPosition_.x) - 0.00001f;
        transform_.position_.x = positionPrev_.x;
    }
    positionPrev_ = transform_.position_;
}

void AttackPlayer::SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed)
{
    transform_.position_.x = transform_.position_.x + _knockbackSpeed * XMVectorGetX(_vecKnockbackDirection);
    transform_.position_.z = transform_.position_.z + _knockbackSpeed * XMVectorGetZ(_vecKnockbackDirection);
}

bool AttackPlayer::IsMoving()
{
    return (transform_.position_.x != positionPrev_.x || transform_.position_.z != positionPrev_.z);
}
