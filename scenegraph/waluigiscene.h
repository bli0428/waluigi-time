#ifndef WALUIGISCENE_H
#define WALUIGISCENE_H

#include "SceneviewScene.h"

class WaluigiScene : public SceneviewScene
{
public:
    WaluigiScene();
    ~WaluigiScene();

protected:
    virtual void setLights() override;
    virtual void renderGeometry() override;
    virtual void drawHands() override;

};

#endif // WALUIGISCENE_H
