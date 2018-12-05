#include "waluigiscene.h"
#include "iostream"
#include "gl/shaders/CS123Shader.h"
#include "glm/gtx/transform.hpp"

/**
 * @brief WaluigiScene::WaluigiScene
 * This scene INHERITS from SceneView, so all the VR hand-stuff and the camera/lights/etc initialization is already there. This just overrides how
 * the geometry/lights are done.
 */
WaluigiScene::WaluigiScene() : SceneviewScene()
{
    m_column = std::make_unique<Column>(20, 10);
}

WaluigiScene::~WaluigiScene() {

}

void WaluigiScene::renderGeometry() {
    m_phongShader->setUniform("m", glm::scale(glm::vec3(1, 4, 1)));

    CS123SceneMaterial material = CS123SceneMaterial();
    material.cDiffuse = glm::vec4(0.5f, 0.2f, 0.2f, 0.f);
    material.cAmbient = glm::vec4(0.2f, 0.f, 0.2f, 0.f);

    m_phongShader->applyMaterial(material);
    m_column->draw();
}

void WaluigiScene::setLights() {
    CS123SceneLightData light = CS123SceneLightData();
    light.type = LightType::LIGHT_POINT;
    light.pos = glm::vec4(0.f, 0.f, 5.f, 1.f);
    light.color = glm::vec4(1.f, 1.f, 1.f, 1.f);

    m_phongShader->setLight(light);
}

/**
 * @brief WaluigiScene::drawHands Moved all Waluigi-specific hand drawing stuff here, which just makes red hands for now.
 * A similar method is still in Sceneview; I didn't really completely refactor this, but this is the method to change for our
 * Waluigi-specific things.
 */
void WaluigiScene::drawHands() {
    if (!didSetMaterial) {
        m_handShape = std::make_unique<Sphere>(4, 4, 4, 0.1f);

        CS123SceneMaterial material = CS123SceneMaterial();
        material.cDiffuse = glm::vec4(1.f, 0.f, 0.f, 1.f);
        material.cAmbient = glm::vec4(0.3f, 0.f, 0.f, 1.f);
        m_material = material;

        didSetMaterial = true;
        updateControllerMaterial(m_leftHand);
        updateControllerMaterial(m_rightHand);
    }


    //now handle the controller stuff-I'll might move this into primitives later,
    //but the hand position needs to be updated every frame, so it might be tough
//    SceneviewScene::drawHand(m_leftHand);
//    SceneviewScene::drawHand(m_rightHand);
}
