#include "waluigiscene.h"
#include "iostream"
#include "gl/shaders/CS123Shader.h"
#include "column.h"
#include "glm/gtx/transform.hpp"
#include "ResourceLoader.h"
#include "QImage"
#include "stdlib.h"
#include "algorithm"

/**
 * @brief WaluigiScene::WaluigiScene
 * This scene INHERITS from SceneView, so all the VR hand-stuff and the camera/lights/etc initialization is already there. This just overrides how
 * the geometry/lights are done.
 */
WaluigiScene::WaluigiScene() : SceneviewScene(),
  m_textureProgramID(0),
  m_textureID(0),
  m_time(0.f),
  m_testNum(1),
  m_leftPressed(false),
  m_rightPressed(false)
{
    // TODO Refactor this setup later lol
    m_column = std::make_unique<Column>(30, 20);
    m_floor = std::make_unique<Cube>(1, 1, 1);
    this->generateColumns(M_FIELDLENGTH, M_FIELDLENGTH, M_COLUMNMINDIST, M_COLUMNK);
}

WaluigiScene::~WaluigiScene() {

}


//void WaluigiScene::render(glm::mat4x4 projectionMatrix, glm::mat4x4 viewMatrix) {
//    m_phongShader->bind();
//    setSceneUniforms(projectionMatrix, viewMatrix);
//    setLights();
//    renderGeometry();
//    glBindTexture(GL_TEXTURE_2D, 0);
//    m_phongShader->unbind();
//}

//void WaluigiScene::render(
//    glm::mat4x4 projectionMatrix,
//    glm::mat4x4 viewMatrix,
//    glm::mat4 m_mat4DevicePose[vr::k_unMaxTrackedDeviceCount],
//    bool m_activeTrackedDevice[vr::k_unMaxTrackedDeviceCount]) {

//    setClearColor();
//    unsigned int gBuffer;
//    glGenBuffers(1, &gBuffer);
//    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    unsigned int gPosition, gNormal, gAlbedoSpec;

//    glGenTextures(1, &gPosition);
//    glBindTexture(GL_TEXTURE_2D, gPosition);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_eyeWidth, m_eyeHeight, 0, GL_RGB, GL_FLOAT, NULL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

//    glGenTextures(1, &gNormal);
//    glBindTexture(GL_TEXTURE_2D, gNormal);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_eyeWidth, m_eyeHeight, 0, GL_RGB, GL_FLOAT, NULL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

//    glGenTextures(1, &gAlbedoSpec);
//    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_eyeWidth, m_eyeHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

//    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
//    glDrawBuffers(3, attachments);

//    m_phongShader->bind();
//    setLights();
//    renderGeometry();
//    glBindTexture(GL_TEXTURE_2D, 0);
//    m_phongShader->unbind();

//    // TODO: use controller positions if necessary
//    render(projectionMatrix, viewMatrix);
//}


void WaluigiScene::renderGeometry() {
    m_phongShader->setUniform("m", glm::scale(glm::vec3(1, 5, 1)));
    CS123SceneMaterial material = CS123SceneMaterial();
    material.cDiffuse = glm::vec4(0.5f, 0.2f, 0.2f, 0.f);
    material.cAmbient = glm::vec4(0.2f, 0.f, 0.2f, 0.f);
    m_phongShader->applyMaterial(material);

    // only draws the same column for now; will explore about other options
    for (ColumnNode node : m_columns) {
        glm::mat4x4 translate = glm::translate(glm::vec3(node.x, node.height / 2.0f - 2, node.z));
        glm::mat4x4 scale = glm::scale(glm::vec3(node.radius * 2, node.height, node.radius * 2));
        m_phongShader->setUniform("m", translate * scale);
        m_column->draw();
    }

    m_phongShader->setUniform("m", glm::translate(glm::vec3(0, -1, 0)) * glm::scale(glm::vec3(500, 0.1, 500)));
    m_floor->draw();

    m_time += 1.f / 60.f;
    drawBalls();

    drawHands();
}

/**
 * @brief WaluigiScene::generateColumns Adds ColumnNodes to the thing. Uses poisson disk sampling.
 * @param width Width of area to generate columsn in
 * @param height Height ^
 * @param min Minimum distance between columns
 * @param k how tightly packed it ends up being
 */
