#include "gencodedatabase.h"

#define ARRAY_SIZE(A)  (sizeof(A)/sizeof(A[0]))

#define DB_NAME "all.db"


#define CREATTABLE(A) "CREATE TABLE  "#A \
    "([ID] INTEGER PRIMARY KEY,"\
    "[content] varchar(100),"\
    "[lantype] varchar(100),"\
    "[keywords] varchar(100),"\
    "[note] varchar(100),"\
    "[vartype] varchar(100),"\
    "[aspect_field] varchar(100),"\
    "CreatedTime TimeStamp NOT NULL DEFAULT (datetime('now','localtime')),"\
    "[delflag] integer DEFAULT 0," \
    "[lowercase_keyworks] varchar(100)" \
    ");"

#define CREATTABLE_ASPECT(A) "CREATE TABLE  "#A \
    "([ID] INTEGER PRIMARY KEY,"\
    "[aspect_field] varchar(100),"\
    "CreatedTime TimeStamp NOT NULL DEFAULT (datetime('now','localtime')));"

#define JOIN(A,B) A##B
#define LANTY_JOIN(A) JOIN(languagetype_,A)

#ifdef GENCODEDATABASE_H
codestructSets codesets[]={{LANTY_JOIN(C_),"c_table",DB_NAME,CREATTABLE(c_table),},
                           {LANTY_JOIN(Cpp_),"cpp_table",DB_NAME,CREATTABLE(cpp_table),},
                           {LANTY_JOIN(Qt_),"qt_table",DB_NAME,CREATTABLE(qt_table),},
                           {LANTY_JOIN(Python_),"python_table",DB_NAME,CREATTABLE(python_table),},
                           {LANTY_JOIN(Jave_),"java_table",DB_NAME,CREATTABLE(java_table),},
                           {LANTY_JOIN(Shell_),"shell_table",DB_NAME,CREATTABLE(shell_table),},
                           {LANTY_JOIN(Oracle_),"oracle_table",DB_NAME,CREATTABLE(oracle_table),},
                           {LANTY_JOIN(Qtquick_),"qtquick_table",DB_NAME,CREATTABLE(qtquick_table),},
                           {LANTY_JOIN(Php_),"php_table",DB_NAME,CREATTABLE(php_table),},
                           {LANTY_JOIN(Mysql_),"mysql_table",DB_NAME,CREATTABLE(mysql_table),},
                           {LANTY_JOIN(JavaScript_),"javascript_table",DB_NAME,CREATTABLE(javascript_table),},
                           {LANTY_JOIN(Debug_),"debug_table",DB_NAME,CREATTABLE(debug_table),},
                           {LANTY_JOIN(Sqlite3_),"sqlite3_table",DB_NAME,CREATTABLE(sqlite3_table),},

                           //��Χ,�洢�ķ��������
                           {LANTY_JOIN(Aspect_),"aspect_table",DB_NAME,CREATTABLE_ASPECT(aspect_table),},
                          };
#endif




