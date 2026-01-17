#include "game.h"
#include "ball.h"
#include "wall.h"
#include <raylib.h>

Game::Game() {
    running = true;
    
    // Create Box2D world with no gravity (top-down view)
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, 0.0f};
    worldDef.enableContinuous = true;  // Enable continuous collision
    worldDef.restitutionThreshold = 0.0f;  // Allow all collisions to bounce
    worldId = b2CreateWorld(&worldDef);
    
    // Create world bounds
    CreateWorldBounds();
    
    // Create player
    player = std::make_unique<Ball>(this, false);  // false = not auto-moving (player-controlled)
    
    // Create 5 enemies with random positions and predefined colors
    Color enemyColors[] = { RED, BLUE, GREEN, YELLOW, ORANGE };
    
    for (int i = 0; i < 5; i++) {
        float x = 50 + (float)(GetRandomValue(0, (int)screenWidth - 100));
        float y = 50 + (float)(GetRandomValue(0, (int)screenHeight - 100));
        enemies.push_back(std::make_unique<Ball>(this, x, y, enemyColors[i]));
    }
    
    // Create 2 brick walls with random lengths
    int wall1Length = GetRandomValue(8, 15);
    int wall2Length = GetRandomValue(8, 15);
    
    // Horizontal wall in middle-upper area
    walls.push_back(std::make_unique<Wall>(this, 200.0f, 150.0f, wall1Length, true, BROWN));
    
    // Vertical wall in middle-right area
    walls.push_back(std::make_unique<Wall>(this, 600.0f, 250.0f, wall2Length, false, GRAY));
}

void Game::CreateWorldBounds() {
    // Create static walls around the screen (convert pixels to meters)
    b2BodyDef wallDef = b2DefaultBodyDef();
    wallDef.type = b2_staticBody;
    
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.material.restitution = 0.8f;  // Bounciness
    
    float wallThickness = 10.0f / PIXELS_PER_METER;
    float halfWidth = screenWidth / (2.0f * PIXELS_PER_METER);
    float halfHeight = screenHeight / (2.0f * PIXELS_PER_METER);
    
    // Bottom wall
    wallDef.position = {halfWidth, (screenHeight / PIXELS_PER_METER) + wallThickness};
    wallBodies[0] = b2CreateBody(worldId, &wallDef);
    b2Polygon bottomBox = b2MakeBox(halfWidth, wallThickness);
    b2CreatePolygonShape(wallBodies[0], &shapeDef, &bottomBox);
    
    // Top wall
    wallDef.position = {halfWidth, -wallThickness};
    wallBodies[1] = b2CreateBody(worldId, &wallDef);
    b2Polygon topBox = b2MakeBox(halfWidth, wallThickness);
    b2CreatePolygonShape(wallBodies[1], &shapeDef, &topBox);
    
    // Left wall
    wallDef.position = {-wallThickness, halfHeight};
    wallBodies[2] = b2CreateBody(worldId, &wallDef);
    b2Polygon leftBox = b2MakeBox(wallThickness, halfHeight);
    b2CreatePolygonShape(wallBodies[2], &shapeDef, &leftBox);
    
    // Right wall
    wallDef.position = {(screenWidth / PIXELS_PER_METER) + wallThickness, halfHeight};
    wallBodies[3] = b2CreateBody(worldId, &wallDef);
    b2Polygon rightBox = b2MakeBox(wallThickness, halfHeight);
    b2CreatePolygonShape(wallBodies[3], &shapeDef, &rightBox);
}

Game::~Game() {
    b2DestroyWorld(worldId);
}

void Game::Update() {
    if (exitRequested) running = false;
    
    // Step the physics world with fixed timestep (Box2D v3 recommendation)
    float timeStep = 1.0f / 60.0f;  // Fixed 60 FPS timestep
    int subStepCount = 4;
    b2World_Step(worldId, timeStep, subStepCount);
    
    // Update all balls (sync from physics)
    if (player) player->Update();
    for (auto& enemy : enemies) {
        if (enemy) enemy->Update();
    }
    
    // Update walls (check for breaks)
    for (auto& wall : walls) {
        if (wall) wall->Update();
    }
}

void Game::Render() {
    BeginDrawing();
    ClearBackground(GetBackgroundColor());
    
    for (auto& enemy : enemies) {
        if (enemy) enemy->Render();
    }
    
    // Render walls
    for (auto& wall : walls) {
        if (wall) wall->Render();
    }
    
    if (player) player->Render();
    
    // Debug info
    b2Counters counters = b2World_GetCounters(worldId);
    DrawText(TextFormat("Bodies: %d, Contacts: %d", counters.bodyCount, counters.contactCount), 10, 10, 20, WHITE);
    
    EndDrawing();
}

bool Game::IsRunning() const {
    return running && !exitRequested;
}

float Game::GameTime() const {
    return (float)(GetFrameTime() * 100.0);
}

double Game::ElapsedTime() const {
    return GetTime();
}

void Game::RequestExit() {
    exitRequested = true;
}

Color Game::GetBackgroundColor() const {
    return DARKGREEN;
}

int Game::TargetFps() const {
    return targetFps;
}

void Game::TargetFps(int value) {
    targetFps = value;
}

float Game::ScreenWidth() const {
    return screenWidth;
}

void Game::ScreenWidth(float value) {
    screenWidth = value;
}

float Game::ScreenHeight() const {
    return screenHeight;
}

void Game::ScreenHeight(float value) {
    screenHeight = value;
}

void Game::ProcessInput() {
    if (exitRequested || IsKeyPressed(KEY_ESCAPE)) {
        RequestExit();
        return;
    }

    if (!player) return;
    
    float forceX = 0.0f;
    float forceY = 0.0f;
    
    if (IsKeyDown(KEY_LEFT)) {
        forceX -= 1.0f;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        forceX += 1.0f;
    }
    if (IsKeyDown(KEY_UP)) {
        forceY -= 1.0f;
    }
    if (IsKeyDown(KEY_DOWN)) {
        forceY += 1.0f;
    }
    
    if (forceX != 0.0f || forceY != 0.0f) {
        player->ApplyForce(forceX, forceY);
    }
}
