#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include "matrix4x4.h"
#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QBasicTimer>
#include <QTime>
#include <QSet>

#include <memory>

class QOpenGLContext;
class Shader;
class MainWindow;
class Triangle;
class SceneObject;
class Texture;
class Matrix4x4;
class QMatrix4x4;
class Camera;

class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow();
    QOpenGLContext *context() { return mContext; }
    void exposeEvent(QExposeEvent *) override;

    void setWireframe(bool wireframe);

    void checkForGLerrors();

    Camera* getCamera();

signals:
    void ready();
    void error(const QString &msg);

private slots:
    void render();

private:
    void init();

    void startOpenGLDebugger();

    QOpenGLContext *mContext;
    bool mInitialized;
    std::unique_ptr<Shader> mTextureShaderProgram;
    std::unique_ptr<Shader> mColorShaderProgram;

    GLint mMVPUniform;

    GLuint mTriangleVAO;

    GLuint mVertexBufferObject;

    float mDeltaTime;
    float mPosition;
    float mRotation;
    float mDistance = 50.f;
    float phi = 0.f;
    float theta = 0.f;

    Vec3 mCamPos;

    std::vector<SceneObject*> objects;

    Camera* mMainCamera;

    QBasicTimer mTimer;     //timer that drives the gameloop
    QTime mTimeStart;       //time variable that reads the actual FPS

    MainWindow *mMainWindow;

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

    Matrix4x4 mMVPmatrix;

    bool mOrtho{false};
    bool mWireframe{false};
    int mouseXlast{0};
    int mouseYlast{0};
    int renderLoop{0};

protected:
    //    void mousePressEvent(QMouseEvent *event) override{}
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void timerEvent(QTimerEvent *) override;
};

#endif // RENDERWINDOW_H
