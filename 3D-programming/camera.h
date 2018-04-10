#ifndef CAMERA_H
#define CAMERA_H

#include "sceneobject.h"

class Camera : public SceneObject
{
public:
    Camera();
    Camera(RenderWindow* Owner,
           Vec3 Eye,  //Remove if possible
           Vec3 Center,
           Vec3 Up,
           float fov,
           float nearPlane,
           float farPlane);

    void createObject() override;
    void drawObject() override;

    Matrix4x4 getPerspectiveMatrix();
    Matrix4x4 getViewMatrix();
private:
    Vec3 mEye;
    Vec3 mCenter;
    Vec3 mUp;

    float mFov;
    float mAspectRatio;
    float mNearPlane;
    float mFarPlane;
};

#endif // CAMERA_H
