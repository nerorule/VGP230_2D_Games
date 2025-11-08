#include "Bullet.h"
#include "Ship.h"

Bullet::Bullet()
	: Entity()
	, Collidable(5.0f)
	, mImageId(0)
	, mPosition(0.0f)
	, mRotation(0.0f)
	, mLifeTime(0.0f)
	, mBulletType(ET_NONE)
	, mDamage(10)
{

}
Bullet::~Bullet()
{

}
void Bullet::Load()
{
	mImageId = X::LoadTexture("bullet1.png");
}
void Bullet::Update(float deltaTime)
{
	if (IsActive())
	{
		mLifeTime -= deltaTime;
		const float speed = 200.0f;
		mPosition += X::Math::Vector2::Forward(mRotation) * speed * deltaTime;
		if (!IsActive())
		{
			SetCollisionFilter(ET_NONE);
		}
	}
}
void Bullet::Render()
{
	if (IsActive())
	{
		X::DrawSprite(mImageId, mPosition, mRotation);
		X::DrawScreenCircle(mPosition, GetRadius(), X::Colors::HotPink);
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
		SetCollisionFilter(ET_SHIP);
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
	// return the runtime bullet type (player or enemy)
	return static_cast<int>(mBulletType);
}

const X::Math::Vector2& Bullet::GetPosition() const
{
	return mPosition;
}

void Bullet::OnCollision(Collidable* collidable)
{
	// If we hit a ship that currently has a shield, do not consume the bullet and do not interact.
	if (collidable != nullptr && collidable->GetType() == ET_SHIP)
	{
		Ship* ship = dynamic_cast<Ship*>(collidable);
		if (ship && ship->IsShieldActive())
		{
			// ignore collision (bullet passes through)
			return;
		}
	}

	mLifeTime = 0.0f;
	SetCollisionFilter(ET_NONE);
}