void WaluigiScene::generateColumns(int width, int height, float min, int k) {
    float cellSize = min / glm::sqrt(2.0f);
    int cellsAcross = std::ceil(width / cellSize);
    int cellsDown = std::ceil(height / cellSize);

    std::vector<glm::vec2> grid;
    grid.resize(cellsAcross * cellsDown, glm::vec2(-1, -1));

    std::vector<glm::vec2> samplePoints;
    std::vector<glm::vec2> processList;

    glm::vec2 firstPoint = glm::vec2(static_cast<float>(rand()) / RAND_MAX * width,
            static_cast<float>(rand()) / RAND_MAX * height);
    samplePoints.push_back(firstPoint);
    processList.push_back(firstPoint);

    grid[this->imageToGrid(firstPoint, cellSize, cellsAcross)] = firstPoint;

    while (!processList.empty()) {
        // get a point from the queue
        glm::vec2 newPoint = processList.back();
        processList.pop_back();

        // attempt to make new points for each count of k
        for (int i = 0; i < k; i++) {
            glm::vec2 pointAttempt = randPointAround(newPoint, min);

            if (pointAttempt.x <= width && pointAttempt.x >= 0 && pointAttempt.y >= 0 && pointAttempt.y <= height) {
                bool isValid = true;
                int gridIndex = imageToGrid(pointAttempt, cellSize, cellsAcross);
                if (grid[gridIndex].x >= 0) {
                    continue;
                }

                std::vector<int> cellsToCheck;
                cellsToCheck.reserve(8);
                cellsToCheck.push_back(gridIndex + 1); // to the right
                cellsToCheck.push_back(gridIndex - 1); // to the left
                cellsToCheck.push_back(gridIndex + cellsAcross); // below
                cellsToCheck.push_back(gridIndex - cellsAcross); // down
                cellsToCheck.push_back(gridIndex + cellsAcross + 1); // southeast
                cellsToCheck.push_back(gridIndex + cellsAcross - 1); // southwest
                cellsToCheck.push_back(gridIndex - cellsAcross + 1); // northeast
                cellsToCheck.push_back(gridIndex - cellsAcross - 1); // northwest

                for (int neighboring : cellsToCheck) {
                    if (grid[neighboring].x >= 0) {
                        if (glm::distance(glm::vec2(pointAttempt.x, pointAttempt.y), glm::vec2(grid[neighboring].x, grid[neighboring].y)) < min) {
                            isValid = false;
                            break;
                        }
                    }
                }

                if (isValid) {
                    samplePoints.push_back(pointAttempt);
                    processList.push_back(pointAttempt);
                    grid[gridIndex] = pointAttempt;
                }
            }
        }
    }

    for (glm::vec2 point : samplePoints) {
        float r = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
        m_columns.push_back(ColumnNode{M_COLUMNHEIGHTAVG + r * M_COLUMNHEIGHTVAR, M_COLUMNRADIUSAVG, point.x - M_FIELDLENGTH / 2, point.y - M_FIELDLENGTH / 2});
    }
}

int WaluigiScene::imageToGrid(glm::vec2 point, float cellSize, int cellsAcross) {
    int x = static_cast<int>(point.x / cellSize);
    int y = static_cast<int>(point.y / cellSize);
    return x * cellsAcross + y;
}

glm::vec2 WaluigiScene::randPointAround(glm::vec2 newPoint, float min) {
    float r = min * (static_cast<float>(rand()) / RAND_MAX + 1);
    float angle = 2 * 3.141592 * static_cast<float>(rand()) / RAND_MAX;

    return glm::vec2(newPoint.x + r * glm::cos(angle), newPoint.y + r * glm::sin(angle));
}

void WaluigiScene::setLights() {
    CS123SceneLightData light = CS123SceneLightData();
    light.type = LightType::LIGHT_POINT;
    //light.pos = glm::vec4(5.f, 10.f, 5.f, 1.f);
    light.pos = glm::vec4(10.f, 1.f, 10.f, 1.f);
    light.color = glm::vec4(1.f, 1.f, 1.f, 1.f);
    light.function = glm::vec3(1.f, 0.f, 0.f);
    light.id = 0;
    m_phongShader->setLight(light);

    for (auto fireball : Fireballs) {
        m_phongShader->setLight(fireball->light);
    }
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
//    if(m_leftPressed) {
        drawHand(m_leftHand);
//    }
//    if(m_rightPressed) {
        drawHand(m_rightHand);
//    }
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


    for(int i = 0; i < Fireballs.size(); i++) {
        float t = Fireballs[i]->time;
        if(t > 10.f) {
            Fireballs[i]->light.color = glm::vec4(0.f, 0.f, 0.f, 1.f);
            m_phongShader->setLight(Fireballs[i]->light);
            delete Fireballs[i];
            Fireballs.removeAt(i);
        }
        else {
            Fireballs[i]->light.id = i + 1;
            drawBall(Fireballs[i]);
        }
    }
}

void WaluigiScene::drawBall(Fireball *fireball) {
    float time = fireball->time;
    glm::vec3 vel = fireball->velocity;
    glm::vec3 pos = fireball->position;
    glm::vec4 func = glm::vec4(pos.x + vel.x * time, pos.y + (vel.y * time) + (-3.f * time * time), pos.z + vel.z * time, 1.f);
    fireball->light.pos = func;
    //m_phongShader->setLight(fireball->light);
    m_phongShader->setUniform("m", glm::translate(func.xyz()));
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
            glm::vec3 pos = glm::vec3(m_leftHand.matrix[3][0], m_leftHand.matrix[3][1], m_leftHand.matrix[3][2]);
            CS123SceneLightData light = CS123SceneLightData();
            light.type = LightType::LIGHT_POINT;
            light.pos = glm::vec4(pos, 1.f);
            light.color = glm::vec4(0.f, 1.f, 1.f, 1.f);
            light.function = glm::vec3(0.f, 1.f, 0.f);
            Fireballs.append(new Fireball{0.f, m_leftVel, pos, light});
        }
    }
    else if(controllerNum == 1) {
        //right hand
        m_rightPressed = pressed;
        if(!pressed /*&& glm::length(m_rightVel) > 1.f*/) {
            glm::vec3 pos = glm::vec3(m_rightHand.matrix[3][0], m_rightHand.matrix[3][1], m_rightHand.matrix[3][2]);
            CS123SceneLightData light = CS123SceneLightData();
            light.type = LightType::LIGHT_POINT;
            light.pos = glm::vec4(pos, 1.f);
            light.color = glm::vec4(0.f, 1.f, 1.f, 1.f);
            light.function = glm::vec3(0.f, 1.f, 0.f);
            Fireballs.append(new Fireball{0.f, m_rightVel, pos, light});
        }
    }
    else {
        //Invalid number
    }
}

void WaluigiScene::setEyeHeight(uint32_t height) {
    m_eyeHeight = height;
}

void WaluigiScene::setEyeWidth(uint32_t width) {
    m_eyeWidth = width;
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

