#ifndef RGBCAMERATHREAD_H
#define RGBCAMERATHREAD_H
#include <QWidget>
#include <QThread>
#include "rkmedia_api.h"
#include "rkmedia_venc.h"
#include "sample_common.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgcodecs/legacy/constants_c.h"
#include "dboperator.h"
#include "palm.h"


#define TEST_ARGB32_YELLOW 0xFFFFFF00
#define TEST_ARGB32_RED 0xFFFF0033
#define TEST_ARGB32_BLUE 0xFF003399
#define TEST_ARGB32_TRANS 0x00000000

class RgbCameraThread:public QThread
{
    Q_OBJECT
public:
    RgbCameraThread(QObject *parent);
    ~RgbCameraThread();

    void run();//线程入口函数（工作线程的主函数）
    void init(int);
    void deinit();

    bool saveFile;
    cv::Mat mapx,mapy;

    cv::Mat score;
    cv::Point point;
    cv::Mat feature;
    Palm_API *palm1;
    DbOperator *dbOperator;
    User *user;
     bool showPic;
     bool startReg;

signals:
    void saveDone();
    void jpegFrame(QImage);
    void badGesture();
    void registerSuccess();

private:
    bool quitDone;

    RK_S32 ret;
    RK_U32 u32SrcWidth ;
    RK_U32 u32SrcHeight ;
    RK_U32 u32DstWidth ;
    RK_U32 u32DstHeight ;
    IMAGE_TYPE_E enPixFmt;
    const RK_CHAR *pcVideoNode;
    const char *pcIqFileDir;
    int s32CamId;
    int u32Rotation;
    RK_BOOL bMultictx ;
    int qfactor;
    VENC_JPEG_PARAM_S stJpegParam;
    VENC_RECV_PIC_PARAM_S stRecvParam;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;

    void set_argb8888_buffer(RK_U32 *buf, RK_U32 size, RK_U32 color);
    void cameraInit();
};

#endif // RGBCAMERATHREAD_H
