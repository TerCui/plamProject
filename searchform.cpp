#include "searchform.h"
#include "ui_searchform.h"

SearchForm::SearchForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchForm)
{
    ui->setupUi(this);
    QPalette label_palette;
    label_palette.setColor(QPalette::Background, QColor(0, 0, 0));
    setPalette(label_palette);


    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(searchTimerCount()));
    timer->start(1000);//1m的定时器


    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::white);
    ui->label_hint->setPalette(pe);
    ui->label_day->setPalette(pe);
    ui->label_time->setPalette(pe);
    ui->label_title->setPalette(pe);

    QFont name_ft;
    name_ft.setPointSize(28);
    ui->label_title->setFont(name_ft);

    QFont id_ft;
    id_ft.setPointSize(18);
    ui->label_hint->setFont(id_ft);
    ui->label_day->setFont(id_ft);
    ui->label_time->setFont(id_ft);

    dbOperator = new DbOperator("plam_data");
}

SearchForm::~SearchForm()
{
    delete ui;
}

void SearchForm::searchTimerCount(void)
{
    QLocale locale = QLocale::Chinese;//指定中文显示
    QString strTime = locale.toString(QTime::currentTime(),QString("h:m"));
    ui->label_time->setText(strTime);
    QString strDate = locale.toString(QDate::currentDate(),QString("yyyy/M/d"));
    ui->label_day->setText (strDate);
}
