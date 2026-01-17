#include "FakeLight.h"
#include <raymath.h>

FakeLight::FakeLight(Vector2 direction)
    : lightType(LightType::Directional)
    , direction(Vector2Normalize(direction))
    , lightPosition{0, 0}
{
}

FakeLight::FakeLight(Vector2 position, LightType type)
    : lightType(type)
    , direction{0, 0}
    , lightPosition(position)
{
}

void FakeLight::SetDirection(Vector2 dir) {
    direction = Vector2Normalize(dir);
}

Vector2 FakeLight::GetHighlightOffset(Vector2 objectPosition, float radius) const {
    Vector2 lightDir;
    
    if (lightType == LightType::Point) {
        // Calculate direction from object to light position
        lightDir = Vector2Subtract(lightPosition, objectPosition);
        float distance = Vector2Length(lightDir);
        
        // Normalize the direction
        if (distance > 0.001f) {
            lightDir = Vector2Normalize(lightDir);
        } else {
            lightDir = {0, -1};  // Default to up if at same position
        }
    } else {
        // Use constant directional light
        lightDir = direction;
    }
    
    // The highlight is offset toward the light direction
    // Scale by radius to make it proportional to object size
    float highlightDistance = radius * 0.4f;  // Highlight is 40% from center
    
    return Vector2 {
        lightDir.x * highlightDistance,
        lightDir.y * highlightDistance
    };
}

float FakeLight::GetIntensityAt(Vector2 objectPosition) const {
    if (lightType == LightType::Directional) {
        return 1.0f;  // Directional light has constant intensity
    }
    
    // Calculate distance from light to object
    float distance = Vector2Distance(lightPosition, objectPosition);
    
    // Apply attenuation formula: 1.0 / (constant + linear*d + quadratic*d²)
    float attenuation = 1.0f / (attenuationConstant + 
                                 attenuationLinear * distance + 
                                 attenuationQuadratic * distance * distance);
    
    // Clamp between 0 and 1
    if (attenuation > 1.0f) attenuation = 1.0f;
    if (attenuation < 0.0f) attenuation = 0.0f;
    
    return attenuation;
}
