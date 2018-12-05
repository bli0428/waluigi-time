#ifndef WALUIGISCENE_H
#define WALUIGISCENE_H

#include "SceneviewScene.h"
#include "column.h"

class WaluigiScene : public SceneviewScene
{
public:
    WaluigiScene();
    ~WaluigiScene();

protected:
    virtual void setLights() override;
    virtual void renderGeometry() override;
    virtual void drawHands() override;

private:
    std::unique_ptr<Column> m_column;

};

#endif // WALUIGISCENE_H
