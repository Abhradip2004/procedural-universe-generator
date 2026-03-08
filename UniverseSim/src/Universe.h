#pragma once

#include <vector>
#include "Body.h"

class Universe
{
public:

    std::vector<Body> bodies;

    float G = 1.0f;

    void update(float dt);
};