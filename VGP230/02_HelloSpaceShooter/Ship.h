#pragma once
#include "Entity.h"
#include "BulletPool.h"

class Ship : public Entity
{
public:
	Ship();
	~Ship() override;

	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

private:
	X::TextureId mImageId = 0;
	X::Math::Vector2 mPosition;
	float mRotation;

	BulletPool mBulletPool;
};
