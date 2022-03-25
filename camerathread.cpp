#include"camerathread.h"
#include<sstream>
#include<iostream>
#include <assert.h>
#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "palm.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgcodecs/legacy/constants_c.h"
#include <QDebug>
#include <QDateTime>

#define TEST_ARGB32_YELLOW 0xFFFFFF00
#define TEST_ARGB32_RED 0xFFFF0033
#define TEST_ARGB32_BLUE 0xFF003399
#define TEST_ARGB32_TRANS 0x00000000

using namespace std;

static bool quitDone = false;
static bool saveFile = false;
static bool startReg = false;
static CameraThread *theFoo;
static Palm_API *palm1;
cv::Mat mapx,mapy;
static int noticeCount = 0;

static void video_packet_rgb(MEDIA_BUFFER mb) {
    static RK_U32 jpeg_id = 0;
      printf("Get rgb JPEG packet[%d]:ptr:%p, fd:%d, size:%zu, mode:%d, channel:%d, "
             "timestamp:%lld\n",
             jpeg_id, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetFD(mb),
             RK_MPI_MB_GetSize(mb), RK_MPI_MB_GetModeID(mb),
             RK_MPI_MB_GetChannelID(mb), RK_MPI_MB_GetTimestamp(mb));
      RK_MPI_MB_ReleaseBuffer(mb);
}

static void video_packet_cb(MEDIA_BUFFER mb) {
  static RK_U32 jpeg_id = 0;
  printf("Get ir JPEG packet[%d]:ptr:%p, fd:%d, size:%zu, mode:%d, channel:%d, "
         "timestamp:%lld\n",
         jpeg_id, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetFD(mb),
         RK_MPI_MB_GetSize(mb), RK_MPI_MB_GetModeID(mb),
         RK_MPI_MB_GetChannelID(mb), RK_MPI_MB_GetTimestamp(mb));
  if(theFoo->showPic){
  //cv::Mat mat(1920,1080,CV_8UC3,(void*)RK_MPI_MB_GetPtr(mb));
  std::vector<uchar> vec_data((uchar*)RK_MPI_MB_GetPtr(mb), (uchar*)RK_MPI_MB_GetPtr(mb) + RK_MPI_MB_GetSize(mb));
  cv::Mat mat = cv::imdecode(vec_data, cv::IMREAD_COLOR);
//  printf("MAT getted:%d\n",mat.channels());


  cv::Mat target_map,map_t,t_map;
  cv::remap(mat, target_map, mapx, mapy, cv::INTER_LINEAR);
//  printf("MAT remaped\n");



//  cv::Mat affine_matrix = cv::getRotationMatrix2D(cv::Point2f(target_map.cols / 2, target_map.rows / 2), 270 * -1, 1.0);//求得旋转矩阵
//  cv::warpAffine(target_map, target_map, affine_matrix, target_map.size());
  cv::resize(target_map,map_t,cv::Size(640,480));
  cv::cvtColor(map_t,map_t,cv::COLOR_BGR2RGB);
  QImage img = QImage((uchar *)map_t.data, map_t.cols,map_t.rows,map_t.cols*map_t.channels(),QImage::Format_RGB888);
  //QImage img = QImage::fromData((uchar *) RK_MPI_MB_GetPtr(mb), 1920*1080);
  QMatrix matrix;
  matrix.rotate(90.0);//以90度为例
  img = img.transformed(matrix,Qt::FastTransformation);

//  QImage r_image;
//  r_image = img.scaled(640, 480, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//  printf("send frame data -------------------------\n");

      t_map = target_map.clone();
      cv::Mat map_r;
      cv::Mat affine_matrix = cv::getRotationMatrix2D(cv::Point2f(t_map.cols / 2, t_map.rows / 2), 180 * -1, 1.0);//求得旋转矩阵
      cv::warpAffine(t_map, map_r, affine_matrix, t_map.size());

      if(saveFile){
          saveFile = false;
          char jpeg_path[128];
          sprintf(jpeg_path, "%s/test_jpeg%d.jpeg", "/tmp", jpeg_id);
          cv::imwrite(jpeg_path,map_r);
    //      FILE *file = fopen(jpeg_path, "w");
    //      if (file) {
    //        fwrite(RK_MPI_MB_GetPtr(mb), 1, RK_MPI_MB_GetSize(mb), file);
    //        fclose(file);
    //      }

          //RK_MPI_MB_ReleaseBuffer(mb);
          jpeg_id++;
          theFoo->saveDone();
      }

      cv::Mat score;
      cv::Point point;
      cv::Mat feature;
      int ret = palm1->Palmclass(map_r,score);
      qDebug()<<"ret:"<<ret<<"------------------------------score:"<<score.at<double>(0,1)<<endl;
      if(score.at<double>(0,1) > 0){ //score.at<double>(0,0)
          //good gesture
          qDebug()<<"good gesture!"<<endl;
          cv::Mat roi = palm1->PalmROI(map_r,point); //关键点检测
          if(!roi.empty()){
              //good point
              palm1->Feature(roi,feature);  //特征提取
              if(startReg){
                  if(theFoo->user!=nullptr){
                      //save plam feature
                      stringstream stream;
                      stream<<palm1;
                      theFoo->user->plam = QString::fromStdString(stream.str());
                      if(theFoo->dbOperator->updateUser(*(theFoo->user))){
//                          theFoo->registerSuccess();
                      }
                  }
              }
          }else{
              //bad point
          }
      }else{
          //bad gestrue notice user change plam locate
          if(noticeCount>5){
              theFoo->badGesture();
              noticeCount = 0;
          }
      }
    theFoo->jpegFrame(img);
  }
  RK_MPI_MB_ReleaseBuffer(mb);
}

