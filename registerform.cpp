#include "registerform.h"
#include "ui_registerform.h"
#include "user.h"
#include "toast.h"
#include <QMessageBox>

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
    name_ft.setPointSize(20);
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

    dbOperator = new DbOperator();
    updateUser = false;
    user = new User();
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
    if(updateUser){
        emit backSearch("");
    }else{
        emit backWorking();
    }
    updateUser = false;
}

void RegisterForm::on_pushButton_2_clicked()
{
    if(addUser()){
      emit backWorking();
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

void RegisterForm::userData(QString id){
    *user =dbOperator->queryUser(id);
    ui->lineEdit_id->setText(user->id.simplified());
    ui->lineEdit_dep->setText(user->department.simplified());
    ui->lineEdit_name->setText(user->name.simplified());
    ui->lineEdit_cardno->setText(user->cardNo.simplified());
    ui->lineEdit_pin->setText(user->pin.simplified());
    ui->comboBox->setCurrentText(user->auth);
    updateUser = true;
    ui->label_title->setText("编辑");
    if(user->plam.isEmpty()){
        ui->pushButton_plam->setIcon(QIcon(":/new/res/images/add.png"));
    }else{
        ui->pushButton_plam->setIcon(QIcon(":/new/res/images/plam.png"));
    }
}

bool RegisterForm::isDigitStr(QString src)
{
    QByteArray ba = src.toLatin1();//QString 转换为 char*
     const char *s = ba.data();

    while(*s && *s>='0' && *s<='9') s++;

    if (*s)
    { //不是纯数字
        return false;
    }
    else
    { //纯数字
        return true;
    }
}


void RegisterForm::on_pushButton_plam_clicked()
{
    qDebug()<<"now start register plam!"<<endl;
    if(user->plam.isEmpty()){
        //to register form
        if(user->id.isEmpty()){
            if(!addUser()){
                return;
            }
            //qDebug()<<user->id<<endl;
        }
        emit startRegCamera(user->id);

    }else{
        QMessageBox:: StandardButton result=QMessageBox::information(NULL, "提示", "是否重新录入掌静脉数据？");
        switch (result)
        {
        case QMessageBox::Ok:{
            emit startRegCamera(user->id);
            break;
        }
        case QMessageBox::No:
            break;
        default:
            break;
        }
    }
}

bool RegisterForm::addUser(){
    QString id = ui->lineEdit_id->text();
    if (id.isEmpty()){
        Toast::instance().show(Toast::ERROR, "id不能为空!");
        return false;
    }
    if(!isDigitStr(id)){
        Toast::instance().show(Toast::ERROR, "id必须为数字!");
        return false;
    }
    QString name = ui->lineEdit_name->text();
    if(name.isEmpty()){
        Toast::instance().show(Toast::ERROR, "名称不能为空!");
        return false;
    }
    QString department = ui->lineEdit_dep->text();
    QString pin = ui->lineEdit_pin->text();
    if (!pin.isEmpty() && !isDigitStr(pin)){
        Toast::instance().show(Toast::ERROR, "pin必须为数字!");
        return false;
    }

    QString cardNo = ui->lineEdit_cardno->text();
    QString auth = ui->comboBox->currentText();
    int authIndex = ui->comboBox->currentIndex();
    if(auth.isEmpty() || authIndex==0){
        Toast::instance().show(Toast::ERROR, "权限不能为空!");
        return false;
    }

    user->id=id;
    user->pin = pin;
    user->auth = auth;
    user->name = name;
    user->cardNo = cardNo;
    user->department = department;
    if(updateUser){
        if(dbOperator->updateUser(*user)){
            Toast::instance().show(Toast::INFO, "保存成功!");
            return true;
        }else{
            Toast::instance().show(Toast::ERROR, "保存失败!");
        }
    }else{
        if(dbOperator->checkUserIfExists(id)){
            Toast::instance().show(Toast::ERROR, "该ID人员已经存在!");
            return false;
        }
        if(dbOperator->insertUser(*user)){
            Toast::instance().show(Toast::INFO, "保存成功!");
            return true;
        }else{
            Toast::instance().show(Toast::ERROR, "保存失败!");
        }
    }
    return false;
}
