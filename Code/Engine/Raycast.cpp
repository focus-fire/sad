#include <iostream>
#include "sadpch.h"
#include "Raycast.h"
#include "Bound.h"
#include "LineRenderer.h"
#include <Core/Log.h>
#include <Core/Assert.h>
#include <Core/Guid.h>
#include <Engine/ECS/Entity.h>
#include <Engine/ECS/Components/LineRendererComponent.h>



namespace sad
{
    bool Raycast::Intersects(glm::vec3 Origin, glm::vec3 Direction, sad::Bound bound)
    {
        // implementation of the raycast algorithm from https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
        glm::vec3 invDir = 1.0f / Direction;
        glm::vec3 tbot = invDir * (bound.GetBoundMin() - Origin);
        glm::vec3 ttop = invDir * (bound.GetBoundMax() - Origin);

        glm::vec3 tmin = glm::min(ttop, tbot);
        glm::vec3 tmax = glm::max(ttop, tbot);

        float t0 = glm::max(tmin.x, glm::max(tmin.y, tmin.z));
        float t1 = glm::min(tmax.x, glm::min(tmax.y, tmax.z));

        // if t0 > t1, ray (line) is intersecting AABB, but the whole AABB is behing us
        if (t0 > t1) {
            core::Log(ELogType::Info, "Raycast::Intersects() - Ray does not intersect bound.");
            return false;

        }

        // if t1 < 0, ray doesn't intersect AABB
        if (t1 < 0) {
            core::Log(ELogType::Info, "Raycast::Intersects() - Ray does not intersect bound.");
            return false;
        }

        // if t0 < 0, ray is inside AABB
        if (t0 < 0) {
            core::Log(ELogType::Info, "Raycast::Intersects() - Ray intersects bound.");
            return true;
        }

        // else, ray intersects AABB
        core::Log(ELogType::Info, "Raycast::Intersects() - Ray intersects bound.");
        return true;
        

    }
    int Raycast::coolFunction() {

        // raycast test that does not intersect.
        /*glm::vec3 origin2 = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 direction2 = glm::vec3(5.0f, -1.0f, -1.0f);
        sad::Bound bound2 = sad::Bound(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        bool intersects2 = sad::Raycast::Intersects(origin2, direction2, bound2);*/

        // raycast test that does not intersect.
        glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 direction = glm::vec3(5.0f, -1.0f, -1.0f);

        sad::Bound bound = sad::Bound(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        bool intersects = sad::Raycast::Intersects(origin, direction, bound);

        return 0;
    }
}

// Get the max and min vectors for the bound.
//glm::vec3 Max = bound.GetBoundMax();
//glm::vec3 Min = bound.GetBoundMin();
//
//float tMin = (Min.x - Origin.x) / Direction.x;
//float tMax = (Max.x - Origin.x) / Direction.x;
//
//if (tMin > tMax)
//{
//float temp = tMin;
//tMin = tMax;
//tMax = temp;
//}
//
//float tyMin = (Min.y - Origin.y) / Direction.y;
//float tyMax = (Max.y - Origin.y) / Direction.y;
//
//if (tyMin > tyMax)
//{
//float temp = tyMin;
//tyMin = tyMax;
//tyMax = temp;
//}
//
//if ((tMin > tyMax) || (tyMin > tMax))
//{
//core::Log(ELogType::Info, "Raycast::Intersects() - Ray does not intersect bound.");
//return false;
//}
//
//if (tyMin > tMin)
//{
//tMin = tyMin;
//}
//
//if (tyMax < tMax)
//{
//tMax = tyMax;
//}
//
//float tzMin = (Min.z - Origin.z) / Direction.z;
//float tzMax = (Max.z - Origin.z) / Direction.z;
//
//if (tzMin > tzMax)
//{
//float temp = tzMin;
//tzMin = tzMax;
//tzMax = temp;
//}
//
//if ((tMin > tzMax) || (tzMin > tMax))
//{
//core::Log(ELogType::Info, "Raycast::Intersects() - Ray does not intersect bound.");
//return false;
//}
//
//if (tzMin > tMin)
//{
//tMin = tzMin;
//}
//
//if (tzMax < tMax)
//{
//tMax = tzMax;
//}
//core::Log(ELogType::Info, "Raycast::Intersects() - Ray intersects bound.");
//return true;
