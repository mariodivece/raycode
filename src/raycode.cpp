// raycode.cpp : Defines the entry point for the application.
//
#include "raycode.h"
using namespace std;

int main()
{
    unique_ptr<Game> game = make_unique<Game>();

    InitWindow(
        game->ScreenWidth(),
        game->ScreenHeight(),
        "Stupid Ball Game!");

    SetTargetFPS(game->TargetFps());

    while (game->IsRunning())
    {
        game->ProcessInput();
        game->Update();
        game->Render();
    }

    CloseWindow();
}