GenCodeDatabase::GenCodeDatabase()
{
    self_print(GenCodeDatabase);

}
codestructSets* GenCodeDatabase::get_table_sets_bytype(LanguageType type)
{
    for(unsigned i=0;i<ARRAY_SIZE(codesets);i++)
        if(type == codesets[i].langtype)
            return &codesets[i];
    return NULL;
}
const char * GenCodeDatabase::get_tablename_bytype(LanguageType type)
{
    for(unsigned i=0;i<ARRAY_SIZE(codesets);i++)
        if(type == codesets[i].langtype)
            return codesets[i].talbename;
    return NULL;
}
int GenCodeDatabase::opendatabase(const char *databases_name,
                                  const char *createtableexpress)
{
    sqlite3 * db = 0;
    char * pErrMsg = 0;
    int ret = 0;
    // �������ݿ�
    ret = sqlite3_open(databases_name, &db);
    if ( ret != SQLITE_OK ){
        fprintf(stderr, "�޷������ݿ�: %s", sqlite3_errmsg(db));
        return(1);
    }
    //    printf("connect database success!\n");
    str_print(createtableexpress);

    ret=sqlite3_exec( db, createtableexpress, 0, 0, &pErrMsg );

    if ( ret != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", pErrMsg);
        sqlite3_free(pErrMsg);
    }else{
        //        printf("SQL create table SUCC!\n");
    }



    //�ر����ݿ⣬�ͷ��ڴ�
    sqlite3_close(db);



    return(0);
}
int GenCodeDatabase::insertdatabase(const char *databases_name,
                                    char *inserttableexpress)
{
    sqlite3 * db = 0;
    char * pErrMsg = 0;
    int ret = 0;
    // �������ݿ�
    ret = sqlite3_open(databases_name, &db);
    if ( ret != SQLITE_OK ){
        fprintf(stderr, "�޷������ݿ�: %s", sqlite3_errmsg(db));
        return(1);
    }
    //    printf("connect database success!\n");
    str_print(inserttableexpress);

    ret=sqlite3_exec( db, inserttableexpress, 0, 0, &pErrMsg );

    if ( ret != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", pErrMsg);
        sqlite3_free(pErrMsg);
    }else{
        //        printf("SQL insert table SUCC!\n");
    }




    //�ر����ݿ⣬�ͷ��ڴ�
    sqlite3_close(db);



    return(0);
}
int GenCodeDatabase::selectdatabase(const char *databases_name,
                                    char *selecttableexpress,
                                    SelectResult &selectres,int aspeactflag)
{
    sqlite3 * db = 0;
    int result;
    char * pErrMsg = 0;
    int ret = 0;
    int nRow=0, nColumn;
    char **dbResult; //�� char ** ���ͣ�����*��
    int index;
    // �������ݿ�
    ret = sqlite3_open(databases_name, &db);
    if ( ret != SQLITE_OK ){
        fprintf(stderr, "�޷������ݿ�: %s", sqlite3_errmsg(db));
        return(1);
    }
    //    str_print(databases_name);
    //    printf("connect database success!\n");


    //    fprintf(stdout,"express:%s\n",selecttableexpress);
    result = sqlite3_get_table( db, selecttableexpress, &dbResult, &nRow, &nColumn, &pErrMsg );
    if( SQLITE_OK == result )
    {
        //��ѯ�ɹ�

        index = nColumn; //ǰ��˵�� dbResult ǰ���һ���������ֶ����ƣ��� nColumn ������ʼ��������������

        //        printf( "�鵽%d����¼\n", nRow );
        for(  int i = 0; i < nRow ; i++ )

        {
            //           printf( "�� %d ����¼\n", i+1 );
            for( int j = 0 ; j < nColumn; j++ )
            {
                if(ASPECT_HAVE == aspeactflag){
                    selectres.aspect_list <<QString::fromUtf8(dbResult [index]);
                }else{
#if 0
                    //                printf( "�ֶ���:%s  ?> �ֶ�ֵ:%s\n",  dbResult[j], dbResult [index] );
                    if(0==j){
                        selectres.contentstr+= QString::fromUtf8(dbResult [index]);
                        selectres.content_list << QString::fromUtf8(dbResult [index]);
                    }else if(2==j)
                        selectres.keyword_list << QString::fromUtf8(dbResult [index]);
                    else if(3==j)
                        selectres.note_list<< QString::fromUtf8(dbResult [index]);
                    else if(4==j)
                        selectres.vartype_list<< QString::fromUtf8(dbResult [index]);
#else
                    if(0==j){
                        selectres.contentstr+= QString::fromUtf8(dbResult [index]);
                        selectres.content_list << QString::fromUtf8(dbResult [index]);
                    }else if(2==j)
                        selectres.keyword_list << QString::fromUtf8(dbResult [index]);
                    else if(3==j)
                        selectres.note_list<< QString::fromUtf8(dbResult [index]);
                    else if(4==j)
                        selectres.vartype_list<< QString::fromUtf8(dbResult [index]);
                    else if(5 == j){
                        selectres.aspect_field << QString::fromUtf8(dbResult [index]);
                        str_print(QString::fromUtf8(dbResult [index]));
                    }

#endif
                    selectres.existflag = 1;
                }

                ++index;
            }
            //            printf( "-------\n" );
        }

    }

    sqlite3_free_table(dbResult);
    //�ر����ݿ⣬�ͷ��ڴ�
    sqlite3_close(db);


    return(0);
}


