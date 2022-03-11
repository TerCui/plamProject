#include "passedform.h"
#include "ui_passedform.h"

PassedForm::PassedForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PassedForm)
{
    ui->setupUi(this);
    QPalette label_palette;
    label_palette.setColor(QPalette::Background, QColor(0, 0, 0));
    setPalette(label_palette);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(passTimerCount()));
//    timer->start(1000);//1m的定时器

    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::white);
    ui->label_name->setPalette(pe);
    ui->label_day->setPalette(pe);
    ui->label_time->setPalette(pe);
    ui->label_count->setPalette(pe);
    ui->label_department->setPalette(pe);
    ui->label_id->setPalette(pe);
    ui->label_reg_id->setPalette(pe);
    ui->label_tem->setPalette(pe);
    ui->label_temperature->setPalette(pe);


    QFont name_ft;
    name_ft.setPointSize(28);
    ui->label_name->setFont(name_ft);
    timeEnd = 5;
    ui->label_count->setText(tr("5s"));

    QFont dep_ft;
    dep_ft.setPointSize(24);
    ui->label_department->setFont(dep_ft);

    QFont id_ft;
    id_ft.setPointSize(18);
    ui->label_id->setFont(id_ft);
    ui->label_reg_id->setFont(id_ft);
    ui->label_tem->setFont(id_ft);
    ui->label_temperature->setFont(id_ft);
}

PassedForm::~PassedForm()
{
    timer->stop();
    delete ui;
}

void PassedForm::passTimerCount(void)
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

void PassedForm::resetPassTimer(){
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
