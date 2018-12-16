#ifndef WALUIGISCENE_H
#define WALUIGISCENE_H

#include "SceneviewScene.h"
#include "column.h"
#include "sphere.h"
#include "Cube.h"
#include "square.h"
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "shatter.h"
#include "list"

struct ColumnNode {
   float height;
   float radius;
   float x;
   float z;
};

struct TargetNode {
    glm::vec3 pos;
    float radians;
    int texIndex;
};

struct Fireball {
    float time;
    float prevTime;
    float spawnTime;
    glm::vec3 velocity;
    glm::vec3 position;
};

struct ShatterNode {
    float spawnTime;
    glm::vec3 pos;
};

class WaluigiScene : public SceneviewScene
{
public:
    WaluigiScene();
    virtual ~WaluigiScene() override;

    void setLeftHand(glm::mat4x4 transform);
    void setRightHand(glm::mat4x4 transform);
    void setLeftHandVelocity(glm::vec3 velocity);
    void setRightHandVelocity(glm::vec3 velocity);
    void setTrigger(int controllerNum, bool pressed);

    void setEyeHeight(uint32_t height);
    void setEyeWidth(uint32_t width);

    QList<Fireball*> Fireballs;


protected:
    virtual void setLights() override;
    virtual void renderGeometry() override;


private:
    void drawHands();
    void updateControllerMaterial(PrimitiveNode hand);
    void drawHand(PrimitiveNode hand);
    void drawBalls();
    void drawBall(Fireball *fireball);
    bool checkForCollision(Fireball *fireball, glm::vec4 newPos);
    bool cylinderCollision(float cylHeight, float cylRad, glm::vec3 firePos, glm::vec3 *intersectPoint, glm::vec3 *normal);
    void hitTarget(TargetNode target, int index);

    void drawColumns();
    void drawTargets();
    void drawEnvironment();
    void drawShatters();

    void initScene();
    GLuint genTexture(std::string filePath);
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

    GLuint m_columnTexID;
    GLuint m_sideTexID;
    GLuint m_skyTexID;
    GLuint m_grassTexID;
    GLuint m_ballTexID;
    std::vector<GLuint> m_targetTexIDs;

    std::unique_ptr<Column> m_column;
    std::unique_ptr<Cylinder> m_target;
    std::unique_ptr<Shatter> m_shatter;
    std::unique_ptr<Square> m_skyboxFace;
    std::vector<ColumnNode> m_columns;
    std::vector<TargetNode> m_targets;
    std::list<ShatterNode> m_shatters;

    QSoundEffect m_wah;

    QMediaPlaylist *m_playlist;
    QMediaPlayer* m_wallelujah;

    // CONSTANTS
    const int M_FIELDLENGTH = 40;
    const float M_COLUMNMINDIST = 3.0f; // min dist between columns
    const float M_COLUMNK = 40; // columns generated on each run of poisson; higher = more clustered usually
    const float M_COLUMNHEIGHTAVG = 17.0f;
    const float M_COLUMNHEIGHTVAR = 8.0f; // variance of columns' heights (note: they are uniformly distributed, not normally)
    const float M_COLUMNRADIUSAVG = 1.0f;
    const int M_SKYBOXLENGTH = 200;

    const float M_GRAV = -6.f;
    const float M_FIREBALLRADIUS = .1f;

    const float M_TARGETRADIUS = 1.0f;
    const float M_TARGETTHICKNESS = 0.2f;
    const float M_TARGETHEIGHTAVG = 5.0f;
    const float M_TARGETHEIGHTVAR = 3.0f;
    const float M_COLUMNTOTARGETRATIO = 0.4f;

};

#endif // WALUIGISCENE_H
