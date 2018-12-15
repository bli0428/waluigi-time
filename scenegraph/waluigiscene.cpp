#include "waluigiscene.h"
#include "iostream"
#include "gl/shaders/CS123Shader.h"
#include "column.h"
#include "glm/gtx/transform.hpp"
#include "ResourceLoader.h"
#include "QImage"
#include "stdlib.h"
#include "algorithm"
#include "chrono"

/**
 * @brief WaluigiScene::WaluigiScene
 * This scene INHERITS from SceneView, so all the VR hand-stuff and the camera/lights/etc initialization is already there. This just overrides how
 * the geometry/lights are done.
 */
WaluigiScene::WaluigiScene() : SceneviewScene(),
  m_columnTexID(0),
  m_sideTexID(0),
  m_skyTexID(0),
  m_grassTexID(0),
  m_targetTexID(0),
  m_time(0.f),
  m_testNum(1),
  m_leftPressed(false),
  m_rightPressed(false)
{
    this->initScene();
}

WaluigiScene::~WaluigiScene() {
    glDeleteTextures(1, &m_columnTexID);
    glDeleteTextures(1, &m_sideTexID);
    glDeleteTextures(1, &m_skyTexID);
    glDeleteTextures(1, &m_grassTexID);
    glDeleteTextures(1, &m_targetTexID);
}

void WaluigiScene::initScene() {
    srand(std::chrono::system_clock::now().time_since_epoch().count());

    // this is all texture stuff
    m_columnTexID = this->genTexture(":/images/images/columnx3.jpg");
    m_skyTexID = this->genTexture(":/images/images/sky.png");
    m_sideTexID = this->genTexture(":/images/images/sides.png");
    m_grassTexID = this->genTexture(":/images/images/grass.jpg");
    m_targetTexID = this->genTexture(":/images/images/target.png");

    // sound?
    m_wah.setSource(QUrl::fromLocalFile("wah.wav"));
    m_wah.setVolume(0.25f);

    m_targets.push_back(TargetNode{glm::vec3(2, 2, 2), 3.14159f/2});

    // this is actual geometry stuff
    m_column = std::make_unique<Column>(30, 20);
    m_target = std::make_unique<Cylinder>(1, 20, 1);
    m_skyboxFace = std::make_unique<Square>();
    m_shatter = std::make_unique<Shatter>();
    m_shatters.push_back(ShatterNode{m_time, glm::vec3(1, 1.5f, 1)});
    this->generateColumns(M_FIELDLENGTH, M_FIELDLENGTH, M_COLUMNMINDIST, M_COLUMNK);
}

