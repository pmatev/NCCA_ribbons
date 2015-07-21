# -------------------------------------------------
# Project created by QtCreator 2009-11-05T22:11:46
# -------------------------------------------------
QT += opengl
QT += core \
    	gui \
    	xml

TARGET=Ribbons
DESTDIR=./
OBJECTS_DIR=obj
# this is where we want to put the intermediate build files ( .o)
OBJECTS_DIR=./obj/
MOC_DIR=./moc/
SOURCES += src/main.cpp \
           src/MainWindow.cpp \
           src/GLWindow.cpp \
    src/Camera.cpp \
    src/Axis.cpp \
    src/Ribbon.cpp \
    src/RibbonManager.cpp \
    src/Bundle.cpp \
    src/Solo.cpp \
    src/VAO.cpp \
    src/DirectedBundle.cpp \
    src/BranchBundle.cpp
INCLUDEPATH+=./include
HEADERS += include/MainWindow.h \
           include/GLWindow.h \
    include/Camera.h \
    include/Axis.h \
    include/Ribbon.h \
    include/RibbonManager.h \
    include/Bundle.h \
    include/Solo.h \
    include/VAO.h \
    include/DirectedBundle.h \
    include/BranchBundle.h
OTHER_FILES+= shaders/Phong.fs \
              shaders/Phong.vs \
CONFIG += console \
    shaders/Surface.vs \
    shaders/Surface.fs \
    shaders/normal.vs \
    shaders/normal.gs \
    shaders/normal.fs \
    shaders/Colour.vs \
    shaders/Colour.fs
CONFIG -= app_bundle

QMAKE_CXXFLAGS+= -msse -msse2 -msse3
macx:QMAKE_CXXFLAGS+= -arch x86_64
macx:INCLUDEPATH+=/usr/local/boost/
linux-g++:QMAKE_CXXFLAGS +=  -march=native
linux-g++-64:QMAKE_CXXFLAGS +=  -march=native
# define the _DEBUG flag for the graphics lib
DEFINES +=NGL_DEBUG

LIBS += -L/usr/local/lib
# add the ngl lib
LIBS +=  -L/$(HOME)/NGL/lib -l NGL

# now if we are under unix and not on a Mac (i.e. linux) define GLEW
linux-g++:linux-g++-64 {
    DEFINES += LINUX
    LIBS+= -lGLEW
}
linux-g++ {
    DEFINES += LINUX
    LIBS+= -lGLEW
}
DEPENDPATH+=include
# if we are on a mac define DARWIN
macx:DEFINES += DARWIN
# this is where to look for includes
INCLUDEPATH += $(HOME)/NGL/include/

win32: {
        DEFINES+=USING_GLEW
        INCLUDEPATH+=-I c:/boost_1_44_0
        INCLUDEPATH+=-I c:/boost

        INCLUDEPATH+= -I C:/NGL/Support/glew
        LIBS+= -L C:/NGL/lib
        LIBS+= -lmingw32
        DEFINES += WIN32
        DEFINES += USING_GLEW
        DEFINES +=GLEW_STATIC
        DEFINES+=_WIN32
        SOURCES+=C:/NGL/Support/glew/glew.c
        INCLUDEPATH+=C:/NGL/Support/glew/
}




