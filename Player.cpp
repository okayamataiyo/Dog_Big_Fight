#include "Engine/SceneManager.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Engine/Direct3D.h"
#include "Engine/ImGui/imgui.h"
#include "Engine/Text.h"
#include "Player.h"
#include "Stage.h"
#include "Floor.h"
#include "WoodBox.h"

Player::Player(GameObject* _pParent)
    :GameObject(_pParent, "Player"), timeCounter_(0), hModel_{ -1 }
, isJump_(false), isOnFloor_(0), isDash_(false), isStun_(0), isKnockBack_(0), number_(0), mouseMoveSpeed_(0.3f), controllerMoveSpeed_(0.3f)
    ,gameState_(GAMESTATE::READY), playerState_(PLAYERSTATE::WAIT),woodBoxName_("WoodBox"),woodBoxNumber_("WoodBox0"), pPlayer_(nullptr),pParent_(nullptr),pPlayScene_(nullptr),pText_(nullptr)
{
    pParent_ = _pParent;
}

Player::~Player()
{
}

void Player::Initialize()
{
    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("DogWalk.fbx");
    assert(hModel_ >= 0);
    transform_.scale_ = { 0.5,0.5,0.5 };
    posY_ = transform_.position_.y;
    prevPosition_ = transform_.position_;
    pCollision_ = new SphereCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), 1.0f);
    AddCollider(pCollision_);
    pPlayScene_ = (PlayScene*)FindObject("PlayScene");
    pText_ = new Text;
    pText_->Initialize();
}

void Player::Update()
{
    if (pPlayer_ == nullptr)
    {
        if (this->GetObjectName() == "PlayerSeconds")
        {
            pPlayer_ = (Player*)FindObject("PlayerFirst");
        }
        if (this->GetObjectName() == "PlayerFirst")
        {
            pPlayer_ = (Player*)FindObject("PlayerSeconds");
        }
    }
    switch (gameState_)
    {
    case GAMESTATE::READY:          UpdateReady();      break;
    case GAMESTATE::PLAY:           UpdatePlay();       break;
    case GAMESTATE::GAMEOVER:       UpdateGameOver();   break;
    }
}

void Player::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
    if (this->GetObjectName() == "PlayerSeconds")
    {
        pText_->Draw(30, 30,"Player1:Score=");
        pText_->Draw(280, 30, score_);
    }
    if (this->GetObjectName() == "PlayerFirst")
    {
        pText_->Draw(30, 60, "Player2:Score=");
        pText_->Draw(280, 60, score_);
    }
}

void Player::Release()
{
}

