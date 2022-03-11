#ifndef NOREGISTEREDFORM_H
#define NOREGISTEREDFORM_H

#include <QWidget>
#include <QTimer>
#include <QDateTime>

namespace Ui {
class noregisteredForm;
}

class NoregisteredForm : public QWidget
{
    Q_OBJECT

public:
    explicit NoregisteredForm(QWidget *parent = nullptr);
    ~NoregisteredForm();
public slots:
    void regTimerCount(void);
    void resetRegTimer();
signals:
    void backWorking();

private:
    Ui::noregisteredForm *ui;
    int  timeEnd;
    QTimer *timer;
};

#endif // NOREGISTEREDFORM_H
