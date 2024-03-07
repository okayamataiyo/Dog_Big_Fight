#include "../Engine/SceneManager.h"
#include "../Engine/Input.h"
#include "../Engine/Model.h"
#include "../Engine/Direct3D.h"
#include "../Engine/ImGui/imgui.h"
#include "../Engine/Text.h"
#include "CollectPlayer.h"
#include "AttackPlayer.h"
#include "../Stage.h"
#include "../Object/Floor.h"
#include "../Object/WoodBox.h"

CollectPlayer::CollectPlayer(GameObject* _pParent)
    :PlayerBase(_pParent, "AttackPlayer"), hModel_{ -1 }, number_(0), playerState_(PLAYERSTATE::WAIT), playerStatePrev_(PLAYERSTATE::WAIT), gameState_(GAMESTATE::READY)
    , pParent_(nullptr), pPlayScene_(nullptr), pAttackPlayer_(nullptr), pCollision_(nullptr), pWoodBox_(nullptr), pText_(nullptr)
{
    pParent_ = _pParent;
    direct_.timeCounter_ = 0;
    direct_.score_ = 0;
    move_.positionPrev_ = { 0.0f,0.0f,0.0f };
    move_.controllerMoveSpeed_ = 0.3f;
    move_.mouseMoveSpeed_ = 0.3f;
    move_.positionY_ = 0.0f;
    move_.isDash_ = false;
    direction_.vecMove_ = { 0.0f,0.0f,0.0f,0.0f };
    direction_.vecLength_ = { 0.0f,0.0f,0.0f,0.0f };
    direction_.vecFront_ = { 0.0f,0.0f,0.0f,0.0f };
    direction_.vecDot_ = { 0.0f,0.0f,0.0f,0.0f };
    direction_.vecCross_ = { 0.0f,0.0f,0.0f,0.0f };
    direction_.length_ = 0.0f;
    direction_.dot_ = 0.0f;
    direction_.angle_ = 0.0f;
    jump_.positionTempY_ = 0.0f;
    jump_.positionPrevY_ = 0.0f;
    jump_.isJump_ = false;
    floor_.isOnFloor_ = false;
    woodBox_.woodBoxName_ = "WoodBox";
    woodBox_.woodBoxNumber_ = "WoodBox0";
    woodBox_.dotProduct_ = 0.0f;
    woodBox_.angleDegrees_ = 0.0f;
    knockback_.stunLimit_ = 0;
    knockback_.isStun_ = 0;
    knockback_.isKnockBack_ = false;
    rayCastDist_.rayStageDist_ = 0.0f;
    rayCastDist_.rayDownFloorDist_ = 0.0f;
    rayCastDist_.rayUpFloorDist_ = 0.0f;
    rayCastDist_.rayFrontDist_ = 0.0f;
    rayCastDist_.rayBackDist_ = 0.0f;
    rayCastDist_.rayLeftDist_ = 0.0f;
    rayCastDist_.rayRightDist_ = 0.0f;
}

CollectPlayer::~CollectPlayer()
{
}

void CollectPlayer::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("CollectDog.fbx");
    assert(hModel_ >= 0);
    transform_.scale_ = { 0.5,0.5,0.5 };
    move_.positionY_ = transform_.position_.y;
    pCollision_ = new SphereCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), 1.0f);
    AddCollider(pCollision_);
    pPlayScene_ = (PlayScene*)FindObject("PlayScene");
    pAttackPlayer_ = (AttackPlayer*)FindObject("AttackPlayer");
    pText_ = new Text;
    pText_->Initialize();
}

void CollectPlayer::Update()
{
    switch (gameState_)
    {
    case GAMESTATE::READY:          UpdateReady();      break;
    case GAMESTATE::PLAY:           UpdatePlay();       break;
    case GAMESTATE::GAMEOVER:       UpdateGameOver();   break;
    }
}

