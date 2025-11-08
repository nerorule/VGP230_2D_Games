#pragma once

#include "Enemy.h"

class BossEnemy : public Enemy
{
public:
	BossEnemy();
	~BossEnemy() override;

	// Entity Overrides (custom visuals/behaviour for boss)
	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	// Collidable Overrides (override if boss behaviour differs)
	int GetType() const override;
	const X::Math::Vector2& GetPosition() const override;
	void OnCollision(Collidable* collidable) override;

	// Uses inherited SetBulletPool, SetShip, SetPosition, SetRotation and IsAlive
};
