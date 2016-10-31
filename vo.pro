QT += widgets gui xml
QT += opengl


QMAKE_CXXFLAGS += -std=c++11

HEADERS += \
    MainWidget.h \
    CameraView.h \
    MapView.h \
    src/ImgIO.h \
    src/Frame.h \
    src/FrameMemory.h \
    src/setting.h \
    src/voSystem.h \
    src/Optimization.h

SOURCES += \
    MainWidget.cpp \
    CameraView.cpp \
    MapView.cpp \
    src/ImgIO.cpp \
    src/Frame.cpp \
    src/FrameMemory.cpp \
    main.cpp \
    src/setting.cpp \
    src/voSystem.cpp \
    src/Optimization.cpp

################################################################################
####################              boost                  #######################
################################################################################
LIBS += /usr/lib/x86_64-linux-gnu/libboost_thread.so
LIBS += /usr/lib/x86_64-linux-gnu/libboost_system.so

################################################################################
####################              qglviewer              #######################
################################################################################

LIBS += /usr/lib/x86_64-linux-gnu/libQGLViewer.so -lGL -lGLU -lglut

################################################################################
#####################                opencv              #######################
################################################################################

INCLUDEPATH += /opt/opencv-2.4.9/include
LIBS += -L/opt/opencv-2.4.9/lib -lopencv_calib3d -lopencv_contrib \
    -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui \
   -lopencv_imgproc -lopencv_legacy -lopencv_ml  \
   -lopencv_nonfree -lopencv_objdetect \
    -lopencv_photo -lopencv_stitching -lopencv_superres \
    -lopencv_ts

################################################################################
#####################                g2o                 #######################
################################################################################
INCLUDEPATH += /usr/local/include/g2o/core/
INCLUDEPATH += /usr/include/suitesparse
INCLUDEPATH += /usr/local/include/eigen3
INCLUDEPATH += /usr/include/eigen3

LIBS += /usr/lib/x86_64-linux-gnu/libcholmod.so.2.1.2
LIBS += -lg2o_core -lg2o_cli -lg2o_solver_cholmod -lg2o_parser -lg2o_stuff


