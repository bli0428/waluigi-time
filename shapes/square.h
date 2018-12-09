#ifndef SQUARE_H
#define SQUARE_H

#include "OpenGLShape.h"

class Square : public OpenGLShape
{
public:
    Square();

protected:
    void initialize() override;

private:
    void addVertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 texture);
};

#endif // SQUARE_H
