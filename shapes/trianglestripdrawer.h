#ifndef TRIANGLESTRIPDRAWER_H
#define TRIANGLESTRIPDRAWER_H

#include <vector>
#include "glm/geometric.hpp"
/**
 * @brief The TriangleStripDrawer class, which draws a flat strip of triangles. The user can specify which direction its axis goes in. Can also draw a flat circle.
 */
class TriangleStripDrawer
{

public:
    TriangleStripDrawer();
    std::vector<glm::vec3> draw(glm::vec3 tip, glm::vec3 bottomLeft, glm::vec3 bottomRight, int numIntervals);
    std::vector<glm::vec3> drawCircle(glm::vec3 center, float radius, int numSlices, int numIntervals, bool reverse);
};

#endif // TRIANGLESTRIPDRAWER_H