static void set_argb8888_buffer(RK_U32 *buf, RK_U32 size, RK_U32 color) {
  for (RK_U32 i = 0; buf && (i < size); i++)
    *(buf + i) = color;
}

CameraThread::CameraThread(QObject *parent)
    :QThread(parent)
{
    theFoo = this;
    cv::Size imageSize(1920,1080);
    printf("MAT size:%d %d\n",imageSize.height,imageSize.width);
    cv::Matx33d K(549.2774657763434, 0.0, 887.1694495529554, 0.0, 544.4169905547323, 538.159623377508, 0, 0, 1);    //   摄像机内参数矩阵
    cv::Vec4d D(0.01738718674473489, -0.10436208298233907, 0.3076884677679181, -0.22413419547802943);     //摄像机的4个畸变系数：k1,k2,k3,k4

    mapx = cv::Mat(imageSize, CV_32FC1);
    mapy = cv::Mat(imageSize, CV_32FC1);
    cv::Mat R = cv::Mat::eye(3, 3, CV_32F);

    cv::Mat newCameraMatrix = cv::Mat(3, 3, CV_32FC1, cv::Scalar::all(0));
    cv::fisheye::initUndistortRectifyMap(K, D, R, K, imageSize, CV_32FC1, mapx, mapy);


    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    int timeT = time.toTime_t();   //将当前时间转为时间戳
    qDebug()<<timeT<<endl;
//    palm1 = new Palm_API("/usr/bin/rknn/clsmodel.rknn","/usr/bin/rknn/palm_k.rknn","/usr/bin/rknn/palm0309.rknn");
//    dbOperator = new DbOperator();
    showPic = false;
//    user = new User();


    RK_U32 u32SrcWidth = 1920;
    RK_U32 u32SrcHeight = 1080;
    RK_U32 u32DstWidth = 1920;
    RK_U32 u32DstHeight = 1080;
    IMAGE_TYPE_E enPixFmt = IMAGE_TYPE_NV12;
    //const RK_CHAR *pcVideoNode = "rkispp_m_bypass";
    const RK_CHAR *pcVideoNode = "rkispp_scale0";


    int u32Rotation = 0;

    RK_BOOL bMultictx = RK_TRUE;
    pcIqFileDir = "/etc/iqfiles";

    printf("#SrcWidth: %d\n", u32SrcWidth);
      printf("#u32SrcHUser eight: %d\n", u32SrcHeight);
      printf("#u32DstWidth: %d\n", u32DstWidth);
      printf("#u32DstHeight: %d\n", u32DstHeight);
      printf("#u32Rotation: %d\n", u32Rotation);
      printf("#CameraIdx: %d\n\n", irCamId);

      if (pcIqFileDir) {

        printf("#Rkaiq XML DirPath: %s\n", pcIqFileDir);
        printf("#bMultictx: %d\n\n", bMultictx);
        rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
        int fps = 30;
        SAMPLE_COMM_ISP_Init(irCamId, hdr_mode, bMultictx, pcIqFileDir);
        SAMPLE_COMM_ISP_Run(irCamId);
        SAMPLE_COMM_ISP_SetFrameRate(irCamId, fps);

        SAMPLE_COMM_ISP_Init(rgbCamId, hdr_mode, bMultictx, pcIqFileDir);
        SAMPLE_COMM_ISP_Run(rgbCamId);
        SAMPLE_COMM_ISP_SetFrameRate(rgbCamId, fps);

      }

      VENC_ROTATION_E enRotation;
        switch (u32Rotation) {
          case 0:
            enRotation = VENC_ROTATION_0;
            break;
          case 90:
            enRotation = VENC_ROTATION_90;
            break;
          case 180:
            enRotation = VENC_ROTATION_180;
            break;
          case 270:
            enRotation = VENC_ROTATION_270;
            break;
          default:
            printf("Invalid rotation(%d), should be 0/90/180/270\n", u32Rotation);
        }

        ret = RK_MPI_SYS_Init();
        if (ret) {
          printf("Sys Init failed! ret=%d\n", ret);
        }

        VI_CHN_ATTR_S vi_chn_attr;
        vi_chn_attr.pcVideoNode = pcVideoNode;
        vi_chn_attr.u32BufCnt = 3;
        vi_chn_attr.u32Width = u32SrcWidth;
        vi_chn_attr.u32Height = u32SrcHeight;
        vi_chn_attr.enPixFmt = enPixFmt;
        vi_chn_attr.enWorkMode = VI_WORK_MODE_NORMAL;
        ret = RK_MPI_VI_SetChnAttr(irCamId, 0, &vi_chn_attr);
        ret |= RK_MPI_VI_EnableChn(irCamId, 0);
        if (ret) {
          printf("Create V0 failed! ret=%d\n", ret);
        }

        ret = RK_MPI_VI_SetChnAttr(rgbCamId, 1, &vi_chn_attr);
        ret |= RK_MPI_VI_EnableChn(rgbCamId, 1);
        if (ret) {
          printf("Create V1 failed! ret=%d\n", ret);
        }


        VENC_CHN_ATTR_S venc_chn_attr;
          memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
          venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_JPEG;
          venc_chn_attr.stVencAttr.imageType = enPixFmt;
          venc_chn_attr.stVencAttr.u32PicWidth = u32SrcWidth;
          venc_chn_attr.stVencAttr.u32PicHeight = u32SrcHeight;
          venc_chn_attr.stVencAttr.u32VirWidth = u32SrcWidth;
          venc_chn_attr.stVencAttr.u32VirHeight = u32SrcHeight;
          venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomWidth = u32DstWidth;
          venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomHeight = u32DstHeight;
          venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomVirWidth = u32DstWidth;
          venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomVirHeight = u32DstHeight;
          venc_chn_attr.stVencAttr.enRotation = enRotation;
          ret = RK_MPI_VENC_CreateChn(0, &venc_chn_attr);
          if (ret) {
            printf("Create Venc failed! ret=%d\n", ret);
          }

          ret = RK_MPI_VENC_CreateChn(1, &venc_chn_attr);
          if (ret) {
            printf("Create Venc2 failed! ret=%d\n", ret);
          }

          MPP_CHN_S stEncChn;
           stEncChn.enModId = RK_ID_VENC;
           stEncChn.s32ChnId = 0;
           ret = RK_MPI_SYS_RegisterOutCb(&stEncChn, video_packet_cb);
           if (ret) {
             printf("Register Output callback failed! ret=%d\n", ret);
           }
           // The encoder defaults to continuously receiving frames from the previous
             // stage. Before performing the bind operation, set s32RecvPicNum to 0 to
             // make the encoding enter the pause state.
           stEncChn.s32ChnId = 1;
           ret = RK_MPI_SYS_RegisterOutCb(&stEncChn, video_packet_rgb);
           if (ret) {
             printf("Register Output callback failed! ret=%d\n", ret);
           }
             stRecvParam.s32RecvPicNum = 0;
for (RK_U16 i = 0; i < 2; i++) {
             RK_MPI_VENC_StartRecvFrame(i, &stRecvParam);


             stSrcChn.enModId = RK_ID_VI;
             stSrcChn.s32ChnId = i;

             stDestChn.enModId = RK_ID_VENC;
             stDestChn.s32ChnId = i;
             ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
             if (ret) {
               printf("Bind VI[1] to VENC[0]::JPEG failed! ret=%d\n", ret);
             }



             RK_MPI_VENC_RGN_Init(i, NULL);

             BITMAP_S BitMap;
             BitMap.enPixelFormat = PIXEL_FORMAT_ARGB_8888;
             BitMap.u32Width = 64;
             BitMap.u32Height = 64;
             BitMap.pData = malloc(BitMap.u32Width * 4 * BitMap.u32Height);
             RK_U8 *ColorData = (RK_U8 *)BitMap.pData;
             RK_U16 ColorBlockSize = BitMap.u32Height * BitMap.u32Width;
             set_argb8888_buffer((RK_U32 *)ColorData, ColorBlockSize / 4,
                                 TEST_ARGB32_YELLOW);
             set_argb8888_buffer((RK_U32 *)(ColorData + ColorBlockSize),
                                 ColorBlockSize / 4, TEST_ARGB32_TRANS);
             set_argb8888_buffer((RK_U32 *)(ColorData + 2 * ColorBlockSize),
                                 ColorBlockSize / 4, TEST_ARGB32_RED);
             set_argb8888_buffer((RK_U32 *)(ColorData + 3 * ColorBlockSize),
                                 ColorBlockSize / 4, TEST_ARGB32_BLUE);

             // Case 1: Canvas and bitmap are equal in size
             OSD_REGION_INFO_S RngInfo;
             RngInfo.enRegionId = REGION_ID_0;
             RngInfo.u32PosX = 0;
             RngInfo.u32PosY = 0;
             RngInfo.u32Width = 64;
             RngInfo.u32Height = 64;
             RngInfo.u8Enable = 1;
             RngInfo.u8Inverse = 0;
             RK_MPI_VENC_RGN_SetBitMap(i, &RngInfo, &BitMap);

             printf("%s initial finish\n", __func__);
//             int qfactor = 0;
//             VENC_JPEG_PARAM_S stJpegParam;
//             printf("#Usage: input 'quit' to exit programe!\n"
//                    "peress any other key to capture one picture to file\n");
}


}
CameraThread::~CameraThread()
{

}
void CameraThread::run()
{
    int qfactor = 0;
    VENC_JPEG_PARAM_S stJpegParam;
     while (!quitDone && !QThread::currentThread()->isInterruptionRequested()) {
         if (qfactor >= 99)
           qfactor = 1;
         else
           qfactor = ((qfactor + 10) > 99) ? 99 : (qfactor + 10);
         stJpegParam.u32Qfactor = 90;
         RK_MPI_VENC_SetJpegParam(0, &stJpegParam);

          RK_MPI_VENC_SetJpegParam(1, &stJpegParam);

         VENC_CHN_PARAM_S venc_chn_param;
         memset(&venc_chn_param, 0, sizeof(venc_chn_param));
         venc_chn_param.stCropCfg.bEnable = RK_TRUE;
         venc_chn_param.stCropCfg.stRect.s32X = 0;
         venc_chn_param.stCropCfg.stRect.s32Y = 0;
         venc_chn_param.stCropCfg.stRect.u32Width = 1920;
         venc_chn_param.stCropCfg.stRect.u32Height = 1080;
         ret = RK_MPI_VENC_SetChnParam(0, &venc_chn_param);
         if (ret) {
           printf("Set Venc Channel failed! ret=%d\n", ret);
           return ;
         }


         ret = RK_MPI_VENC_SetChnParam(1, &venc_chn_param);
         if (ret) {
           printf("Set Venc Channel failed! ret=%d\n", ret);
           return ;
         }

         stRecvParam.s32RecvPicNum = 1;

         ret = RK_MPI_VENC_StartRecvFrame(0, &stRecvParam);
         if (ret) {
           printf("RK_MPI_VENC_StartRecvFrame failed!\n");
           break;
         }

         ret = RK_MPI_VENC_StartRecvFrame(1, &stRecvParam);
         if (ret) {
           printf("RK_MPI_VENC_StartRecvFrame failed!\n");
           break;
         }
         usleep(30000); // sleep 30 ms.
       }

}

