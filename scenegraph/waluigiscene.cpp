#include "waluigiscene.h"
#include "iostream"
#include "gl/shaders/CS123Shader.h"

WaluigiScene::WaluigiScene() : SceneviewScene()
{
}

WaluigiScene::~WaluigiScene() {

}

void WaluigiScene::renderGeometry() {
    m_cube = std::make_unique<Cube>(4, 4, 4);
    m_phongShader->setUniform("m", glm::mat4x4());

    CS123SceneMaterial material = CS123SceneMaterial();
    material.cDiffuse = glm::vec4(1.f, 0.f, 0.f, 0.f);
    material.cAmbient = glm::vec4(0.3f, 0.f, 0.f, 0.f);

    m_phongShader->applyMaterial(material);
    m_cube->draw();
}

void WaluigiScene::setLights() {
    CS123SceneLightData light = CS123SceneLightData();
    light.type = LightType::LIGHT_POINT;
    light.pos = glm::vec4(0.f, 0.f, 5.f, 1.f);
    light.color = glm::vec4(1.f, 1.f, 1.f, 1.f);

    m_phongShader->setLight(light);
}

void WaluigiScene::drawHands() {
    //Ideally, if we put more work into the hands (create textures/colors, etc), we can delete the if statement
    //for now, I'm just stealing the color of the first node m_primitives so that you can actually see the hands
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
    SceneviewScene::drawHand(m_leftHand);
    SceneviewScene::drawHand(m_rightHand);
}
