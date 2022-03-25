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
    name_ft.setPointSize(18);
    ui->label_title->setFont(name_ft);

    QFont id_ft;
    id_ft.setPointSize(14);
    ui->label_hint->setFont(id_ft);
    ui->label_day->setFont(id_ft);
    ui->label_time->setFont(id_ft);

    dbOperator = new DbOperator();
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

void SearchForm::on_backButton_clicked()
{
    emit backUserList();
}

void SearchForm::on_pushButton_1_clicked()
{
    QString data = ui->lineEdit->text();
    data+="1";
    ui->lineEdit->setText(data);
}

void SearchForm::on_pushButton_2_clicked()
{
    QString data = ui->lineEdit->text();
    data+="2";
    ui->lineEdit->setText(data);
}

void SearchForm::on_pushButton_3_clicked()
{
    QString data = ui->lineEdit->text();
    data+="3";
    ui->lineEdit->setText(data);
}

void SearchForm::on_pushButton_4_clicked()
{
    QString data = ui->lineEdit->text();
    data+="4";
    ui->lineEdit->setText(data);
}

void SearchForm::on_pushButton_5_clicked()
{
    QString data = ui->lineEdit->text();
    data+="5";
    ui->lineEdit->setText(data);
}

void SearchForm::on_pushButton_6_clicked()
{
    QString data = ui->lineEdit->text();
    data+="6";
    ui->lineEdit->setText(data);
}

void SearchForm::on_pushButton_7_clicked()
{
    QString data = ui->lineEdit->text();
    data+="7";
    ui->lineEdit->setText(data);
}

void SearchForm::on_pushButton_8_clicked()
{
    QString data = ui->lineEdit->text();
    data+="8";
    ui->lineEdit->setText(data);
}

void SearchForm::on_pushButton_9_clicked()
{
    QString data = ui->lineEdit->text();
    data+="9";
    ui->lineEdit->setText(data);
}

void SearchForm::on_pushButton_10_clicked()
{
    ui->lineEdit->setText("");
}

void SearchForm::on_pushButton_11_clicked()
{
    QString data = ui->lineEdit->text();
    data+="0";
    ui->lineEdit->setText(data);
}

void SearchForm::on_pushButton_12_clicked()
{
    QString data = ui->lineEdit->text();
    emit searchResult(data);
}

void SearchForm::resetForm(){
    if(ui->lineEdit->text().length()>0)
        ui->lineEdit->setText(ui->lineEdit->text().left(ui->lineEdit->text().length()-1));
}
