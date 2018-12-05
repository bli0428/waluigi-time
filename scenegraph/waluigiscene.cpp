#include "waluigiscene.h"
#include "iostream"
#include "gl/shaders/CS123Shader.h"
#include "column.h"
#include "glm/gtx/transform.hpp"

/**
 * @brief WaluigiScene::WaluigiScene
 * This scene INHERITS from SceneView, so all the VR hand-stuff and the camera/lights/etc initialization is already there. This just overrides how
 * the geometry/lights are done.
 */
WaluigiScene::WaluigiScene() : SceneviewScene()
{
    m_column = std::make_unique<Column>(20, 10);
    m_time = 0.f;
    m_testNum = 1;
    m_leftPressed = false;
    m_rightPressed = false;
}

WaluigiScene::~WaluigiScene() {

}

void WaluigiScene::renderGeometry() {
    m_phongShader->setUniform("m", glm::scale(glm::vec3(1, 5, 1)));
    CS123SceneMaterial material = CS123SceneMaterial();
    material.cDiffuse = glm::vec4(0.5f, 0.2f, 0.2f, 0.f);
    material.cAmbient = glm::vec4(0.2f, 0.f, 0.2f, 0.f);

    m_phongShader->applyMaterial(material);
    m_column->draw();



    m_time += 1.f / 60.f;
    drawBalls();

    drawHands();
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
        m_ball = std::make_unique<Sphere>(20, 20, 20, 0.1f);
        m_handShape = std::make_unique<Sphere>(4, 4, 4, 0.1f);
        //m_testSphere = std::make_unique<Sphere>(20, 20, 20, 0.1f);

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
    if(m_leftPressed) {
        drawHand(m_leftHand);
    }
    if(m_rightPressed) {
        drawHand(m_rightHand);
    }
}

void WaluigiScene::drawBalls() {
    /*QMutableListIterator<float> i(m_ballTimes);
    while(i.hasNext()) {
        float t = i.next();
        if(t > 10.f) {
            i.remove();
        }
        else {

        }
    }*/

    for(int i = m_ballTimes.size() - 1; i >= 0; i--) {
        float t = m_ballTimes.at(i);
        if(t > 10.f) {
            m_ballTimes.removeAt(i);
            m_ballVelocities.removeAt(i);
            m_ballPositions.removeAt(i);
        }
        else {
            t += 1.f / 60.f;
            drawBall(t, m_ballVelocities.at(i), m_ballPositions.at(i));
            m_ballTimes.replace(i, t);
        }
    }
}

void WaluigiScene::drawBall(float time, glm::vec3 vel, glm::mat4x4 pos) {
    glm::mat3x3 invRot = glm::transpose(glm::mat3x3(pos));
    glm::vec3 vel2 = invRot * vel;
    glm::vec4 func = glm::vec4(vel2.x * time, (vel2.y * time) + (.5f * -1.f * time * time), vel2.z * time, 1.f);
    m_phongShader->setUniform("m", glm::translate(pos, (func).xyz()));
    m_phongShader->applyMaterial(m_material);
    m_ball->draw();
}

void WaluigiScene::updateControllerMaterial(PrimitiveNode hand) {

    if (!didSetMaterial) {
        hand.material = m_material;
    }
    hand.type = PrimitiveType::PRIMITIVE_SPHERE;
}

//These will be used in view.cpp to update the hand positions every frame
void WaluigiScene::setLeftHand(glm::mat4x4 transform) {
    m_leftHand.matrix = transform;
}

void WaluigiScene::setRightHand(glm::mat4x4 transform) {
    m_rightHand.matrix = transform;
}

void WaluigiScene::setLeftHandVelocity(glm::vec3 velocity) {
    //m_testNum = (int)velocity.length();
    m_testNum = (int)glm::length(velocity);

    m_leftVel = velocity;
}

void WaluigiScene::setRightHandVelocity(glm::vec3 velocity) {
    m_rightVel = velocity;
}

void WaluigiScene::setTrigger(int controllerNum, bool pressed) {
    if(controllerNum == 0) {
        //left hand
        m_leftPressed = pressed;
        if(!pressed /*&& glm::length(m_leftVel) > 1.f*/) {
            m_ballTimes.append(0.f);
            m_ballVelocities.append(m_leftVel);
            m_ballPositions.append(m_leftHand.matrix);
        }
    }
    else if(controllerNum == 1) {
        //right hand
        m_rightPressed = pressed;
        if(!pressed /*&& glm::length(m_rightVel) > 1.f*/) {
            m_ballTimes.append(0.f);
            m_ballVelocities.append(m_rightVel);
            m_ballPositions.append(m_rightHand.matrix);
        }
    }
    else {
        //Invalid number
    }
}


void WaluigiScene::drawTestSphere(int x) {

    m_phongShader->setUniform("m", glm::translate(glm::vec3(0.f, 2.f * fabs(sin(m_time)), x)));
    m_phongShader->applyMaterial(m_material);
    m_ball->draw();
}

void WaluigiScene::drawHand(PrimitiveNode hand) {
    m_phongShader->setUniform("m", hand.matrix);
    m_phongShader->applyMaterial(m_material);
    m_handShape->draw();
}

