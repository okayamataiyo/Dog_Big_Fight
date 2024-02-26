#include "Player.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Engine/Direct3D.h"
#include "Engine/ImGui/imgui.h"
#include "Stage.h"

Player::Player(GameObject* _pParent)
    :GameObject(_pParent, "Player"), TimeCounter_(0), hModel_{ -1 }, camType_(0), playerNum_(0), jumpFlg_(false), 
    state_(READY),gameState_(WAIT), isFloor_(0)
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
        pCollision_ = new SphereCollider(XMFLOAT3(0.0, 0.0, 0.0), 1);
        AddCollider(pCollision_);
        Stage* pStage = (Stage*)FindObject("Stage");    //�X�e�[�W�I�u�W�F�N�g
        hStageModel_[i] = pStage->GetModelHandle(i);   //���f���ԍ����擾
    }
}

void Player::Update()
{
    switch (state_)
    {
    case STATE::READY:     UpdateReady();    break;
    case STATE::PLAY:      UpdatePlay();     break;
    case STATE::GAMEOVER:  UpdateGameOver(); break;
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
        state_ = STATE::PLAY;
        TimeCounter_ = 0;
    }
}

void Player::UpdatePlay()
{
    if (prevState_ != gameState_)
    {
        switch (gameState_)
        {
        case GAMESTATE::WAIT:       Model::SetAnimFrame(hModel_, 0, 0, 1); break;
        case GAMESTATE::WALK:       Model::SetAnimFrame(hModel_, 20, 60, 0.5); break;
        case GAMESTATE::RUN:        Model::SetAnimFrame(hModel_, 80, 120, 0.5); break;
        case GAMESTATE::JUMP:       Model::SetAnimFrame(hModel_, 120, 120, 1); break;
        }
    }
    prevState_ = gameState_;

    PlayerMove();
    PlayerRayCast();
    ImGui::Text("gameState_=%i", gameState_);
    //ImGui::Text("moveYPrev_=%f", moveYPrev_);
    //ImGui::Text("moveYTemp_=%f", moveYTemp_);
    ImGui::Text("Transform_.position_.x=%f", transform_.position_.x);
    ImGui::Text("Transform_.position_.y=%f", transform_.position_.y);
    ImGui::Text("Transform_.position_.z=%f", transform_.position_.z);
    ImGui::Text("prevPosition_.x=%f", prevPosition_.x);
    ImGui::Text("prevPosition_.y=%f", prevPosition_.y);
    ImGui::Text("prevPosition_.z=%f", prevPosition_.z);
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
        if (state_ != STATE::GAMEOVER)
        {
            Direct3D::SetIsChangeView(1);
            state_ = STATE::GAMEOVER;
        }
    }
}

