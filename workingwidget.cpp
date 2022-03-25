#include "workingwidget.h"
#include "ui_workingwidget.h"
#include <QDebug>
#include "toast.h"
#include "palm.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgcodecs/legacy/constants_c.h"

#include "rgbcamerathread.h"
using namespace std;


WorkingWidget::WorkingWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WorkingWidget)
{
    ui->setupUi(this);
    QPalette label_palette;
    label_palette.setColor(QPalette::Background, QColor(0, 0, 0));
    setPalette(label_palette);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    timer->start(1000);//1m的定时器

    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::white);
    ui->label_hint->setPalette(pe);

    ui->label_day->setPalette(pe);
    ui->label_time->setPalette(pe);

    QLocale locale = QLocale::Chinese;//指定中文显示
    QString strTime = locale.toString(QTime::currentTime(),QString("h:m ap"));
//        qDebug()<<"Time:"<<strTime<<endl;
    ui->label_time->setText(strTime);
//        QDate qdateObj = QDate::currentDate();
    QString strDate = locale.toString(QDate::currentDate(),QString("yyyy/M/d dddd"));
//        qDebug()<<"Date:"<<strDate<<endl;
    ui->label_day->setText (strDate);

    movie=new QMovie(":/new/res/images/plam.gif");
    ui->label_plam->setMovie(movie);
    QSize si(500,500);
    movie->setScaledSize(si);
    movie->start();
    regStart = false;


//    rgbcamera = new RgbCameraThread(this);
//    connect(rgbcamera,SIGNAL(jpegFrame(QImage)),this,SLOT(showFrame(QImage)),Qt::QueuedConnection);
    ircamera = new CameraThread(this);


}

WorkingWidget::~WorkingWidget()
{
    ircamera->exitThread();
    delete ircamera;
    timer->stop();
//    emit isDone();
//    rgbcamera->terminate();
//    delete rgbcamera;
    delete ui;
}

void WorkingWidget::timerUpdate(void)
{
    QLocale locale = QLocale::Chinese;//指定中文显示
    QString strTime = locale.toString(QTime::currentTime(),QString("h:m ap"));
//        qDebug()<<"Time:"<<strTime<<endl;
    ui->label_time->setText(strTime);
//        QDate qdateObj = QDate::currentDate();
    QString strDate = locale.toString(QDate::currentDate(),QString("yyyy/M/d dddd"));
//        qDebug()<<"Date:"<<strDate<<endl;
    ui->label_day->setText (strDate);
    if(regStart){
        regStart = false;
//        qDebug()<<"resume working time:"<<strTime<<endl;
        emit resumeDone();
    }
}



void WorkingWidget::on_pushButton_clicked()
{
//    qDebug()<<"sended"<<endl;
//    emit noRegResult();
//    emit startIR();
//    ui->pushButton->hide();
//    ui->pushButton_2->hide();
//    ui->pushButton_3->hide();
//    ui->pushButton_4->hide();
//    ui->pushButton_5->hide();
//    ui->label_hint->hide();


//    rgbcamera->init(0);
//    rgbcamera->start();

    ircamera->start();
}

void WorkingWidget::on_pushButton_2_clicked()
{
//    emit passResult();
//    emit saveJpg();
//    emit isDone();
    rgbcamera->terminate();
    rgbcamera->deinit();
}

void WorkingWidget::on_pushButton_3_clicked()
{
//    rgbcamera->init(1);
//    rgbcamera->start();
//    emit warnResult();
//    Palm_API *palm1 = new Palm_API("/usr/bin/rknn/clsmodel.rknn","/usr/bin/rknn/palm_k.rknn","/usr/bin/rknn/palm0309.rknn");
//    cv::Mat score;
//    cv::Point point;
//    cv::Mat feature;
//    cv::Mat t_map = cv::imread("/test1.jpeg");
//    int ret = palm1->Palmclass(t_map,score);
//    qDebug()<<"ret:"<<ret<<"----score0:"<<score.at<double>(0,0)<<"-----score1:"<<score.at<double>(0,1)<<endl;
//    cv::Mat roi = palm1->PalmROI(t_map,point); //关键点检测
//    if(!roi.empty()){
//        //good point
//         qDebug()<<"roi good "<<endl;
//        palm1->Feature(roi,feature);  //特征提取
//        stringstream stream;
//        stream<<palm1;
//         qDebug()<<"feature size "<<QString::fromStdString(stream.str())<<endl;
//    }

    Palm_API *palm1 = new Palm_API("/usr/bin/rknn/clsmodel_rgb.rknn","/usr/bin/rknn/palm_k_rgb.rknn","/usr/bin/rknn/palm_rgb.rknn");
    cv::Mat score;
    cv::Point point;
    cv::Mat feature;
    cv::Mat t_map = cv::imread("/test_rgb.jpg");
    int ret = palm1->Palmclass(t_map,score);
    qDebug()<<"ret:"<<ret<<"----score0:"<<score.at<double>(0,0)<<"-----score1:"<<score.at<double>(0,1)<<endl;
    cv::Mat roi = palm1->PalmROI(t_map,point); //关键点检测
    if(!roi.empty()){
        //good point
         qDebug()<<"roi good "<<endl;
        palm1->Feature(roi,feature);  //特征提取
        stringstream stream;
        stream<<palm1;
         qDebug()<<"feature str "<<QString::fromStdString(stream.str())<<endl;
    }
}

void WorkingWidget::on_pushButton_4_clicked()
{
    emit addUser();
}

void WorkingWidget::on_pushButton_5_clicked()
{
    emit queryUser();
}

void WorkingWidget::saveFinish()
{
    Toast::instance().show(Toast::INFO, "拍摄完成!");
}

void WorkingWidget::showFrame(QImage img)
{

//    printf("收到帧数据!\n");
    ui->label_plam->clear();
    ui->label_plam->setPixmap(QPixmap::fromImage(img));

    ui->label_plam->show();
//    ui->label_left->hide();
//    ui->label_right->hide();
//    if(leftShow){
//        ui->label_left->show();

//    }else{
//        ui->label_right->show();
//    }
}

void WorkingWidget::badGesture()
{
    Toast::instance().show(Toast::INFO, "姿势不正确!");
}

void WorkingWidget::resume()
{
    movie->start();
}

