#include "BossEnemy.h"
#include "BulletPool.h"
#include "Bullet.h"
#include "Ship.h"
#include "AnimSpriteSheet.h"
#include <XEngine.h>

BossEnemy::BossEnemy()
	: Enemy()
{
	// Enemy() already initializes most members; override boss-specific defaults:
	mHealth = 700;
}

BossEnemy::~BossEnemy()
{

}

void BossEnemy::Load()
{
	mImageId = X::LoadTexture("BattleCruiser.png");
	XASSERT(mImageId > 0, "Enemy: image not found");

	mPosition = X::Math::Vector2::Zero();
	mRotation = 0.0f;

	SetCollisionFilter(ET_BULLET_PLAYER);

	mExplosion = new AnimSpriteSheet();
	mExplosion->Load();

	mTargetPositionUpdate = 0.0f;
	mFireRate = 5.0f;
}

void BossEnemy::Update(float deltaTime)
{
	if (IsAlive())
	{
		const float speed = 70.0f;
		const float rotationSpeed = X::Math::kPiByTwo;
		const float offSetDistance = 200.0f;

		mTargetPositionUpdate -= deltaTime;
		if (mTargetPositionUpdate <= 0.0f || X::Math::MagnitudeSqr(mTargetPosition - mPosition) <= 100.0f)
		{
			mTargetPosition = mCenterPoint + (X::RandomUnitCircle() * offSetDistance);
			mTargetPositionUpdate = X::RandomFloat(3.0f, 5.0f);
		}

		X::Math::Vector2 moveDirection = X::Math::Normalize(mTargetPosition - mPosition);
		mPosition += moveDirection * speed * deltaTime;

		if (mShip != nullptr && mShip->IsAlive())
		{
			moveDirection = X::Math::Normalize(mShip->GetPosition() - mPosition);
			float targetRotation = atan2(moveDirection.x, -moveDirection.y);
			mRotation = targetRotation;

			mFireRate -= deltaTime;
			if (mFireRate <= 0.0f)
			{
				// Fire a burst of 3 bullets in a cone (center, left, right)
				const float spread = X::Math::kPi / 18.0f; // ~10 degrees
				float angles[3] = { mRotation, mRotation - spread, mRotation + spread };

				for (int i = 0; i < 3; ++i)
				{
					Bullet* bullet = mBulletPool->GetBullet();
					if (bullet != nullptr)
					{
						X::Math::Vector2 bulletPos = mPosition + X::Math::Vector2::Forward(angles[i]) * 50.0f;
						bullet->SetEntityType(ET_BULLET_ENEMY);
						bullet->SetActive(bulletPos, angles[i], 3.0f);
					}
				}

				// reset fire timer
				mFireRate = X::RandomFloat(1.0f, 3.0f);
			}
		}
	}
	mExplosion->Update(deltaTime);
}

void BossEnemy::Render()
{
	if (IsAlive())
	{
		X::DrawSprite(mImageId, mPosition, mRotation);
		X::DrawScreenCircle(mPosition, GetRadius(), X::Colors::AliceBlue);
	}
	mExplosion->Render();
}

void BossEnemy::Unload()
{
}

int BossEnemy::GetType() const
{
	return ET_ENEMY;
}

const X::Math::Vector2& BossEnemy::GetPosition() const
{
	return mPosition;
}

void BossEnemy::OnCollision(Collidable* collidable)
{
	if (IsAlive())
	{
		int damage = 0;
		if (collidable->GetType() == ET_SHIP)
		{
			damage = mHealth;
		}
		else
		{
			damage = 10;
		}
		mHealth -= damage;
		if (!IsAlive())
		{
			SetCollisionFilter(0);
			mExplosion->SetActive(mPosition);
		}
	}
}



