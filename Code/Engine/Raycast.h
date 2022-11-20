#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Bound.h"

namespace sad
{
    class Raycast
    {
    public:
        static bool Intersects(glm::vec3 Origin, glm::vec3 Direction, sad::Bound bound);
    
    };
}
