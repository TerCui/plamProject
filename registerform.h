#ifndef REGISTERFORM_H
#define REGISTERFORM_H

#include <QWidget>
#include <QTimer>
#include <QDateTime>
#include "dboperator.h"

namespace Ui {
class RegisterForm;
}

class RegisterForm : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterForm(QWidget *parent = nullptr);
    ~RegisterForm();
    bool isDigitStr(QString src);
public slots:
    void regTimerCount(void);
signals:
    void backWorking();
    void backSearch(QString id);
    void startRegCamera(QString id);


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void resetForm();
    void userData(QString);

    void on_pushButton_plam_clicked();

private:
    Ui::RegisterForm *ui;
    QTimer *timer;
    DbOperator *dbOperator;
    bool updateUser;
    User *user;

    bool addUser();

};

#endif // REGISTERFORM_H