void Player::UpdateReady()
{
    ++timeCounter_;
    if (timeCounter_ >= 60)
    {
        gameState_ = GAMESTATE::PLAY;
        timeCounter_ = 0;
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
    PlayerKnockback();
    transform_.position_.y = posY_;
    if (isStun_ == 1)
    {
        timeCounter_++;
        if (timeCounter_ >= stunLimit_)
        {
            gameState_ = GAMESTATE::PLAY;
            isStun_ = 0;
            isKnockBack_ = false;
            timeCounter_ = 0;
        }
    }
    if (isStun_ == 0)
    {
        PlayerMove();
    }
    //ImGui::Text("playerState_=%i", playerState_);
    //ImGui::Text("posYPrev_=%f", posYPrev_);
    //ImGui::Text("posYTemp_=%f", posYTemp_);
    /*ImGui::Text("Transform_.position_.x=%f", transform_.position_.x);
    ImGui::Text("Transform_.position_.y=%f", transform_.position_.y);
    ImGui::Text("Transform_.position_.z=%f", transform_.position_.z);
    ImGui::Text("prevPosition_.x=%f", prevPosition_.x);
    ImGui::Text("prevPosition_.y=%f", prevPosition_.y);
    ImGui::Text("prevPosition_.z=%f", prevPosition_.z);*/
    ImGui::Text("angleDegrees_=%f", angleDegrees_);
    ImGui::Text("timeCounter_=%i", timeCounter_);
}

void Player::UpdateGameOver()
{
    if (Input::IsKey(DIK_SPACE))
    {
        SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
        pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
    }
}

void Player::Stun(int _timeLimit)
{
    //transform_.position_.y = posY_;
    isStun_ = 1;
    stunLimit_ = _timeLimit;
}

void Player::OnCollision(GameObject* _pTarget)
{
    if (_pTarget->GetObjectName() == "Item")
    {
        score_ += 10;
    }
    std::vector<int> woodBoxs = pPlayScene_->GetWoodBoxs();
    woodBoxNumber_ = woodBoxName_ + std::to_string(number_);
    if (_pTarget->GetObjectName() == woodBoxNumber_)
    {
        pWoodBox_ = (WoodBox*)FindObject(woodBoxNumber_);
        XMVECTOR vecPos = XMLoadFloat3(&transform_.position_) - pWoodBox_->GetVecPos();
        vecPos = XMVector3Normalize(vecPos);
        XMVECTOR vecUp = { 0,1,0,0 };
        dotProduct_ = XMVectorGetX(XMVector3Dot(vecPos,vecUp));
        float angleRadians = acosf(dotProduct_);
        angleDegrees_ = XMConvertToDegrees(angleRadians);
        if (angleDegrees_ <= 80)
        {
            PlayerJump();
            pWoodBox_->KillMe();
        }
    }
    //Item�Ƃ������O�����S�ẴI�u�W�F�N�g�̋@�\������
    if (_pTarget->GetObjectName().find("WoodBox") != std::string::npos)
    {
        if(angleDegrees_ > 80)
        {
            transform_.position_ = prevPosition_;
        }
    }
    number_++;
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
    if (_pTarget->GetObjectName() == "PlayerSeconds")
    {
        Stun(10);
        pPlayer_->Stun(10);
        isKnockBack_ = true;
    }
}

void Player::PlayerMove()
{
    for (int i = 0u; i <= 1; i++)
    {
        if (this->GetObjectName() == "PlayerFirst")
        {
            if (isDash_ == false)
            {
                //controllerMoveSpeed_ *= 0.9f;
            }
            else
            {
                //controllerMoveSpeed_ *= 1.1f;
            }
            if (!(Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER)))
            {
                XMVECTOR vecCam = {};
                vecCam = -(Camera::VecGetPosition(0) - Camera::VecGetTarget(0));
                XMFLOAT3 camRot = {};
                XMStoreFloat3(&camRot, vecCam);
                camRot.y = 0;
                vecCam = XMLoadFloat3(&camRot);
                vecCam = XMVector3Normalize(vecCam);
                vecMove_ = vecCam;
            }
        }

        if (this->GetObjectName() == "PlayerSeconds")
        {
            if (isDash_ == false)
            {
                //mouseMoveSpeed_ *= 0.9f;
            }
            else
            {
                //mouseMoveSpeed_ *= 1.1f;
            }
            if (!(Input::IsKey(DIK_F)))
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
        }
        //�����ύX
        vecLength_ = XMVector3Length(vecMove_);
        length_ = XMVectorGetX(vecLength_);
        if (length_ != 0)
        {
            //�v���C���[�����̓L�[�ɉ����āA���̌����ɕς���(�������ɂ͏o���Ȃ�)
            vecFront_ = { 0,0,1,0 };
            vecMove_ = XMVector3Normalize(vecMove_);

            vecDot_ = XMVector3Dot(vecFront_, vecMove_);
            dot_ = XMVectorGetX(vecDot_);
            angle_ = acos(dot_);

            //�E�����ɂ��������Ȃ��������̂��������ɂ��鎖���ł���
            vecCross_ = XMVector3Cross(vecFront_, vecMove_);
            if (XMVectorGetY(vecCross_) < 0)
            {
                angle_ *= -1;
            }
        }

        if (IsMoving())
        {
            playerState_ = PLAYERSTATE::WALK;
        }
        else if (isJump_ == false)
        {
            playerState_ = PLAYERSTATE::WAIT;
        }

        if (this->GetObjectName() == "PlayerFirst")
        {
            transform_.rotate_.y = XMConvertToDegrees(angle_);
            if (Input::GetPadStickL().y > 0.3)
            {
                XMVECTOR vecDirection = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(0);
                vecDirection = XMVector3Normalize(vecDirection);
                transform_.position_.x = transform_.position_.x + controllerMoveSpeed_ * XMVectorGetX(vecDirection);
                transform_.position_.z = transform_.position_.z + controllerMoveSpeed_ * XMVectorGetZ(vecDirection);
            }
            if (Input::GetPadStickL().y < -0.3)
            {
                XMVECTOR vecDirection = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(0);
                vecDirection = XMVector3Normalize(vecDirection);
                transform_.position_.x = transform_.position_.x + controllerMoveSpeed_ * XMVectorGetX(-vecDirection);
                transform_.position_.z = transform_.position_.z + controllerMoveSpeed_ * XMVectorGetZ(-vecDirection);
            }
            if (Input::GetPadStickL().x > 0.3)
            {
                XMMATRIX rotmat = XMMatrixRotationY(3.14 / 2);                          //XMMatrixRotationY = Y���W�𒆐S�ɉ�]������s������֐�,//XMConvertToRadians = degree�p��radian�p��(����)�ϊ�����
                XMVECTOR vecDirection = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(0);
                vecDirection = XMVector3Normalize(vecDirection);
                XMVECTOR tempvec = XMVector3Transform(vecDirection, rotmat);
                transform_.position_.x = transform_.position_.x + controllerMoveSpeed_ * XMVectorGetX(tempvec);
                transform_.position_.z = transform_.position_.z + controllerMoveSpeed_ * XMVectorGetZ(tempvec);
            }
            if (Input::GetPadStickL().x < -0.3)
            {
                XMMATRIX rotmat = XMMatrixRotationY(3.14 / 2);
                XMVECTOR vecDirection = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(0);
                vecDirection = XMVector3Normalize(vecDirection);
                XMVECTOR tempvec = XMVector3Transform(vecDirection, -rotmat);
                transform_.position_.x = transform_.position_.x + controllerMoveSpeed_ * XMVectorGetX(tempvec);
                transform_.position_.z = transform_.position_.z + controllerMoveSpeed_ * XMVectorGetZ(tempvec);
            }
            if (Input::IsPadButton(XINPUT_GAMEPAD_A) && isJump_ == false)
            {
                PlayerJump();
            }
            if (Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER) && isJump_ == false)
            {
                playerState_ = PLAYERSTATE::RUN;
                isDash_ = true;
            }
            else
            {
                isDash_ = false;
            }
        }
        if (this->GetObjectName() == "PlayerSeconds")
        {
            transform_.rotate_.y = XMConvertToDegrees(angle_);
            if (Input::IsKey(DIK_W))
            {
                XMVECTOR vecDirection = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(1);
                vecDirection = XMVector3Normalize(vecDirection);
                transform_.position_.x = transform_.position_.x + mouseMoveSpeed_ * XMVectorGetX(vecDirection);
                transform_.position_.z = transform_.position_.z + mouseMoveSpeed_ * XMVectorGetZ(vecDirection);
            }
            if (Input::IsKey(DIK_S))
            {
                XMVECTOR vecDirection = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(1);
                vecDirection = XMVector3Normalize(vecDirection);
                transform_.position_.x = transform_.position_.x + mouseMoveSpeed_ * XMVectorGetX(-vecDirection);
                transform_.position_.z = transform_.position_.z + mouseMoveSpeed_ * XMVectorGetZ(-vecDirection);
            }
            if (Input::IsKey(DIK_D))
            {
                XMMATRIX rotmat = XMMatrixRotationY(3.14 / 2);
                XMVECTOR vecDirection = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(1);
                vecDirection = XMVector3Normalize(vecDirection);
                XMVECTOR tempvec = XMVector3Transform(vecDirection, rotmat);
                transform_.position_.x = transform_.position_.x + mouseMoveSpeed_ * XMVectorGetX(tempvec);
                transform_.position_.z = transform_.position_.z + mouseMoveSpeed_ * XMVectorGetZ(tempvec);
            }
            if (Input::IsKey(DIK_A))
            {
                XMMATRIX rotmat = XMMatrixRotationY(3.14 / 2);
                XMVECTOR vecDirection = XMLoadFloat3(&transform_.position_) - Camera::VecGetPosition(1);
                vecDirection = XMVector3Normalize(vecDirection);
                XMVECTOR tempvec = XMVector3Transform(vecDirection, -rotmat);
                transform_.position_.x = transform_.position_.x + mouseMoveSpeed_ * XMVectorGetX(tempvec);
                transform_.position_.z = transform_.position_.z + mouseMoveSpeed_ * XMVectorGetZ(tempvec);
            }
            if (Input::IsKeyDown(DIK_SPACE) && isJump_ == false)
            {
               PlayerJump();
            }
            if (Input::IsKey(DIK_LSHIFT) && isJump_ == false)
            {
                playerState_ = PLAYERSTATE::RUN;
                isDash_ = true;
            }
            else
            {
                isDash_ = false;
            }
        }
    }

    if(isJump_ == true)
    {
        playerState_ = PLAYERSTATE::JUMP;
    }
}

