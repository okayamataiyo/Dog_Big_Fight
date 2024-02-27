#include "Engine/SceneManager.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Engine/Direct3D.h"
#include "Engine/ImGui/imgui.h"
#include "Player.h"
#include "Stage.h"
#include "Floor.h"

Player::Player(GameObject* _pParent)
    :GameObject(_pParent, "Player"), TimeCounter_(0), hModel_{ -1 },jumpFlg_(false), 
    gameState_(READY),playerState_(WAIT), isFloor_(0)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("DogWalk.fbx");
    assert(hModel_ >= 0);
    transform_.scale_ = { 0.5,0.5,0.5 };
    posY_ = transform_.position_.y;
    prevPosition_ = transform_.position_;
    for (int i = 0u; i <= 1; i++)
    {
        pCollision_ = new SphereCollider(XMFLOAT3(0.0, 0.0, 0.0), 1);
        AddCollider(pCollision_);
    }
}

void Player::Update()
{
    switch (gameState_)
    {
    case GAMESTATE::READY:     UpdateReady();    break;
    case GAMESTATE::PLAY:      UpdatePlay();     break;
    case GAMESTATE::GAMEOVER:  UpdateGameOver(); break;
    }
}

void Player::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void Player::Release()
{
}

void Player::UpdateReady()
{
    //PlayerGravity();
    ++TimeCounter_;
    if (TimeCounter_ >= 60)
    {
        gameState_ = GAMESTATE::PLAY;
        TimeCounter_ = 0;
    }
}

void Player::UpdatePlay()
{
    if (prevState_ != playerState_)
    {
        switch (playerState_)
        {
        case PLAYERSTATE::WAIT:       Model::SetAnimFrame(hModel_, 0, 0, 1); break;
        case PLAYERSTATE::WALK:       Model::SetAnimFrame(hModel_, 20, 60, 0.5); break;
        case PLAYERSTATE::RUN:        Model::SetAnimFrame(hModel_, 80, 120, 0.5); break;
        case PLAYERSTATE::JUMP:       Model::SetAnimFrame(hModel_, 120, 120, 1); break;
        }
    }
    prevState_ = playerState_;

    PlayerRayCast();
    PlayerMove();
    //ImGui::Text("playerState_=%i", playerState_);
    //ImGui::Text("moveYPrev_=%f", moveYPrev_);
    //ImGui::Text("moveYTemp_=%f", moveYTemp_);
    /*ImGui::Text("Transform_.position_.x=%f", transform_.position_.x);
    ImGui::Text("Transform_.position_.y=%f", transform_.position_.y);
    ImGui::Text("Transform_.position_.z=%f", transform_.position_.z);
    ImGui::Text("prevPosition_.x=%f", prevPosition_.x);
    ImGui::Text("prevPosition_.y=%f", prevPosition_.y);
    ImGui::Text("prevPosition_.z=%f", prevPosition_.z);*/
}

void Player::UpdateGameOver()
{
    ++TimeCounter_;
    if (Input::IsKey(DIK_SPACE))
    {
        SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
        pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
    }
}

void Player::OnCollision(GameObject* _pTarget)
{
    if (_pTarget->GetObjectName() == "Item")
    {
        PlayerJump();
    }

    if (_pTarget->GetObjectName() == "PlayerSeconds")
    {
        if (gameState_ != GAMESTATE::GAMEOVER)
        {
            Direct3D::SetIsChangeView(1);
            gameState_ = GAMESTATE::GAMEOVER;
        }
    }
}

