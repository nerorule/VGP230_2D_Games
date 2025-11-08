#pragma once
#include "Entity.h"
#include "Collidable.h"

class BulletPool;
class AnimSpriteSheet;


class Ship : public Entity, public Collidable
{
public:
	Ship();
	~Ship() override;

	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	int GetType() const override;
	const X::Math::Vector2& GetPosition() const override;
	void OnCollision(Collidable* collidable) override;

	void SetBullePool(BulletPool* bulletPool);

	int GetHealth() const;
	int GetMaxHealth() const;
	bool IsAlive() const;

	// Power-ups
	void ApplySpeedBoost(float multiplier, float duration);
	void ApplyShield(float duration);
	void ApplyWeaponUp(int extraDamage, float duration);
	bool IsShieldActive() const;

	enum class FireMode
	{
		Single,
		Burst
	};
	void ToggleFireMode();
	FireMode GetFireMode() const;
private:
	X::TextureId mImageId = 0;
	X::Math::Vector2 mPosition;
	float mRotation;
	int mHealth;
	int mMaxHealth;

	AnimSpriteSheet* mExplosion;
	BulletPool* mBulletPool;

	// power-up state
	float mBaseSpeed = 100.0f;
	float mSpeedMultiplier = 1.0f;
	float mSpeedTimer = 0.0f;

	bool mShieldActive = false;
	float mShieldTimer = 0.0f;

	int mBaseBulletDamage = 10;
	int mBulletDamage = 10;
	float mWeaponTimer = 0.0f;

	// firing mode
	FireMode mFireMode = FireMode::Single;
};
