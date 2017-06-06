# -------------------------------------------------
# qvoronoi.pro -- Qt project file for qvoronoi.exe
# -------------------------------------------------

include(../pri/qhull-app-c_r.pri)

TARGET = voronoi

QT += widgets 3dcore 3drender 3dinput 3dextras opengl

SOURCES += \
    main.cpp \
    model.cpp \
    mainwindow.cpp \
    gl2dwidget.cpp \
    voronoidiagramhelper.cpp \
    conerenderer.cpp \
    ../cg/widget.cpp \
    ../cg/GL_side/GLDisplay.cpp \
    ../cg/GL_side/vi_point.cpp

HEADERS += \
    model.h \
    mainwindow.h \
    gl2dwidget.h \
    voronoidiagramhelper.h \
    conerenderer.h \
    ../cg/widget.h \
    ../cg/GL_side/GLDisplay.h \
    ../cg/GL_side/vi_point.h

FORMS += \
    mainwindow.ui \
    ../cg/widget.ui

LIBS += -lOpenGL32 -lGLU32

