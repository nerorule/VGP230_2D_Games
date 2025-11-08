#include "Game.h"
#include "Ship.h"
#include "BulletPool.h"
#include "Bullet.h"
#include "Enemy.h"
#include "BossEnemy.h"
#include "ProgressBar.h"
#include "PowerUp.h"
#include <XEngine.h>

Game::Game()
	: Entity()
	, mPlayer(nullptr)
	, mBulletPool(nullptr)
	, mHealthBar(nullptr)
	, mTotalWaves(3)
	, mEnemiesPerWave(5)
	, mCurrentWave(-1)
	, mVictory(false)
	, mMusicId(0)
{

}
Game::~Game()
{
	XASSERT(mPlayer == nullptr, "Game: Unload must be called");
}

void Game::Load()
{
	mPlayer = new Ship();
	mBulletPool = new BulletPool();
	mHealthBar = new ProgressBar();
	mHealthBar->Load();

	mPlayer->Load();
	mPlayer->SetBullePool(mBulletPool);
	AddCollidable(mPlayer);

	// Spawn the first wave (subsequent waves spawn when previous wave is cleared)
	SpawnNextWave();

	mBulletPool->Load();
	std::vector<Bullet*>& bullets = mBulletPool->GetBullets();
	for (Bullet* bullet : bullets)
	{
		AddCollidable(bullet);
	}

	mBackgroundId = X::LoadTexture("space_background.png");
	if (mBackgroundId != 0)
	{
		// draw from top-left
		mBackgroundPos.x = 0.0f;
		mBackgroundPos.y = 0.0f;
	}

	// Load and start background music (looping).
	mMusicId = X::LoadSound("background.wav");
	if (mMusicId != 0)
	{
		X::PlaySoundLoop(mMusicId);
	}
}
void Game::Update(float deltaTime)
{
	mPlayer->Update(deltaTime);
	for (Enemy* enemy : mEnemies)
	{
		if (enemy)
		{
			enemy->Update(deltaTime);
		}
	}
	for (PowerUp* pu : mPowerUps)
	{
		if (pu)
			pu->Update(deltaTime);
	}
	mBulletPool->Update(deltaTime);

	int numCollidables = mCollidables.size();
	for (int i = 0; i < numCollidables - 1; ++i)
	{
		for (int n = i + 1; n < numCollidables; ++n)
		{
			if (mCollidables[i]->DidCollide(mCollidables[n]))
			{
				mCollidables[i]->OnCollision(mCollidables[n]);
				mCollidables[n]->OnCollision(mCollidables[i]);
			}
		}
	}

	// post-collision: check dead enemies and spawn power-ups (one-time per enemy)
	for (int i = 0; i < static_cast<int>(mEnemies.size()); ++i)
	{
		Enemy* e = mEnemies[i];
		if (e && !mEnemyProcessed[i] && !e->IsAlive())
		{
			mEnemyProcessed[i] = true;
			// 50% chance to drop a power-up
			if (X::RandomFloat(0.0f, 1.0f) < 0.50f)
			{
				SpawnPowerUp(e->GetPosition());
			}
		}
	}

	// If the current wave has been cleared spawn the next wave (if any left)
	if (mCurrentWave >= 0 && IsCurrentWaveCleared())
	{
		SpawnNextWave();
	}

	mHealthBar->SetBarValue(mPlayer->GetHealth(), mPlayer->GetMaxHealth());
}
void Game::Render()
{
	// Draw background first so it's behind everything
	if (mBackgroundId != 0)
	{
		X::DrawSprite(mBackgroundId, mBackgroundPos, X::Pivot::TopLeft);
	}

	mPlayer->Render();
	for (Enemy* enemy : mEnemies)
	{
		if (enemy)
		{
			enemy->Render();
		}
	}
	for (PowerUp* pu : mPowerUps)
	{
		if (pu)
			pu->Render();
	}
	mBulletPool->Render();
	mHealthBar->Render();

}
void Game::Unload()
{
	// Stop background music if playing
	if (mMusicId != 0)
	{
		X::StopSoundLoop(mMusicId);
		mMusicId = 0;
	}

	mBulletPool->Unload();
	delete mBulletPool;
	mBulletPool = nullptr;

	mHealthBar->Unload();
	delete mHealthBar;
	mHealthBar = nullptr;

	mPlayer->Unload();
	delete mPlayer;
	mPlayer = nullptr;
	for (Enemy* enemy : mEnemies)
	{
		if (enemy)
		{
			enemy->Unload();
			delete enemy;
			enemy = nullptr;
		}
	}
	for (PowerUp* pu : mPowerUps)
	{
		if (pu)
		{
			pu->Unload();
			delete pu;
			pu = nullptr;
		}
	}
}

