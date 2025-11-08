#pragma once

#include "Entity.h"
#include "Collidable.h"
#include "Enum.h"

class Ship;

class PowerUp : public Entity, public Collidable
{
public:
	enum class Type
	{
		Speed,
		Shield,
		Weapon
	};

	PowerUp(Type type);
	~PowerUp() override;

	// Entity
	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	// Collidable
	int GetType() const override;
	const X::Math::Vector2& GetPosition() const override;
	void OnCollision(Collidable* collidable) override;

	void SetPosition(const X::Math::Vector2& pos);

private:
	Type mType;
	X::TextureId mImageId;
	X::Math::Vector2 mPosition;
	float mRotation;
	bool mCollected;
};