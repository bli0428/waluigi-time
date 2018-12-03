#include "SceneviewScene.h"
#include "GL/glew.h"
#include <QGLWidget>
#include <iostream>
#include "lib/ResourceLoader.h"
#include "gl/shaders/CS123Shader.h"
#include "shapes/Cube.h"
using namespace CS123::GL;


SceneviewScene::SceneviewScene()
{
    // TODO: [SCENEVIEW] Set up anything you need for your Sceneview scene here...
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

        // we use flyweight patterns here, so we only ever save one of each type to draw instead of making one for all of them
        if (m_rerender) {
            m_cube = std::make_unique<Cube>(4, 4, 4);
            m_cone = std::make_unique<Cone>(4, 4, 4);
            m_sphere = std::make_unique<Sphere>(4, 4, 4);
            m_cylinder = std::make_unique<Cylinder>(4, 4, 4);
            m_rerender = false;
        }

        // for each node, we set its model uniform and apply its material, and then draw the appropriate shape
        std::cout << m_primitives.size() << std::endl;
        for (PrimitiveNode node : m_primitives) {
            m_phongShader->setUniform("m", node.matrix);

            // make material lighting adjustments to account for global lighting
            node.material.cDiffuse *= m_global.kd;
            node.material.cAmbient *= m_global.ka;

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
}

void SceneviewScene::settingsChanged() {
    m_rerender = true;
}