GLuint WaluigiScene::genTexture(std::string filePath) {
    QString path = QString::fromStdString(filePath);
    QImage image(path);
    GLuint id;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

    return id;
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_phongShader->setUniform("m", glm::scale(glm::vec3(1, 5, 1)));
    CS123SceneMaterial material = CS123SceneMaterial();
    material.cDiffuse = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
    material.cAmbient = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
    m_phongShader->applyMaterial(material);

    // draw the columns
    m_phongShader->setUniform("useTexture", 1);
    m_phongShader->setUniform("repeatBottomHalf", 1); // column-specific
    glBindTexture(GL_TEXTURE_2D, m_columnTexID);
    // only draws the same column for now; will explore about other options
    for (ColumnNode node : m_columns) {
        m_phongShader->setUniform("repeatUV", glm::vec2(std::ceil(node.radius * 4), node.height / 2));
        glm::mat4x4 translate = glm::translate(glm::vec3(node.x, node.height / 2.0f - 2, node.z));
        glm::mat4x4 scale = glm::scale(glm::vec3(node.radius * 2, node.height, node.radius * 2));
        m_phongShader->setUniform("m", translate * scale);
        m_column->draw();
    }
    m_phongShader->setUniform("repeatBottomHalf", 0); // unset

    // draw the targets
    glBindTexture(GL_TEXTURE_2D, m_targetTexID);
    m_phongShader->setUniform("repeatUV", glm::vec2(1, 1));
    for (TargetNode node : m_targets) {
        // @DAIN this is where all the matrices are made
        glm::mat4x4 translate = glm::translate(node.pos);
        glm::mat4x4 scale = glm::scale(glm::vec3(M_TARGETRADIUS * 2, M_TARGETTHICKNESS, M_TARGETRADIUS * 2));
        glm::mat4x4 rotation = glm::rotate(node.radians, glm::vec3(0, 1, 0)) * glm::rotate(3.14159f / 2, glm::vec3(1, 0, 0));
        m_phongShader->setUniform("m", translate * rotation * scale);
        m_target->draw();
    }

    // draw the floor
    glBindTexture(GL_TEXTURE_2D, m_grassTexID);
    material.cAmbient = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    m_phongShader->applyMaterial(material);
    m_phongShader->setUniform("repeatUV", glm::vec2(60, 60));
    m_phongShader->setUniform("m", glm::rotate(3.14159f * 1.5f, glm::vec3(-1, 0, 0)) * glm::scale(glm::vec3(300, 300, 1)));
    m_skyboxFace->draw();

    // draw the skybox
    m_phongShader->setUniform("skybox", 1);
    m_phongShader->setUniform("repeatUV", glm::vec2(1, 1));

    glBindTexture(GL_TEXTURE_2D, m_sideTexID);
    m_phongShader->setUniform("m", glm::translate(glm::vec3(0, M_SKYBOXLENGTH / 2, M_SKYBOXLENGTH / 2)) * glm::scale(glm::vec3(M_SKYBOXLENGTH, M_SKYBOXLENGTH, 1)));
    m_skyboxFace->draw();

    m_phongShader->setUniform("m", glm::translate(glm::vec3(M_SKYBOXLENGTH / 2, M_SKYBOXLENGTH / 2, 0)) * glm::rotate(3.14159f / 2.0f, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(M_SKYBOXLENGTH, M_SKYBOXLENGTH, 1)));
    m_skyboxFace->draw();

    m_phongShader->setUniform("m", glm::translate(glm::vec3(0, M_SKYBOXLENGTH / 2, -M_SKYBOXLENGTH / 2)) * glm::rotate(3.14159f, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(M_SKYBOXLENGTH, M_SKYBOXLENGTH, 1)));
    m_skyboxFace->draw();

    m_phongShader->setUniform("m", glm::translate(glm::vec3(-M_SKYBOXLENGTH / 2, M_SKYBOXLENGTH / 2, 0)) * glm::rotate(3.14159f * 1.5f, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(M_SKYBOXLENGTH, M_SKYBOXLENGTH, 1)));
    m_skyboxFace->draw();

    glBindTexture(GL_TEXTURE_2D, m_skyTexID);
    m_phongShader->setUniform("m", glm::translate(glm::vec3(0, M_SKYBOXLENGTH - 2, 0)) * glm::rotate(3.14159f / 2.0f, glm::vec3(-1, 0, 0)) * glm::scale(glm::vec3(M_SKYBOXLENGTH, M_SKYBOXLENGTH, 1)));
    m_skyboxFace->draw();
    m_phongShader->setUniform("skybox", 0);


    // TEST
    glBindTexture(GL_TEXTURE_2D, 0);
    m_phongShader->setUniform("useTexture", 0);

    for (ShatterNode node : m_shatters) {
        m_shatter->draw(m_time - node.spawnTime, node.pos, m_phongShader.get());
    }

    if (m_time - m_shatters.front().spawnTime > 2.0f) {
        m_shatters.pop_front();
    }

    m_time += 1.f / 60.f;
    drawBalls();

    drawHands();



}

