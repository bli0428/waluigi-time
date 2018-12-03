#include "trianglestripdrawer.h"
#include "glm/trigonometric.hpp"
#include <iostream>

TriangleStripDrawer::TriangleStripDrawer()
{
}

/**
 * @brief TriangleStripDrawer::draw Returns a vector of coordinates that correspond to the triangle.
 * @param tip Coordinate of the tip of the triangle
 * @param bottomLeft Coordinate of the base left
 * @param bottomRight Coordinate of the base right
 * @param numIntervals Number of intermediate levels to draw in between from top to bottom
 */
std::vector<glm::vec3> TriangleStripDrawer::draw(glm::vec3 tip, glm::vec3 bottomLeft, glm::vec3 bottomRight, int numIntervals) {
    glm::vec3 leftIncrement = (bottomLeft - tip) / static_cast<float>(numIntervals); // the vector to add to move down towards the base by 1 level to the left
    glm::vec3 rightIncrement = (bottomRight - tip) / static_cast<float>(numIntervals); // see above, but to the right

    std::vector<glm::vec3> coords;
    coords.reserve(2*numIntervals + 1);

    for (float row = 0.0; row <= numIntervals; row++) {
        coords.push_back(tip + leftIncrement * row);
        coords.push_back(tip + rightIncrement * row);
    }

    // "resets" back at the tip to prevent additional triangles from being drawn
    coords.push_back(tip);
    coords.push_back(tip);

    return coords;
}

/**
 * @brief TriangleStripDrawer::drawCircle Draws a flat circle using draw as a helper method
 * @param tip the center of the circle
 * @param radius the radius of the circle
 * @param numSlices number of pizza wedges
 * @param numIntervals number of levels in between the outside and the center
 * @param reverse draw the triangles reversed (depending on orientation)
 * @return
 */
std::vector<glm::vec3> TriangleStripDrawer::drawCircle(glm::vec3 center, float radius, int numSlices, int numIntervals, bool reverse) {
    std::vector<glm::vec3> coords;
    coords.reserve((2 * numIntervals + 1) * numSlices);

    // get the theta interval (the angles we should space strips out with)
    float thetaInterval = 2 * M_PI / numSlices;

    // draw a triangle strip for each interval
    for (int side = 0; side < numSlices; side++) {
        // calculate the start and end as well as Cartesian coordinates for the triangle's bottom vertices
        float startArcAngle = thetaInterval * (side + 1);
        float endArcAngle = thetaInterval * side;
        glm::vec3 bottomLeft = glm::vec3(glm::cos(startArcAngle) * radius, center.y, glm::sin(startArcAngle) * radius);
        glm::vec3 bottomRight = glm::vec3(glm::cos(endArcAngle) * radius, center.y, glm::sin(endArcAngle) * radius);

        // get the coordinates and push to the return vector
        std::vector<glm::vec3> stripCoords = this->draw(center, reverse ? bottomRight : bottomLeft, reverse ? bottomLeft : bottomRight, numIntervals);
        for (glm::vec3 coord : stripCoords) {
            coords.push_back(coord);
        }
    }
    return coords;
}
