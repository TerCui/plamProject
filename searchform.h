#ifndef SEARCHFORM_H
#define SEARCHFORM_H

#include <QWidget>
#include <QTimer>
#include <QDateTime>
#include "dboperator.h"

namespace Ui {
class SearchForm;
}

class SearchForm : public QWidget
{
    Q_OBJECT

public:
    explicit SearchForm(QWidget *parent = nullptr);
    ~SearchForm();
public slots:
    void searchTimerCount(void);

private:
    Ui::SearchForm *ui;
    QTimer *timer;
    DbOperator *dbOperator;
};

#endif // SEARCHFORM_H
