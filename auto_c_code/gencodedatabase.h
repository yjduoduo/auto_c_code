#ifndef GENCODEDATABASE_H
#define GENCODEDATABASE_H

#include "sqlite3.h"
#include "prefix_string.h"
#include <QtGui>

enum LanguageType{
    languagetype_C_,
    languagetypeCpp_,
    languagetype_Qt_,
    languagetype_Python_,
    languagetype_Jave_,
    languagetype_Aspect_,
    languagetype_Err_,
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
    int opendatabase(const char *databases_name, const char *createtableexpress);
    int insertdatabase(const char *databases_name,
                       char *inserttableexpress);
    int selectdatabase(const char *databases_name,
                       char *selecttableexpress,
                       SelectResult &selectres,
                       int aspeactflag);
    QString getLanguageStr(LanguageType type);
protected:
    codestructSets* get_table_sets_bytype(LanguageType type);
    const char * get_tablename_bytype(LanguageType type);
};

#endif // GENCODEDATABASE_H
