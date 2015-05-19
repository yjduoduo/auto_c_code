#ifndef GENCODEDATABASE_H
#define GENCODEDATABASE_H

#include "sqlite3.h"
#include "prefix_string.h"
#include <QtGui>

enum LanguageType{
    languagetype_Err_,
    languagetype_C_ ,
    languagetype_Cpp_,
    languagetype_Android_,
    languagetype_Erlang_,
    languagetype_Qt_,
    languagetype_Python_,
    languagetype_Shell_,
    languagetype_Jave_,
    languagetype_Oracle_,
    languagetype_Qtquick_,
    languagetype_Php_,
    languagetype_Mysql_,
    languagetype_Sqlite3_,
    languagetype_Postgresql_,
    languagetype_JavaScript_,
    languagetype_Debug_,

    languagetype_Aspect_,
};

typedef struct
{
    LanguageType langtype;
    const char *talbename;
    const char *databasename;
    const char *creat_table_express;
    const char *insert_table_express;
}codestructSets;


typedef struct
{
    QString content;
    LanguageType languagetype;
    QString keyword;
    QString note;
    QString vartype;
    QString aspect;//范围
}InsertCon;

typedef struct
{
    QString contentstr;
    QStringList content_list;
    QStringList keyword_list;
    QStringList note_list;
    QStringList vartype_list;
    int existflag;//检测是否存在内容 标志
    QStringList aspect_list;
    QStringList aspect_field;//自定义区域
}SelectResult;

enum aspect{
    ASPECT_HAVE,
    ASPECT_NONE
};




class GenCodeDatabase
{
public:
    GenCodeDatabase();
    void creatable(InsertCon *cont);
    void inserttable(InsertCon *cont );
    void updatetable(LanguageType languagetype,QString &insertexpress);
    int opendatabase(const char *databases_name, const char *createtableexpress);
    int insertdatabase(const char *databases_name,
                       char *inserttableexpress);
    int selectdatabase(const char *databases_name,
                       char *selecttableexpress,
                       SelectResult &selectres,
                       int aspeactflag);
    int searchdatabase(const char *databases_name,
                       char *selecttableexpress,
                       SelectResult &selectres,
                       const QString &searchtext);
    QString getLanguageStr(LanguageType type);
protected:
    codestructSets* get_table_sets_bytype(LanguageType type);
    const char * get_tablename_bytype(LanguageType type);
};

#endif // GENCODEDATABASE_H
