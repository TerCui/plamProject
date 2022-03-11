#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "workingwidget.h"
#include "noregisteredform.h"
#include "passedform.h"
#include "warnform.h"
#include "dboperator.h"
#include "user.h"
#include "registerform.h"
#include "userlistform.h"
#include "searchform.h"

namespace Ui {
class mainwidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

public slots:
    void switchToWorking();
    void switchToNoReg();
    void switchToPass();
    void switchToWarn();
    void switchToRegister();
    void addDbUser();
    void queryDbUser();
    void switchToSearch();

signals:
    void resetPassCount();
    void resetRegCount();
    void resetWarnCount();
    void resetRegisterForm();

private:
    Ui::mainwidget *ui;
    WorkingWidget * workingWidget;
    NoregisteredForm * noregisteredForm;
    PassedForm *passedForm;
    WarnForm *warnForm;
    DbOperator *dbOperator;
    RegisterForm * registerForm;
    UserListForm * userListForm;
    SearchForm * searchForm;
};

#endif // MAINWIDGET_H