int GenCodeDatabase::searchdatabase(const char *databases_name,
                                    char *selecttableexpress,
                                    SelectResult &selectres,
                                    const QString &searchtext)
{
    sqlite3 * db = 0;
    int result;
    char * pErrMsg = 0;
    int ret = 0;
    int nRow=0, nColumn;
    char **dbResult; //�� char ** ���ͣ�����*��
    int index;
    // �������ݿ�
    ret = sqlite3_open(databases_name, &db);
    if ( ret != SQLITE_OK ){
        fprintf(stderr, "�޷������ݿ�: %s", sqlite3_errmsg(db));
        return(1);
    }
    //    str_print(databases_name);
    //    printf("connect database success!\n");


    //    fprintf(stdout,"express:%s\n",selecttableexpress);
    result = sqlite3_get_table( db, selecttableexpress, &dbResult, &nRow, &nColumn, &pErrMsg );
    if( SQLITE_OK == result )
    {
        //��ѯ�ɹ�

        index = nColumn; //ǰ��˵�� dbResult ǰ���һ���������ֶ����ƣ��� nColumn ������ʼ��������������
        int searchflag = 0;
        //        printf( "�鵽%d����¼\n", nRow );
        for(  int i = 0; i < nRow ; i++ )

        {
            //           printf( "�� %d ����¼\n", i+1 );
            for( int j = 0 ; j < nColumn; j++ )
            {//��ѯ˳���
                /* lowercase_keyworks,keywords,content,lantype,note,vartype */
                if(0==j){
                    if(QString::fromUtf8(dbResult [index]).contains(searchtext)){
                        searchflag = 1;
                    }
                }else if(1==j){
                    if(searchflag){
                        selectres.keyword_list << QString::fromUtf8(dbResult [index]);

                    }

                }
                else if(2==j){
                    if(searchflag){
                        selectres.contentstr+= QString::fromUtf8(dbResult [index]);
                        selectres.content_list << QString::fromUtf8(dbResult [index]);
                    }

                }
                else if(4==j){
                    if(searchflag)
                        selectres.note_list<< QString::fromUtf8(dbResult [index]);
                }
                else if(5==j){
                    if(searchflag)
                        selectres.vartype_list<< QString::fromUtf8(dbResult [index]);
                }

                if(nColumn-1 == j)
                    searchflag = 0;

                selectres.existflag = 1;


                ++index;
            }
            //            printf( "-------\n" );
        }

    }

    sqlite3_free_table(dbResult);
    //�ر����ݿ⣬�ͷ��ڴ�
    sqlite3_close(db);


    return(0);
}



void GenCodeDatabase::creatable(InsertCon *cont)
{
    self_print(creatable);
    codestructSets *sets = get_table_sets_bytype(cont->languagetype);
    if(!sets)
        return;
    //    self_print(tablename);
    str_print(sets->talbename);
    switch(cont->languagetype)
    {
    case    languagetype_C_:
    case    languagetype_Cpp_:
    case    languagetype_Qt_:
    case    languagetype_Python_:
    case    languagetype_Jave_:
    case    languagetype_Shell_:
    case    languagetype_Aspect_:
    case    languagetype_Oracle_:
    case    languagetype_Qtquick_:
    case    languagetype_Php_:
    case    languagetype_Mysql_:
    case    languagetype_Sqlite3_:
    case    languagetype_JavaScript_:
    case    languagetype_Debug_:

        opendatabase(sets->databasename,sets->creat_table_express);
    default:
        break;
    }

}

