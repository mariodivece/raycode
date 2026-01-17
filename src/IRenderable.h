#pragma once

class IRenderable {
public:
    virtual ~IRenderable() = default;
    virtual void Render() const = 0;
};
