#ifndef OPENGLSHAPE_H
#define OPENGLSHAPE_H

#include "GL/glew.h"

#include <memory>
#include <vector>

#include "../gl/datatype/VBO.h"
#include "../gl/datatype/VBOAttribMarker.h"
#include "glm/geometric.hpp"

namespace CS123 { namespace GL {

class VAO;

}}

using namespace CS123::GL;

class OpenGLShape {
public:
    OpenGLShape(int p1, int p2, int p3, int shapeType);
    virtual ~OpenGLShape();

    /** Draw the initialized geometry. */
    void draw();
    bool isEquals(int p1, int p2, int p3, int shapeType);

protected:
    /** Initialize the VBO with the given vertex data. */
    void setVertexData(GLfloat *data, int size, VBO::GEOMETRY_LAYOUT drawMode, int numVertices);

    /** Enables the specified attribute and calls glVertexAttribPointer with the given arguments. */
    void setAttribute(GLuint index, GLuint numElementsPerVertex, int offset, VBOAttribMarker::DATA_TYPE type,
                      bool normalize);

    void pushCoord(glm::vec3 coord);

    /** Build the VAO given the specified vertex data and atrributes */
    void buildVAO();

    virtual void initialize();

    int m_p1;
    int m_p2;
    int m_p3;
    int m_shapeType;
    std::vector<float> m_coordinates;

private:
    GLfloat *m_data;                            /// vector of floats containing the vertex data.
    GLsizeiptr m_size;                          /// size of the data array, in bytes.
    VBO::GEOMETRY_LAYOUT m_drawMode;            /// drawing mode
    int m_numVertices;                          /// number of vertices to be rendered
    std::vector<VBOAttribMarker> m_markers;     /// list of VBOAttribMarkers that describe how the data is laid out.
    std::unique_ptr<CS123::GL::VAO> m_VAO;      /// a wrapper for the vertex array object (VAO)


};

#endif // OPENGLSHAPE_H
