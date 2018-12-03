#include "SceneviewScene.h"
#include "GL/glew.h"
#include <QGLWidget>

#include "lib/ResourceLoader.h"
#include "gl/shaders/CS123Shader.h"
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
    // TODO: [SCENEVIEW] Fill this in...
    //
    // Set up the lighting for your scene using m_phongShader.
    // The lighting information will most likely be stored in CS123SceneLightData structures.
    //
}

void SceneviewScene::renderGeometry() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // TODO: [SCENEVIEW] Fill this in...
    // You shouldn't need to write *any* OpenGL in this class!
    //
    //
    // This is where you should render the geometry of the scene. Use what you
    // know about OpenGL and leverage your Shapes classes to get the job done.
    //
}

void SceneviewScene::settingsChanged() {
    // TODO: [SCENEVIEW] Fill this in if applicable.
}

