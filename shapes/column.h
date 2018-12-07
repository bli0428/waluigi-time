#ifndef COLUMN_H
#define COLUMN_H

#include "OpenGLShape.h"
#include "trianglestripdrawer.h"
#include "squareringdrawer.h"

/**
 * @brief The Cylinder class, which inherits from OpenGLShape
 */
class Column : public OpenGLShape
{

public:
    Column(int p1, int p2);

protected:
    void initialize() override;

private:
    void generateVertices();
    void generateCap();
    void generateRing(int floor);
    void generateOffsets();
    glm::vec3 getPosition(int level, int wedge);
    glm::vec3 getNormal(int level, int wedge);
    void addVertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 texture);
    glm::vec2 getUV(glm::vec3 point);

    std::vector<glm::vec2> m_offsets;

    // CONSTANTS
    const float M_SMOOTHNESS = 8.0f; // higher is smoother; 8 is a bit rough, 15 is gentle-smooth
};

#endif // COLUMN_H
