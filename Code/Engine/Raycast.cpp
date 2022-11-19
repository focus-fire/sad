#include <iostream>
#include "sadpch.h"
#include "Raycast.h"
#include "Bound.h"
#include <Core/Log.h>
#include <Core/Assert.h>
#include <Core/Guid.h>

namespace sad
{
    bool Raycast::Intersects(glm::vec3 Origin, glm::vec3 Direction, sad::Bound bound)
    {
        // Get the max and min vectors for the bound.
        glm::vec3 Max = bound.GetBoundMax();
        glm::vec3 Min = bound.GetBoundMin();

        float tMin = (Min.x - Origin.x) / Direction.x;
        float tMax = (Max.x - Origin.x) / Direction.x;

        if (tMin > tMax)
        {
            float temp = tMin;
            tMin = tMax;
            tMax = temp;
        }

        float tyMin = (Min.y - Origin.y) / Direction.y;
        float tyMax = (Max.y - Origin.y) / Direction.y;

        if (tyMin > tyMax)
        {
            float temp = tyMin;
            tyMin = tyMax;
            tyMax = temp;
        }

        if ((tMin > tyMax) || (tyMin > tMax))
        {
            core::Log(ELogType::Info, "Raycast::Intersects() - Ray does not intersect bound.");
            return false;
        }

        if (tyMin > tMin)
        {
            tMin = tyMin;
        }

        if (tyMax < tMax)
        {
            tMax = tyMax;
        }

        float tzMin = (Min.z - Origin.z) / Direction.z;
        float tzMax = (Max.z - Origin.z) / Direction.z;

        if (tzMin > tzMax)
        {
            float temp = tzMin;
            tzMin = tzMax;
            tzMax = temp;
        }

        if ((tMin > tzMax) || (tzMin > tMax))
        {
            core::Log(ELogType::Info, "Raycast::Intersects() - Ray does not intersect bound.");
            return false;
        }

        if (tzMin > tMin)
        {
            tMin = tzMin;
        }

        if (tzMax < tMax)
        {
            tMax = tzMax;
        }
        core::Log(ELogType::Info, "Raycast::Intersects() - Ray intersects bound.");
        return true;
    }
}
//
//namespace glm 
//{
//
//    bool sad::Raycast::Intersects(vec3 Origin, vec3 Direction, sad::Bound bound) {
//        glm::vec3 max = bound.GetBoundMax();
//        glm::vec3 min = bound.GetBoundMin();
//
//        glm::vec3 t1 = (min - Origin) / Direction;
//        glm::vec3 t2 = (max - Origin) / Direction;
//
//        glm::vec3 tmin = glm::min(t1, t2);
//        glm::vec3 tmax = glm::max(t1, t2);
//
//        float t00 = glm::max(tmin.x, glm::max(tmin.y, tmin.z));
//        float t11 = glm::min(tmax.x, glm::min(tmax.y, tmax.z));
//
//        if (t00 < t11) {
//            return true;
//        }
//        else {
//            return false;
//        }
//
//
//    }
//}
