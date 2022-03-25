#ifndef WORKINGWIDGET_H
#define WORKINGWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QDateTime>
#include <QMovie>
#include <QStackedWidget>
#include "noregisteredform.h"
#include "rgbcamerathread.h"
#include "camerathread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class WorkingWidget; }
QT_END_NAMESPACE

class WorkingWidget : public QWidget
{
    Q_OBJECT

public:
    WorkingWidget(QWidget *parent = nullptr);
    ~WorkingWidget();
    void resume();
    bool regStart;
signals:
    void noRegResult();
    void passResult();
    void warnResult();
    void addUser();
    void queryUser();
    void isDone();
    void saveJpg();
    void startIR();
    void resumeDone();
    void registerSuccess();

private slots:
    void on_pushButton_clicked();
    void timerUpdate(void);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();
    void saveFinish();
    void showFrame(QImage);
    void badGesture();


private:
    Ui::WorkingWidget *ui;
    QTimer *timer;
    QMovie *movie;
    RgbCameraThread *rgbcamera;
    CameraThread * ircamera;
};
#endif // WORKINGWIDGET_H
