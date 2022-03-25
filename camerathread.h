#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H
#include <QWidget>
#include <QThread>
#include "rkmedia_api.h"
#include "rkmedia_venc.h"
#include "sample_common.h"
#include <QMutex>
#include "dboperator.h"
#include "user.h"

class CameraThread:public QThread
{
    Q_OBJECT
public:
    CameraThread(QObject *parent);
    ~CameraThread();
    void run();//线程入口函数（工作线程的主函数）
    void pause();
    void resume(QString,bool);
    void exitThread();
    DbOperator *dbOperator;
    User *user;
     bool showPic;
//    static void video_packet_cb(MEDIA_BUFFER mb);
public slots:
    void dealDone();
    void startSave();
signals:
    void saveDone();
    void jpegFrame(QImage);
    void badGesture();
    void registerSuccess();
private:
    VENC_RECV_PIC_PARAM_S stRecvParam;
    RK_S32 ret;
    const char *pcIqFileDir = NULL;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;
    int irCamId = 0;
    int rgbCamId = 1;
};

#endif // CAMERATHREAD_H