void Game::AddCollidable(Collidable* collidable)
{
	mCollidables.push_back(collidable);
}

bool Game::IsGameOver()
{
	// Game over if player died OR player has completed all waves (victory)
	return !mPlayer->IsAlive() || mVictory;
}

// Waves
void Game::SpawnNextWave()
{
	// if all waves already spawned, mark victory and return
	if (mCurrentWave + 1 >= mTotalWaves)
	{
		mVictory = true;
		return;
	}

	++mCurrentWave; // now current wave index (0-based)

	X::Math::Vector2 SpawnPosition = X::Math::Vector2::Zero();
	X::Math::Vector2 SpawnDirection = X::Math::Vector2::Zero();
	X::Math::Vector2 center = { X::GetScreenWidth() * 0.5f, X::GetScreenHeight() * 0.5f };
	const float minOffSet = 100.0f;
	const float maxOffSet = center.y;

	// If this is the final wave, spawn only the boss
	if (mCurrentWave == mTotalWaves - 1)
	{
		SpawnDirection = X::RandomUnitCircle();
		SpawnPosition = center + (SpawnDirection * X::RandomFloat(minOffSet, maxOffSet));

		BossEnemy* boss = new BossEnemy();
		boss->Load();
		boss->SetBulletPool(mBulletPool);
		boss->SetShip(mPlayer);
		boss->SetPosition(SpawnPosition);
		boss->SetRotation(X::RandomFloat() * X::Math::kTwoPi);
		AddCollidable(boss);
		mEnemies.push_back(boss);
		mEnemyProcessed.push_back(false);
	}
	else
	{
		for (int i = 0; i < mEnemiesPerWave; ++i)
		{
			SpawnDirection = X::RandomUnitCircle();
			SpawnPosition = center + (SpawnDirection * X::RandomFloat(minOffSet, maxOffSet));

			Enemy* newEnemy = new Enemy();
			newEnemy->Load();
			newEnemy->SetBulletPool(mBulletPool);
			newEnemy->SetShip(mPlayer);
			newEnemy->SetPosition(SpawnPosition);
			newEnemy->SetRotation(X::RandomFloat() * X::Math::kTwoPi);
			AddCollidable(newEnemy);
			mEnemies.push_back(newEnemy);
			mEnemyProcessed.push_back(false);
		}
	}
}

bool Game::IsCurrentWaveCleared() const
{
	if (mCurrentWave < 0)
	{
		return false;
	}

	int startIndex = mCurrentWave * mEnemiesPerWave;
	int endIndex = startIndex + mEnemiesPerWave;

	// guard - if indices are out of range treat wave as not cleared (shouldn't happen)
	if (startIndex >= static_cast<int>(mEnemies.size()))
	{
		return false;
	}

	// ensure we don't read past the vector
	endIndex = std::min(endIndex, static_cast<int>(mEnemies.size()));

	for (int i = startIndex; i < endIndex; ++i)
	{
		if (mEnemies[i] != nullptr && mEnemies[i]->IsAlive())
		{
			return false;
		}
	}
	return true;
}

void Game::SpawnPowerUp(const X::Math::Vector2& pos)
{
	// choose type randomly
	int t = static_cast<int>(X::RandomFloat(0.0f, 3.0f));
	PowerUp::Type puType = PowerUp::Type::Speed; // default
	if (t == 1)
	{
		puType = PowerUp::Type::Shield;
	}
	else if (t == 2)
	{
		puType = PowerUp::Type::Weapon;
	}

	PowerUp* pu = new PowerUp(puType);
	pu->SetPosition(pos);
	pu->Load();
	AddCollidable(pu);
	mPowerUps.push_back(pu);
}