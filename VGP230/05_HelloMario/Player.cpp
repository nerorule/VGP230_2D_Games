#include "Player.h"
#include "TileMap.h"
#include "CollisionManager.h"
#include "BulletPool.h"
#include "Bullet.h"
#include "Pickup.h"

Player::Player()
	: Entity()
	, Collidable()
	, mImageId(0)
	, mPosition(0.0f, 0.0f)
	, mHealth(kMaxHealth)
	, mRemoveCollider(false)
	, mFacing(0.0f, -1.0f)
	, mShootCooldownTimer(0.0f)
	, mAmmo(kMaxAmmo)
{
}

Player::~Player()
{

}

void Player::Load()
{
	mImageId = X::LoadTexture("Character_2.png");

	const Tile* safeTile = TileMap::Get()->GetFirstWalkableTile();
	mPosition = safeTile->GetPosition();

	float halfWidth = X::GetSpriteWidth(mImageId) * 0.5f;
	float halfHeight = X::GetSpriteHeight(mImageId) * 0.5f;
	mPlayerRect.left = -halfWidth;
	mPlayerRect.right = halfWidth;
	mPlayerRect.top = -halfHeight;
	mPlayerRect.bottom = halfHeight;

	//Collider Stufff
	// set rect in world-space
	X::Math::Rect currentRect = mPlayerRect;
	currentRect.min += mPosition;
	currentRect.max += mPosition;
	SetRect(currentRect);

	SetCollisionFilter(ET_ENEMY | ET_PICKUP);
	mRemoveCollider = false;
	CollisionManager::Get()->AddCollidable(this);

	// initialize shoot cooldown and ammo
	mShootCooldownTimer = 0.0f;
	mAmmo = kMaxAmmo;
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

	// update shoot cooldown timer
	if (mShootCooldownTimer > 0.0f)
	{
		mShootCooldownTimer -= deltaTime;
		if (mShootCooldownTimer < 0.0f)
		{
			mShootCooldownTimer = 0.0f;
		}
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
	// Shooting: SPACE fires a bullet aimed at the mouse position with cooldown and limited ammo
	if (X::IsKeyPressed(X::Keys::SPACE) && mShootCooldownTimer <= 0.0f && mAmmo > 0)
	{
		Bullet* bullet = BulletPool::Get()->GetBullet();
		if (bullet)
		{
			bullet->SetEntityType(ET_BULLET_PLAYER);
			bullet->SetDamage(10);

			// get mouse position in screen/world coordinates
			X::Math::Vector2 mousePos = { static_cast<float>(X::GetMouseScreenX()), static_cast<float>(X::GetMouseScreenY()) };

			// compute direction from player to mouse
			X::Math::Vector2 aimDir = mousePos - mPosition;
			if (X::Math::Vector2::SqrMagnitude(aimDir) > 0.0f)
			{
				aimDir = X::Math::Normalize(aimDir);
			}
			else
			{
				// fallback to current facing if mouse is exactly at player position
				aimDir = mFacing;
			}

			// spawn slightly in front of player along aim direction
			float offset = X::GetSpriteHeight(mImageId) * 0.5f + 8.0f;
			X::Math::Vector2 spawnPos = mPosition + (aimDir * offset);

			// compute rotation consistent with existing bullet code 
			float rotation = atan2(aimDir.x, -aimDir.y);
			bullet->SetActive(spawnPos, rotation, 2.0f);

			// deduct ammo and set cooldown
			--mAmmo;
			mShootCooldownTimer = kShootCooldown;
		}
	}
}

void Player::Render()
{
	if (mHealth > 0)
	{
		// rotate sprite to face the mouse cursor (same convention as bullets)
		X::Math::Vector2 mousePos = { static_cast<float>(X::GetMouseScreenX()), static_cast<float>(X::GetMouseScreenY()) };
		X::Math::Vector2 aimDir = mousePos - mPosition;
		float rotation = 0.0f;
		if (X::Math::Vector2::SqrMagnitude(aimDir) > 0.0f)
		{
			aimDir = X::Math::Normalize(aimDir);
			rotation = atan2(aimDir.x, -aimDir.y); // same as bullet rotation
		}
		X::DrawSprite(mImageId, mPosition, rotation);
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
	// DONT REMOVE COLLIDABlES DURING THIS FUNCTION
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
		
	}
	mHealth = X::Math::Clamp(mHealth, 0, 100);
	if (mHealth <= 0)
	{
		mRemoveCollider = true;
	}
}

void Player::TakeDamage(int amount)
{
	if (mHealth <= 0)
	{
		return;
	}
	mHealth -= amount;
	mHealth = X::Math::Clamp(mHealth, 0, 100);
	if (mHealth <= 0)
	{
		mRemoveCollider = true;
	}
}

void Player::AddAmmo(int amount)
{
	mAmmo += amount;
	if (mAmmo > kMaxAmmo)
	{
		mAmmo = kMaxAmmo;
	}
}

const X::Math::Vector2& Player::GetPosition() const
{
	return mPosition;
}

void Player::SetPosition(const X::Math::Vector2& position)
{
	mPosition = position;
}

void Player::SetVelocity(const X::Math::Vector2& velocity)
{
	mVelocity = velocity;
}

const X::Math::Vector2& Player::GetVelocity() const
{
	return mVelocity;
}
int Player::GetAmmo() const
{
	return mAmmo;
}
int Player::GetMaxAmmo() const
{
	return kMaxAmmo;
}
int Player::GetHealth() const
{
	return mHealth;
}
int Player::GetMaxHealth() const
{
	return kMaxHealth;
}
