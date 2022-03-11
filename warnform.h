#ifndef WARNFORM_H
#define WARNFORM_H

#include <QWidget>
#include <QTimer>
#include <QDateTime>

namespace Ui {
class WarnForm;
}

class WarnForm : public QWidget
{
    Q_OBJECT

public:
    explicit WarnForm(QWidget *parent = nullptr);
    ~WarnForm();
public slots:
    void warnTimerCount(void);
    void resetWarnTimer();
signals:
    void backWorking();

private:
    Ui::WarnForm *ui;
    int  timeEnd;
    QTimer *timer;
};

#endif // WARNFORM_H
