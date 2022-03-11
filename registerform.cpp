#include "registerform.h"
#include "ui_registerform.h"
#include "user.h"
#include "toast.h"

RegisterForm::RegisterForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterForm)
{
    ui->setupUi(this);
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    QPalette label_palette;
    label_palette.setColor(QPalette::Background, QColor(0, 0, 0));
    setPalette(label_palette);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(regTimerCount()));
    timer->start(1000);//1m的定时器

    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::white);
    ui->label_name->setPalette(pe);
    ui->label_day->setPalette(pe);
    ui->label_time->setPalette(pe);
    ui->label_title->setPalette(pe);
    ui->label_dep->setPalette(pe);
    ui->label_id->setPalette(pe);
    ui->label_pin->setPalette(pe);
    ui->label_cardno->setPalette(pe);
    ui->label_auth->setPalette(pe);
    ui->label_plam->setPalette(pe);
    ui->label_face->setPalette(pe);

    QFont name_ft;
    name_ft.setPointSize(28);
    ui->label_title->setFont(name_ft);

    QFont id_ft;
    id_ft.setPointSize(18);
    ui->label_id->setFont(id_ft);
    ui->label_day->setFont(id_ft);
    ui->label_time->setFont(id_ft);
    ui->label_name->setFont(id_ft);
    ui->label_dep->setFont(id_ft);
    ui->label_pin->setFont(id_ft);
    ui->label_cardno->setFont(id_ft);
    ui->label_auth->setFont(id_ft);
    ui->label_plam->setFont(id_ft);
    ui->label_face->setFont(id_ft);
    ui->comboBox->setFont(id_ft);
    ui->lineEdit_id->setFont(id_ft);
    ui->lineEdit_name->setFont(id_ft);
    ui->lineEdit_dep->setFont(id_ft);
    ui->lineEdit_pin->setFont(id_ft);
    ui->lineEdit_cardno->setFont(id_ft);

    dbOperator = new DbOperator("plam_data");

}

RegisterForm::~RegisterForm()
{
    delete ui;
}

void RegisterForm::regTimerCount(void)
{
        QLocale locale = QLocale::Chinese;//指定中文显示
        QString strTime = locale.toString(QTime::currentTime(),QString("h:m"));
        ui->label_time->setText(strTime);
        QString strDate = locale.toString(QDate::currentDate(),QString("yyyy/M/d"));
        ui->label_day->setText (strDate);
}

void RegisterForm::on_pushButton_clicked()
{
    emit backWorking();
}

void RegisterForm::on_pushButton_2_clicked()
{
    QString id = ui->lineEdit_id->text();
    if (id.isEmpty()){
        Toast::instance().show(Toast::ERROR, "id不能为空!");
        return;
    }
    QString name = ui->lineEdit_name->text();
    if(name.isEmpty()){
        Toast::instance().show(Toast::ERROR, "名称不能为空!");
        return;
    }
    QString department = ui->lineEdit_dep->text();
    QString pin = ui->lineEdit_pin->text();
    QString cardNo = ui->lineEdit_cardno->text();
    QString auth = ui->comboBox->currentText();
    if(auth.isEmpty() || QString::fromLocal8Bit("请选择")==auth){
        Toast::instance().show(Toast::ERROR, "权限不能为空!");
        return;
    }
    User *user = new User();
    user->id=id;
    user->pin = pin;
    user->auth = auth;
    user->name = name;
    user->cardNo = cardNo;
    user->department = department;
    if(dbOperator->checkUserIfExists(id)){
        Toast::instance().show(Toast::ERROR, "该ID人员已经存在!");
        return;
    }
    if(dbOperator->insertUser(*user)){
        Toast::instance().show(Toast::INFO, "保存成功!");
        emit backWorking();
    }else{
        Toast::instance().show(Toast::ERROR, "保存失败!");
    }
}

void RegisterForm::resetForm(){
    ui->lineEdit_id->setText("");
    ui->lineEdit_dep->setText("");
    ui->lineEdit_name->setText("");
    ui->lineEdit_cardno->setText("");
    ui->lineEdit_pin->setText("");
    ui->comboBox->setCurrentIndex(0);
    //TODO:bottom buttons should change icons
}
