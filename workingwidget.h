#ifndef WORKINGWIDGET_H
#define WORKINGWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QDateTime>
#include <QMovie>
#include <QStackedWidget>
#include "noregisteredform.h"

QT_BEGIN_NAMESPACE
namespace Ui { class WorkingWidget; }
QT_END_NAMESPACE

class WorkingWidget : public QWidget
{
    Q_OBJECT

public:
    WorkingWidget(QWidget *parent = nullptr);
    ~WorkingWidget();
signals:
    void noRegResult();
    void passResult();
    void warnResult();
    void addUser();
    void queryUser();


private slots:
    void on_pushButton_clicked();
    void timerUpdate(void);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::WorkingWidget *ui;
    QTimer *timer;
};
#endif // WORKINGWIDGET_H
