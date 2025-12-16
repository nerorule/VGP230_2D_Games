#pragma once

#include "Enemy.h"

class EnemyManager : public Entity
{
	public:
	~EnemyManager() override;
	static EnemyManager* Get();

	// Entity override
	void Load() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Unload() override;

	void SpawnEnemies(int amount);

private:
	EnemyManager();
	static EnemyManager* mInstance;

	std::vector<Enemy*> mEnemies;
	int mNextAvailableIndex;
	// Wave spawn state
	float mWaveTimer;
	static constexpr float kWaveInterval = 5.0f; // seconds between waves
	static constexpr int   kWaveSize = 5;         // enemies
};