void Player::PlayerMove()
{
    XMVECTOR vecPos = XMLoadFloat3(&transform_.position_);
    transform_.position_.y = posY_;
    for (int i = 0u; i <= 1; i++)
    {
        XMVECTOR vecCam[2] = {};
        vecCam[i] = -(Camera::VecGetPosition(i) - Camera::VecGetTarget(i));
        vecCam[i] = XMVector3Normalize(vecCam[i]);
        vecMove_[i] = vecCam[i];
        vecMove_[i] *= 0.005f;

        //向き変更
        vecLength_ = XMVector3Length(vecMove_[i]);
        length_ = XMVectorGetX(vecLength_);

        if (length_ != 0)
        {
            //プレイヤーが入力キーに応じて、その向きに変える(左向きには出来ない)
            vecFront_ = { 0,0,1,0 };
            vecMove_[i] = XMVector3Normalize(vecMove_[i]);

            vecDot_[i] = XMVector3Dot(vecFront_, vecMove_[i]);
            dot_ = XMVectorGetX(vecDot_[i]);
            angle_[i] = acos(dot_);

            //右向きにしか向けなかったものを左向きにする事ができる
            vecCross_ = XMVector3Cross(vecFront_, vecMove_[i]);
            if (XMVectorGetY(vecCross_) < 0)
            {
                angle_[i] *= -1;
            }
        }

        if (IsMoving())
        {
            playerState_ = WALK;
        }
        else if (jumpFlg_ == false)
        {
            playerState_ = WAIT;
        }

        if (this->GetObjectName() == "PlayerFirst")
        {
            transform_.rotate_.y = XMConvertToDegrees(angle_[0]);
            if (Input::GetPadStickL().y > 0.3)
            {
                XMVECTOR vectorMove = XMLoadFloat3(&transform_.position_) + (vecMove_[0] / 4);
                XMStoreFloat3(&transform_.position_, vectorMove);
            }
            if (Input::GetPadStickL().y < -0.3)
            {
                XMVECTOR vectorMove = XMLoadFloat3(&transform_.position_) - (vecMove_[0] / 4);
                XMStoreFloat3(&transform_.position_, vectorMove);
            }
            if (Input::GetPadStickL().x > 0.3)
            {
                XMMATRIX rotmat = XMMatrixRotationY(3.14 / 2);                          //XMMatrixRotationY = Y座標を中心に回転させる行列を作る関数
                XMVECTOR tempvec = XMVector3Transform((vecMove_[0] / 4), rotmat);       //XMConvertToRadians = degree角をradian角に(ただ)変換する
                XMVECTOR vectorMove = XMLoadFloat3(&transform_.position_) + tempvec;
                XMStoreFloat3(&transform_.position_, vectorMove);
            }
            if (Input::GetPadStickL().x < -0.3)
            {
                XMMATRIX rotmat = XMMatrixRotationY(3.14 / 2);
                XMVECTOR tempvec = XMVector3Transform((vecMove_[0] / 4), -rotmat);
                XMVECTOR vectorMove = XMLoadFloat3(&transform_.position_) + tempvec;
                XMStoreFloat3(&transform_.position_, vectorMove);
            }
            if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A) && jumpFlg_ == false)
            {
                PlayerJump();
            }
            if (Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER) && jumpFlg_ == false)
            {
                playerState_ = RUN;
            }
        }
        if (this->GetObjectName() == "PlayerSeconds")
        {
            transform_.rotate_.y = XMConvertToDegrees(angle_[1]);
            if (Input::IsKey(DIK_W))
            {
                XMVECTOR vectorMove = XMLoadFloat3(&transform_.position_) + (vecMove_[1] / 4);
                XMStoreFloat3(&transform_.position_, vectorMove);
            }
            if (Input::IsKey(DIK_S))
            {
                XMVECTOR vectorMove = XMLoadFloat3(&transform_.position_) - (vecMove_[1] / 4);
                XMStoreFloat3(&transform_.position_, vectorMove);
            }
            if (Input::IsKey(DIK_D))
            {
                XMMATRIX rotmat = XMMatrixRotationY(3.14 / 2);
                XMVECTOR tempvec = XMVector3Transform((vecMove_[1] / 4), rotmat);
                XMVECTOR vectorMove = XMLoadFloat3(&transform_.position_) + tempvec;
                XMStoreFloat3(&transform_.position_, vectorMove);
            }
            if (Input::IsKey(DIK_A))
            {
                XMMATRIX rotmat = XMMatrixRotationY(3.14 / 2);
                XMVECTOR tempvec = XMVector3Transform((vecMove_[1] / 4), -rotmat);
                XMVECTOR vectorMove = XMLoadFloat3(&transform_.position_) + tempvec;
                XMStoreFloat3(&transform_.position_, vectorMove);
            }
            if (Input::IsKeyDown(DIK_SPACE) && jumpFlg_ == false)
            {
               PlayerJump();
            }
            if (Input::IsKey(DIK_LSHIFT) && jumpFlg_ == false)
            {
                playerState_ = RUN;
            }
        }
    }

    if (jumpFlg_ == true)
    {
        playerState_ = JUMP;
    }
}

void Player::PlayerJump()
{
    //ジャンプの処理
    jumpFlg_ = true;
    moveYPrev_ = posY_;
    posY_ = posY_ + 0.3;
}

