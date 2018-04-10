QT          += core gui widgets

TEMPLATE    = app
CONFIG      += c++11

TARGET      = 3D-programming

SOURCES += main.cpp \
    matrix4x4.cpp \
    vec3.cpp \
    vec2.cpp \
    vec4.cpp \
    renderwindow.cpp \
    shader.cpp \
    mainwindow.cpp \
    vertex.cpp \
    triangle.cpp \
    octahedron.cpp \
    sceneobject.cpp \
    texture.cpp \
    axis.cpp \
    quaternion.cpp \
    ball.cpp \
    transform.cpp \
    camera.cpp \
    plane.cpp \
    door.cpp \
    beziercurve.cpp

HEADERS += \
    matrix4x4.h \
    vec3.h \
    vec2.h \
    vec4.h \
    renderwindow.h \
    shader.h \
    mainwindow.h \
    vertex.h \
    triangle.h \
    octahedron.h \
    sceneobject.h \
    texture.h \
    axis.h \
    quaternion.h \
    ball.h \
    transform.h \
    camera.h \
    plane.h \
    constants.h \
    door.h \
    beziercurve.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    plainfragment.frag \
    plainvertex.vert \
    textureshader.frag \
    textureshader.vert

RESOURCES += \
    programicons.qrc
