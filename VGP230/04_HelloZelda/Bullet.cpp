
#include <XEngine.h>
#include "Bullet.h"
#include "TileMap.h"
#include "CollisionManager.h"

Bullet::Bullet()
	: Entity()
	, Collidable()
	, mImageId(0)
	, mPosition(0.0f, 0.0f)
	, mRotation(0.0f)
	, mLifeTime(0.0f)
	, mBulletType(ET_NONE)
	, mDamage(10)
	, mBulletRect(0.0f, 0.0f, 0.0f, 0.0f)
	, mRemoveCollider(false)
{
}

Bullet::~Bullet()
{
}

void Bullet::Load()
{
	mImageId = X::LoadTexture("bullet1.png");
	// initialize local collision rect from sprite size (centered)
	if (mImageId != 0)
	{
		float halfW = X::GetSpriteWidth(mImageId) * 0.5f;
		float halfH = X::GetSpriteHeight(mImageId) * 0.5f;
		mBulletRect.left = -halfW;
		mBulletRect.right = halfW;
		mBulletRect.top = -halfH;
		mBulletRect.bottom = halfH;
	}
}

void Bullet::Update(float deltaTime)
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

	mLifeTime -= deltaTime;
	const float speed = 400.0f;

	X::Math::Vector2 displacement = X::Math::Vector2::Forward(mRotation) * speed * deltaTime;
	X::Math::Vector2 maxDisplacement = displacement;

	// world-space rect for collision query
	X::Math::Rect currentRect = mBulletRect;
	currentRect.min += mPosition;
	currentRect.max += mPosition;

	if (TileMap::Get()->HasCollision(currentRect, maxDisplacement, displacement))
	{
		// displacement adjusted by tile collisions
		mPosition += displacement;
		// if movement is prevented (very small displacement) expire bullet
		if (X::Math::Vector2::SqrMagnitude(displacement) <= 1.0f)
		{
			mLifeTime = 0.0f;
			SetCollisionFilter(ET_NONE);
			// delay removal to next Update to avoid modifying collidables while iterating
			mRemoveCollider = true;
		}
	}
	else
	{
		mPosition += displacement;
	}

	// update rect stored in collidable for intersection tests
	currentRect = mBulletRect;
	currentRect.min += mPosition;
	currentRect.max += mPosition;
	SetRect(currentRect);

	if (!IsActive())
	{
		SetCollisionFilter(ET_NONE);
		mRemoveCollider = true;
	}
}

void Bullet::Render()
{
	if (IsActive())
	{
		X::DrawSprite(mImageId, mPosition, mRotation);
		// debug circle
		X::DrawScreenCircle(mPosition, X::GetSpriteWidth(mImageId) * 0.5f, X::Colors::HotPink);
	}
}

void Bullet::Unload()
{

}

void Bullet::SetActive(const X::Math::Vector2& pos, float rotation, float lifeTime)
{
	mPosition = pos;
	mRotation = rotation;
	mLifeTime = lifeTime;

	// set world rect immediately so CollisionManager can see it
	X::Math::Rect currentRect = mBulletRect;
	currentRect.min += mPosition;
	currentRect.max += mPosition;
	SetRect(currentRect);

	// add to collision manager
	CollisionManager::Get()->AddCollidable(this);
	mRemoveCollider = false;
}

void Bullet::SetEntityType(EntityType entitytype)
{
	mBulletType = entitytype;
	if (entitytype == ET_BULLET_PLAYER)
	{
		SetCollisionFilter(ET_ENEMY);
	}
	else if (entitytype == ET_BULLET_ENEMY)
	{
		SetCollisionFilter(ET_PLAYER);
	}
	else
	{
		SetCollisionFilter(0);
	}
}

bool Bullet::IsActive() const
{
	return mLifeTime > 0.0f;
}

void Bullet::SetDamage(int dmg)
{
	mDamage = dmg;
}

int Bullet::GetDamage() const
{
	return mDamage;
}

int Bullet::GetType() const
{
	return static_cast<int>(mBulletType);
}

const X::Math::Vector2& Bullet::GetPosition() const
{
	return mPosition;
}

void Bullet::OnCollision(Collidable* collidable)
{
	if (collidable == nullptr)
	{
		return;
	}

	mLifeTime = 0.0f;
	SetCollisionFilter(ET_NONE);
	mRemoveCollider = true;
}