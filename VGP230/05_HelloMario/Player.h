#pragma once

#include "Entity.h"
#include "Collidable.h"

class Player : public Entity, public Collidable
{
public:
	Player();
	~Player();

	// Entity interface
	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;
	// Collidable interface
	int GetType() const override;
	void OnCollision(Collidable* collidable) override;
	const X::Math::Vector2& GetPosition() const override;

	//Player Functions
	void SetVelocity(const X::Math::Vector2& velocity);
	void SetPosition(const X::Math::Vector2& velocity);
	const X::Math::Vector2& GetVelocity() const;
	// apply damage to player
	void TakeDamage(int amount);
	// add ammo to player (clamped to max)
	void AddAmmo(int amount);
	int GetAmmo() const;
	int GetMaxAmmo() const;
	int GetHealth() const;
	int GetMaxHealth() const;

private:
	X::TextureId mImageId;
	X::Math::Vector2 mPosition;
	X::Math::Vector2 mVelocity;
	X::Math::Rect mPlayerRect;
	int mHealth;
	bool mRemoveCollider;
	X::Math::Vector2 mFacing;

	// shooting cooldown (seconds)
	float mShootCooldownTimer;
	static constexpr float kShootCooldown = 0.2f;
	// ammo
	int mAmmo;
	static constexpr int kMaxAmmo = 30;
	static constexpr int kMaxHealth = 100;
};