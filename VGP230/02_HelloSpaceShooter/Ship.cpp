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

	SetCollisionFilter(ET_ENEMY | ET_BULLET_ENEMY | ET_POWERUP);

	mHealth = mMaxHealth;
	mExplosion = new AnimSpriteSheet();
	mExplosion->Load();
}

void Ship::Update(float deltaTime)
{
	// Toggle firing mode with E
	if (X::IsKeyPressed(X::Keys::E))
	{
		ToggleFireMode();
	}

	// update timers
	if (mSpeedTimer > 0.0f)
	{
		mSpeedTimer -= deltaTime;
		if (mSpeedTimer <= 0.0f)
		{
			mSpeedMultiplier = 1.0f;
		}
	}
	if (mShieldTimer > 0.0f)
	{
		mShieldTimer -= deltaTime;
		if (mShieldTimer <= 0.0f)
		{
			mShieldActive = false;
		}
	}
	if (mWeaponTimer > 0.0f)
	{
		mWeaponTimer -= deltaTime;
		if (mWeaponTimer <= 0.0f)
		{
			mBulletDamage = mBaseBulletDamage;
		}
	}

	// Use the ship's base speed and current multiplier (so speed power-up works)
	const float speed = mBaseSpeed * mSpeedMultiplier;
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

		if (mFireMode == FireMode::Single)
		{
			Bullet* bullet = mBulletPool->GetBullet();
			if (bullet)
			{
				bullet->SetEntityType(ET_BULLET_PLAYER);
				bullet->SetDamage(mBulletDamage);
				bullet->SetActive(spawnPos, mRotation);
			}
		}
		else // Burst: 3 bullets in a cone (center, left, right)
		{
			const float spread = X::Math::kPi / 18.0f; // ~10 degrees
			float angles[3] = { mRotation, mRotation - spread, mRotation + spread };
			for (int i = 0; i < 3; ++i)
			{
				Bullet* bullet = mBulletPool->GetBullet();
				if (bullet)
				{
					bullet->SetEntityType(ET_BULLET_PLAYER);
					bullet->SetDamage(mBulletDamage);
					bullet->SetActive(spawnPos, angles[i]);
				}
			}
		}
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
	if (!IsAlive())
	{
		return;
	}

	// Only handle damage from enemies and enemy bullets.
	if (collidable->GetType() == ET_ENEMY)
	{
		mHealth -= 10;
	}
	else if (collidable->GetType() == ET_BULLET_ENEMY)
	{
		// if shield active, ignore enemy bullets
		if (!mShieldActive)
		{
			mHealth -= 2;
		}
	}
	else
	{
		// ignore other collision types (powerups handled by PowerUp::OnCollision)
		return;
	}

	if (!IsAlive())
	{
		mExplosion->SetActive(mPosition);
		SetCollisionFilter(0);

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

void Ship::ApplySpeedBoost(float multiplier, float duration)
{
	mSpeedMultiplier = multiplier;
	mSpeedTimer = duration;
}

void Ship::ApplyShield(float duration)
{
	mShieldActive = true;
	mShieldTimer = duration;
}

void Ship::ApplyWeaponUp(int extraDamage, float duration)
{
	mBulletDamage = mBaseBulletDamage + extraDamage;
	mWeaponTimer = duration;
}

bool Ship::IsShieldActive() const
{
	return mShieldActive;
}

void Ship::ToggleFireMode()
{
	mFireMode = (mFireMode == FireMode::Single) ? FireMode::Burst : FireMode::Single;
}

Ship::FireMode Ship::GetFireMode() const
{
	return mFireMode;
}