void Player::PlayerJump()
{
    //�W�����v�̏���
    isJump_ = true;
    posYPrev_ = posY_;
    posY_ = posY_ + 0.3;
}

void Player::PlayerKnockback()
{
    if (isKnockBack_ == true)
    {
        XMVECTOR vecKnockbackDirection = (XMLoadFloat3(&transform_.position_) - pPlayer_->GetVecPos());
        vecKnockbackDirection = XMVector3Normalize(vecKnockbackDirection);
        float knockbackSpeed = 0.3f;
        SetKnockback(vecKnockbackDirection, knockbackSpeed);
        pPlayer_->SetKnockback(-vecKnockbackDirection, knockbackSpeed);
        Stun(30);
        pPlayer_->Stun(30);
    }
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
    float playerFling           = 1.0f;                             //�v���C���[���n�ʂ���ǂ̂��炢����Ă����畂���Ă��锻��ɂ��邩
    Stage* pStage               = (Stage*)FindObject("Stage");      //�X�e�[�W�I�u�W�F�N�g
    int hStageModel_            = pStage->GetModelHandle();         //���f���ԍ����擾
    Floor* pFloor               = (Floor*)FindObject("Floor");
    int hFloorModel_            = pFloor->GetModelHandle();
    if (isJump_ == true)
    {
        //�������ɉ����鏈��
        posYTemp_ = posY_;
        posY_ += (posY_ - posYPrev_) - 0.007;
        posYPrev_ = posYTemp_;
        if (posY_ <= -rayFloorDist_ + 0.6)
        {
            isJump_ = false;
        }
        if (posY_ <= -rayStageDist_ + 0.6)
        {
            isJump_ = false;
        }
    }

    for (int i = 0; i <= 2; i++)
    {
        //����̖@��(���蔲�����̂���)
        upData.start = transform_.position_;           //���C�̔��ˈʒu
        upData.dir = XMFLOAT3(0, 1, 0);                //���C�̕���
        Model::RayCast(hFloorModel_ + i, &upData);         //���C�𔭎�
        rayUpDist_ = upData.dist;
        //ImGui::Text("rayUpDist_=%f", rayUpDist_);

        //�����̖@��(���蔲����)
        downFloorData.start = transform_.position_;    //���C�̔��ˈʒu
        downFloorData.start.y = 0.0f;
        downFloorData.dir = XMFLOAT3(0, -1, 0);        //���C�̕���
        if (upData.dist == 99999)
        {
            Model::RayCast(hFloorModel_ + i, &downFloorData);  //���C�𔭎�
        }
        rayFloorDist_ = downFloorData.dist;
        //ImGui::Text("rayFloorDist_=%f", rayFloorDist_);
        if (rayFloorDist_ + posY_ <= playerFling)
        {
            if (isJump_ == false)
            {
                posY_ = -downFloorData.dist + 0.6f;
                isOnFloor_ = 1;
                posYTemp_ = posY_;
                posYPrev_ = posYTemp_;
            }
        }
        else
        {
            isOnFloor_ = 0;
        }

    }
    //�����̖@��(���ɒ���t��)
    downData.start = transform_.position_;  //���C�̔��ˈʒu
    downData.start.y = 0;
    downData.dir = XMFLOAT3(0, -1, 0);       //���C�̕���
    Model::RayCast(hStageModel_, &downData); //���C�𔭎�
    rayStageDist_ = downData.dist;
    //�v���C���[�������Ă��Ȃ��Ƃ�
    //ImGui::Text("rayGravityDist_=%f", rayGravityDist_);
    if (rayStageDist_ + posY_ <= playerFling)
    {
        //�W�����v���ĂȂ������蔲�����̏�ɂ��Ȃ�
        if (isJump_ == false && isOnFloor_ == 0)
        {
            //�n�ʂɒ���t��
            posY_ = -downData.dist + 0.6;
            posYTemp_ = posY_;
            posYPrev_ = posYTemp_;
        }
    }
    else if (isOnFloor_ == 0)
    {
        isJump_ = true;
    }
    //���O�̖@��(�ǂ̓����蔻��)
    frontData.start = transform_.position_;       //���C�̔��ˈʒu
    frontData.dir = XMFLOAT3(0, 1, 1);            //���C�̕���
    Model::RayCast(hStageModel_, &frontData);  //���C�𔭎�
    rayFrontDist_ = frontData.dist;
    //ImGui::Text("rayFrontDist_=%f", rayFrontDist_);
    if (rayFrontDist_ <= 1.5f)
    {
        //transform_.position_.z = (float)((int)prevPosition_.z) - 0.00001f;
        transform_.position_.z = prevPosition_.z;
    }
    //�����̖@��(�ǂ̓����蔻��)
    backData.start = transform_.position_;       //���C�̔��ˈʒu
    backData.dir = XMFLOAT3(0, 1, -1);           //���C�̕���
    Model::RayCast(hStageModel_, &backData);  //���C�𔭎�
    rayBackDist_ = backData.dist;
    //ImGui::Text("rayBackDist_=%f", rayBackDist_);
    if (rayBackDist_ <= 1.5f)
    {
        //transform_.position_.z = (float)((int)prevPosition_.z) + 0.00001f;
        transform_.position_.z = prevPosition_.z;
    }
    //�����̖@��(�ǂ̓����蔻��)
    leftData.start = transform_.position_;       //���C�̔��ˈʒu
    leftData.dir = XMFLOAT3(-1, 1, 0);           //���C�̕���
    Model::RayCast(hStageModel_, &leftData);  //���C�𔭎�
    rayLeftDist_ = leftData.dist;
    //ImGui::Text("rayLeftDist_=%f", rayLeftDist_);
    if (rayLeftDist_ <= 1.5f)
    {
        //transform_.position_.x = (float)((int)prevPosition_.x) + 0.00001f;
        transform_.position_.x = prevPosition_.x;
    }
    //���E�̖@��(�ǂ̓����蔻��)
    rightData.start = transform_.position_;       //���C�̔��ˈʒu
    rightData.dir = XMFLOAT3(1, 1, 0);           //���C�̕���
    Model::RayCast(hStageModel_, &rightData);  //���C�𔭎�
    rayRightDist_ = rightData.dist;
    //ImGui::Text("rayRightDist_=%f", rayRightDist_);
    if (rayRightDist_ <= 1.5f)
    {
        //transform_.position_.x = (float)((int)prevPosition_.x) - 0.00001f;
        transform_.position_.x = prevPosition_.x;
    }
    prevPosition_ = transform_.position_;
}

void Player::SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed)
{
    transform_.position_.x = transform_.position_.x + _knockbackSpeed * XMVectorGetX(_vecKnockbackDirection);
    transform_.position_.z = transform_.position_.z + _knockbackSpeed * XMVectorGetZ(_vecKnockbackDirection);
}

bool Player::IsMoving()
{
    return (transform_.position_.x != prevPosition_.x || transform_.position_.z != prevPosition_.z);
}