void CollectPlayer::Draw()
{
    pText_->Draw(30, 60, "CollectPlayer:Score=");
    pText_->Draw(360, 60, direct_.score_);

    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void CollectPlayer::Release()
{
}

void CollectPlayer::UpdateReady()
{
    ++direct_.timeCounter_;
    if (direct_.timeCounter_ >= 60)
    {
        gameState_ = GAMESTATE::PLAY;
        direct_.timeCounter_ = 0;
    }
}

void CollectPlayer::UpdatePlay()
{
    if (playerStatePrev_ != playerState_)
    {
        switch (playerState_)
        {
        case PLAYERSTATE::WAIT:       Model::SetAnimFrame(hModel_, 0, 0, 1); break;
        case PLAYERSTATE::WALK:       Model::SetAnimFrame(hModel_, 20, 60, 0.5); break;
        case PLAYERSTATE::RUN:        Model::SetAnimFrame(hModel_, 80, 120, 0.5); break;
        case PLAYERSTATE::JUMP:       Model::SetAnimFrame(hModel_, 120, 120, 1); break;
        }
    }
    playerStatePrev_ = playerState_;
    PlayerRayCast();
    PlayerKnockback();
    transform_.position_.y = move_.positionY_;
    if (knockback_.isStun_ == 1)
    {
        direct_.timeCounter_++;
        if (direct_.timeCounter_ >= knockback_.stunLimit_)
        {
            gameState_ = GAMESTATE::PLAY;
            knockback_.isStun_ = 0;
            knockback_.isKnockBack_ = false;
            direct_.timeCounter_ = 0;
        }
    }
    if (knockback_.isStun_ == 0)
    {
        PlayerMove();
    }
    if (direct_.score_ >= 150)
    {
        SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
        pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
        Direct3D::SetIsChangeView(1);
    }
    //ImGui::Text("playerState_=%i", playerState_);
    //ImGui::Text("posYPrev_=%f", posYPrev_);
    //ImGui::Text("posYTemp_=%f", posYTemp_);
    ImGui::Text("Transform_.position_.x=%f", transform_.position_.x);
    ImGui::Text("Transform_.position_.y=%f", transform_.position_.y);
    ImGui::Text("Transform_.position_.z=%f", transform_.position_.z);
    /*ImGui::Text("prevPosition_.x=%f", prevPosition_.x);
    ImGui::Text("prevPosition_.y=%f", prevPosition_.y);
    ImGui::Text("prevPosition_.z=%f", prevPosition_.z);*/
    //ImGui::Text("angleDegrees_=%f", angleDegrees_);
    //ImGui::Text("timeCounter_=%i", timeCounter_);
}

void CollectPlayer::UpdateGameOver()
{
    if (Input::IsKey(DIK_SPACE))
    {
        SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
        pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
    }
}

void CollectPlayer::Stun(int _timeLimit)
{
    //transform_.position_.y = posY_;
    knockback_.isStun_ = 1;
    knockback_.stunLimit_ = _timeLimit;
}

void CollectPlayer::OnCollision(GameObject* _pTarget)
{
    std::vector<int> woodBoxs = pPlayScene_->GetWoodBoxs();
    woodBox_.woodBoxNumber_ = woodBox_.woodBoxName_ + std::to_string(number_);
    if (_pTarget->GetObjectName() == woodBox_.woodBoxNumber_)
    {
        pWoodBox_ = (WoodBox*)FindObject(woodBox_.woodBoxNumber_);
        XMVECTOR vecPos = XMLoadFloat3(&transform_.position_) - pWoodBox_->GetVecPos();
        vecPos = XMVector3Normalize(vecPos);
        XMVECTOR vecUp = { 0,1,0,0 };
        woodBox_.dotProduct_ = XMVectorGetX(XMVector3Dot(vecPos, vecUp));
        float angleRadians = acosf(woodBox_.dotProduct_);
        woodBox_.angleDegrees_ = XMConvertToDegrees(angleRadians);
        if (woodBox_.angleDegrees_ <= 80)
        {
            PlayerJump();
            pWoodBox_->KillMe();
            pPlayScene_->AddPlayerFirstWoodBoxNum(-1);
        }
    }
    //WoodBoxという名前を持つ全てのオブジェクトの機能を実装
    if (_pTarget->GetObjectName().find("WoodBox") != std::string::npos)
    {
        if (woodBox_.angleDegrees_ > 80)
        {
            transform_.position_ = move_.positionPrev_;
        }
    }
    if (_pTarget->GetObjectName().find("Bone") != std::string::npos)
    {
        direct_.score_ += 10;
        _pTarget->KillMe();
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
    if (_pTarget->GetObjectName() == "AttackPlayer")
    {
        Stun(10);
        pAttackPlayer_->Stun(10);
        knockback_.isKnockBack_ = true;
    }
}

void CollectPlayer::PlayerMove()
{
    if (move_.isDash_ == false)
    {
        //controllerMoveSpeed_ *= 0.9f;
    }
    else
    {
        //controllerMoveSpeed_ *= 1.1f;
    }
    if (!(Input::IsKey(DIK_F)))
    {
        XMVECTOR vecCam = {};
        vecCam = -(Camera::VecGetPosition(0) - Camera::VecGetTarget(0));
        XMFLOAT3 camRot = {};
        XMStoreFloat3(&camRot, vecCam);
        camRot.y = 0;
        vecCam = XMLoadFloat3(&camRot);
        vecCam = XMVector3Normalize(vecCam);
        direction_.vecMove_ = vecCam;
    }
    //向き変更
    direction_.vecLength_ = XMVector3Length(direction_.vecMove_);
    direction_.length_ = XMVectorGetX(direction_.vecLength_);
    if (direction_.length_ != 0)
    {
        //プレイヤーが入力キーに応じて、その向きに変える(左向きには出来ない)
        direction_.vecFront_ = { 0,0,1,0 };
        direction_.vecMove_ = XMVector3Normalize(direction_.vecMove_);

        direction_.vecDot_ = XMVector3Dot(direction_.vecFront_, direction_.vecMove_);
        direction_.dot_ = XMVectorGetX(direction_.vecDot_);
        direction_.angle_ = acos(direction_.dot_);

        //右向きにしか向けなかったものを左向きにする事ができる
        direction_.vecCross_ = XMVector3Cross(direction_.vecFront_, direction_.vecMove_);
        if (XMVectorGetY(direction_.vecCross_) < 0)
        {
            direction_.angle_ *= -1;
        }
    }

    if (IsMoving())
    {
        playerState_ = PLAYERSTATE::WALK;
    }
    else if (jump_.isJump_ == false)
    {
        playerState_ = PLAYERSTATE::WAIT;
    }

    transform_.rotate_.y = XMConvertToDegrees(direction_.angle_);
    if (Input::IsKey(DIK_W))
    {
        XMVECTOR vecDirection = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(0);
        vecDirection = XMVectorSetY(vecDirection, 0);
        vecDirection = XMVector3Normalize(vecDirection);
        transform_.position_.x = transform_.position_.x + move_.controllerMoveSpeed_ * XMVectorGetX(vecDirection);
        transform_.position_.z = transform_.position_.z + move_.controllerMoveSpeed_ * XMVectorGetZ(vecDirection);
    }
    if (Input::IsKey(DIK_S))
    {
        XMVECTOR vecDirection = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(0);
        vecDirection = XMVectorSetY(vecDirection, 0);
        vecDirection = XMVector3Normalize(vecDirection);
        transform_.position_.x = transform_.position_.x + move_.controllerMoveSpeed_ * XMVectorGetX(-vecDirection);
        transform_.position_.z = transform_.position_.z + move_.controllerMoveSpeed_ * XMVectorGetZ(-vecDirection);
    }
    if (Input::IsKey(DIK_D))
    {
        XMMATRIX rotmat = XMMatrixRotationY(3.14 / 2);                          //XMMatrixRotationY = Y座標を中心に回転させる行列を作る関数,//XMConvertToRadians = degree角をradian角に(ただ)変換する
        XMVECTOR vecDirection = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(0);
        vecDirection = XMVectorSetY(vecDirection, 0);
        vecDirection = XMVector3Normalize(vecDirection);
        XMVECTOR tempvec = XMVector3Transform(vecDirection, rotmat);
        transform_.position_.x = transform_.position_.x + move_.controllerMoveSpeed_ * XMVectorGetX(tempvec);
        transform_.position_.z = transform_.position_.z + move_.controllerMoveSpeed_ * XMVectorGetZ(tempvec);
    }
    if (Input::IsKey(DIK_A))
    {
        XMMATRIX rotmat = XMMatrixRotationY(3.14 / 2);
        XMVECTOR vecDirection = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(0);
        vecDirection = XMVectorSetY(vecDirection, 0);
        vecDirection = XMVector3Normalize(vecDirection);
        XMVECTOR tempvec = XMVector3Transform(vecDirection, -rotmat);
        transform_.position_.x = transform_.position_.x + move_.controllerMoveSpeed_ * XMVectorGetX(tempvec);
        transform_.position_.z = transform_.position_.z + move_.controllerMoveSpeed_ * XMVectorGetZ(tempvec);
    }
    if (Input::IsKeyDown(DIK_SPACE) && jump_.isJump_ == false)
    {
        PlayerJump();
    }
    if (Input::IsKey(DIK_LSHIFT) && jump_.isJump_ == false)
    {
        playerState_ = PLAYERSTATE::RUN;
        move_.isDash_ = true;
    }
    else
    {
        move_.isDash_ = false;
    }

    if (jump_.isJump_ == true)
    {
        playerState_ = PLAYERSTATE::JUMP;
    }
}

void CollectPlayer::PlayerJump()
{
    //ジャンプの処理
    jump_.isJump_ = true;
    jump_.positionPrevY_ = move_.positionY_;
    move_.positionY_ = move_.positionY_ + 0.3;
}

void CollectPlayer::PlayerKnockback()
{
    if (knockback_.isKnockBack_ == true)
    {
        XMVECTOR vecKnockbackDirection = (XMLoadFloat3(&transform_.position_) - pAttackPlayer_->GetVecPos());
        vecKnockbackDirection = XMVector3Normalize(vecKnockbackDirection);
        float knockbackSpeed = 0.3f;
        SetKnockback(vecKnockbackDirection, knockbackSpeed);
        pAttackPlayer_->SetKnockback(-vecKnockbackDirection, knockbackSpeed);
        Stun(30);
        pAttackPlayer_->Stun(30);
    }
}

void CollectPlayer::PlayerRayCast()
{
    RayCastData upData;
    RayCastData downFloorData;
    RayCastData downData;
    RayCastData frontData;
    RayCastData backData;
    RayCastData leftData;
    RayCastData rightData;
    float playerFling = 1.0f;                             //プレイヤーが地面からどのくらい離れていたら浮いている判定にするか
    Stage* pStage = (Stage*)FindObject("Stage");      //ステージオブジェクト
    int hStageModel_ = pStage->GetModelHandle();         //モデル番号を取得
    Floor* pFloor = (Floor*)FindObject("Floor");
    int hFloorModel_ = pFloor->GetModelHandle();
    if (jump_.isJump_ == true)
    {
        //放物線に下がる処理
        jump_.positionTempY_ = move_.positionY_;
        move_.positionY_ += (move_.positionY_ - jump_.positionPrevY_) - 0.007;
        jump_.positionPrevY_ = jump_.positionTempY_;
        if (move_.positionY_ <= -rayCastDist_.rayDownFloorDist_ + 0.6)
        {
            jump_.isJump_ = false;
        }
        if (move_.positionY_ <= -rayCastDist_.rayStageDist_ + 0.6)
        {
            jump_.isJump_ = false;
        }
    }

    for (int i = 0; i <= 2; i++)
    {
        //▼上の法線(すり抜け床のため)
        upData.start = transform_.position_;           //レイの発射位置
        upData.dir = XMFLOAT3(0, 1, 0);                //レイの方向
        Model::RayCast(hFloorModel_ + i, &upData);         //レイを発射
        rayCastDist_.rayUpFloorDist_ = upData.dist;
        //ImGui::Text("rayUpDist_=%f", rayUpDist_);

        //▼下の法線(すり抜け床)
        downFloorData.start = transform_.position_;    //レイの発射位置
        downFloorData.start.y = 0.0f;
        downFloorData.dir = XMFLOAT3(0, -1, 0);        //レイの方向
        if (upData.dist == 99999)
        {
            Model::RayCast(hFloorModel_ + i, &downFloorData);  //レイを発射
        }
        rayCastDist_.rayDownFloorDist_ = downFloorData.dist;
        //ImGui::Text("rayFloorDist_=%f", rayFloorDist_);
        if (rayCastDist_.rayDownFloorDist_ + move_.positionY_ <= playerFling)
        {
            if (jump_.isJump_ == false)
            {
                move_.positionY_ = -downFloorData.dist + 0.6f;
                floor_.isOnFloor_ = 1;
                jump_.positionTempY_ = move_.positionY_;
                jump_.positionPrevY_ = jump_.positionTempY_;
            }
        }
        else
        {
            floor_.isOnFloor_ = 0;
        }

    }
    //▼下の法線(床に張り付き)
    downData.start = transform_.position_;  //レイの発射位置
    downData.start.y = 0;
    downData.dir = XMFLOAT3(0, -1, 0);       //レイの方向
    Model::RayCast(hStageModel_, &downData); //レイを発射
    rayCastDist_.rayStageDist_ = downData.dist;
    //プレイヤーが浮いていないとき
    //ImGui::Text("rayGravityDist_=%f", rayGravityDist_);
    if (rayCastDist_.rayStageDist_ + move_.positionY_ <= playerFling)
    {
        //ジャンプしてない＆すり抜け床の上にいない
        if (jump_.isJump_ == false && floor_.isOnFloor_ == 0)
        {
            //地面に張り付き
            move_.positionY_ = -downData.dist + 0.6;
            jump_.positionTempY_ = move_.positionY_;
            jump_.positionPrevY_ = jump_.positionTempY_;
        }
    }
    else if (floor_.isOnFloor_ == 0)
    {
        jump_.isJump_ = true;
    }
    //▼前の法線(壁の当たり判定)
    frontData.start = transform_.position_;       //レイの発射位置
    frontData.dir = XMFLOAT3(0, 1, 1);            //レイの方向
    Model::RayCast(hStageModel_, &frontData);  //レイを発射
    rayCastDist_.rayFrontDist_ = frontData.dist;
    //ImGui::Text("rayFrontDist_=%f", rayFrontDist_);
    if (rayCastDist_.rayFrontDist_ <= 1.5f)
    {
        //transform_.position_.z = (float)((int)prevPosition_.z) - 0.00001f;
        transform_.position_.z = move_.positionPrev_.z;
    }
    //▼後ろの法線(壁の当たり判定)
    backData.start = transform_.position_;       //レイの発射位置
    backData.dir = XMFLOAT3(0, 1, -1);           //レイの方向
    Model::RayCast(hStageModel_, &backData);  //レイを発射
    rayCastDist_.rayBackDist_ = backData.dist;
    //ImGui::Text("rayBackDist_=%f", rayBackDist_);
    if (rayCastDist_.rayBackDist_ <= 1.5f)
    {
        //transform_.position_.z = (float)((int)prevPosition_.z) + 0.00001f;
        transform_.position_.z = move_.positionPrev_.z;
    }
    //▼左の法線(壁の当たり判定)
    leftData.start = transform_.position_;       //レイの発射位置
    leftData.dir = XMFLOAT3(-1, 1, 0);           //レイの方向
    Model::RayCast(hStageModel_, &leftData);  //レイを発射
    rayCastDist_.rayLeftDist_ = leftData.dist;
    //ImGui::Text("rayLeftDist_=%f", rayLeftDist_);
    if (rayCastDist_.rayLeftDist_ <= 1.5f)
    {
        //transform_.position_.x = (float)((int)prevPosition_.x) + 0.00001f;
        transform_.position_.x = move_.positionPrev_.x;
    }
    //▼右の法線(壁の当たり判定)
    rightData.start = transform_.position_;       //レイの発射位置
    rightData.dir = XMFLOAT3(1, 1, 0);           //レイの方向
    Model::RayCast(hStageModel_, &rightData);  //レイを発射
    rayCastDist_.rayRightDist_ = rightData.dist;
    //ImGui::Text("rayRightDist_=%f", rayRightDist_);
    if (rayCastDist_.rayRightDist_ <= 1.5f)
    {
        //transform_.position_.x = (float)((int)prevPosition_.x) - 0.00001f;
        transform_.position_.x = move_.positionPrev_.x;
    }
    move_.positionPrev_ = transform_.position_;
}

void CollectPlayer::SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed)
{
    transform_.position_.x = transform_.position_.x + _knockbackSpeed * XMVectorGetX(_vecKnockbackDirection);
    transform_.position_.z = transform_.position_.z + _knockbackSpeed * XMVectorGetZ(_vecKnockbackDirection);
}

bool CollectPlayer::IsMoving()
{
    return (transform_.position_.x != move_.positionPrev_.x || transform_.position_.z != move_.positionPrev_.z);
}