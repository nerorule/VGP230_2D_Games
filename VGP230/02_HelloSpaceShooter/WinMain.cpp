#include <XEngine.h>
#include "Ship.h"

Ship mShip;


// Game setup
void GameInit()
{
	mShip.Load();
}

void GameRender()
{

}

// Per-frame logic
bool GameLoop(float deltaTime)
{
	mShip.Update(deltaTime);
	mShip.Render();
	// Exit if Escape is pressed
	return X::IsKeyPressed(X::Keys::ESCAPE);
}

void GameCleanup()
{
	mShip.Unload();
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