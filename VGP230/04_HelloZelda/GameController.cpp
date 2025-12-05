#include "GameController.h"
#include "TileMap.h"
#include "CollisionManager.h"
#include "PickupManager.h"
#include "EnemyManager.h"
#include "BulletPool.h"

GameController* GameController::mInstance = nullptr;

GameController::GameController()
	: Entity()
{
}
GameController::~GameController()
{

}

GameController* GameController::Get()
{
	if (mInstance == nullptr)
	{
		mInstance = new GameController();
	}
	return mInstance;
}

void GameController::Load()
{
	CollisionManager::Get()->Load();
	TileMap::Get()->Load();
	PickupManager::Get()->Load();
	EnemyManager::Get()->Load();
	BulletPool::Get()->Load();

	mPlayer.Load();

}
void GameController::Update(float deltaTime)
{
	TileMap::Get()->Update(deltaTime);
	PickupManager::Get()->Update(deltaTime);
	EnemyManager::Get()->Update(deltaTime);
	// do actions / inputs
	mPlayer.Update(deltaTime);
	BulletPool::Get()->Update(deltaTime);

	// do physics / collisions
	CollisionManager::Get()->Update(deltaTime);
}
void GameController::Render()
{
	TileMap::Get()->Render();
	PickupManager::Get()->Render();
	EnemyManager::Get()->Render();
	mPlayer.Render();
	BulletPool::Get()->Render();

	// only render debug
	CollisionManager::Get()->Render();
}

void GameController::Unload()
{
	mPlayer.Unload();
	BulletPool::Get()->Unload();
	EnemyManager::Get()->Unload();
	TileMap::Get()->Unload();
	PickupManager::Get()->Unload();
	CollisionManager::Get()->Unload();
}