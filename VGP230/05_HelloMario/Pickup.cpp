#include "Pickup.h"
#include "CollisionManager.h"

Pickup::Pickup()
	: Entity()
	, Collidable()
	, mIsActive(false)
	, mRemoveCollider(false)
	, mImageId(0)
	, mPosition(0.0f, 0.0f)
{
}

Pickup::~Pickup()
{
}

void Pickup::Load()
{
	mImageId = X::LoadTexture("qmark.png");
	mIsActive = false;
	mRemoveCollider = false;
}

void Pickup::Update(float deltaTime)
{
	if (mRemoveCollider)
	{
		CollisionManager::Get()->RemoveCollidable(this);
		mRemoveCollider = false;
	}
}

void Pickup::Render()
{
	if (mIsActive)
	{
		X::DrawSprite(mImageId, mPosition);
	}
}

void Pickup::Unload()
{

}

int Pickup::GetType() const
{
	return  mIsActive? ET_PICKUP : 0;
}

void Pickup::OnCollision(Collidable* collidable)
{
	// Do not remove collidables during this function
	if (mIsActive)
	{
		mIsActive = false;
		mRemoveCollider = true;
	}
}

const X::Math::Vector2& Pickup::GetPosition() const
{
	return mPosition;
}

bool Pickup::IsActive() const
{
	return mIsActive;
}

void Pickup::SetActive(const X::Math::Vector2& position)
{
	mIsActive = true;
	mPosition = position;
	float halfWidth = X::GetSpriteWidth(mImageId) * 0.5f;
	float halfHeight = X::GetSpriteHeight(mImageId) * 0.5f;
	X::Math::Rect newRect;
	newRect.left = mPosition.x - halfWidth;
	newRect.right = mPosition.x + halfWidth;
	newRect.top = mPosition.y - halfHeight;
	newRect.bottom = mPosition.y + halfHeight;

	SetRect(newRect);
	SetCollisionFilter(ET_PLAYER);
	CollisionManager::Get()->AddCollidable(this);
}