void Player::PlayerMove()
{
    if (IsMoving())
    {
        gameState_ = WALK;
    }
    else if (jumpFlg_ == false)
    {
        gameState_ = WAIT;
    }
    XMVECTOR vecPos = XMLoadFloat3(&transform_.position_);
    transform_.position_.y = posY_;
    for (int i = 0u; i <= 1; i++)
    {
        XMVECTOR vecCam[2] = {};
        vecCam[i] = -(Camera::VecGetPosition(i) - Camera::VecGetTarget(i));
        vecCam[i] = XMVector3Normalize(vecCam[i]);
        vecMove_[i] = vecCam[i];
        vecMove_[i] *= 0.005f;

        //�����ύX
        vecLength_ = XMVector3Length(vecMove_[i]);
        length_ = XMVectorGetX(vecLength_);

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
            if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A) && jumpFlg_ == false)
            {
                PlayerJump();
            }
            if (Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
            {
                if (jumpFlg_ == false)
                {
                    gameState_ = RUN;
                }
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
            if (Input::IsKey(DIK_SPACE) && jumpFlg_ == false)
            {
                PlayerJump();
            }
            if (Input::IsKey(DIK_LSHIFT))
            {
                if (jumpFlg_ == false)
                {
                    gameState_ = RUN;
                }
            }
        }
    }

    if (jumpFlg_ == true)
    {
        gameState_ = JUMP;
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
    prevPosition_ = transform_.position_;
    prevIsFloor_ = isFloor_;
    RayCastData upData;
    RayCastData downFloorData;
    RayCastData downData;
    RayCastData frontData;
    RayCastData backData;
    RayCastData leftData;
    RayCastData rightData;
    if (jumpFlg_ == true)
    {
        //�������ɉ����鏈��
        moveYTemp_ = posY_;
        posY_ += (posY_ - moveYPrev_) - 0.007;
        moveYPrev_ = moveYTemp_;
        if (posY_ <= -rayDownDist_ + 0.6)
        {
            jumpFlg_ = false;
        }
        if (posY_ <= -rayGravityDist_ + 0.6)
        {
            jumpFlg_ = false;
        }
    }

    //����̖@��(���蔲�����̂���)
    upData.start = transform_.position_;       //���C�̔��ˈʒu
    upData.dir = XMFLOAT3(0, 1, 0);            //���C�̕���
    Model::RayCast(hStageModel_[1], &upData);  //���C�𔭎�
    rayUpDist_ = upData.dist;

    //�����̖@��(���蔲����)
    downFloorData.start = transform_.position_;    //���C�̔��ˈʒu
    downFloorData.start.y = 0.0f;
    downFloorData.dir = XMFLOAT3(0, -1, 0);        //���C�̕���
    if (upData.dist == 99999)
    {
        Model::RayCast(hStageModel_[1], &downFloorData);  //���C�𔭎�
    }
    rayDownDist_ = downFloorData.dist;

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

    //�����̖@��(���ɒ���t��)
    downData.start = transform_.position_;  //���C�̔��ˈʒu
    downData.start.y = 0;
    downData.dir = XMFLOAT3(0, -1, 0);       //���C�̕���
    Model::RayCast(hStageModel_[0], &downData);  //���C�𔭎�
    rayGravityDist_ = downData.dist;
    float playerFling = 0.7;
    //�v���C���[�������Ă��Ȃ��Ƃ�
    ImGui::Text("rayGravityDist_=%f", rayGravityDist_);
    if (rayGravityDist_ + posY_ <= playerFling)
    {
        //�W�����v���ĂȂ������蔲�����̏�ɂ��Ȃ�
        if (jumpFlg_ == false && isFloor_ == 0)
        {
            //�n�ʂɒ���t��
            posY_ = -downData.dist + 0.6;
        }
    }
    else if (isFloor_ == 0)
    {
        jumpFlg_ = true;
    }
    //���O�̖@��(�ǂ̓����蔻��)
    frontData.start = transform_.position_;       //���C�̔��ˈʒu
    frontData.dir = XMFLOAT3(0, 1, 1);            //���C�̕���
    Model::RayCast(hStageModel_[0], &frontData);  //���C�𔭎�
    rayFrontDist_ = frontData.dist;
    ImGui::Text("rayFrontDist_=%f", rayFrontDist_);
    if (rayFrontDist_ <= 1.5f)
    {
        transform_.position_.z = (float)((int)prevPosition_.z) - 0.00001f;
    }
    //�����̖@��(�ǂ̓����蔻��)
    backData.start = transform_.position_;       //���C�̔��ˈʒu
    backData.dir = XMFLOAT3(0, 1, -1);           //���C�̕���
    Model::RayCast(hStageModel_[0], &backData);  //���C�𔭎�
    rayBackDist_ = backData.dist;
    ImGui::Text("rayBackDist_=%f", rayBackDist_);
    if (rayBackDist_ <= 1.5f)
    {
        transform_.position_.z = (float)((int)prevPosition_.z) + 0.00001f;
    }
    //�����̖@��(�ǂ̓����蔻��)
    leftData.start = transform_.position_;       //���C�̔��ˈʒu
    leftData.dir = XMFLOAT3(-1, 1, 0);           //���C�̕���
    Model::RayCast(hStageModel_[0], &leftData);  //���C�𔭎�
    rayLeftDist_ = leftData.dist;
    ImGui::Text("rayLeftDist_=%f", rayLeftDist_);
    if (rayLeftDist_ <= 1.5f)
    {
        transform_.position_.x = (float)((int)prevPosition_.x) + 0.00001f;
    }
    //���E�̖@��(�ǂ̓����蔻��)
    rightData.start = transform_.position_;       //���C�̔��ˈʒu
    rightData.dir = XMFLOAT3(1, 1, 0);           //���C�̕���
    Model::RayCast(hStageModel_[0], &rightData);  //���C�𔭎�
    rayRightDist_ = rightData.dist;
    ImGui::Text("rayRightDist_=%f", rayRightDist_);
    if (rayRightDist_ <= 1.5f)
    {
        transform_.position_.x = (float)((int)prevPosition_.x) - 0.00001f;
    }
}

bool Player::IsMoving()
{
    return (transform_.position_.x != prevPosition_.x || transform_.position_.z != prevPosition_.z);
}
