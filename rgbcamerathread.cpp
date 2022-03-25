#include "rgbcamerathread.h"
#include <QDebug>

using namespace std;

static RgbCameraThread *theRgb;
static int noticeCount = 0;

static void video_packet_cb(MEDIA_BUFFER mb) {
  static RK_U32 jpeg_id = 0;
//    printf("Get JPEG packet[%d]:ptr:%p, fd:%d, size:%zu, mode:%d, channel:%d, "
//           "timestamp:%lld\n",
//           jpeg_id, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetFD(mb),
//           RK_MPI_MB_GetSize(mb), RK_MPI_MB_GetModeID(mb),
//           RK_MPI_MB_GetChannelID(mb), RK_MPI_MB_GetTimestamp(mb));
    if(theRgb->showPic){
    //cv::Mat mat(1920,1080,CV_8UC3,(void*)RK_MPI_MB_GetPtr(mb));
    std::vector<uchar> vec_data((uchar*)RK_MPI_MB_GetPtr(mb), (uchar*)RK_MPI_MB_GetPtr(mb) + RK_MPI_MB_GetSize(mb));
    cv::Mat mat = cv::imdecode(vec_data, cv::IMREAD_COLOR);
    printf("MAT getted:%d\n",mat.channels());


    cv::Mat target_map,map_t,t_map;
    cv::remap(mat, target_map, theRgb->mapx, theRgb->mapy, cv::INTER_LINEAR);
    printf("MAT remaped\n");



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

        if(theRgb->saveFile){
            theRgb->saveFile = false;
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
            theRgb->saveDone();
        }

        cv::Mat mat_s;// sub rect jpeg
        cv::Rect rect(360,0,1200,1080);
        mat_s = map_r(rect);
        cv::Mat score;
        cv::Point point;
        cv::Mat feature;
        int ret = theRgb->palm1->Palmclass(mat_s,score);
        qDebug()<<"ret:"<<ret<<"------------------------------score:"<<score.at<double>(0,1)<<endl;
        if(score.at<double>(0,1) > score.at<double>(0,0)){ //score.at<double>(0,0)
            //good gesture
            qDebug()<<"good gesture!"<<endl;
            cv::Mat roi = theRgb->palm1->PalmROI(mat_s,point); //关键点检测
            if(!roi.empty()){
                //good point
                theRgb->palm1->Feature(roi,feature);  //特征提取
                if(theRgb->startReg){
                    if(theRgb->user!=nullptr){
                        //save plam feature
                        stringstream stream;
                        stream<<theRgb->palm1;
                        theRgb->user->plam = QString::fromStdString(stream.str());
                        if(theRgb->dbOperator->updateUser(*(theRgb->user))){
                            theRgb->registerSuccess();
                        }
                    }
                }else{
                    //todo: compare feature in db
                    for(int i=0; i< theRgb->users.size(); ++i){
                        if(!(theRgb->users)[i].plam.isEmpty()){
                            cv::Mat featureU = theRgb->QString2Mat((theRgb->users)[i].plam);
                            double compare = theRgb->palm1->Compare(feature,featureU);
                            qDebug()<<"compare score:"<<compare<<endl;
                            //todo:set score limit of pass

                        }
                    }
                }
            }else{
                //bad point
            }
        }else{
            //bad gestrue notice user change plam locate
            if(noticeCount>5){
                theRgb->badGesture();
                noticeCount = 0;
            }
        }
      theRgb->jpegFrame(img);
    }
    RK_MPI_MB_ReleaseBuffer(mb);
}

RgbCameraThread::RgbCameraThread(QObject *parent)
    :QThread(parent)
{
    theRgb = this;
    showPic = true;
    quitDone = false;
    saveFile = false;
    startReg = false;
    cv::Size imageSize(1920,1080);
    printf("MAT size:%d %d\n",imageSize.height,imageSize.width);
//    cv::Matx33d K(581.2409139566812, 0.0, 989.5375252375054, 0.0, 569.7979030653797, 530.0555164233762, 0, 0, 1);    //   摄像机内参数矩阵
//    cv::Vec4d D(0.0010060727640258285, -0.10391709648496211,0.12131104140679484, -0.04422047339573606);     //摄像机的4个畸变系数：k1,k2,k3,k4

    cv::Matx33d K(549.2774657763434, 0.0, 887.1694495529554, 0.0, 544.4169905547323, 538.159623377508, 0, 0, 1);    //   摄像机内参数矩阵
    cv::Vec4d D(0.01738718674473489, -0.10436208298233907,0.3076884677679181, -0.22413419547802943);     //摄像机的4个畸变系数：k1,k2,k3,k4

    mapx = cv::Mat(imageSize, CV_32FC1);
    mapy = cv::Mat(imageSize, CV_32FC1);
    cv::Mat R = cv::Mat::eye(3, 3, CV_32F);

    cv::Mat newCameraMatrix = cv::Mat(3, 3, CV_32FC1, cv::Scalar::all(0));
    cv::fisheye::initUndistortRectifyMap(K, D, R, K, imageSize, CV_32FC1, mapx, mapy);

    palm1 = new Palm_API("/usr/bin/rknn/clsmodel_rgb.rknn","/usr/bin/rknn/palm_k_rgb.rknn","/usr/bin/rknn/palm_rgb.rknn");
    dbOperator = new DbOperator();
    user = new User();
}

