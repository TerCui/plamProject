#include "noregisteredform.h"
#include "ui_noregisteredform.h"

NoregisteredForm::NoregisteredForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::noregisteredForm)
{
    ui->setupUi(this);
    QPalette label_palette;
    label_palette.setColor(QPalette::Background, QColor(0, 0, 0));
    setPalette(label_palette);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(regTimerCount()));
//    timer->start(1000);//1m的定时器

    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::white);
    ui->label_hint->setPalette(pe);
    ui->label_day->setPalette(pe);
    ui->label_time->setPalette(pe);
    ui->label_count->setPalette(pe);

    QFont ft;
    ft.setPointSize(28);
    ui->label_hint->setFont(ft);
    timeEnd = 5;
    ui->label_count->setText(tr("5s"));
}

NoregisteredForm::~NoregisteredForm()
{
    timer->stop();
    delete ui;
}

void NoregisteredForm::regTimerCount(void)
{
        QLocale locale = QLocale::Chinese;//指定中文显示
        QString strTime = locale.toString(QTime::currentTime(),QString("h:m"));
//        qDebug()<<"Time:"<<strTime<<endl;
        ui->label_time->setText(strTime);
//        QDate qdateObj = QDate::currentDate();
        QString strDate = locale.toString(QDate::currentDate(),QString("yyyy/M/d"));
//        qDebug()<<"Date:"<<strDate<<endl;
        ui->label_day->setText (strDate);
        timeEnd--;
        ui->label_count->setText(QString::number(timeEnd)+tr(+"s"));
        if(timeEnd<1){
            emit backWorking();
            timer->stop();
        }
}

void NoregisteredForm::resetRegTimer(){
    timeEnd = 5;
    QLocale locale = QLocale::Chinese;//指定中文显示
    QString strTime = locale.toString(QTime::currentTime(),QString("h:m"));
//        qDebug()<<"Time:"<<strTime<<endl;
    ui->label_time->setText(strTime);
//        QDate qdateObj = QDate::currentDate();
    QString strDate = locale.toString(QDate::currentDate(),QString("yyyy/M/d"));
//        qDebug()<<"Date:"<<strDate<<endl;
    ui->label_day->setText (strDate);
    ui->label_count->setText(tr("5s"));
    timer->start(1000);//1m的定时器
}
