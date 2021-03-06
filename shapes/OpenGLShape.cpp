#include "OpenGLShape.h"

#include "glm/geometric.hpp"
#include "gl/datatype/VAO.h"
#include "../gl/shaders/ShaderAttribLocations.h"

OpenGLShape::OpenGLShape(int p1, int p2, int p3, int shapeType) :
    m_size(0),
    m_drawMode(VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES),
    m_numVertices(0),
    m_VAO(nullptr),
    m_p1(p1),
    m_p2(p2),
    m_p3(p3),
    m_shapeType(shapeType)
{
    // You shouldn't need to do anything here
    // Aren't initializer lists amazing?
}

OpenGLShape::~OpenGLShape()
{
    // You shouldn't need to do anything here
    // m_VAO's destructor will automatically be called
    // Aren't smart pointers amazing?
}

/**
 * @brief OpenGLShape::isEquals Tests if a shape is equal to this one. Used by the scenegraph to determine if we should recalculate the vertices.
 * @param p1
 * @param p2
 * @param p3
 * @param shapeType
 * @return
 */
bool OpenGLShape::isEquals(int p1, int p2, int p3, int shapeType) {
    return (p1 == m_p1 && p2 == m_p2 && p3 == m_p3 && shapeType == m_shapeType);
}

void OpenGLShape::initialize() {
    int elementsPerVertex = 8; // 3 position coordinates and 3 normal coordinates and 2 texture coordinates
    this->setVertexData(&m_coordinates[0], m_coordinates.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, m_coordinates.size() / elementsPerVertex);
    this->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    this->setAttribute(ShaderAttrib::NORMAL, 3, 12, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    this->setAttribute(ShaderAttrib::TEXCOORD0, 2, 24, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    this->buildVAO();
}

/**
 * @param data - Vector of floats containing the vertex data.
 * @param size - number of elements in the array.
 * @param drawMode - Drawing mode
 * @param numVertices - Number of vertices to be rendered.
 */
void OpenGLShape::setVertexData(GLfloat *data, int size, VBO::GEOMETRY_LAYOUT drawMode, int numVertices) {
    // Store the vertex data and other values to be used later when constructing the VAO
    m_data = data;
    m_size = size;
    m_drawMode = drawMode;
    m_numVertices = numVertices;
}


/**
 * @param index OpenGL handle to the attribute location. These are specified in ShaderAttribLocations.h
 * @param numElementsPerVertex Number of elements per vertex. Must be 1, 2, 3 or 4 (e.g. position = 3 for x,y,z)
 * @param offset Offset in BYTES from the start of the array to the beginning of the first element
 * @param type Primitive type (FLOAT, INT, UNSIGNED_BYTE)
 * @param normalize
 */
void OpenGLShape::setAttribute(
         GLuint index, GLuint numElementsPerVertex, int offset, VBOAttribMarker::DATA_TYPE type, bool normalize) {
    m_markers.push_back(VBOAttribMarker(index, numElementsPerVertex, offset, type, normalize));
}

void OpenGLShape::buildVAO() {
    CS123::GL::VBO vbo = VBO(m_data, m_size, m_markers, m_drawMode);
    m_VAO = std::make_unique<VAO>(vbo, m_numVertices);
}

void OpenGLShape::draw() {
    if (m_VAO) {
        m_VAO->bind();
        m_VAO->draw();
        m_VAO->unbind();
    }
}

/**
 * @brief OpenGLShape::pushCoord Helper method for adding an entire glm::vec3 coordinate to m_coordinates
 * @param coord The coordinate to push
 */
void OpenGLShape::pushCoord(glm::vec3 coord) {
    m_coordinates.push_back(coord.x);
    m_coordinates.push_back(coord.y);
    m_coordinates.push_back(coord.z);
}

void OpenGLShape::pushCoord(glm::vec2 coord) {
    m_coordinates.push_back(coord.x);
    m_coordinates.push_back(coord.y);
}
