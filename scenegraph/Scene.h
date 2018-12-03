#ifndef SCENE_H
#define SCENE_H

#include "CS123SceneData.h"
#include "shapes/OpenGLShape.h"

class Camera;
class CS123ISceneParser;


/**
 * @class Scene
 *
 * @brief This is the base class for all scenes. Modify this class if you want to provide
 * common functionality to all your scenes.
 */
struct PrimitiveNode {
   PrimitiveType type;
   CS123SceneMaterial material;
   glm::mat4x4 matrix;
};

class Scene {
public:
    Scene();
    Scene(Scene &scene);
    virtual ~Scene();

    virtual void settingsChanged() {}

    static void parse(Scene *sceneToFill, CS123ISceneParser *parser);

    std::vector<PrimitiveNode> getPrimitives() { return m_primitives; }
    std::vector<CS123SceneLightData> getLights() { return m_lights; }
    CS123SceneGlobalData getGlobal() { return m_global; }

protected:

    // Adds a primitive to the scene.
    virtual void addPrimitive(const CS123ScenePrimitive scenePrimitive, const glm::mat4x4 matrix);

    // Adds a light to the scene.
    virtual void addLight(const CS123SceneLightData sceneLight);

    // Sets the global data for the scene.
    virtual void setGlobal(const CS123SceneGlobalData global);

    bool m_rerender = false;
    std::vector<PrimitiveNode> m_primitives;
    std::vector<CS123SceneLightData> m_lights;
    CS123SceneGlobalData m_global;



private:
    void finishParsing();
    void startParsing();
    void processNode(CS123SceneNode *node, glm::mat4x4 parentMatrix);
};

#endif // SCENE_H
