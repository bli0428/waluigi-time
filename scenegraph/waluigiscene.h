#ifndef WALUIGISCENE_H
#define WALUIGISCENE_H

#include "SceneviewScene.h"
#include "column.h"
#include "sphere.h"
#include "QPoint"

struct ColumnNode {
   float height;
   float radius;
   int x;
   int z;
};

class WaluigiScene : public SceneviewScene
{
public:
    WaluigiScene();
    virtual ~WaluigiScene();
    void setLeftHand(glm::mat4x4 transform);
    void setRightHand(glm::mat4x4 transform);
    void setLeftHandVelocity(glm::vec3 velocity);
    void setRightHandVelocity(glm::vec3 velocity);
    void setTrigger(int controllerNum, bool pressed);

protected:
    virtual void setLights() override;
    virtual void renderGeometry() override;


private:
    void drawHands();
    void updateControllerMaterial(PrimitiveNode hand);
    void drawHand(PrimitiveNode hand);
    void drawBalls();
    void drawBall(float time, glm::vec3 vel, glm::mat4x4 pos);

    void generateColumns(int width, int height, float min, int k);
    int imageToGrid(QPoint point, float cellSize, int cellsAcross);
    QPoint randPointAround(QPoint newPoint, float min);

    void drawTestSphere(int x);
    std::unique_ptr<Sphere> m_handShape;
    PrimitiveNode m_leftHand;
    PrimitiveNode m_rightHand;
    CS123SceneMaterial m_material;
    bool didSetMaterial = false;

    std::unique_ptr<Sphere> m_ball;
    int m_testNum;
    QList<float> m_ballTimes;
    QList<glm::vec3> m_ballVelocities;
    QList<glm::mat4x4> m_ballPositions;

    float m_time;
    glm::vec3 m_leftVel;
    glm::vec3 m_rightVel;
    bool m_leftPressed;
    bool m_rightPressed;

    GLuint m_textureProgramID;
    GLuint m_textureID;

    std::unique_ptr<Column> m_column;
    std::vector<ColumnNode> m_columns;

};

#endif // WALUIGISCENE_H