/**
 * @brief WaluigiScene::generateColumns Adds ColumnNodes and TargetNodes to the thing. Uses poisson disk sampling.
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
                    if (neighboring >= 0 && grid[neighboring].x >= 0) {
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

        // prevents generating on top of the dude
        if (std::abs(point.x - M_FIELDLENGTH / 2) < 3 && std::abs(point.y - M_FIELDLENGTH / 2) < 3) {
            continue;
            //            glm::vec3 pos = glm::vec3(point.x - M_FIELDLENGTH / 2, M_TARGETHEIGHTAVG + r * M_TARGETHEIGHTVAR, point.y - M_FIELDLENGTH / 2);
//            m_targets.push_back(TargetNode{pos, glm::atan(pos.x, pos.z)});
        }

        float isColumn = static_cast<float>(rand()) / RAND_MAX;

        if (isColumn < M_COLUMNTOTARGETRATIO) {
            m_columns.push_back(ColumnNode{M_COLUMNHEIGHTAVG + r * M_COLUMNHEIGHTVAR, M_COLUMNRADIUSAVG, point.x - M_FIELDLENGTH / 2, point.y - M_FIELDLENGTH / 2});
        } else {
            glm::vec3 pos = glm::vec3(point.x - M_FIELDLENGTH / 2, M_TARGETHEIGHTAVG + r * M_TARGETHEIGHTVAR, point.y - M_FIELDLENGTH / 2);
            m_targets.push_back(TargetNode{pos, glm::atan(pos.x, pos.z)});
        }


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
        m_ball = std::make_unique<Sphere>(20, 20, 20, M_FIREBALLRADIUS);
        m_handShape = std::make_unique<Sphere>(4, 4, 4, 0.1f);
        //m_testSphere = std::make_unique<Sphere>(20, 20, 20, 0.1f);

        CS123SceneMaterial material = CS123SceneMaterial();
        material.cDiffuse = glm::vec4(0.7f, 0.f, 1.f, 1.f);
        material.cAmbient = glm::vec4(0.2f, 0.f, 0.3f, 1.f);
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
    for(int i = 0; i < Fireballs.size(); i++) {
        float t = Fireballs[i]->spawnTime;
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
    glm::vec4 func = glm::vec4(pos.x + vel.x * time, pos.y + (vel.y * time) + (.5f * M_GRAV * time * time), pos.z + vel.z * time, 1.f);

    if(checkForCollision(fireball, func)) {
        fireball->light.pos = glm::vec4(fireball->position, 1.f);
    }
    else {
        fireball->light.pos = func;
    }
    //m_phongShader->setLight(fireball->light);
    m_phongShader->setUniform("m", glm::translate(func.xyz()));
    m_phongShader->applyMaterial(m_material);
    fireball->prevTime = fireball->time;
    m_ball->draw();
}

bool WaluigiScene::checkForCollision(Fireball *fireball, glm::vec4 newPos) {

    bool collision = false;

    if(newPos.y <= .1f) {
        float time = fireball->time;
        glm::vec3 vel = fireball->velocity;
        glm::vec3 pos = fireball->position;
        float prevTime = fireball->prevTime;

        fireball->position = glm::vec3(pos.x + vel.x * prevTime, pos.y + (vel.y * prevTime) + (.5f * M_GRAV * prevTime * prevTime), pos.z + vel.z * prevTime);
        newPos = glm::vec4(fireball->position, 1.f);
        fireball->velocity = glm::vec3(vel.x, .75f * -(vel.y + (M_GRAV * time)) , vel.z);
        fireball->time = 0.f;
        collision = true;
    }
    //loop through each cylinder and check for collisions
    for(int i = 0; i < m_columns.size(); i++) {
        ColumnNode c = m_columns[i];
        if(glm::distance(glm::vec2(c.x, c.z), glm::vec2(newPos.x, newPos.z)) < c.radius + M_FIREBALLRADIUS) {

            //transform cylinder to origin
            //transform sphere similarly
            glm::vec3 point = glm::vec3(0, 0, 0);
            glm::vec3 normal = glm::vec3(0, 0, 0);
            glm::vec3 firePos = newPos.xyz();
            firePos.x = firePos.x - c.x;
            firePos.z = firePos.z - c.z;
            if(cylinderCollision(c.height - 2.f, c.radius, firePos, &point, &normal)) {
                normal = glm::normalize(normal);

                float time = fireball->time;
                glm::vec3 vel = fireball->velocity;
                glm::vec3 pos = fireball->position;
                float prevTime = fireball->prevTime;

                fireball->position = glm::vec3(pos.x + vel.x * prevTime, pos.y + (vel.y * prevTime) + (.5f * M_GRAV * prevTime * prevTime), pos.z + vel.z * prevTime);
                newPos = glm::vec4(fireball->position, 1.f);
                vel = glm::vec3(vel.x, vel.y + (M_GRAV * time) , vel.z);
                fireball->velocity = glm::reflect(vel, normal);
                fireball->time = 0.f;
                collision = true;

                break;
            }
        }
    }

    for(int i = 0; i < m_targets.size(); i++) {
        TargetNode t = m_targets[i];
        if(glm::distance(t.pos, newPos.xyz()) < M_TARGETRADIUS + M_FIREBALLRADIUS) {

            //transform cylinder to origin
            //transform sphere similarly
            glm::vec3 point = glm::vec3(0, 0, 0);
            glm::vec3 normal = glm::vec3(0, 0, 0);

            glm::mat4x4 rotation = glm::rotate(t.radians, glm::vec3(0, 1, 0)) * glm::rotate(3.14159f / 2, glm::vec3(1, 0, 0));

            glm::vec3 firePos = newPos.xyz();
            firePos.x = firePos.x - t.pos.x;
            firePos.y = firePos.y - t.pos.y;
            firePos.z = firePos.z - t.pos.z;

            firePos = glm::inverse(glm::mat3x3(rotation)) * firePos;

            if(cylinderCollision(M_TARGETTHICKNESS, M_TARGETRADIUS, firePos.xyz(), &point, &normal)) {
                normal = glm::normalize(glm::mat3x3(rotation) * normal);

                float time = fireball->time;
                glm::vec3 vel = fireball->velocity;
                glm::vec3 pos = fireball->position;
                float prevTime = fireball->prevTime;

                fireball->position = glm::vec3(pos.x + vel.x * prevTime, pos.y + (vel.y * prevTime) + (.5f * M_GRAV * prevTime * prevTime), pos.z + vel.z * prevTime);
                newPos = glm::vec4(fireball->position, 1.f);
                vel = glm::vec3(vel.x, vel.y + (M_GRAV * time) , vel.z);
                fireball->velocity = glm::reflect(vel, normal);
                fireball->time = 0.f;
                collision = true;

                hitTarget(t, i);

                break;
            }

        }
    }
    return collision;
}

void WaluigiScene::hitTarget(TargetNode target, int index) {
    m_targets.erase(m_targets.begin() + index);

}

bool WaluigiScene::cylinderCollision(float cylHeight, float cylRad, glm::vec3 firePos, glm::vec3 *intersectPoint, glm::vec3 *normal) {

    //check side
    if(firePos.y >= 0.f && firePos.y <= cylHeight) {
        intersectPoint->y = firePos.y;
        float theta = glm::atan(firePos.x / firePos.z);
        intersectPoint->x = cylRad * glm::cos(theta);
        intersectPoint->z = cylRad * glm::sin(theta);
        float theta2 = glm::atan(intersectPoint->x / intersectPoint->z);
        normal->x = glm::cos(theta2);
        normal->y = 0.f;
        normal->z = glm::sin(theta2);
        std::cout << "side" << std::endl;
        return true;
    }
    //check top
    float root = glm::sqrt((firePos.x * firePos.x) + (firePos.z * firePos.z));
    if(root <= cylRad && firePos.y <= cylHeight + M_FIREBALLRADIUS && firePos.y >= cylHeight / 2.f) {
        intersectPoint->x = firePos.x;
        intersectPoint->y = cylHeight;
        intersectPoint->z = firePos.z;
        normal->x = 0.f;
        normal->y = 1.f;
        normal->z = 0.f;
        std::cout << "top" << std::endl;
        return true;
    }
    if(root <= cylRad && firePos.y >= -M_FIREBALLRADIUS && firePos.y <= cylHeight / 2.f) {
        intersectPoint->x = firePos.x;
        intersectPoint->y = 0;
        intersectPoint->z = firePos.z;
        normal->x = 0.f;
        normal->y = -1.f;
        normal->z = 0.f;
        std::cout << "bot" << std::endl;
        return true;
    }

    //check bottom corner
    if(firePos.y < 0.f){
        float theta = glm::atan(firePos.x / firePos.z);
        float x = cylRad * glm::cos(theta);
        float z = cylRad * glm::sin(theta);
        if(glm::distance(glm::vec3(x, 0.f, z), firePos) <= M_FIREBALLRADIUS) {
            intersectPoint->x = x;
            intersectPoint->y = 0;
            intersectPoint->z = z;
            float theta2 = glm::atan(intersectPoint->x / intersectPoint->z);
            normal->x = glm::cos(theta2);
            normal->y = -1.f;
            normal->z = glm::sin(theta2);
            std::cout << "bot cor" << std::endl;
            return true;
        }
    }
    //check top corner
    if(firePos.y > cylHeight) {
        float theta = glm::atan(firePos.x / firePos.z);
        float x = cylRad * glm::cos(theta);
        float z = cylRad * glm::sin(theta);
        if(glm::distance(glm::vec3(x, cylHeight, z), firePos) <= M_FIREBALLRADIUS) {
            intersectPoint->x = x;
            intersectPoint->y = cylHeight;
            intersectPoint->z = z;
            float theta2 = glm::atan(intersectPoint->x / intersectPoint->z);
            normal->x = glm::cos(theta2);
            normal->y = 1.f;
            normal->z = glm::sin(theta2);
            std::cout << "top cor" << std::endl;
            return true;
        }
    }
    return false;
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
            m_wah.play();
            glm::vec3 pos = glm::vec3(m_leftHand.matrix[3][0], m_leftHand.matrix[3][1], m_leftHand.matrix[3][2]);
            CS123SceneLightData light = CS123SceneLightData();
            light.type = LightType::LIGHT_POINT;
            light.pos = glm::vec4(pos, 1.f);
            light.color = glm::vec4(1.f, 0.f, 1.f, 1.f);
            light.function = glm::vec3(0.f, 1.f, 0.f);
            Fireballs.append(new Fireball{0.f, 0.f, 0.f, m_leftVel, pos, light});
        }
    }
    else if(controllerNum == 1) {
        //right hand
        m_rightPressed = pressed;
        if(!pressed /*&& glm::length(m_rightVel) > 1.f*/) {
            m_wah.play();
            glm::vec3 pos = glm::vec3(m_rightHand.matrix[3][0], m_rightHand.matrix[3][1], m_rightHand.matrix[3][2]);
            CS123SceneLightData light = CS123SceneLightData();
            light.type = LightType::LIGHT_POINT;
            light.pos = glm::vec4(pos, 1.f);
            light.color = glm::vec4(0.f, 1.f, 1.f, 1.f);
            light.function = glm::vec3(0.f, 1.f, 0.f);
            Fireballs.append(new Fireball{0.f, 0.f, 0.f, m_rightVel, pos, light});
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

