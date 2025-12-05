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
	
private:
	X::TextureId mImageId;
	X::Math::Vector2 mPosition;
	X::Math::Rect mPlayerRect;
	int mHealth;
	bool mRemoveCollider;

	X::Math::Vector2 mFacing;
};