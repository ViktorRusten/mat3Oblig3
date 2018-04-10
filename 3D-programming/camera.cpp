#include "camera.h"
#include "renderwindow.h"

Camera::Camera()
{

}

Camera::Camera(RenderWindow* Owner,
               Vec3 Eye,
               Vec3 Center,
               Vec3 Up,
               float fov,
               float nearPlane,
               float farPlane)
    : SceneObject(Owner),
      mEye{Eye},
      mCenter{Center},
      mUp{Up},
      mFov{fov},
      mNearPlane{nearPlane},
      mFarPlane{farPlane}
{
    mAspectRatio = (float)mOwner->width() / mOwner->height();
}

void Camera::createObject()
{

}

void Camera::drawObject()
{

}

Matrix4x4 Camera::getPerspectiveMatrix()
{
    Matrix4x4 Perspective(true);

    Perspective.perspective(mFov, mAspectRatio, mNearPlane, mFarPlane);

    return Perspective;
}

Matrix4x4 Camera::getViewMatrix()
{
    Matrix4x4 View(true);
    View.lookAt(getTransform().getPosition(), mCenter, mUp);

    return View;
}
