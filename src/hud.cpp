#include "hud.h"
#include "game.h"
#include <box2d/box2d.h>

Hud::Hud(Game* game)
    : game(game)
{
}

void Hud::Render() const {
    // Debug info
    b2Counters counters = b2World_GetCounters(game->GetWorldId());
    DrawText(TextFormat("Bodies: %d, Contacts: %d", counters.bodyCount, counters.contactCount), 10, 10, 20, WHITE);
}
