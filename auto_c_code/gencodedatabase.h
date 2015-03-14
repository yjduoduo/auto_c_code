#ifndef GENCODEDATABASE_H
#define GENCODEDATABASE_H

#include "sqlite3.h"
#include "prefix_string.h"
#include "md5.h"
#include <QtGui>
#include <QVector>

#ifndef CODE_TRUE
#define CODE_TRUE TRUE
#endif

#ifndef CODE_FALSE
#define CODE_FALSE FALSE
#endif

#define STRSELF(A) #A

#define DB_NAME "all.db"

#if 0
enum LanguageType{
    languagetype_Err_,
    languagetype_C_ ,
    languagetype_Cpp_,
    languagetype_Qt_,
    languagetype_Python_,
    languagetype_Shell_,
    languagetype_Jave_,
    languagetype_Oracle_,
    languagetype_Object_C_,
    languagetype_Qtquick_,
    languagetype_Php_,
    languagetype_Mysql_,
    languagetype_Sqlite3_,
    languagetype_JavaScript_,
    languagetype_Debug_,

    languagetype_Aspect_
};
#else
typedef struct{
    unsigned char con[16];
}LanguageType;
#endif

typedef struct{
    const char *name;
}CodeSet;



typedef struct
{
#if 0
    LanguageType langtype;
#else
    LanguageType langtype;
#endif
    char name[64];
    char talbename[64];
    char databasename[64];
    char creat_table_express[512];
    char *insert_table_express;
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
    void InitCodeSets(); /* initial code sets*/
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
    qint8 addcodesets(codestructSets &elm);//添加内容到集合
    void getLanguageErr(LanguageType langtype);//language err type
    LanguageType getLanguageType(QString &name);
    qint8 IsLanguageTypeInScope(LanguageType &type);
    LanguageType getLanguageAspect();
    qint8 IsLanguageTypeSame(LanguageType lan1,LanguageType lan2);
    void GetLanList(QStringList &strlist);//显示语言列表
protected:
    codestructSets get_table_sets_bytype(LanguageType type);
    const char * get_tablename_bytype(LanguageType type);
    codestructSets getDefaultcodestructSets();

protected:
    QVector<codestructSets> codesets;
    Md5 md5;
};

#endif // GENCODEDATABASE_H
