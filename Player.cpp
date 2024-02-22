#include "Player.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Engine/Direct3D.h"

Player::Player(GameObject* _pParent)
    :GameObject(_pParent, "Player")
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
    Model::SetAnimFrame(hModel_, 1, 60, 1);
}

void Player::Update()
{
    transform_.rotate_.y += 1;
}

void Player::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void Player::Release()
{
}