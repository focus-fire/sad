#include "sadpch.h"
#include "Raycast.h"

bool sad::Raycast::Intersects(glm::vec3 origin, glm::vec3 direction, sad::Bound bound)
{
    // Algorithm
    glm::vec3 invDir = 1.0f / direction;
    glm::vec3 tbot = invDir * (bound.GetBoundMin() - origin);
    glm::vec3 ttop = invDir * (bound.GetBoundMax() - origin);

    glm::vec3 tmin = glm::min(ttop, tbot);
    glm::vec3 tmax = glm::max(ttop, tbot);

    float t0 = glm::max(tmin.x, glm::max(tmin.y, tmin.z));
    float t1 = glm::min(tmax.x, glm::min(tmax.y, tmax.z));

    // if t0 > t1, ray (line) is intersecting AABB, but the whole AABB is behind us
    if (t0 > t1)
    {
        core::Log(ELogType::Trace, "Raycast::Intersects() - Ray does not intersect bound.");
        return false;
    }

    // if t1 < 0, ray doesn't intersect AABB
    if (t1 < 0)
    {
        core::Log(ELogType::Trace, "Raycast::Intersects() - Ray does not intersect bound.");
        return false;
    }

    // if t0 < 0, ray is inside AABB
    if (t0 < 0)
    {
        core::Log(ELogType::Trace, "Raycast::Intersects() - Ray intersects bound.");
        return true;
    }

    // else, ray intersects AABB
    core::Log(ELogType::Trace, "Raycast::Intersects() - Ray intersects bound.");
    return true;
}
