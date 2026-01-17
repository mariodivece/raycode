#pragma once

#include <raylib.h>

enum class LightType {
    Directional,  // Light direction is constant for all objects
    Point         // Light emanates from a point position
};

class FakeLight {
public:
    FakeLight(Vector2 direction);  // Directional light
    FakeLight(Vector2 position, LightType type);  // Point light
    ~FakeLight() = default;

    Vector2 GetDirection() const { return direction; }
    void SetDirection(Vector2 dir);
    
    LightType GetType() const { return lightType; }
    Vector2 GetPosition() const { return lightPosition; }
    void SetPosition(Vector2 pos) { lightPosition = pos; }
    
    // Calculate the highlight offset for a sphere at a given position
    Vector2 GetHighlightOffset(Vector2 objectPosition, float radius) const;
    
    // Calculate light intensity at a position (0.0 to 1.0)
    // For directional light, returns 1.0 always
    // For point light, returns falloff based on distance
    float GetIntensityAt(Vector2 objectPosition) const;
    
    void SetAttenuation(float constant, float linear, float quadratic) {
        attenuationConstant = constant;
        attenuationLinear = linear;
        attenuationQuadratic = quadratic;
    }

private:
    LightType lightType;
    Vector2 direction;      // For directional light
    Vector2 lightPosition;  // For point light
    
    // Attenuation parameters (for point light)
    float attenuationConstant = 1.0f;
    float attenuationLinear = 0.0014f;
    float attenuationQuadratic = 0.000007f;
};
