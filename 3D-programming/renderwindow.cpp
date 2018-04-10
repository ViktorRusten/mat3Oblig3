#include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>
#include <vector>
#include <fstream>

#include <memory>

#include <math.h>
#include "shader.h"
#include "mainwindow.h"
#include "matrix4x4.h"
#include "triangle.h"
#include "octahedron.h"
#include "plane.h"
#include "texture.h"
#include "vertex.h"
#include "axis.h"
#include "camera.h"
#include "door.h"
#include "ball.h"
#include "beziercurve.h"

RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext{0}, mInitialized{false}, mPosition{0.0f}, mRotation{0.f}, mMainWindow{mainWindow},
      mDeltaTime{0.016f}

{
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    mContext = new QOpenGLContext(this);    //automatically deleted
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = 0;
    }
}

RenderWindow::~RenderWindow()
{
    glDeleteBuffers( 1, &mVertexBufferObject );
}

void RenderWindow::init()
{
    mMainCamera = new Camera(this,
                             Vec3(0.f, -15.f, 15.f),
                             Vec3(0.f, 0.f, 0.f),
                             Vec3(0.f, 1.f, 0.f),
                             45.f,
                             0.01f,
                             1000.f);

    int numObjects = -1;

    Vec3 color = Vec3(0.f, 0.f, 0.f);

    Vec3 point(-1.25f, 0.f, -0.25f);
    Vec3 planeStart(-2.f,0.f,-2.f);

//    Plane* triPlane = new Plane(this,
//              Vec3(0.f, 0.f, 0.f),
//              Quaternion(1.f, Vec3(0.f, 0.f, 0.f)),
//              color);

//    Plane* triPlane = new Plane(this,
//              Vec3(0.f, 0.f, 0.f),
//              Quaternion(1.f, Vec3(0.f, 0.f, 0.f)),
//              color,
//                                -50.f,50.f,-50.f,50.f,1.f);

//    Plane* triPlane = new Plane(this,
//              Vec3(0.f, 0.f, 0.f),
//              Quaternion(1.f, Vec3(0.f, 0.f, 0.f)),
//              color,
//                                -2.f,50.5f,-2.f,40.5f,5.f);

    Plane* triPlane = new Plane(this,
              Vec3(0.f, 0.f, 0.f),
              Quaternion(1.f, Vec3(0.f, 0.f, 0.f)),
              color,
                                -2.f,50.5f,-2.f,40.5f,0.1f);

    objects.push_back(static_cast<SceneObject*>(triPlane));
                           numObjects++;



                           std::cout << "The point is:" << point << std::endl;
                           std::cout << "it is in triangle: " << triPlane->getTriangle(point) << std::endl;




    objects.push_back(static_cast<SceneObject*>(
                          new BezierCurve(this,
                                    Vec3(0.f, 0.5f, 0.f),
                                    Quaternion(1.f, Vec3(0.f, 0.f, 0.f)),
                                    color,
                                    planeStart,
                                    Vec3(point.getX(),0.f,planeStart.getZ()),
                                    Vec3(planeStart.getX(),0.f,point.getZ()),
                                    point))); numObjects++;

    if (!mContext->makeCurrent(this)) {
        emit error(tr("makeCurrent() failed"));
        return;
    }

    if (!mInitialized)
        mInitialized = true;

    startOpenGLDebugger();

    initializeOpenGLFunctions();


    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);    //enables depth sorting - must use GL_DEPTH_BUFFER_BIT in glClear
    //glEnable(GL_CULL_FACE);     //draws only front side of models
    glClearColor(0.2f, 0.2f, 0.2f,1.0f);    //color used in glClear GL_COLOR_BUFFER_BIT

    for(auto obj : objects)
        obj->createObject();

    emit ready();   //tell the mainWindow that init is finished



}

void RenderWindow::render()
{
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)
    initializeOpenGLFunctions();

    if (mWireframe)
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );    //turn on wireframe mode
    else
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );    //turn off wireframe mode

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(auto obj : objects)
        obj->drawObject();

    for(auto obj : objects)
        obj->update(mDeltaTime);

    mCamPos.setX(-std::cos(phi)*std::sin(theta));
    mCamPos.setY(-std::sin(phi));
    mCamPos.setZ(-std::cos(theta)*std::cos(phi));

    mMainCamera->getTransform().setPosition(mCamPos * mDistance);

    mContext->swapBuffers(this);
}

void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        qDebug() << "Can this system run QOpenGLDebugLogger? :" << format.testOption(QSurfaceFormat::DebugContext);

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            qDebug() << "System can log OpenGL errors!";
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);  //will be deleted automatically
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                qDebug() << "Started OpenGL debug logger!";
            //Try to silence some messages that is not critical
            mOpenGLDebugLogger->disableMessages(QOpenGLDebugMessage::AnySource, QOpenGLDebugMessage::AnyType, QOpenGLDebugMessage::NotificationSeverity);
        }
    }
}

void RenderWindow::setWireframe(bool wireframe)
{
    mWireframe = wireframe;
}

void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
            qDebug() << message;
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            std::cout << "glGetError returns " << err;
        }
    }
}

Camera *RenderWindow::getCamera()
{
    return mMainCamera;
}

//This function is called from Qt when window is exposed / shown
void RenderWindow::exposeEvent(QExposeEvent *)
{
    if (!mInitialized)
        init();

    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    if (isExposed())
    {
        mTimer.start(16, this);
        mTimeStart.start();
    }
}

void RenderWindow::mouseMoveEvent(QMouseEvent *event)
{
    //using mouseXYlast as deltaXY so we don't need extra variables
    mouseXlast = event->pos().x() - mouseXlast;
    mouseYlast = event->pos().y() - mouseYlast;

    //if delta is to big the movement will be jerky
    //Happens if mouse is moved much between presses.
    if (mouseXlast > 40 || mouseYlast > 40 || mouseXlast < -40 || mouseYlast < -40)
    {
        mouseXlast = 0;
        mouseYlast = 0;
    }

    if(event->buttons() == Qt::LeftButton)
    {
        float CameraSpeed{0.01f};

        theta += mouseXlast*CameraSpeed;

        phi += mouseYlast*CameraSpeed;
    }

    mouseXlast = event->pos().x();
    mouseYlast = event->pos().y();
}

void RenderWindow::wheelEvent(QWheelEvent* event)
{
    //http://doc.qt.io/qt-4.8/qwheelevent.html
    int numDegrees = event->delta() / 8;
    float numSteps = numDegrees / 15;
    mDistance -= numSteps;
    event->accept();
}

void RenderWindow::timerEvent(QTimerEvent *)
{
    render();

    //The rest here is just to show the frame rate:
    int msSinceLastFrame = mTimeStart.restart();    //restart() returns ms since last restart.

    mDeltaTime = msSinceLastFrame/1000.f;

    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)    //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 60) //once pr 60 frames =  update the message once pr second
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" FrameDraw: " + QString::number(msSinceLastFrame) + " ms");
            frameCount = 0;     //reset to show a new message in 60 frames
        }
    }
}



