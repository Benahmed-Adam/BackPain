#include "PathFinder.h"

#include <algorithm>
#include <cmath>
#include <map>
#include <queue>
#include <vector>

Vector2
PathFinder::worldToMap(Vector3 worldPos)
{
    return { (float)round(worldPos.x / MAP_SCALE),
        (float)round(worldPos.z / MAP_SCALE) };
}

Vector3
PathFinder::mapToWorld(Vector2 mapPos)
{
    return { mapPos.x * MAP_SCALE, 0, mapPos.y * MAP_SCALE };
}

struct Node {
    Vector2 pos;
    float g_cost;
    float h_cost;
    float f_cost;
    Vector2 parent;

    bool
    operator>(const Node& other) const
    {
        return f_cost > other.f_cost;
    }
};

static float
heuristic(Vector2 a, Vector2 b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

PathFinder::PathFinder(Scene& scene)
    : scene(scene)
{
}

bool PathFinder::hasLineOfSight(Vector3 from, Vector3 to)
{
    Vector2 p1 = worldToMap(from);
    Vector2 p2 = worldToMap(to);

    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;

    float steps = fmax(abs(dx), abs(dy));

    if (steps == 0)
        return true;

    float xInc = dx / steps;
    float yInc = dy / steps;

    float x = p1.x;
    float y = p1.y;

    for (int i = 0; i <= steps; i++) {
        if (scene.isWall(round(x), round(y))) {
            return false;
        }
        x += xInc;
        y += yInc;
    }

    return true;
}

Vector3
PathFinder::getNextPosition(Vector3 from, Vector3 to)
{
    Vector2 startPos = PathFinder::worldToMap(from);
    Vector2 goalPos = PathFinder::worldToMap(to);

    if (startPos.x == goalPos.x && startPos.y == goalPos.y) {
        return from;
    }

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open_set;
    std::map<std::pair<float, float>, Node> all_nodes;

    Node startNode;
    startNode.pos = startPos;
    startNode.g_cost = 0;
    startNode.h_cost = heuristic(startPos, goalPos);
    startNode.f_cost = startNode.g_cost + startNode.h_cost;
    startNode.parent = { -1, -1 };

    open_set.push(startNode);
    all_nodes[{ startPos.x, startPos.y }] = startNode;

    std::vector<Vector2> neighbors_diff
        = { { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } };

    while (!open_set.empty()) {
        Node current = open_set.top();
        open_set.pop();

        if (current.pos.x == goalPos.x && current.pos.y == goalPos.y) {
            std::vector<Vector2> path;
            Vector2 current_pos = goalPos;
            while (current_pos.x != -1) {
                path.push_back(current_pos);
                current_pos
                    = all_nodes.at({ current_pos.x, current_pos.y }).parent;
            }
            std::reverse(path.begin(), path.end());

            if (path.size() > 1) {
                return PathFinder::mapToWorld(path[1]);
            } else {
                return from;
            }
        }

        for (auto& diff : neighbors_diff) {
            Vector2 neighbor_pos
                = { current.pos.x + diff.x, current.pos.y + diff.y };

            if (scene.isWall(neighbor_pos.x, neighbor_pos.y)) {
                continue;
            }

            float new_g_cost = current.g_cost + 1;

            if (all_nodes.find({ neighbor_pos.x, neighbor_pos.y })
                    == all_nodes.end()
                || new_g_cost < all_nodes.at({ neighbor_pos.x, neighbor_pos.y })
                                    .g_cost) {
                Node neighbor_node;
                neighbor_node.pos = neighbor_pos;
                neighbor_node.g_cost = new_g_cost;
                neighbor_node.h_cost = heuristic(neighbor_pos, goalPos);
                neighbor_node.f_cost
                    = neighbor_node.g_cost + neighbor_node.h_cost;
                neighbor_node.parent = current.pos;

                all_nodes[{ neighbor_pos.x, neighbor_pos.y }] = neighbor_node;
                open_set.push(neighbor_node);
            }
        }
    }

    return from;
}