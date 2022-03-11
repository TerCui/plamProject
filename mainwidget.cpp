#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainwidget)
{
    ui->setupUi(this);
    QPalette label_palette;
    label_palette.setColor(QPalette::Background, QColor(0, 0, 0));
    setPalette(label_palette);

    workingWidget = new WorkingWidget (this);
    noregisteredForm = new NoregisteredForm (this);
    passedForm = new PassedForm(this);
    warnForm = new WarnForm(this);
    dbOperator = new DbOperator("plam_data");
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

    workingWidget->raise();
    connect(workingWidget,SIGNAL(noRegResult()),this,SLOT(switchToNoReg()));
    connect(noregisteredForm,SIGNAL(backWorking()),this,SLOT(switchToWorking()));
    connect(this,SIGNAL(resetRegCount()),noregisteredForm,SLOT(resetRegTimer()));

    connect(workingWidget,SIGNAL(passResult()),this,SLOT(switchToPass()));
    connect(passedForm,SIGNAL(backWorking()),this,SLOT(switchToWorking()));
    connect(this,SIGNAL(resetPassCount()),passedForm,SLOT(resetPassTimer()));

    connect(workingWidget,SIGNAL(warnResult()),this,SLOT(switchToWarn()));
    connect(warnForm,SIGNAL(backWorking()),this,SLOT(switchToWorking()));
    connect(this,SIGNAL(resetWarnCount()),warnForm,SLOT(resetWarnTimer()));

    connect(workingWidget,SIGNAL(addUser()),this,SLOT(switchToRegister()));
    connect(registerForm,SIGNAL(backWorking()),this,SLOT(switchToWorking()));
    connect(this,SIGNAL(resetRegisterForm()),registerForm,SLOT(resetForm()));

    connect(workingWidget,SIGNAL(queryUser()),this,SLOT(queryDbUser()));
    connect(userListForm,SIGNAL(backWorking()),this,SLOT(switchToWorking()));
    connect(userListForm,SIGNAL(toSearch()),this,SLOT(switchToSearch()));
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
    qDebug()<<"switch working!!!!!!!!!"<<endl;
    ui->stackedWidget->setCurrentWidget(workingWidget);
    workingWidget->raise();
}

void MainWidget::addDbUser(){
    qDebug()<<"start test!"<<endl;
    User *test = new User();
    test->id = tr("111");
    test->name= tr("Test User");
    test->department = tr("Test Dep");
    test->pin = tr("123455");
    test->cardNo = tr("123455");
    test->auth = tr("admin");
    test->face = tr("wait");
    test->plam = tr("wait");
    if(!dbOperator->checkUserIfExists(test->id)){
        dbOperator->insertUser(*test);
        qDebug()<<"added"<<endl;
    }
    else{
        dbOperator->updateUser(*test);
        qDebug()<<"updated"<<endl;
    }

}

void MainWidget::queryDbUser(){
    ui->stackedWidget->setCurrentWidget(userListForm);
}

void MainWidget::switchToRegister(){
    emit resetRegisterForm();
    ui->stackedWidget->setCurrentWidget(registerForm);
}

void MainWidget::switchToSearch(){
    ui->stackedWidget->setCurrentWidget(searchForm);
}
