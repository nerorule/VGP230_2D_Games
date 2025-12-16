#include "Enemy.h"
#include "Enum.h"
#include "TileMap.h"
#include "CollisionManager.h"
#include "Bullet.h"
#include "Player.h"
#include "GameController.h"
#include "PickupManager.h"

Enemy::Enemy()
	: Entity()
	, Collidable()
	, mImageId(0)
	, mPosition(0.0f, 0.0f)
	, mCenterPoint(0.0f, 0.0f)
	, mTargetPoint(0.0f, 0.0f)
	, mTargetPointUpdate(0.0f)
	, mHealth(0)
	, mRemoveCollider(false)
	, mEnemyRect(0, 0, 0, 0)
{
}

Enemy::~Enemy()
{
}

void Enemy::Load()
{
	mImageId = X::LoadTexture("Enemy.png");
	mTargetPointUpdate = 0.0f;
	mHealth = -1;
	mRemoveCollider = false;

	float halfWidth = X::GetSpriteWidth(mImageId) * 0.5f;
	float halfHeight = X::GetSpriteHeight(mImageId) * 0.5f;
	mEnemyRect.left = -halfWidth;
	mEnemyRect.right = halfWidth;
	mEnemyRect.top = -halfHeight;
	mEnemyRect.bottom = halfHeight;
}

void Enemy::Update(float deltaTime)
{
	if (mRemoveCollider)
	{
		CollisionManager::Get()->RemoveCollidable(this);
		mRemoveCollider = false;
	}

	if (!IsActive())
	{
		return;
	}

	const float speed = 70.0f;

	// chase the player: always set target to player's current position
	if (GameController::Get())
	{
		mTargetPoint = GameController::Get()->GetPlayerPosition();
	}

	X::Math::Vector2 direction = X::Math::Normalize(mTargetPoint - mPosition);
	if (X::Math::Vector2::SqrMagnitude(direction) > 0.0f)
	{
		X::Math::Vector2 displacement = direction * speed * deltaTime;
		X::Math::Vector2 maxDisplacement = displacement;
		X::Math::Rect currentRect = mEnemyRect;
		currentRect.min += mPosition;
		currentRect.max += mPosition;
		if (TileMap::Get()->HasCollision(currentRect, maxDisplacement, displacement))
		{
			mPosition += displacement;
			if (X::Math::Vector2::SqrMagnitude(displacement) <= 10.0f)
			{
				// if stuck, nothing special for now
			}
		}
		else
		{
			mPosition += displacement;
		}
		currentRect = mEnemyRect;
		currentRect.min += mPosition;
		currentRect.max += mPosition;
		SetRect(currentRect);
	}
}

void Enemy::Render()
{
	if (IsActive())
	{
		// rotate enemy to face the player
		float rotation = 0.0f;
		if (GameController::Get())
		{
			const X::Math::Vector2& playerPos = GameController::Get()->GetPlayerPosition();
			X::Math::Vector2 toPlayer = playerPos - mPosition;
			if (X::Math::Vector2::SqrMagnitude(toPlayer) > 0.0f)
			{
				toPlayer = X::Math::Normalize(toPlayer);
				rotation = atan2(toPlayer.x, -toPlayer.y); 
			}
		}
		X::DrawSprite(mImageId, mPosition, rotation);
	}
}

void Enemy::Unload()
{
}

int Enemy::GetType() const
{
	return (IsActive()) ? ET_ENEMY : ET_NONE;
}

const X::Math::Vector2& Enemy::GetPosition() const
{
	return mPosition;
}

void Enemy::OnCollision(Collidable* collidable)
{
	if (!IsActive())
	{
		return;
	}

	int type = collidable->GetType();
	if (type == ET_PLAYER)
	{
		// apply 1 damage to the player (if possible) then kill this enemy
		Player* player = dynamic_cast<Player*>(collidable);
		if (player)
		{
			player->TakeDamage(1);
		}
		if (PickupManager::Get() && X::Random(0, 1) == 0)
		{
			PickupManager::Get()->SpawnPickupAt(mPosition);
		}
		// destroy self
		mHealth = -1;
		mRemoveCollider = true;
	}
	else if (type == ET_BULLET_PLAYER)
	{
		// apply damage from the bullet
		Bullet* bullet = dynamic_cast<Bullet*>(collidable);
		if (bullet)
		{
			mHealth -= bullet->GetDamage();
		}
		else
		{
			// unknown collidable type, just kill
			mHealth = -1;
		}

		if (mHealth <= 0)
		{
			if (PickupManager::Get() && X::Random(0, 1) == 0)
			{
				PickupManager::Get()->SpawnPickupAt(mPosition);
			}
			mHealth = -1;
			mRemoveCollider = true;
		}
	}
}

bool Enemy::IsActive() const
{
	return mHealth > 0;
}

void Enemy::SetActive(const X::Math::Vector2& position, int health)
{

	mPosition = position;
	mCenterPoint = position;
	mTargetPoint = position;
	mTargetPointUpdate = 0.0f;
	mHealth = health;

	X::Math::Rect currentRect = mEnemyRect;
	currentRect.min += mPosition;
	currentRect.max += mPosition;
	SetRect(currentRect); // was SetRect(mEnemyRect);
	SetCollisionFilter(ET_ENEMY);

	CollisionManager::Get()->AddCollidable(this);
	mRemoveCollider = false;
}