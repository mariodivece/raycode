#pragma once

#include <box2d/box2d.h>

class Game;

class IPhysicsBody {
protected:
    b2BodyId bodyId;
    Game* game;
    
public:
    IPhysicsBody(Game* game) : game(game), bodyId{} {}
    
    virtual ~IPhysicsBody() {
        // Only destroy if the body is still valid (world hasn't been destroyed yet)
        if (b2Body_IsValid(bodyId)) {
            b2DestroyBody(bodyId);
        }
    }
    
    b2BodyId GetBodyId() const { return bodyId; }
    virtual void Update() = 0;
};
