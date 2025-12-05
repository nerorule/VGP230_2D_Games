#include "Player.h"
#include "TileMap.h"
#include "CollisionManager.h"
#include "BulletPool.h"
#include "Bullet.h"

Player::Player()
	: Entity()
	, Collidable()
	, mImageId(0)
	, mPosition(0.0f, 0.0f)
	, mHealth(100)
	, mRemoveCollider(false)
	, mFacing(0.0f, -1.0f) 

{
}

Player::~Player()
{

}

void Player::Load()
{
	mImageId = X::LoadTexture("stone.png");

	const Tile* safeTile = TileMap::Get()->GetFirstWalkableTile();
	mPosition = safeTile->GetPosition();

	float halfWidth = X::GetSpriteWidth(mImageId) * 0.5f;
	float halfHeight = X::GetSpriteHeight(mImageId) * 0.5f;
	mPlayerRect.left = -halfWidth;
	mPlayerRect.right = halfWidth;
	mPlayerRect.top = -halfHeight;
	mPlayerRect.bottom = halfHeight;

	//Collider Stufff
	SetRect(mPlayerRect);
	SetCollisionFilter(ET_ENEMY | ET_PICKUP);
	mRemoveCollider = false;
	CollisionManager::Get()->AddCollidable(this);
}

void Player::Update(float deltaTime)
{
	if (mHealth <= 0)
	{
		if (mRemoveCollider)
		{
			CollisionManager::Get()->RemoveCollidable(this);
			mRemoveCollider = false;
		}
		return;
	}

	const float speed = 200.0f;
	X::Math::Vector2 direction = X::Math::Vector2::Zero();
	X::Math::Vector2 displacement = X::Math::Vector2::Zero();
	if (X::IsKeyDown(X::Keys::W))
	{
		direction.y -= 1.0f;
	}
	if (X::IsKeyDown(X::Keys::S))
	{
		direction.y += 1.0f;
	}
	if (X::IsKeyDown(X::Keys::A))
	{
		direction.x -= 1.0f;
	}
	if (X::IsKeyDown(X::Keys::D))
	{
		direction.x += 1.0f;
	}
	if (X::Math::MagnitudeSqr(direction) > 0.0f)
	{
		direction = X::Math::Normalize(direction);
		// update facing to last movement direction
		mFacing = direction;
		displacement = direction * speed * deltaTime;
		X::Math::Vector2 maxDisplacement = displacement;
		X::Math::Rect currentRect = mPlayerRect;
		currentRect.min += mPosition;
		currentRect.max += mPosition;
		if (TileMap::Get()->HasCollision(currentRect, maxDisplacement, displacement))
		{
			mPosition += displacement;
		}
		else
		{
			mPosition += displacement;
		}

		currentRect = mPlayerRect;
		currentRect.min += mPosition;
		currentRect.max += mPosition;
		SetRect(currentRect);
	}
	// Shooting: SPACE fires a bullet in the facing direction
	if (X::IsKeyPressed(X::Keys::SPACE))
	{
		Bullet* bullet = BulletPool::Get()->GetBullet();
		if (bullet)
		{
			bullet->SetEntityType(ET_BULLET_PLAYER);
			bullet->SetDamage(10);
			// spawn slightly in front of player
			float offset = X::GetSpriteHeight(mImageId) * 0.5f + 8.0f;
			X::Math::Vector2 spawnPos = mPosition + (mFacing * offset);
			float rotation = atan2(mFacing.x, -mFacing.y);
			bullet->SetActive(spawnPos, rotation, 2.0f);
		}
	}
}

void Player::Render()
{
	if (mHealth > 0)
	{
		X::DrawSprite(mImageId, mPosition);
	}
}

void Player::Unload()
{

}

int Player::GetType() const
{
	return ET_PLAYER;
}

void Player::OnCollision(Collidable* collidable)
{
	// DONT REMOVE COLLIDABKES DURING THIS FUNCTION
	if (mHealth <= 0)
	{
		return;
	}

	if (collidable->GetType() == ET_ENEMY)
	{
		mHealth -= 10;

	}
	else if (collidable->GetType() == ET_PICKUP)
	{
		mHealth += 20;
	}
	mHealth = X::Math::Clamp(mHealth, 0, 100);
	if (mHealth <= 0)
	{
		mRemoveCollider = true;
	}
}

const X::Math::Vector2& Player::GetPosition() const
{
	return mPosition;
}



