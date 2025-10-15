#include <XEngine.h>

struct BouncingSprite
{
	X::TextureId textureId = 0;
	X::Math::Vector2 position = { 0.0f, 0.0f };
	X::Math::Vector2 velocity = { 0.0f, 0.0f };
	float rotation = 0.0f;
};

BouncingSprite spriteA;
BouncingSprite spriteB;

static void UpdateSpriteBounce(BouncingSprite& sprite, float deltaTime)
{
	// Move sprite
	sprite.position += sprite.velocity * deltaTime;

	// Get screen boundaries and half sprite size
	const float screenWidth = static_cast<float>(X::GetScreenWidth());
	const float screenHeight = static_cast<float>(X::GetScreenHeight());
	const float halfWidth = static_cast<float>(X::GetSpriteWidth(sprite.textureId)) * 0.5f;
	const float halfHeight = static_cast<float>(X::GetSpriteHeight(sprite.textureId)) * 0.5f;

	// Horizontal bounce
	if (sprite.position.x < halfWidth)
	{
		sprite.position.x = halfWidth;
		sprite.velocity.x *= -1.0f;
	}
	else if (sprite.position.x > screenWidth - halfWidth)
	{
		sprite.position.x = screenWidth - halfWidth;
		sprite.velocity.x *= -1.0f;
	}

	// Vertical bounce
	if (sprite.position.y < halfHeight)
	{
		sprite.position.y = halfHeight;
		sprite.velocity.y *= -1.0f;
	}
	else if (sprite.position.y > screenHeight - halfHeight)
	{
		sprite.position.y = screenHeight - halfHeight;
		sprite.velocity.y *= -1.0f;
	}
}

// Game setup
void GameInit()
{
	XLOG("Game Init");

	// Load textures
	spriteA.textureId = X::LoadTexture("ImpressCat.png");
	spriteB.textureId = X::LoadTexture("SadCat.png");

	// Set starting positions
	spriteA.position = { X::GetScreenWidth() * 0.40f, X::GetScreenHeight() * 0.35f };
	spriteB.position = { X::GetScreenWidth() * 0.75f, X::GetScreenHeight() * 0.65f };

	// Set velocities
	spriteA.velocity = { 100.0f, 140.0f };
	spriteB.velocity = { -500.0f, -100.0f };
}

void GameRender()
{
	X::DrawSprite(spriteA.textureId, spriteA.position, spriteA.rotation);
	X::DrawSprite(spriteB.textureId, spriteB.position, spriteB.rotation);
}

// Per-frame logic
bool GameLoop(float deltaTime)
{
	UpdateSpriteBounce(spriteA, deltaTime);
	UpdateSpriteBounce(spriteB, deltaTime);

	// do physics and physics results
	// render final positions
	XLOG("Game Loop");
	GameRender();

	// Exit if Escape is pressed
	return X::IsKeyPressed(X::Keys::ESCAPE);
}

void GameCleanup()
{
	XLOG("Game Cleanup");
}

// Main function
int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("xconfig.json");
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();

	return 0;
}