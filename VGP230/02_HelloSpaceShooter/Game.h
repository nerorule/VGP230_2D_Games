#pragma once

#include "Entity.h"
#include <vector>

class Ship;
class BulletPool;
class Collidable;
class Enemy;
class ProgressBar;
class PowerUp;

class Game : public Entity
{
public:
	Game();
	~Game() override;

	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	void AddCollidable(Collidable* collidable);
	bool IsGameOver();

	void SpawnNextWave();
	bool IsCurrentWaveCleared() const;
private:
	Ship* mPlayer;
	BulletPool* mBulletPool;
	ProgressBar* mHealthBar;
	std::vector<Collidable*> mCollidables;

	std::vector<Enemy*> mEnemies;
	std::vector<bool> mEnemyProcessed; // mark enemies already processed for drops
	std::vector<PowerUp*> mPowerUps;

	// Wave management
	int mTotalWaves = 3;
	int mEnemiesPerWave = 10;
	int mCurrentWave = -1; // -1 means no wave spawned yet

	// True when the player has completed all waves (victory)
	bool mVictory = false;

	// spawn helper
	void SpawnPowerUp(const X::Math::Vector2& pos);

	// Background music sound id (loaded from Assets/Sounds)
	X::SoundId mMusicId = 0;

	// Background image 
	X::TextureId mBackgroundId = 0;
	X::Math::Vector2 mBackgroundPos{ 0.0f, 0.0f };
};