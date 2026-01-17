#pragma once

#include <raylib.h>
#include "IRenderable.h"

class Game;

class Hud : public IRenderable {
public:
    Hud(Game* game);
    ~Hud() override = default;

    void Render() const override;

private:
    Game* game;
};