void GenCodeDatabase::inserttable(InsertCon *cont)
{
    self_print(creatable);
    codestructSets *sets = get_table_sets_bytype(cont->languagetype);
    if(!sets)
        return;
    QString langtype = getLanguageStr(cont->languagetype);
    str_print(langtype);
    QString insertexpress;
    insertexpress.clear();

    QString content ;
    QString keyword ;
    QString note    ;
    QString vartype ;
    QString aspect ;
    content.clear();
    keyword.clear();
    note.clear();
    vartype.clear();
    aspect.clear();

    if(languagetype_Aspect_ == cont->languagetype)
    {
        aspect = cont->aspect.replace("\'","\'\'");
        insertexpress = QString("insert into %1([aspect_field])  VALUES('%2');")
                .arg(sets->talbename)
                .arg(aspect);
    }else{
        content = cont->content.replace("\'","\'\'");
        keyword =  cont->keyword.replace("\'","\'\'");
        note    =  cont->note.replace("\'","\'\'");
        vartype    =  cont->vartype.replace("\'","\'\'");
        aspect = cont->aspect.replace("\'","\'\'");

#if 0
        insertexpress = QString("insert into %1([content],[lantype] ,[keywords] ,[note] ,[vartype], [aspect_field])  VALUES('%2','%3','%4','%5','%6','%7')")
                .arg(sets->talbename)
                .arg(content)
                .arg(langtype)
                .arg(keyword)
                .arg(note)
                .arg(vartype)
                .arg(aspect);
#endif
        QString tempstr;
        tempstr.clear();

        insertexpress += "insert into ";
        insertexpress += sets->talbename;
        insertexpress += "([content],[lantype] ,[keywords] ,[note] ,[vartype], [aspect_field], [lowercase_keyworks])  VALUES(";

        tempstr = QString("'%1',").arg(content);
        insertexpress += tempstr;

        tempstr = QString("'%1',").arg(langtype);
        insertexpress += tempstr;

        tempstr = QString("'%1',").arg(keyword);
        insertexpress += tempstr;

        tempstr = QString("'%1',").arg(note);
        insertexpress += tempstr;

        tempstr = QString("'%1',").arg(vartype);
        insertexpress += tempstr;

        tempstr = QString("'%1',").arg(aspect);
        insertexpress += tempstr;

        tempstr = QString("'%1');").arg(keyword.toLower());
        insertexpress += tempstr;

        str_print(tempstr);
        str_print(cont->content);

    }



    str_print(insertexpress);
    str_print(sets->talbename);

    switch(cont->languagetype)
    {
    case    languagetype_C_:
    case    languagetype_Cpp_:
    case    languagetype_Qt_:
    case    languagetype_Python_:
    case    languagetype_Jave_:
    case    languagetype_Shell_:
    case    languagetype_Aspect_:
    case    languagetype_Oracle_:
    case    languagetype_Qtquick_:
    case    languagetype_Php_:
    case    languagetype_Mysql_:
    case    languagetype_Sqlite3_:
    case    languagetype_JavaScript_:
    case    languagetype_Debug_:

        insertdatabase(sets->databasename,insertexpress.toUtf8().data());
        break;

    default:
        break;
    }

}
void GenCodeDatabase::updatetable(LanguageType languagetype,QString &insertexpress)
{
    self_print(updatetable);
    codestructSets *sets = get_table_sets_bytype(languagetype);
    if(!sets)
        return;
    str_print(insertexpress);
    str_print(sets->talbename);

    switch(languagetype)
    {
    case    languagetype_C_:
    case    languagetype_Cpp_:
    case    languagetype_Qt_:
    case    languagetype_Python_:
    case    languagetype_Jave_:
    case    languagetype_Shell_:
    case    languagetype_Aspect_:
    case    languagetype_Oracle_:
    case    languagetype_Qtquick_:
    case    languagetype_Php_:
    case    languagetype_Mysql_:
    case    languagetype_Sqlite3_:
    case    languagetype_JavaScript_:
    case    languagetype_Debug_:

        insertdatabase(sets->databasename,insertexpress.toUtf8().data());
        break;

    default:
        break;
    }

}


QString GenCodeDatabase::getLanguageStr(LanguageType type)
{
    switch(type)
    {
    case languagetype_C_:
        return "C";
    case languagetype_Qt_:
        return "Qt";
    case languagetype_Python_:
        return "Python";
    case languagetype_Shell_:
        return "shell";
    case languagetype_Jave_:
        return "Jave";
    case languagetype_Cpp_:
        return "C++";
    case languagetype_Oracle_:
        return "Oracle";
    case    languagetype_Qtquick_:
        return "Qtquick";
    case    languagetype_Php_:
        return "Php";
    case    languagetype_Mysql_:
        return "Mysql";
    case    languagetype_Sqlite3_:
        return "Sqlite3";
    case    languagetype_JavaScript_:
        return "JavaScript";
    case    languagetype_Debug_:
        return "Debug";

    default:
        return "Err";
    }
}
