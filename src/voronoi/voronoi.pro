# -------------------------------------------------
# qvoronoi.pro -- Qt project file for qvoronoi.exe
# -------------------------------------------------

include(../pri/qhull-app-c_r.pri)

TARGET = voronoi

QT += widgets 3dcore 3drender 3dinput 3dextras

SOURCES += \
    main.cpp \
    model.cpp \
    mainwindow.cpp \
    gl2dwidget.cpp \
    voronoidiagramhelper.cpp \
    conerenderer.cpp

HEADERS += \
    model.h \
    mainwindow.h \
    gl2dwidget.h \
    voronoidiagramhelper.h \
    conerenderer.h

FORMS += \
    mainwindow.ui


