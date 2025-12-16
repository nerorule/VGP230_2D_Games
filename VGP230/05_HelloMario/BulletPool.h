#pragma once

#include "Entity.h"
#include <vector>

class Bullet;

class BulletPool : public Entity
{
public:
	BulletPool();
	~BulletPool() override;

	static BulletPool* Get();

	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	Bullet* GetBullet();
	std::vector<Bullet*>& GetBullets();
private:
	static BulletPool* mInstance;
	int mNextAvaibleIndex;
	std::vector<Bullet*> mBullets;
};