#ifndef WALUIGISCENE_H
#define WALUIGISCENE_H

#include "SceneviewScene.h"

class WaluigiScene : public SceneviewScene
{
public:
    WaluigiScene();
    virtual ~WaluigiScene();
    void setLeftHand(glm::mat4x4 transform);
    void setRightHand(glm::mat4x4 transform);
    void setLeftHandVelocity(glm::vec3 velocity);
    void setRightHandVelocity(glm::vec3 velocity);

protected:
    virtual void setLights() override;
    virtual void renderGeometry() override;


private:
    void drawHands();
    void updateControllerMaterial(PrimitiveNode hand);
    void drawHand(PrimitiveNode hand);

    void drawTestSphere(int x);
    std::unique_ptr<Sphere> m_handShape;
    PrimitiveNode m_leftHand;
    PrimitiveNode m_rightHand;
    CS123SceneMaterial m_material;
    bool didSetMaterial = false;

    std::unique_ptr<Sphere> m_testSphere;
    int m_testNum;

    float m_time;
    glm::vec3 m_leftVel;
    glm::vec3 m_rightVel;

};

#endif // WALUIGISCENE_H