RgbCameraThread::~RgbCameraThread()
{

}
void RgbCameraThread::run()
{
    cameraInit();
    while (!quitDone) {
        if (qfactor >= 99)
          qfactor = 1;
        else
          qfactor = ((qfactor + 10) > 99) ? 99 : (qfactor + 10);
        stJpegParam.u32Qfactor = 90;
        RK_MPI_VENC_SetJpegParam(0, &stJpegParam);

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

        stRecvParam.s32RecvPicNum = 1;

        ret = RK_MPI_VENC_StartRecvFrame(0, &stRecvParam);
        if (ret) {
          printf("RK_MPI_VENC_StartRecvFrame failed!\n");
          break;
        }
        usleep(30000); // sleep 30 ms.
      }
}

void RgbCameraThread::init(int cameraId){
    s32CamId = cameraId;
}
void RgbCameraThread::cameraInit(){
    users = dbOperator->queryAll();

    u32SrcWidth = 1920;
    u32SrcHeight = 1080;
    u32DstWidth = 1920;
    u32DstHeight = 1080;
    enPixFmt = IMAGE_TYPE_NV12;
    //const RK_CHAR *pcVideoNode = "rkispp_m_bypass";
    pcVideoNode = "rkispp_scale0";
    pcIqFileDir = NULL;

    u32Rotation = 0;

    bMultictx = RK_TRUE;
    pcIqFileDir = "/etc/iqfiles";

    printf("#SrcWidth: %d\n", u32SrcWidth);
      printf("#u32SrcHeight: %d\n", u32SrcHeight);
      printf("#u32DstWidth: %d\n", u32DstWidth);
      printf("#u32DstHeight: %d\n", u32DstHeight);
      printf("#u32Rotation: %d\n", u32Rotation);
      printf("#CameraIdx: %d\n\n", s32CamId);

      if (pcIqFileDir) {

        printf("#Rkaiq XML DirPath: %s\n", pcIqFileDir);
        printf("#bMultictx: %d\n\n", bMultictx);
        rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
        int fps = 30;
        SAMPLE_COMM_ISP_Init(s32CamId, hdr_mode, bMultictx, pcIqFileDir);
        SAMPLE_COMM_ISP_Run(s32CamId);
        SAMPLE_COMM_ISP_SetFrameRate(s32CamId, fps);

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
        ret = RK_MPI_VI_SetChnAttr(s32CamId, 1, &vi_chn_attr);
        ret |= RK_MPI_VI_EnableChn(s32CamId, 1);
        if (ret) {
          printf("Create Vi failed! ret=%d\n", ret);
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

             stRecvParam.s32RecvPicNum = 0;
             RK_MPI_VENC_StartRecvFrame(0, &stRecvParam);

             stSrcChn.enModId = RK_ID_VI;
             stSrcChn.s32ChnId = 1;

             stDestChn.enModId = RK_ID_VENC;
             stDestChn.s32ChnId = 0;
             ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
             if (ret) {
               printf("Bind VI[1] to VENC[0]::JPEG failed! ret=%d\n", ret);
             }


             RK_MPI_VENC_RGN_Init(0, NULL);

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
             RK_MPI_VENC_RGN_SetBitMap(0, &RngInfo, &BitMap);

             printf("%s initial finish\n", __func__);
             qfactor = 0;

             printf("#Usage: input 'quit' to exit programe!\n"
                    "peress any other key to capture one picture to file\n");
}

void RgbCameraThread::deinit(){
    RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    RK_MPI_VENC_DestroyChn(0);
    RK_MPI_VI_DisableChn(s32CamId, 1);

    if (pcIqFileDir) {
      SAMPLE_COMM_ISP_Stop(s32CamId);
    }

    printf("%s exit!\n", __func__);
}

void RgbCameraThread::set_argb8888_buffer(RK_U32 *buf, RK_U32 size, RK_U32 color) {
  for (RK_U32 i = 0; buf && (i < size); i++)
    *(buf + i) = color;
}

cv::Mat RgbCameraThread::QString2Mat(QString text)
{
    QStringList rowvalueList=text.split(";");
    int Matrow=rowvalueList.size();
    int Matcol=QString(rowvalueList[0]).split(",").size();
    float *matrix = new float[Matrow*Matcol];
    for(int row=0;row<Matrow;++row){
        QString rowvalues=rowvalueList[row];
        QStringList colvalues=  rowvalues.split(",");

        for(int col=0;col<Matcol;++col){
           float value=QString(colvalues[col]).replace("\n","").replace("[","").replace("]","").toFloat();
           matrix[row*Matcol+col]=value;
        }
    }
    cv::Mat Matrix(cv::Size(Matcol, Matrow), CV_32FC1, matrix);//注意：opencv里的行列顺序是和maltab相反的
    delete []matrix;
    return Matrix;
}
