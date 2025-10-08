#include <XEngine.h>

X::TextureId imageId = 0;
X::Math::Vector2 imagePosition = { 0.0f, 0.0f };
float imageRotation = 0.0f;

void GameInit()
{
	imageId = X::LoadTexture("mario.png");
	imagePosition.x = X::GetScreenWidth() * 0.5f;
	imagePosition.y = X::GetScreenWidth() * 0.5f;

	float spriteWidth = X::GetSpriteHeight(imageId);
	float spriteHeight = X::GetSpriteHeight(imageId);
}

void GameRender()
{
	X::DrawSprite(imageId, imagePosition, imageRotation);
}

bool GameLoop(float deltaTime)
{
	// do inputs and effects
	const float moveSpeed = 200.0f;
	const float rotateSpeed = 100.0f;
	if (X::IsKeyDown(X::Keys::UP))
	{
		imagePosition.y -= moveSpeed * deltaTime;
	}
	else if (X::IsKeyDown(X::Keys::DOWN))
	{
		imagePosition.y += moveSpeed * deltaTime;
	}
	if (X::IsKeyDown(X::Keys::LEFT))
	{
		imagePosition.x -= moveSpeed * deltaTime;
	}
	else if (X::IsKeyDown(X::Keys::RIGHT))
	{
		imagePosition.x += moveSpeed * deltaTime;
	}
	if (X::IsKeyDown(X::Keys::N))
	{
		imageRotation -= rotateSpeed * deltaTime;
	}
	else if (X::IsKeyDown(X::Keys::M))
	{
		imageRotation += rotateSpeed * deltaTime;
	}
	//do physics and physics results
	// render final position
	GameRender();
	return X::IsKeyPressed(X::Keys::ESCAPE);
}

void GameCleanup()
{
	XLOG("GameCleanup()");
}

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("xconfig.json");
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();

	return 0;
}
