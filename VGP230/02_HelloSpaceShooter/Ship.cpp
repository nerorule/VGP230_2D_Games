#include "Ship.h"
#include "Bullet.h"
#include "BulletPool.h"
#include "AnimSpriteSheet.h"

Ship::Ship()
	: Entity()
	, Collidable(30.0f)
	, mImageId(0)
	, mPosition(0.0f)
	, mRotation(0.0f)
	, mHealth(0)
	, mMaxHealth(100)
	, mExplosion(nullptr)
	, mBulletPool(nullptr)
{
	
}

Ship::~Ship()
{

}

void Ship::Load()
{
	mImageId = X::LoadTexture("fighter.png");
	XASSERT(mImageId != 0, "Ship: image not found");

	mPosition.x = X::GetScreenWidth() * 0.5f;
	mPosition.y = X::GetScreenHeight() * 0.5f;

	SetCollisionFilter(ET_ENEMY | ET_BULLET_ENEMY);
	
	mHealth = mMaxHealth;
	mExplosion = new AnimSpriteSheet();
	mExplosion->Load();
}

void Ship::Update(float deltaTime)
{
	const float speed = 100.0f;
	const float turnSpeed = X::Math::kPiByTwo;
	//Up-down movement
	if (X::IsKeyDown(X::Keys::W))
	{
		mPosition += X::Math::Vector2::Forward(mRotation) * speed * deltaTime;
	}
	else if (X::IsKeyDown(X::Keys::S))
	{
		mPosition -= X::Math::Vector2::Forward(mRotation) * speed * deltaTime;
	}

	//Left-right rotation
	if (X::IsKeyDown(X::Keys::A))
	{
		mRotation -= turnSpeed * deltaTime;
	}
	else if (X::IsKeyDown(X::Keys::D))
	{
		mRotation += turnSpeed * deltaTime;
	}

	//Shooting
	if (X::IsKeyPressed(X::Keys::SPACE))
	{
		X::Math::Vector2 spawnPos = mPosition + X::Math::Vector2::Forward(mRotation) * 50.0f;
		Bullet* bullet = mBulletPool->GetBullet();
		bullet->SetEntityType(ET_BULLET_PLAYER);
		bullet->SetActive(spawnPos, mRotation);
	}

	mExplosion->Update(deltaTime);
}

void Ship::Render()
{
	if (IsAlive()) 
	{
		X::DrawSprite(mImageId, mPosition, mRotation);
		X::DrawScreenCircle(mPosition, GetRadius(), X::Colors::Yellow);
	}
	mExplosion->Render();
}

void Ship::Unload()
{
	mExplosion->Unload();
	delete mExplosion;
	mExplosion = nullptr;
}

int Ship::GetType() const
{
	return ET_SHIP;
}

const X::Math::Vector2& Ship::GetPosition() const
{
	return mPosition;
}

void Ship::OnCollision(Collidable* collidable)
{
	if (IsAlive())
	{
		int damage = 0;
		if (collidable->GetType() == ET_ENEMY)
		{
			damage = 10;
		}
		else
		{
			damage = 2;
		}
		mHealth -= damage;
		if (!IsAlive())
		{
			mExplosion->SetActive(mPosition);
			SetCollisionFilter(0);
			
		}
	}
}

void Ship::SetBullePool(BulletPool* bulletPool)
{
	mBulletPool = bulletPool;
}

int Ship::GetHealth() const
{
	return mHealth;
}

int Ship::GetMaxHealth() const
{
	return mMaxHealth;
}

bool Ship::IsAlive() const
{
	return mHealth > 0;
}