void Player::PlayerRayCast()
{
    RayCastData upData;
    RayCastData downFloorData;
    RayCastData downData;
    RayCastData frontData;
    RayCastData backData;
    RayCastData leftData;
    RayCastData rightData;
    Stage* pStage = (Stage*)FindObject("Stage");    //ステージオブジェクト
    int hStageModel_ = pStage->GetModelHandle();    //モデル番号を取得

    Floor* pFloor = (Floor*)FindObject("Floor");
    int hFloorModel_ = pFloor->GetModelHandle();

    if (jumpFlg_ == true || transform_.position_.y >= 10)
    {
        //放物線に下がる処理
        moveYTemp_ = posY_;
        posY_ += (posY_ - moveYPrev_) - 0.007;
        moveYPrev_ = moveYTemp_;
        if (posY_ <= -rayFloorDist_ + 0.6)
        {
            jumpFlg_ = false;
        }
        if (posY_ <= -rayGravityDist_ + 0.6)
        {
            jumpFlg_ = false;
        }
    }

    //▼上の法線(すり抜け床のため)
    upData.start = transform_.position_;       //レイの発射位置
    upData.dir = XMFLOAT3(0, 1, 0);            //レイの方向
    Model::RayCast(hFloorModel_, &upData);  //レイを発射
    rayUpDist_ = upData.dist;
    //ImGui::Text("rayUpDist_=%f", rayUpDist_);

    //▼下の法線(すり抜け床)
    downFloorData.start = transform_.position_;    //レイの発射位置
    downFloorData.start.y = 0.0f;
    downFloorData.dir = XMFLOAT3(0, -1, 0);        //レイの方向
    if (upData.dist == 99999)
    {
        Model::RayCast(hFloorModel_, &downFloorData);  //レイを発射
    }
    rayFloorDist_ = downFloorData.dist;
    //ImGui::Text("rayFloorDist_=%f", rayFloorDist_);

    if (downFloorData.hit == true)
    {
        if (jumpFlg_ == false)
        {
            posY_ = -downFloorData.dist + 0.6f;
            isFloor_ = 1;
        }
    }
    else
    {
        isFloor_ = 0;
    }

    //▼下の法線(床に張り付き)
    downData.start = transform_.position_;  //レイの発射位置
    downData.start.y = 0;
    downData.dir = XMFLOAT3(0, -1, 0);       //レイの方向
    Model::RayCast(hStageModel_, &downData);  //レイを発射
    rayGravityDist_ = downData.dist;
    float playerFling = 1;
    //プレイヤーが浮いていないとき
    //ImGui::Text("rayGravityDist_=%f", rayGravityDist_);
    if (rayGravityDist_ + posY_ <= playerFling)
    {
        //ジャンプしてない＆すり抜け床の上にいない
        if (jumpFlg_ == false && isFloor_ == 0)
        {
            //地面に張り付き
            posY_ = -downData.dist + 0.6;
            moveYTemp_ = posY_;
            moveYPrev_ = moveYTemp_;
        }
    }
    else if (isFloor_ == 0)
    {
        jumpFlg_ = true;
    }
    //▼前の法線(壁の当たり判定)
    frontData.start = transform_.position_;       //レイの発射位置
    frontData.dir = XMFLOAT3(0, 1, 1);            //レイの方向
    Model::RayCast(hStageModel_, &frontData);  //レイを発射
    rayFrontDist_ = frontData.dist;
    //ImGui::Text("rayFrontDist_=%f", rayFrontDist_);
    if (rayFrontDist_ <= 1.5f)
    {
        transform_.position_.z = (float)((int)prevPosition_.z) - 0.00001f;
    }
    //▼後ろの法線(壁の当たり判定)
    backData.start = transform_.position_;       //レイの発射位置
    backData.dir = XMFLOAT3(0, 1, -1);           //レイの方向
    Model::RayCast(hStageModel_, &backData);  //レイを発射
    rayBackDist_ = backData.dist;
    //ImGui::Text("rayBackDist_=%f", rayBackDist_);
    if (rayBackDist_ <= 1.5f)
    {
        transform_.position_.z = (float)((int)prevPosition_.z) + 0.00001f;
    }
    //▼左の法線(壁の当たり判定)
    leftData.start = transform_.position_;       //レイの発射位置
    leftData.dir = XMFLOAT3(-1, 1, 0);           //レイの方向
    Model::RayCast(hStageModel_, &leftData);  //レイを発射
    rayLeftDist_ = leftData.dist;
    //ImGui::Text("rayLeftDist_=%f", rayLeftDist_);
    if (rayLeftDist_ <= 1.5f)
    {
        transform_.position_.x = (float)((int)prevPosition_.x) + 0.00001f;
    }
    //▼右の法線(壁の当たり判定)
    rightData.start = transform_.position_;       //レイの発射位置
    rightData.dir = XMFLOAT3(1, 1, 0);           //レイの方向
    Model::RayCast(hStageModel_, &rightData);  //レイを発射
    rayRightDist_ = rightData.dist;
    //ImGui::Text("rayRightDist_=%f", rayRightDist_);
    if (rayRightDist_ <= 1.5f)
    {
        transform_.position_.x = (float)((int)prevPosition_.x) - 0.00001f;
    }
    prevPosition_ = transform_.position_;
    prevIsFloor_ = isFloor_;
}

bool Player::IsMoving()
{
    return (transform_.position_.x != prevPosition_.x || transform_.position_.z != prevPosition_.z);
}
