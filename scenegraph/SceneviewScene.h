#ifndef SCENEVIEWSCENE_H
#define SCENEVIEWSCENE_H

#include "OpenGLScene.h"
#include "Cube.h"
#include "cylinder.h"
#include "sphere.h"
#include "cone.h"
#include <memory>

#include <QTimer>


namespace CS123 { namespace GL {

    class CS123Shader;
    class Texture2D;
}}

/**
 *
 * @class SceneviewScene
 *
 * A complex scene consisting of multiple objects. Students will implement this class in the
 * Sceneview assignment.
 *
 * Here you will implement your scene graph. The structure is up to you - feel free to create new
 * classes and data structures as you see fit. We are providing this SceneviewScene class for you
 * to use as a stencil if you like.
 *
 * Keep in mind that you'll also be rendering entire scenes in the next two assignments, Intersect
 * and Ray. The difference between this assignment and those that follow is here, we are using
 * OpenGL to do the rendering. In Intersect and Ray, you will be responsible for that.
 */
class SceneviewScene : public OpenGLScene {
public:
    SceneviewScene();
    virtual ~SceneviewScene();

    virtual void render(glm::mat4x4 projectionMatrix, glm::mat4x4 viewMatrix) override;
    virtual void render(
            glm::mat4x4 projectionMatrix,
            glm::mat4x4 viewMatrix,
            glm::mat4 m_mat4DevicePose [vr::k_unMaxTrackedDeviceCount],
            bool m_activeTrackedDevice[vr::k_unMaxTrackedDeviceCount]) override;

    virtual void settingsChanged() override;

    // Use this method to set an internal selection, based on the (x, y) position of the mouse
    // pointer.  This will be used during the "modeler" lab, so don't worry about it for now.
    void setSelection(int x, int y);

    void setLeftHand(glm::mat4x4 transform);
    void setRightHand(glm::mat4x4 transform);
    void setLeftHandVelocity(glm::vec3 velocity);
    void setRightHandVelocity(glm::vec3 velocity);

private:


private:

    void loadPhongShader();
    void setSceneUniforms(glm::mat4x4 &projectionMatrix, glm::mat4x4 &viewMatrix);
    void setLights();
    void renderGeometry();
    void updateControllerMaterial(PrimitiveNode hand);
    void drawHand(PrimitiveNode hand);

    void drawTestSphere(int x);

    std::unique_ptr<CS123::GL::CS123Shader> m_phongShader;
    std::unique_ptr<Cube> m_cube;
    std::unique_ptr<Sphere> m_sphere;
    std::unique_ptr<Cylinder> m_cylinder;
    std::unique_ptr<Cone> m_cone;

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

#endif // SCENEVIEWSCENE_H
