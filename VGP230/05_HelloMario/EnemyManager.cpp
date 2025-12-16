#include "EnemyManager.h"
#include "TileMap.h"
#include "GameController.h" 

EnemyManager* EnemyManager::mInstance = nullptr;

EnemyManager::EnemyManager()
	: Entity()
	, mNextAvailableIndex(0)
	, mWaveTimer(0.0f)
{
}
EnemyManager::~EnemyManager()
{
}
EnemyManager* EnemyManager::Get()
{
	if (mInstance == nullptr)
	{
		mInstance = new EnemyManager();
	}
	return mInstance;
}

void EnemyManager::Load()
{
	const int maxEnemies = 100;
	for (int i = 0; i < maxEnemies; ++i)
	{
		Enemy* newEnemy = new Enemy();
		newEnemy->Load();
		mEnemies.push_back(newEnemy);
	}
	mNextAvailableIndex = 0;

	// start timer so first wave spawns after kWaveInterval seconds
	mWaveTimer = 0.1f;
}

void EnemyManager::Update(float deltaTime)
{
	// manual spawn for debugging
	if (X::IsKeyPressed(X::Keys::E))
	{
		SpawnEnemies(3);
	}

	// wave timer
	mWaveTimer -= deltaTime;
	if (mWaveTimer <= 0.0f)
	{
		SpawnEnemies(kWaveSize);
		mWaveTimer = kWaveInterval;
	}

	for (Enemy* enemy : mEnemies)
	{
		enemy->Update(deltaTime);
	}
}

void EnemyManager::Render()
{
	for (Enemy* enemy : mEnemies)
	{
		enemy->Render();
	}
}
void EnemyManager::Unload()
{
	for (Enemy* enemy : mEnemies)
	{
		enemy->Unload();
		delete enemy;
		enemy = nullptr;
	}
	mEnemies.clear();
}

void EnemyManager::SpawnEnemies(int amount)
{
	std::vector<Tile*> walkableTiles;
	TileMap::Get()->ObtainAllWalkableTiles(walkableTiles);

	// Build candidate spawn tiles that are sufficiently far from the player
	std::vector<Tile*> candidateTiles;
	const float minSpawnDistance = 300.0f;
	const float minSpawnDistSqr = minSpawnDistance * minSpawnDistance;
	X::Math::Vector2 playerPos = X::Math::Vector2::Zero();
	if (GameController::Get())
	{
		playerPos = GameController::Get()->GetPlayerPosition();
	}

	for (Tile* t : walkableTiles)
	{
		if (X::Math::Vector2::SqrMagnitude(t->GetPosition() - playerPos) >= minSpawnDistSqr)
		{
			candidateTiles.push_back(t);
		}
	}

	// Fallback to any walkable tile if no far tiles are available
	if (candidateTiles.empty())
	{
		candidateTiles = walkableTiles;
	}

	for (Enemy* enemy : mEnemies)
	{
		if (enemy->IsActive())
		{
			for (int i = 0; i < walkableTiles.size(); ++i)
			{
				if (walkableTiles[i]->HasCollision(enemy->GetRect()))
				{
					if (i < walkableTiles.size() - 1)
					{
						Tile* temp = walkableTiles[walkableTiles.size() - 1];
						walkableTiles[walkableTiles.size() - 1] = walkableTiles[i];
						walkableTiles[i] = temp;
					}
				}
				walkableTiles.pop_back();
				break;
			}
		}
	}
	int prevUsedIndex = 0;
	for (int i = 0; i < amount; ++i)
	{
		prevUsedIndex = mNextAvailableIndex - 1;
		if (prevUsedIndex < 0)
		{
			prevUsedIndex = mEnemies.size() - 1;
		}

		while (mNextAvailableIndex != prevUsedIndex)
		{
			Enemy* enemy = mEnemies[mNextAvailableIndex];
			mNextAvailableIndex = (mNextAvailableIndex + 1) % mEnemies.size();
			if (!enemy->IsActive())
			{
				if (candidateTiles.empty())
				{
					break;
				}
				int randIndex = X::Random(0, candidateTiles.size() - 1);
				Tile* randomTile = candidateTiles[randIndex];
				enemy->SetActive(randomTile->GetPosition(), 10);
				// remove used tile from candidate list (swap/pop)
				if (randIndex < candidateTiles.size() - 1)
				{
					Tile* temp = candidateTiles[candidateTiles.size() - 1];
					candidateTiles[candidateTiles.size() - 1] = candidateTiles[randIndex];
					candidateTiles[randIndex] = temp;
				}
				candidateTiles.pop_back();
				break;
			}
		}
	}
}