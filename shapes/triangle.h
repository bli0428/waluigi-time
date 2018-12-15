#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "OpenGLShape.h"

class Triangle : public OpenGLShape
{
public:
    Triangle();

protected:
    void initialize() override;

private:
    void addVertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 texture);
};

#endif // TRIANGLE_H
