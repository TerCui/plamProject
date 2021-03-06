#ifndef USERLISTFORM_H
#define USERLISTFORM_H

#include <QWidget>
#include <QTimer>
#include <QDateTime>
#include "dboperator.h"
#include <QCheckBox>

namespace Ui {
class UserListForm;
}

class UserListForm : public QWidget
{
    Q_OBJECT

public:
    explicit UserListForm(QWidget *parent = nullptr);
    ~UserListForm();
public slots:
    void userListTimerCount(void);

signals:
    void backWorking();
    void toSearch();
    void resetSearchForm();
    void editUser(QString id);


private slots:
    void on_pushButton_delete_clicked();

    void on_pushButton_clicked();

    void on_pushButton_search_clicked();

    void searchUsers(QString);
    void ProcessSelect();

private:
    Ui::UserListForm *ui;
    QTimer *timer;
    DbOperator *dbOperator;
    void initData(QString id);
};

#endif // USERLISTFORM_H
