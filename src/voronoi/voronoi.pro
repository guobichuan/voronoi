# -------------------------------------------------
# qvoronoi.pro -- Qt project file for qvoronoi.exe
# -------------------------------------------------

include(../pri/qhull-app-c_r.pri)

TARGET = voronoi

QT += widgets

SOURCES += \
    main.cpp \
    model.cpp \
    mainwindow.cpp \
    gl2dwidget.cpp \
    voronoidiagramhelper.cpp

HEADERS += \
    model.h \
    mainwindow.h \
    gl2dwidget.h \
    voronoidiagramhelper.h

FORMS += \
    mainwindow.ui


