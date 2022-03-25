QT  += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
TARGET = plam_proj
SOURCES += \
    camerathread.cpp \
    dboperator.cpp \
    main.cpp \
    mainwidget.cpp \
    noregisteredform.cpp \
    passedform.cpp \
    registerform.cpp \
    rgbcamerathread.cpp \
    sample_common.cpp \
    searchform.cpp \
    toast.cpp \
    user.cpp \
    userlistform.cpp \
    warnform.cpp \
    workingwidget.cpp

HEADERS += \
    camerathread.h \
    dboperator.h \
    mainwidget.h \
    noregisteredform.h \
    passedform.h \
    registerform.h \
    rgbcamerathread.h \
    sample_common.h \
    searchform.h \
    toast.h \
    user.h \
    userlistform.h \
    warnform.h \
    workingwidget.h

FORMS += \
    mainwidget.ui \
    noregisteredform.ui \
    passedform.ui \
    registerform.ui \
    searchform.ui \
    userlistform.ui \
    warnform.ui \
    workingwidget.ui

INCLUDEPATH += /home/terry/opencv/arm-4.1.2/include/opencv4
LIBS += -L/home/terry/opencv/arm-4.1.2/lib -lopencv_calib3d \
    -lopencv_core -lopencv_dnn -lopencv_features2d \
    -lopencv_flann -lopencv_highgui -lopencv_imgcodecs \
    -lopencv_imgproc -lopencv_ml -lopencv_objdetect \
    -lopencv_photo -lopencv_stitching \
    -lopencv_video -lopencv_videoio

INCLUDEPATH += /home/terry/qt/alg/test
LIBS += /home/terry/qt/alg/lib/libpalm.a

INCLUDEPATH += /home/terry/rv1109/pro-rv1126_rv1109/01-SDK/external/rknpu/rknn/rknn_api/librknn_api/include
LIBS += /home/terry/rv1109/pro-rv1126_rv1109/01-SDK/external/rknpu/rknn/rknn_api/librknn_api/lib/librknn_api.so

# Default rules for deployment.
LIBS += -L$$PWD/home/terry/rv1109/pro-rv1126_rv1109/01-SDK/buildroot/output/rockchip_rv1126_rv1109_facial_gate/target/usr/lib/ -leasymedia -lrkaiq

INCLUDEPATH += rkmedia uAPI xcore algos common iq_parser
DEPENDPATH += rkmedia uAPI xcore algos common iq_parser

RESOURCES += \
    plamres.qrc
