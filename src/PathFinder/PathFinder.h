#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include "../Scene/Scene.h"

const float MAP_SCALE = 5.0f;

class PathFinder {
public:
    PathFinder(Scene& scene);
    Vector3 getNextPosition(Vector3 from, Vector3 to);
    bool hasLineOfSight(Vector3 from, Vector3 to);
    static Vector2 worldToMap(Vector3 worldPos);
    static Vector3 mapToWorld(Vector2 mapPos);

private:
    Scene& scene;
};

#endif