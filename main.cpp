#include <QApplication>
#include <QTextCodec>   //添加中文支持2-1
//#include <QtSql>        //数据库

#include "client.h"
//#include "database.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
 //   QTextCodec::setCodecForTr(QTextCodec::codecForLocale());//添加中文支持2-2
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

//    //添加数据库代码
//    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");//添加数据库驱动
//    db.setDatabaseName(":memory:");//数据库连接命名
//    if(!db.open())//打开数据库p
//    {
//        return false;
//    }
//    QSqlQuery query;
//    query.exec("create table student(id int primary key,name varchar)");
    
//    //另外一段数据库代码
//    Database d;
//    d.createConnection();
//    d.queryAll();



//    Widget w;
//    w.show();
    client w1;
    w1.show();

    return a.exec();
}
