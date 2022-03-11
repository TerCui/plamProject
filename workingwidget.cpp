#include "workingwidget.h"
#include "ui_workingwidget.h"
#include <QDebug>

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

    QMovie *movie=new QMovie(":/new/res/images/plam.gif");
    ui->label_plam->setMovie(movie);
    QSize si(500,500);
    movie->setScaledSize(si);

    movie->start();

}

WorkingWidget::~WorkingWidget()
{
    timer->stop();
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
}



void WorkingWidget::on_pushButton_clicked()
{
//    qDebug()<<"sended"<<endl;
    emit noRegResult();
}

void WorkingWidget::on_pushButton_2_clicked()
{
    emit passResult();
}

void WorkingWidget::on_pushButton_3_clicked()
{
    emit warnResult();
}

void WorkingWidget::on_pushButton_4_clicked()
{
    emit addUser();
}

void WorkingWidget::on_pushButton_5_clicked()
{
    emit queryUser();
}
