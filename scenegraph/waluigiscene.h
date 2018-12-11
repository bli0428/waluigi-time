#ifndef WALUIGISCENE_H
#define WALUIGISCENE_H

#include "SceneviewScene.h"
#include "column.h"
#include "sphere.h"
#include "Cube.h"
#include "gl/datatype/FBO.h"

struct ColumnNode {
   float height;
   float radius;
   float x;
   float z;
};

struct Fireball {
    float time;
    glm::vec3 velocity;
    glm::vec3 position;
    CS123SceneLightData light;
};

class WaluigiScene : public SceneviewScene
{
public:
    WaluigiScene();
    virtual ~WaluigiScene() override;
    virtual void render(glm::mat4x4 projectionMatrix, glm::mat4x4 viewMatrix) override;
    virtual void render(
            glm::mat4x4 projectionMatrix,
            glm::mat4x4 viewMatrix,
            glm::mat4 m_mat4DevicePose [vr::k_unMaxTrackedDeviceCount],
            bool m_activeTrackedDevice[vr::k_unMaxTrackedDeviceCount]) override;

    void setLeftHand(glm::mat4x4 transform);
    void setRightHand(glm::mat4x4 transform);
    void setLeftHandVelocity(glm::vec3 velocity);
    void setRightHandVelocity(glm::vec3 velocity);
    void setTrigger(int controllerNum, bool pressed);

    void setEyeHeight(uint32_t height);
    void setEyeWidth(uint32_t width);

    void resize(int w, int h);

    QList<Fireball*> Fireballs;


protected:
    void loadShaders();
    virtual void setLights() override;
    virtual void renderGeometry() override;


private:
    void drawHands();
    void updateControllerMaterial(PrimitiveNode hand);
    void drawHand(PrimitiveNode hand);
    void drawBalls();
    void drawBall(Fireball *fireball);

    void generateColumns(int width, int height, float min, int k);
    int imageToGrid(glm::vec2 point, float cellSize, int cellsAcross);
    glm::vec2 randPointAround(glm::vec2 newPoint, float min);

    void drawTestSphere(int x);
    std::unique_ptr<Sphere> m_handShape;
    PrimitiveNode m_leftHand;
    PrimitiveNode m_rightHand;
    CS123SceneMaterial m_material;
    uint32_t m_eyeHeight;
    uint32_t m_eyeWidth;
    bool didSetMaterial = false;

    std::unique_ptr<Sphere> m_ball;
    int m_testNum;
    QList<float> m_ballTimes;
    QList<glm::vec3> m_ballVelocities;
    QList<glm::vec3> m_ballPositions;

    float m_time;
    glm::vec3 m_leftVel;
    glm::vec3 m_rightVel;
    bool m_leftPressed;
    bool m_rightPressed;

    GLuint m_textureProgramID;
    GLuint m_textureID;

    std::unique_ptr<Column> m_column;
    std::unique_ptr<Cube> m_floor;
    std::vector<ColumnNode> m_columns;

    std::unique_ptr<CS123::GL::FBO> m_gBuffer;
    GLuint m_gProgram;
    GLuint m_lightProgram;


    // CONSTANTS
    const int M_FIELDLENGTH = 60;
    const float M_COLUMNMINDIST = 5.0f; // min dist between columns
    const float M_COLUMNK = 30; // columns generated on each run of poisson; higher = more clustered usually
    const float M_COLUMNHEIGHTAVG = 25.0f;
    const float M_COLUMNHEIGHTVAR = 5.0f; // variance of columns' heights
    const float M_COLUMNRADIUSAVG = 1.0f;

};

#endif // WALUIGISCENE_H
