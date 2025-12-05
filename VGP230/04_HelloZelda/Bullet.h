
#pragma once

#include <XEngine.h>
#include "Entity.h"
#include "Collidable.h"
#include "Enum.h"

class Bullet : public Entity, public Collidable
{
public:
	Bullet();
	~Bullet() override;

	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	void SetActive(const X::Math::Vector2& pos, float rotation, float lifeTime = 2.0f);
	void SetEntityType(EntityType entitytype);
	bool IsActive() const;

	void SetDamage(int dmg);
	int GetDamage() const;

	int GetType() const override;
	const X::Math::Vector2& GetPosition() const override;
	void OnCollision(Collidable* collidable) override;

private:
	X::TextureId mImageId;
	X::Math::Vector2 mPosition;
	float mRotation;
	float mLifeTime;
	EntityType mBulletType;
	int mDamage;

	// rect for collisions 
	X::Math::Rect mBulletRect;
	bool mRemoveCollider;
};