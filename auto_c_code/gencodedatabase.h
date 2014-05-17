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
    languagetype_Err_
};

typedef struct
{
    LanguageType type;
    const char *talbename;
    const char *databasename;
    const char *creat_table_express;
    const char *insert_table_express;
}codestructSets;


typedef struct
{
    QString content;
    LanguageType languageType;
    QString keyword;
    QString note;
}InsertCon;



class GenCodeDatabase
{
public:
    GenCodeDatabase();
    void creatable(InsertCon *cont);
    void inserttable(InsertCon *cont );
    int opendatabase(const char *databases_name, const char *createtableexpress);
    int insertdatabase(const char *databases_name,
                                      char *inserttableexpress);
    QString getLanguageStr(LanguageType type);
protected:
    codestructSets* get_table_bytype(LanguageType type);
};

#endif // GENCODEDATABASE_H
