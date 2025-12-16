#include <XEngine.h>
#include "BulletPool.h"
#include "Bullet.h"

BulletPool* BulletPool::mInstance = nullptr;

BulletPool::BulletPool()
	: Entity()
	, mNextAvaibleIndex(0)
{
}

BulletPool::~BulletPool()
{
	XASSERT(mBullets.empty(), "BulletPool: Unload must be called");
}

BulletPool* BulletPool::Get()
{
	if (mInstance == nullptr)
	{
		mInstance = new BulletPool();
	}
	return mInstance;
}

void BulletPool::Load()
{
	const int maxBullets = 50;
	for (int i = 0; i < maxBullets; ++i)
	{
		Bullet* newBullet = new Bullet();
		newBullet->Load();
		mBullets.push_back(newBullet);
	}
	mNextAvaibleIndex = 0;
}

void BulletPool::Update(float deltaTime)
{
	for (Bullet* bullet : mBullets)
	{
		if (bullet)
			bullet->Update(deltaTime);
	}
}

void BulletPool::Render()
{
	for (Bullet* bullet : mBullets)
	{
		if (bullet)
		{
			bullet->Render();
		}
	}
}

void BulletPool::Unload()
{
	for (Bullet* bullet : mBullets)
	{
		if (bullet)
		{
			bullet->Unload();
			delete bullet;
		}
	}
	mBullets.clear();
}

Bullet* BulletPool::GetBullet()
{
	if (mBullets.empty())
	{
		return nullptr;
	}
	Bullet* bullet = mBullets[mNextAvaibleIndex];
	mNextAvaibleIndex = (mNextAvaibleIndex + 1) % mBullets.size();
	return bullet;
}

std::vector<Bullet*>& BulletPool::GetBullets()
{
	return mBullets;
}