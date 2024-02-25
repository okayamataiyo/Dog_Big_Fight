#include "Player.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Engine/Direct3D.h"
#include "Stage.h"
#include "Engine/ImGui/imgui.h"

Player::Player(GameObject* _pParent)
    :GameObject(_pParent, "Player"), TimeCounter_(0), hModel_{ -1 }, camType_(0), playerNum_(0), jumpFlg_(false), State_(READY), isFloor_(0)
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
    Model::SetAnimFrame(hModel_, 1, 60, 1);
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
    switch (State_)
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
        State_ = STATE::PLAY;
        TimeCounter_ = 0;
    }
}

void Player::UpdatePlay()
{
    PlayerGravity();
    PlayerMove();
    ImGui::Text("AnimFrame = %i", Model::GetAnimFrame(hModel_));
    ImGui::Text("moveYPrev_=%f", moveYPrev_);
    ImGui::Text("moveYTemp_=%f", moveYTemp_);
    ImGui::Text("Transform_.position_.x=%f", transform_.position_.x);
    ImGui::Text("Transform_.position_.y=%f", transform_.position_.y);
    ImGui::Text("Transform_.position_.z=%f", transform_.position_.z);
    ImGui::Text("prevPosition_.x=%f", prevPosition_.x);
    ImGui::Text("prevPosition_.y=%f", prevPosition_.y);
    ImGui::Text("prevPosition_.z=%f", prevPosition_.z);
    ImGui::Text("posY_=%f", posY_);
    ImGui::Text("jumpFlg_=%s", jumpFlg_ ? "true" : "false");
    ImGui::Text("angle_=%f", angle_);
    PlayerWall();
}

void Player::UpdateGameOver()
{
    ++TimeCounter_;
    if (Input::IsKey(DIK_SPACE))
    {
        SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
//        pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
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
        if (State_ != STATE::GAMEOVER)
        {
            Direct3D::SetIsChangeView(1);
            State_ = STATE::GAMEOVER;
        }
    }
}

void Player::PlayerMove()
{
    if (jumpFlg_ == false)
    {
        //        velocity_.x *= 0.9f;    //X�������̊���
        //        velocity_.z *= 0.9f;    //Z�������̊���
    }
    else
    {
        //        velocity_.x *= 0.97f;
        //        velocity_.z *= 0.97f;
    }


    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));

    XMVECTOR vecPos = XMLoadFloat3(&transform_.position_);
    //XMVECTOR vecMove = camera_.GetPosition(0);
    //vecMove = XMVector3TransformCoord(vecMove, mRotY);

    //transform_.position_.x += velocity_.x;
    //transform_.position_.z += velocity_.z;
    transform_.position_.y = posY_;
    XMVECTOR vecCam = -Camera::VecGetPosition(0) - Camera::VecGetTarget(0);
    vecCam = XMVector3Normalize(vecCam);
    //vecMove_ = XMLoadFloat3(&velocity_);
    //vecMove_ = XMVector3Normalize(vecMove_);
    vecMove_ = vecCam;
    vecMove_ *= 0.005f;

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

        transform_.rotate_.y = XMConvertToDegrees(angle_);
    }

    if (Input::IsKey(DIK_LSHIFT))
    {
        if (jumpFlg_ == false)
        {
            //            velocity_.x = velocity_.x * 1.1;
            //            velocity_.z = velocity_.z * 1.1;
            GameSta_ = RUN;
        }
    }

    if (this->GetObjectName() == "PlayerSeconds")
    {
        if (Input::IsKey(DIK_W))
        {
            XMVECTOR vectorMove = vecMove_ + XMLoadFloat3(&transform_.position_);

            XMStoreFloat3(&transform_.position_, vectorMove);
        }
        if (Input::IsKey(DIK_S))
        {
            XMVECTOR vectorMove = XMLoadFloat3(&transform_.position_) - vecMove_;

            XMStoreFloat3(&transform_.position_, vectorMove);
        }

        if (Input::IsKey(DIK_D))
        {
            //XMMatrixRotationY = Y���W�𒆐S�ɉ�]������s������֐�
            //XMConvertToRadians = degree�p��radian�p��(����)�ϊ�����
            XMMATRIX rotmat = XMMatrixRotationY(3.14 / 2);
            XMVECTOR tempvec = XMVector3Transform(vecMove_, rotmat);
            XMVECTOR vectorMove = XMLoadFloat3(&transform_.position_) + tempvec;
            XMStoreFloat3(&transform_.position_, vectorMove);
        }
        if (Input::IsKey(DIK_A))
        {
            XMMATRIX rotmat = XMMatrixRotationY(3.14 / 2);
            XMVECTOR tempvec = XMVector3Transform(vecMove_, -rotmat);
            XMVECTOR vectorMove = XMLoadFloat3(&transform_.position_) + tempvec;
            XMStoreFloat3(&transform_.position_, vectorMove);
        }
        if (Input::IsKey(DIK_SPACE) && jumpFlg_ == false)
        {
            PlayerJump();
        }
    }
    if (this->GetObjectName() == "PlayerFirst")
    {
        transform_.position_.x += Input::GetPadStickL().x / 2;
        transform_.position_.z += Input::GetPadStickL().y / 2;
        if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A) && jumpFlg_ == false)
        {
            PlayerJump();
        }
    }
}

void Player::PlayerJump()
{
    //�W�����v�̏���
    jumpFlg_ = true;
    moveYPrev_ = posY_;
    posY_ = posY_ + 0.3;
}

void Player::PlayerWall()
{

}

void Player::PlayerGravity()
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