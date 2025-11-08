#include "PowerUp.h"
#include "Ship.h"
#include <XEngine.h>

PowerUp::PowerUp(Type type)
	: Entity()
	, Collidable(15.0f)
	, mType(type)
	, mImageId(0)
	, mPosition(0.0f)
	, mRotation(0.0f)
	, mCollected(false)
{
	// powerups should be collected by ship
	SetCollisionFilter(ET_SHIP);
}

PowerUp::~PowerUp()
{
}

void PowerUp::Load()
{
	// try loading a texture per type;
	switch (mType)
	{
	case Type::Speed:
		mImageId = X::LoadTexture("SpeedBoost.png");
		break;
	case Type::Shield:
		mImageId = X::LoadTexture("shield.png");
		break;
	case Type::Weapon:
		mImageId = X::LoadTexture("Fire_Power.png");
		break;
	default:
		mImageId = 0;
		break;
	}
}

void PowerUp::Update(float deltaTime)
{
	mRotation += deltaTime;
}

void PowerUp::Render()
{
	if (mCollected)
		return;

	if (mImageId != 0)
	{
		X::DrawSprite(mImageId, mPosition, mRotation);
	}
}

void PowerUp::Unload()
{
	
}

int PowerUp::GetType() const
{
	return ET_POWERUP;
}

const X::Math::Vector2& PowerUp::GetPosition() const
{
	return mPosition;
}

void PowerUp::OnCollision(Collidable* collidable)
{
	if (mCollected)
		return;

	if (collidable->GetType() == ET_SHIP)
	{
		Ship* ship = dynamic_cast<Ship*>(collidable);
		if (ship)
		{
			switch (mType)
			{
			case Type::Speed:
				ship->ApplySpeedBoost(2.0f, 10.0f); // 2x speed for 10s
				break;
			case Type::Shield:
				ship->ApplyShield(4.0f); // 4s shield
				break;
			case Type::Weapon:
				ship->ApplyWeaponUp(10, 8.0f); // +10 damage for 8s
				break;
			}
		}
		mCollected = true;
		SetCollisionFilter(0);
	}
}

void PowerUp::SetPosition(const X::Math::Vector2& pos)
{
	mPosition = pos;
}