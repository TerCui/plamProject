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
public slots:
    void regTimerCount(void);
signals:
    void backWorking();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void resetForm();

private:
    Ui::RegisterForm *ui;
    QTimer *timer;
    DbOperator *dbOperator;
};

#endif // REGISTERFORM_H
