#include "Scene.h"
#include "CS123ISceneParser.h"
#include <iostream>
#include "glm/ext.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/ext.hpp"


Scene::Scene()
{
}

Scene::Scene(Scene &scene)
{
    // We need to set the global constants to one when we duplicate a scene,
    // otherwise the global constants will be double counted (squared)
    CS123SceneGlobalData global = { 1, 1, 1, 1};
    setGlobal(global);

    // TODO [INTERSECT]
    // Make sure to copy over the lights and the scenegraph from the old scene,
    // as well as any other member variables your new scene will need.

}

Scene::~Scene()
{
    // Do not delete m_camera, it is owned by SupportCanvas3D
}

/**
 * @brief Scene::parse A static method that parses a scenegraph returned from the given parser, loading them into sceneToFill.
 * @param sceneToFill
 * @param parser
 */
void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser) {
    sceneToFill->startParsing();

    // load global data
    CS123SceneGlobalData global;
    parser->getGlobalData(global);
    sceneToFill->setGlobal(global);

    // load light data
    for (int i = 0; i < parser->getNumLights(); i++) {
        CS123SceneLightData light;
        parser->getLightData(i, light);
        sceneToFill->addLight(light);
    }

    // start processing the primitive tree
    sceneToFill->processNode(parser->getRootNode(), glm::mat4x4()); // pass in identity matrix as the parent
    sceneToFill->finishParsing();
}

/**
 * @brief Scene::processNode A recursive method that visits every node in post-order traversal, collapsing the matrix up to this point.
 * @param node
 * @param parentMatrix
 */
void Scene::processNode(CS123SceneNode *node, glm::mat4x4 parentMatrix) {
    glm::mat4x4 selfMatrix = glm::mat4x4(); // matrix of its own transformations

    // for each transformation, get its appropriate matrix and compound it onto selfMatrix
    for (CS123SceneTransformation *transform : node->transformations) {
        glm::mat4x4 transformMatrix;

        switch (transform->type) {
        case TRANSFORMATION_SCALE:
            transformMatrix = glm::scale(transform->scale);
            break;
        case TRANSFORMATION_TRANSLATE:
            transformMatrix = glm::translate(transform->translate);
            break;
        case TRANSFORMATION_ROTATE:
            transformMatrix = glm::rotate(transform->angle, transform->rotate);
            break;
        case TRANSFORMATION_MATRIX:
            transformMatrix = transform->matrix;
            break;
        default:
            break;
        }

        selfMatrix *= transformMatrix;
    }

    // selfMatrix is the parentMatrix multiplied with itself to get the final position in world space
    selfMatrix = parentMatrix * selfMatrix;

    // loop through each primitive in this block and add it to our list of primitives
    for (CS123ScenePrimitive *prim : node->primitives) {
        CS123ScenePrimitive primitive = *prim;

        this->addPrimitive(primitive, selfMatrix); // collapsed cumulative matrix from root node to here
    }

    // now visit every child node in post-order traversal
    for (CS123SceneNode *child : node->children) {
        this->processNode(child, selfMatrix);
    }
}

/**
 * @brief Scene::addPrimitive Adds a primitive to the vector of primitives
 * @param scenePrimitive
 * @param matrix
 */
void Scene::addPrimitive(const CS123ScenePrimitive scenePrimitive, const glm::mat4x4 matrix) {
    m_primitives.push_back(PrimitiveNode{scenePrimitive.type, scenePrimitive.material, matrix});
}

/**
 * @brief Scene::addLight Add light to the vector of lights
 * @param sceneLight
 */
void Scene::addLight(const CS123SceneLightData sceneLight) {
    m_lights.push_back(sceneLight);
}

/**
 * @brief Scene::setGlobal Set the global data
 * @param global
 */
void Scene::setGlobal(const CS123SceneGlobalData global) {
    m_global = global;
}

/**
 * @brief Scene::startParsing Clears vectors so we don't render scenes on top of scenes
 */
void Scene::startParsing() {
    m_primitives.clear();
    m_lights.clear();
}

/**
 * @brief Scene::finishParsing Finishes parsing
 */
void Scene::finishParsing() {
    m_rerender = true;
}
