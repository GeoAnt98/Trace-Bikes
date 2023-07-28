#include <GameDev2D.h>
#include "Game.h"

// Function signatures.
void Init();
void Shutdown();
void Update(double delta);
void Draw();
void HandleLeftMouseClick(float mouseX, float mouseY);
void HandleRightMouseClick(float mouseX, float mouseY);
void HandleKeyPress(GameDev2D::Keyboard::Key key);

GameDev2D::Game* game = nullptr;

int WINAPI WinMain(HINSTANCE aCurrentInstance, HINSTANCE aPreviousInstance, LPSTR aCommandLine, int aCommandShow)
{
    GameDev2D::Run(Init, Shutdown, Update, Draw);
    return 0;
}

void Init()
{
	// Register input callbacks.
	GameDev2D::RegisterLeftMouseClickCallback(HandleLeftMouseClick);
	GameDev2D::RegisterRightMouseClickCallback(HandleRightMouseClick);
	GameDev2D::RegisterKeyPressedCallback(HandleKeyPress);

	// Create the game instance.
	game = new GameDev2D::Game();
}

void Shutdown()
{
	if (game != nullptr)
	{
		delete game;
		game = nullptr;
	}
}

void Update(double aDelta)
{
	if (game != nullptr)
	{
		game->Update(aDelta);
	}
}

void Draw()
{
	if (game != nullptr)
	{
		game->Draw();
	}
}

void HandleLeftMouseClick(float mouseX, float mouseY)
{
	if (game != nullptr)
	{
		game->HandleLeftMouseClick(mouseX, mouseY);
	}
}

void HandleRightMouseClick(float mouseX, float mouseY)
{
	if (game != nullptr)
	{
		game->HandleRightMouseClick(mouseX, mouseY);
	}
}

void HandleKeyPress(GameDev2D::Keyboard::Key key)
{
	if (game != nullptr)
	{
		game->HandleKeyPress(key);
	}
}
