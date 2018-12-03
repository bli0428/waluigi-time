#include "squareringdrawer.h"
#include "math.h"
#include "glm/trigonometric.hpp"

SquareRingDrawer::SquareRingDrawer()
{
}

/**
 * @brief SquareRingDrawer::draw Draws a ring in a rectangular formation.
 * @param upperRadius Radius of the upper ring
 * @param lowerRadius Radius of the lower ring
 * @param upperY Height of the upper ring
 * @param lowerY Height of the lower ring
 * @param numIntervals Number of intervals to draw
 */
std::vector<glm::vec3> SquareRingDrawer::draw(float upperRadius, float lowerRadius, float upperY, float lowerY, int numIntervals) {
    std::vector<glm::vec3> coords;
    coords.reserve(2 * (numIntervals + 1));

    // calculates the theta interval
    float thetaInterval = 2.0 * M_PI / numIntervals;
    for (int i = 0; i <= numIntervals; i++) {
        // for each interval we're at, plot two points; this will create the zig-zag triangle effect
        coords.push_back(glm::vec3(glm::cos(thetaInterval * i) * upperRadius, upperY, glm::sin(thetaInterval * i) * upperRadius));
        coords.push_back(glm::vec3(glm::cos(thetaInterval * i) * lowerRadius, lowerY, glm::sin(thetaInterval * i) * lowerRadius));
    }

    return coords;
}
