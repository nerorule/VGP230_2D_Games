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
};