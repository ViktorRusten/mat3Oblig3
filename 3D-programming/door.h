#ifndef DOOR_H
#define DOOR_H

#include "plane.h"
#include "renderwindow.h"

class Shader;

class Door : public SceneObject
{
public:
    Door();
    Door(RenderWindow* Owner, Vec3 Position, Quaternion Rotation, Vec3 colors);

    void update(float deltaTime) override;

    void createObject() override;
    void drawObject() override;
    bool openDoor();

    Plane* planeMesh;

    float xAxis = 0.4f;

private:
    bool open = false;
};

#endif // DOOR_H