void CameraThread::dealDone(){
    quitDone = true;
    QThread::currentThread()->wait();
    qDebug() << "exit done";

}

void CameraThread::startSave(){
    saveFile = true;
     //printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
}

void CameraThread::pause(){
    showPic = false;
}

void CameraThread::resume(QString id,bool sReg){
 qDebug()<<"resuming id "<<id <<endl;
    startReg = sReg;
     *user = dbOperator->queryUser(id);
     qDebug()<<"resuming"<<endl;
     showPic = true;

}

void CameraThread::exitThread(){
    stSrcChn.enModId = RK_ID_VI;
    stSrcChn.s32ChnId = 0;

    stDestChn.enModId = RK_ID_VENC;
    stDestChn.s32ChnId = 0;
    RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);

    stSrcChn.enModId = RK_ID_VI;
    stSrcChn.s32ChnId = 1;

    stDestChn.enModId = RK_ID_VENC;
    stDestChn.s32ChnId = 1;
    RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);

    RK_MPI_VENC_DestroyChn(0);
    RK_MPI_VI_DisableChn(irCamId, 1);

    RK_MPI_VENC_DestroyChn(1);
    RK_MPI_VI_DisableChn(rgbCamId, 1);

    if (pcIqFileDir) {

      SAMPLE_COMM_ISP_Stop(irCamId);

       SAMPLE_COMM_ISP_Stop(rgbCamId);

    }

    printf("%s exit!\n", __func__);
    QThread::currentThread()->requestInterruption();

    QThread::currentThread()->wait();

    delete QThread::currentThread();

}
