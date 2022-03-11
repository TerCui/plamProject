#ifndef PASSEDFORM_H
#define PASSEDFORM_H

#include <QWidget>
#include <QTimer>
#include <QDateTime>

namespace Ui {
class PassedForm;
}

class PassedForm : public QWidget
{
    Q_OBJECT

public:
    explicit PassedForm(QWidget *parent = nullptr);
    ~PassedForm();
public slots:
    void passTimerCount(void);
    void resetPassTimer();
signals:
    void backWorking();

private:
    Ui::PassedForm *ui;
    int  timeEnd;
    QTimer *timer;
};

#endif // PASSEDFORM_H
