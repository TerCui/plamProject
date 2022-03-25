#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "toast.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainwidget)
{
    ui->setupUi(this);
    QPalette label_palette;
    label_palette.setColor(QPalette::Background, QColor(0, 0, 0));
    setPalette(label_palette);

    regStarted = false;

    workingWidget = new WorkingWidget (this);
    noregisteredForm = new NoregisteredForm (this);
    passedForm = new PassedForm(this);
    warnForm = new WarnForm(this);
    dbOperator = new DbOperator();
    registerForm = new RegisterForm(this);
    userListForm = new UserListForm(this);
    searchForm = new SearchForm(this);

    ui->stackedWidget->addWidget(workingWidget);
    ui->stackedWidget->addWidget(noregisteredForm);
    ui->stackedWidget->addWidget(passedForm);
    ui->stackedWidget->addWidget(warnForm);
    ui->stackedWidget->addWidget(registerForm);
    ui->stackedWidget->addWidget(userListForm);
    ui->stackedWidget->addWidget(searchForm);
    ui->stackedWidget->setCurrentWidget(workingWidget);


    connect(workingWidget,SIGNAL(noRegResult()),this,SLOT(switchToNoReg()));
    connect(noregisteredForm,SIGNAL(backWorking()),this,SLOT(switchToWorking()));
    connect(this,SIGNAL(resetRegCount()),noregisteredForm,SLOT(resetRegTimer()));


    connect(passedForm,SIGNAL(backWorking()),this,SLOT(switchToWorking()));
    connect(this,SIGNAL(resetPassCount()),passedForm,SLOT(resetPassTimer()));

    connect(workingWidget,SIGNAL(warnResult()),this,SLOT(switchToWarn()));
    connect(warnForm,SIGNAL(backWorking()),this,SLOT(switchToWorking()));
    connect(this,SIGNAL(resetWarnCount()),warnForm,SLOT(resetWarnTimer()));

    connect(workingWidget,SIGNAL(addUser()),this,SLOT(switchToRegister()));
    connect(registerForm,SIGNAL(backWorking()),this,SLOT(switchToWorking()));
    connect(this,SIGNAL(resetRegisterForm()),registerForm,SLOT(resetForm()));
    connect(this,SIGNAL(userData(QString)),registerForm,SLOT(userData(QString)));
    connect(registerForm,SIGNAL(backSearch(QString)),this,SLOT(searchResult(QString)));

    connect(workingWidget,SIGNAL(queryUser()),this,SLOT(queryDbUser()));
    connect(this,SIGNAL(resetUserList(QString)),userListForm,SLOT(searchUsers(QString)));
    connect(userListForm,SIGNAL(backWorking()),this,SLOT(switchToWorking()));
    connect(userListForm,SIGNAL(toSearch()),this,SLOT(switchToSearch()));

    connect(searchForm,SIGNAL(backUserList()),this,SLOT(queryDbUser()));
    connect(searchForm,SIGNAL(searchResult(QString)),this,SLOT(searchResult(QString)));
    connect(searchForm,SIGNAL(searchResult(QString)),userListForm,SLOT(searchUsers(QString)));
    connect(userListForm,SIGNAL(resetSearchForm()),searchForm,SLOT(resetForm()));
    connect(userListForm,SIGNAL(editUser(QString)),this,SLOT(editUser(QString)));

    qRegisterMetaType<QImage>("QImage");

//    rgbcamera = new RgbCameraThread(this);
//    connect(rgbcamera,SIGNAL(jpegFrame(QImage)),workingWidget,SLOT(showFrame(QImage)),Qt::QueuedConnection);
//    cameraThread = new CameraThread(this);

//    connect(cameraThread,SIGNAL(jpegFrame(QImage)),workingWidget,SLOT(showFrame(QImage)),Qt::QueuedConnection);
//    connect(cameraThread,SIGNAL(saveDone()),workingWidget,SLOT(saveFinish()),Qt::QueuedConnection);
    connect(workingWidget,SIGNAL(isDone()),this,SLOT(regSuccess()),Qt::QueuedConnection);
//    connect(workingWidget,SIGNAL(saveJpg()),cameraThread,SLOT(startSave()),Qt::QueuedConnection);
    connect(workingWidget,SIGNAL(passResult()),this,SLOT(switchToPass()));

//    connect(workingWidget,SIGNAL(startIR()),this,SLOT(startIRCamera()));
//    connect(cameraThread,SIGNAL(registerSuccess()),this,SLOT(regSuccess()));
//    connect(cameraThread,SIGNAL(badGesture()),workingWidget,SLOT(badGesture()));
//    connect(cameraThread,SIGNAL(registerSuccess()),this,SLOT(regSuccess()));
    connect(registerForm,SIGNAL(startRegCamera(QString)),this,SLOT(regStart(QString)));
    connect(workingWidget,SIGNAL(resumeDone()),this,SLOT(workingResumeDone()));
//    cameraThread->start();
//    cameraThread->pause();

    workingWidget->raise();
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::switchToNoReg(){
    emit resetRegCount();
    ui->stackedWidget->setCurrentWidget(noregisteredForm);
}

void MainWidget::switchToPass(){
    emit resetPassCount();
    ui->stackedWidget->setCurrentWidget(passedForm);
}

void MainWidget::switchToWarn(){
    emit resetWarnCount();
    ui->stackedWidget->setCurrentWidget(warnForm);
}

void MainWidget::switchToWorking(){
//    qDebug()<<"switch working!!!!!!!!!"<<endl;
    ui->stackedWidget->setCurrentWidget(workingWidget);
    workingWidget->raise();
}

void MainWidget::queryDbUser(){
    emit resetUserList("");
    ui->stackedWidget->setCurrentWidget(userListForm);
}

void MainWidget::switchToRegister(){
    emit resetRegisterForm();
    ui->stackedWidget->setCurrentWidget(registerForm);
}

void MainWidget::switchToSearch(){
    ui->stackedWidget->setCurrentWidget(searchForm);
}

void MainWidget::searchResult(QString id){
    ui->stackedWidget->setCurrentWidget(userListForm);
}

void MainWidget::editUser(QString id){
    emit userData(id);
    ui->stackedWidget->setCurrentWidget(registerForm);
}


void MainWidget::startIRCamera(){
    qDebug()<<"______________________2"<<endl;
}

void MainWidget::regSuccess()
{
//    rgbcamera->terminate();
    Toast::instance().show(Toast::INFO, "注册成功!");
    workingWidget->resume();
}

void MainWidget::regStart(QString id){
    ui->stackedWidget->setCurrentWidget(workingWidget);
    workingWidget->raise();
    regStarted = true;
    regId = id;
    workingWidget->regStart = true;
}

void MainWidget::workingResumeDone(){
    if(regStarted){
        regStarted = false;
        qDebug()<<"______________________1"<<endl;
//        rgbcamera->start();
    }
}
