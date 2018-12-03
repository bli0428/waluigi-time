#ifndef SQUARERINGDRAWER_H
#define SQUARERINGDRAWER_H

#include <vector>
#include "glm/geometric.hpp"

/**
 * @brief The SquareRingDrawer class, a helper class that draws a ring of squares.
 */
class SquareRingDrawer
{
public:
    SquareRingDrawer();
    std::vector<glm::vec3> draw(float upperRadius, float lowerRadius, float upperY, float lowerY, int numIntervals);
};

#endif // SQUARERINGDRAWER_H
