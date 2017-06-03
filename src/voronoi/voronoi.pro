# -------------------------------------------------
# qvoronoi.pro -- Qt project file for qvoronoi.exe
# -------------------------------------------------

include(../pri/qhull-app-c_r.pri)

TARGET = voronoi

QT += widgets

SOURCES += \
    main.cpp \
    model.cpp \
    mainwindow.cpp

HEADERS += \
    model.h \
    mainwindow.h

FORMS += \
    mainwindow.ui


