#include "SceneviewScene.h"
#include "GL/glew.h"
#include <QGLWidget>
#include <iostream>
#include "lib/ResourceLoader.h"
#include "gl/shaders/CS123Shader.h"
#include "shapes/Cube.h"
#include "glm/gtx/transform.hpp"
using namespace CS123::GL;


SceneviewScene::SceneviewScene()
{
    // TODO: [SCENEVIEW] Set up anything you need for your Sceneview scene here...
    //connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
    m_time = 0.f;
    m_testNum = 1;
    loadPhongShader();
}

SceneviewScene::~SceneviewScene()
{
}

void SceneviewScene::loadPhongShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/shader.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/shader.frag");
    m_phongShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::render(glm::mat4x4 projectionMatrix, glm::mat4x4 viewMatrix) {
    m_phongShader->bind();
    setSceneUniforms(projectionMatrix, viewMatrix);
    setLights();
    renderGeometry();
    glBindTexture(GL_TEXTURE_2D, 0);
    m_phongShader->unbind();
}

void SceneviewScene::render(
    glm::mat4x4 projectionMatrix,
    glm::mat4x4 viewMatrix,
    glm::mat4 m_mat4DevicePose[vr::k_unMaxTrackedDeviceCount],
    bool m_activeTrackedDevice[vr::k_unMaxTrackedDeviceCount]) {

    setClearColor();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_phongShader->bind();
    setLights();
    renderGeometry();
    glBindTexture(GL_TEXTURE_2D, 0);
    m_phongShader->unbind();

    // TODO: use controller positions if necessary
    render(projectionMatrix, viewMatrix);
}


void SceneviewScene::setSceneUniforms(glm::mat4x4 &projectionMatrix, glm::mat4x4 &viewMatrix) {
    m_phongShader->setUniform("useLighting", true);
    m_phongShader->setUniform("useArrowOffsets", false);
    m_phongShader->setUniform("p", projectionMatrix);
    m_phongShader->setUniform("v", viewMatrix);
}

void SceneviewScene::setLights()
{
    for (CS123SceneLightData light : m_lights) {
        m_phongShader->setLight(light);
    }
}

void SceneviewScene::renderGeometry() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    m_time += 1.f / 60.f;
        // we use flyweight patterns here, so we only ever save one of each type to draw instead of making one for all of them
        if (m_rerender) {
            m_cube = std::make_unique<Cube>(4, 4, 4);
            m_cone = std::make_unique<Cone>(4, 4, 4);
            m_sphere = std::make_unique<Sphere>(4, 4, 4);
            m_cylinder = std::make_unique<Cylinder>(4, 4, 4);
            m_rerender = false;

            m_testSphere = std::make_unique<Sphere>(20, 20, 20, 0.1f);

            m_handShape = std::make_unique<Sphere>(4, 4, 4, 0.1f);
        }

        // for each node, we set its model uniform and apply its material, and then draw the appropriate shape
        for (PrimitiveNode node : m_primitives) {
            m_phongShader->setUniform("m", node.matrix);

            // make material lighting adjustments to account for global lighting
            node.material.cDiffuse *= m_global.kd;
            node.material.cAmbient *= m_global.ka;

            //Ideally, if we put more work into the hands (create textures/colors, etc), we can delete the if statement
            //for now, I'm just stealing the color of the first node m_primitives so that you can actually see the hands
            if (!didSetMaterial) {
                m_material = node.material;
                didSetMaterial = true;
                updateControllerMaterial(m_leftHand);
                updateControllerMaterial(m_rightHand);
            };

            m_phongShader->applyMaterial(node.material);

            switch (node.type) {
            case PrimitiveType::PRIMITIVE_CUBE:
                m_cube->draw();
                break;
            case PrimitiveType::PRIMITIVE_CONE:
                m_cone->draw();
                break;
            case PrimitiveType::PRIMITIVE_CYLINDER:
                m_cylinder->draw();
                break;
            case PrimitiveType::PRIMITIVE_SPHERE:
                m_sphere->draw();
                break;
            default:
                m_cube->draw();
                break;
            }
        }

        //now handle the controller stuff-I'll might move this into primitives later,
        //but the hand position needs to be updated every frame, so it might be tough

        for(int i = 0; i < m_testNum; i++) {
            drawTestSphere(i);
        }
        drawHand(m_leftHand);
        drawHand(m_rightHand);
}

void SceneviewScene::drawTestSphere(int x) {

    m_phongShader->setUniform("m", glm::translate(glm::vec3(0.f, 2.f * fabs(sin(m_time)), x)));
    m_phongShader->applyMaterial(m_material);
    m_testSphere->draw();
}

void SceneviewScene::drawHand(PrimitiveNode hand) {
    m_phongShader->setUniform("m", hand.matrix);
    m_phongShader->applyMaterial(m_material);
    m_handShape->draw();
}

void SceneviewScene::updateControllerMaterial(PrimitiveNode hand) {

    if (!didSetMaterial) {
        hand.material = m_material;
    }
    hand.type = PrimitiveType::PRIMITIVE_SPHERE;
}

//These will be used in view.cpp to update the hand positions every frame
void SceneviewScene::setLeftHand(glm::mat4x4 transform) {
    m_leftHand.matrix = transform;
}

void SceneviewScene::setRightHand(glm::mat4x4 transform) {
    m_rightHand.matrix = transform;
}

void SceneviewScene::setLeftHandVelocity(glm::vec3 velocity) {
    //m_testNum = (int)velocity.length();
    m_testNum = (int)glm::length(velocity);

    m_leftVel = velocity;
}

void SceneviewScene::setRightHandVelocity(glm::vec3 velocity) {
    m_rightVel = velocity;
}

void SceneviewScene::settingsChanged() {
    m_rerender = true;
}

