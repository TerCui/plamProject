#ifndef DBOPERATOR_H
#define DBOPERATOR_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QDir>
#include "user.h"

class DbOperator
{
public:
    DbOperator(std::string connection);
    ~DbOperator();
    /// 插入记录
    bool insertUser(User info);
    /// update记录
    bool updateUser(User info);
    /// 查询是否已录入
    bool checkUserIfExists(QString eNumber);

    QList<User> queryAll();

    void deleteUsers(QList<QString> idList);
private:
    std::string connection_;

    QSqlDatabase db;

    void initDb();
    /// 查询表是否存在
    bool exists(std::string table);
    /// 初始化记录表
    bool initUserTable();
};

#endif // DBOPERATOR_H
