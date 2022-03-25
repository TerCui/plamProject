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
signals:
    void backUserList();
    void searchResult(QString id);
public slots:
    void searchTimerCount(void);
    void resetForm();

private slots:
    void on_backButton_clicked();

    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

private:
    Ui::SearchForm *ui;
    QTimer *timer;
    DbOperator *dbOperator;
};

#endif // SEARCHFORM_H
