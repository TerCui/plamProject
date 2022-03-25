#include "dboperator.h"

DbOperator::DbOperator()
{
    initDb();
}
void DbOperator::initDb()
{
    QString connection = QString::fromStdString("plam_data");

    /// 创建连接
    if (QSqlDatabase::contains(connection))
    {
        db = QSqlDatabase::database(connection);
    }else {
        QString DBPath = "/data";
        QDir my_dir(DBPath);
        if (!my_dir.exists())
            my_dir.mkpath(DBPath);

        DBPath += "/" + connection + ".db";

        qDebug() << DBPath.toUtf8() << "##########################";
        /// 第二个参数可以设置连接名字，这里为default
        db = QSqlDatabase::addDatabase("QSQLITE", connection);
        /// 设置数据库名与路径
        db.setDatabaseName(DBPath);
    }

    //打开连接
    if (!db.open())
    {
        qDebug() << db.lastError();
        qFatal("Failed to connect.");
    }
}

bool DbOperator::exists(std::string table) {
    try {
        QSqlQuery query(db);
        query.exec(QString::fromStdString(u8"SELECT * FROM " + table));
        return query.next();
    }
    catch (...) {
        return false;
    }
}

bool DbOperator::insertUser(User info)
{
    QSqlQuery qry(db);
    qry.prepare(u8"INSERT INTO user_info (id,name,department,pin,card_no,auth,plam,face)"
                " VALUES (:id,:name,:department,:pin,:cardNo,:auth,:plam,:face)");
    qry.bindValue(":id", QVariant(info.id));
    qry.bindValue(":name", QVariant(info.name));
    qry.bindValue(":department", QVariant(info.department));
    qry.bindValue(":pin", QVariant(info.pin));
    qry.bindValue(":cardNo", QVariant(info.cardNo));
    qry.bindValue(":auth", QVariant(info.auth));
    qry.bindValue(":plam", QVariant(info.plam));
    qry.bindValue(":face", QVariant(info.face));
    if (!qry.exec()) {
        qDebug() << qry.lastError();
        return false;
    }
    else {
        return true;
    }
}

bool DbOperator::updateUser(User info)
{
    QSqlQuery qry(db);
    qry.prepare(u8"update user_info set name = :name,department = :department,pin=:pin,"
                "card_no = :cardNo,auth = :auth,plam = :plam,face = :face where id = :id");
    qry.bindValue(":name", QVariant(info.name));
    qry.bindValue(":department", QVariant(info.department));
    qry.bindValue(":pin", QVariant(info.pin));
    qry.bindValue(":cardNo", QVariant(info.cardNo));
    qry.bindValue(":auth", QVariant(info.auth));
    qry.bindValue(":plam", QVariant(info.plam));
    qry.bindValue(":face", QVariant(info.face));
    qry.bindValue(":id", QVariant(info.id));
    if (!qry.exec()) {
        qDebug() << qry.lastError();
        return false;
    }
    else {
        return true;
    }
}


bool DbOperator::initUserTable()
{
    QSqlQuery query(db);
    return query.exec(QString::fromStdString(u8"create table user_info(id varchar(20) primary key,name varchar(200)"
                                             ",department varchar(200),pin varchar(20),card_no varchar(50),auth varchar(20)"
                                             ",plam varchar(2000),face varchar(2000))"));
}

bool DbOperator::checkUserIfExists(QString eNumber)
{
    if (!exists("user_info"))
    {
//         qDebug() <<QString("not exist!")<<endl;
        initUserTable();
    }
    else {
//        qDebug() <<QString("querying!")<<endl;
        QVariant en(eNumber);
        QSqlQuery qry(db);
        qry.prepare(u8"SELECT * FROM user_info WHERE id=:id");
        qry.bindValue(":id", en);
        if (!qry.exec())
            qDebug() << qry.lastError();
        else if (qry.next())
        {
             qDebug() <<QString("founded!\n")<<endl;
            QSqlRecord rec = qry.record();

            int cols = rec.count();


            for (int c = 0; c < cols; c++)
                qDebug() << QString("Row %1, %2: %3").arg(1).arg(rec.fieldName(c)).arg(qry.value(c).toString());

            return true;
        }
         qDebug() <<QString("not founded!\n")<<endl;
    }
    return false;
}

QList<User> DbOperator::queryAll(){
    QList<User> list;
    if (!exists("user_info"))
    {
        initUserTable();
    }
    else {
         QSqlQuery query(db);
         query.prepare(u8"SELECT * FROM user_info order by id");
         if (!query.exec())
             qDebug() << query.lastError();
         else
         {

             while(query.next())
             {
                 User *user = new User();
                 user->id = query.value(0).toString();
                 user->name = query.value(1).toString();
                 user->department = query.value(2).toString();
                 user->pin = query.value(3).toString();
                 user->cardNo = query.value(4).toString();
                 user->auth = query.value(5).toString();
                 user->plam = query.value(6).toString();
                 user->face = query.value(7).toString();

                 list.append(*user);
             }
         }
    }
    return list;
}

QList<User> DbOperator::queryUsers(QString eNumber)
{
    QList<User> list;
    if (!exists("user_info"))
    {
         qDebug() <<QString("not exist!")<<endl;
        initUserTable();
    }
    else {

        QSqlQuery query(db);
        query.prepare(u8"SELECT * FROM user_info WHERE id like :id");
        query.bindValue(":id", QString("%%1%").arg(eNumber));
        if (!query.exec())
            qDebug() << query.lastError();
        else
        {

            while(query.next())
            {
                User *user = new User();
                user->id = query.value(0).toString();
                user->name = query.value(1).toString();
                user->department = query.value(2).toString();
                user->pin = query.value(3).toString();
                user->cardNo = query.value(4).toString();
                user->auth = query.value(5).toString();
                user->plam = query.value(6).toString();
                user->face = query.value(7).toString();

                list.append(*user);
            }
        }
    }
    return list;
}

User DbOperator::queryUser(QString eNumber)
{
   User *user = new User();
   if (!exists("user_info"))
   {
        qDebug() <<QString("not exist!")<<endl;
       initUserTable();
   }
   else {

       QSqlQuery query(db);
       query.prepare(u8"SELECT * FROM user_info WHERE id = :id");
       query.bindValue(":id", eNumber);
       if (!query.exec())
           qDebug() << query.lastError();
       else
       {

           while(query.next())
           {
               user->id = query.value(0).toString();
               user->name = query.value(1).toString();
               user->department = query.value(2).toString();
               user->pin = query.value(3).toString();
               user->cardNo = query.value(4).toString();
               user->auth = query.value(5).toString();
               user->plam = query.value(6).toString();
               user->face = query.value(7).toString();

           }
       }
   }
   return *user;
}


void DbOperator::deleteUsers(QList<QString> idList){
    if (!exists("user_info"))
    {
        initUserTable();
    }
    else {
        QSqlQuery query(db);
        query.prepare(u8"DELETE FROM user_info where id = :id");
        foreach (QString id, idList) {
            query.bindValue(":id", id);
            if (!query.exec())
                qDebug() << query.lastError();
        }
    }
}
