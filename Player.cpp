#include "Engine/SceneManager.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Engine/Direct3D.h"
#include "Engine/ImGui/imgui.h"
#include "Player.h"
#include "Stage.h"
#include "Floor.h"
#include "WoodBox.h"

Player::Player(GameObject* _pParent)
    :GameObject(_pParent, "Player"), TimeCounter_(0), hModel_{ -1 },jumpFlg_(false), 
    gameState_(GAMESTATE::READY),playerState_(PLAYERSTATE::WAIT), isFloor_(0),isDash_(false)
{
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
    for (int i = 0u; i <= 1; i++)
    {
        pCollision_ = new BoxCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f,1.0f,2.0f));
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
    pCollision_->
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
    ImGui::Text("angleDegrees=%f", angleDegrees_);
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

    if (_pTarget->GetObjectName() == "WoodBox")
    {

        pWoodBox_ = (WoodBox*)FindObject("WoodBox");
        XMVECTOR vecPos = XMLoadFloat3(&transform_.position_) - pWoodBox_->GetVecPos();
        vecPos = XMVector3Normalize(vecPos);
        XMVECTOR vecUp = { 0,1,0,0 };
        dotProduct_ = XMVectorGetX(XMVector3Dot(vecPos,vecUp));
        float angleRadians = acosf(dotProduct_);
        angleDegrees_ = XMConvertToDegrees(angleRadians);
        if (angleDegrees_ <= 30)
        {
            PlayerJump();
        }
        else
        {
            transform_.position_ = prevPosition_;
        }
    }

    if (_pTarget->GetObjectName() == "PlayerSeconds")
    {
        if (gameState_ != GAMESTATE::GAMEOVER)
        {
            pPlayer_ = (Player*)FindObject("PlayerFirst");

        }
    }

    //Item�Ƃ������O�����S�ẴI�u�W�F�N�g�̋@�\������
//if (_pTarget->GetObjectName().find("Item") != std::string::npos)
//{

//}
}

void Player::PlayerMove()
{
    transform_.position_.y = posY_;
    for (int i = 0u; i <= 1; i++)
    {
        if (this->GetObjectName() == "PlayerFirst")
        {
            if (isDash_ == false)
            {
                vecMove_[0] *= 0.9f;
            }
            else
            {
                vecMove_[0] *= 1.1f;
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
                vecMove_[0] = vecCam;
            }
        }

        if (this->GetObjectName() == "PlayerSeconds")
        {
            if (isDash_ == false)
            {
                vecMove_[1] *= 0.9f;
            }
            else
            {
                vecMove_[1] *= 1.4f;
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
                vecMove_[1] = vecCam;
            }
        }
        //�����ύX
        vecLength_[1] = XMVector3Length(vecMove_[1]);
        length_[1] = XMVectorGetX(vecLength_[1]);
        if (length_ != 0)
        {
            //�v���C���[�����̓L�[�ɉ����āA���̌����ɕς���(�������ɂ͏o���Ȃ�)
            vecFront_ = { 0,0,1,0 };
            vecMove_[i] = XMVector3Normalize(vecMove_[i]);

            vecDot_[i] = XMVector3Dot(vecFront_, vecMove_[i]);
            dot_ = XMVectorGetX(vecDot_[i]);
            angle_[i] = acos(dot_);

            //�E�����ɂ��������Ȃ��������̂��������ɂ��鎖���ł���
            vecCross_ = XMVector3Cross(vecFront_, vecMove_[i]);
            if (XMVectorGetY(vecCross_) < 0)
            {
                angle_[i] *= -1;
            }
        }

        if (IsMoving())
        {
            playerState_ = PLAYERSTATE::WALK;
        }
        else if (jumpFlg_ == false)
        {
            playerState_ = PLAYERSTATE::WAIT;
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
                XMMATRIX rotmat = XMMatrixRotationY(3.14 / 2);                          //XMMatrixRotationY = Y���W�𒆐S�ɉ�]������s������֐�
                XMVECTOR tempvec = XMVector3Transform((vecMove_[0] / 4), rotmat);       //XMConvertToRadians = degree�p��radian�p��(����)�ϊ�����
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
            if (Input::IsPadButton(XINPUT_GAMEPAD_A) && jumpFlg_ == false)
            {
                PlayerJump();
            }
            if (Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER) && jumpFlg_ == false)
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
                playerState_ = PLAYERSTATE::RUN;
                isDash_ = true;
            }
            else
            {
                isDash_ = false;
            }
        }
    }

    if(jumpFlg_ == true)
    {
        playerState_ = PLAYERSTATE::JUMP;
    }
}

void Player::PlayerJump()
{
    //�W�����v�̏���
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
    float playerFling           = 1.0f;                             //�v���C���[���n�ʂ���ǂ̂��炢����Ă����畂���Ă��锻��ɂ��邩
    Stage* pStage               = (Stage*)FindObject("Stage");      //�X�e�[�W�I�u�W�F�N�g
    int hStageModel_            = pStage->GetModelHandle();         //���f���ԍ����擾
    Floor* pFloor               = (Floor*)FindObject("Floor");
    int hFloorModel_            = pFloor->GetModelHandle();
    if (jumpFlg_ == true || transform_.position_.y >= 10)
    {
        //�������ɉ����鏈��
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
            if (jumpFlg_ == false)
            {
                posY_ = -downFloorData.dist + 0.6f;
                isFloor_ = 1;
                moveYTemp_ = posY_;
                moveYPrev_ = moveYTemp_;
            }
        }
        else
        {
            isFloor_ = 0;
        }

    }
    //�����̖@��(���ɒ���t��)
    downData.start = transform_.position_;  //���C�̔��ˈʒu
    downData.start.y = 0;
    downData.dir = XMFLOAT3(0, -1, 0);       //���C�̕���
    Model::RayCast(hStageModel_, &downData);  //���C�𔭎�
    rayGravityDist_ = downData.dist;
    //�v���C���[�������Ă��Ȃ��Ƃ�
    //ImGui::Text("rayGravityDist_=%f", rayGravityDist_);
    if (rayGravityDist_ + posY_ <= playerFling)
    {
        //�W�����v���ĂȂ������蔲�����̏�ɂ��Ȃ�
        if (jumpFlg_ == false && isFloor_ == 0)
        {
            //�n�ʂɒ���t��
            posY_ = -downData.dist + 0.6;
            moveYTemp_ = posY_;
            moveYPrev_ = moveYTemp_;
        }
    }
    else if (isFloor_ == 0)
    {
        jumpFlg_ = true;
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
    prevIsFloor_ = isFloor_;
}

bool Player::IsMoving()
{
    return (transform_.position_.x != prevPosition_.x || transform_.position_.z != prevPosition_.z);
}
