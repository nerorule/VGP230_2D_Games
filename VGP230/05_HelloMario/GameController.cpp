#include "GameController.h"
#include "TileMap.h"
#include "CollisionManager.h"
#include "PickupManager.h"
#include "EnemyManager.h"
#include "BulletPool.h"

GameController* GameController::mInstance = nullptr;

GameController::GameController()
	: Entity()
	, mMusicId(0)
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

	// Load and start background music (looping).
	mMusicId = X::LoadSound("Background_2.wav");
	if (mMusicId != 0)
	{
		X::PlaySoundLoop(mMusicId);
	}
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

	// render HUD
	// Health Bar (top-left)
	const float margin = 16.0f;
	const float barW = 220.0f;
	const float barH = 20.0f;
	X::Math::Rect bg;
	bg.left = margin;
	bg.top = margin;
	bg.right = margin + barW;
	bg.bottom = margin + barH;
	// Health bar background
	X::DrawScreenRect(bg, X::Colors::Black);

	// Health bar fill
	float hp = static_cast<float>(mPlayer.GetHealth());
	float maxHp = static_cast<float>(mPlayer.GetMaxHealth());
	float fillW = (maxHp > 0.0f) ? X::Math::Clamp(hp / maxHp, 0.0f, 1.0f) * barW : 0.0f;
	X::Math::Rect fill;
	fill.left = margin;
	fill.top = margin;
	fill.right = margin + fillW;
	fill.bottom = margin + barH;
	X::DrawScreenRect(fill, X::Colors::Red);

	// Ammo (top-right)
	{
		const float size = 18.0f;
		const float rightMargin = 16.0f;
		int ammo = mPlayer.GetAmmo();
		std::string ammoStr = std::to_string(ammo);
		float textWidth = X::GetTextWidth(ammoStr.c_str(), size);
		float x = static_cast<float>(X::GetScreenWidth()) - rightMargin - textWidth;
		float y = margin;
		X::DrawScreenText(ammoStr.c_str(), x, y, size, X::Colors::Yellow);
	}
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

const X::Math::Vector2& GameController::GetPlayerPosition() const
{
	return mPlayer.GetPosition();
}