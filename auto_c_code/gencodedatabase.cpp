#include "gencodedatabase.h"


#define ARRAY_SIZE(A)  (sizeof(A)/sizeof(A[0]))

#define DB_NAME "all.db"

#if 0
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
#else
void CREATTABLE(const char *name,char *buf,unsigned int len)
{
    memset(buf,0,len);
    snprintf(buf,len,"CREATE TABLE  %s"
    "([ID] INTEGER PRIMARY KEY,"
    "[content] varchar(100),"
    "[lantype] varchar(100),"
    "[keywords] varchar(100),"
    "[note] varchar(100),"
    "[vartype] varchar(100),"
    "[aspect_field] varchar(100),"
    "CreatedTime TimeStamp NOT NULL DEFAULT (datetime('now','localtime')),"
    "[delflag] integer DEFAULT 0,"
    "[lowercase_keyworks] varchar(100)"
    ");",name);

}
#endif



#if 0
#define CREATTABLE_ASPECT(A) "CREATE TABLE  "#A \
    "([ID] INTEGER PRIMARY KEY,"\
    "[aspect_field] varchar(100),"\
    "CreatedTime TimeStamp NOT NULL DEFAULT (datetime('now','localtime')));"
#else
void CREATTABLE_ASPECT(const char *name,char *buf,unsigned int len)
{
    memset(buf,0,len);
    snprintf(buf,len,"CREATE TABLE  %s([ID] INTEGER PRIMARY KEY,"
             "[aspect_field] varchar(100),"
             "CreatedTime TimeStamp NOT NULL DEFAULT (datetime('now','localtime')));",name);
}

#endif

#define JOIN(A,B) A##B
#define LANTY_JOIN(A) JOIN(languagetype_,A)


/*  最初始的名称集合，暂时只支持英文 */
CodeSet namesets[]={STRSELF(C),
                    STRSELF(CPP),
                    STRSELF(Debug),
                    STRSELF(Erlang),
                    STRSELF(Java),
                    STRSELF(JavaScript),
                    STRSELF(Mysql),
                    STRSELF(Object_C),
                    STRSELF(Oracle),
                    STRSELF(Python),
                    STRSELF(Qt),
                    STRSELF(Qtquick),
                    STRSELF(Php),
                    STRSELF(Shell),
                    STRSELF(Sqlite3_3),
                    STRSELF(AAA),

                    STRSELF(Aspect),
                   };





GenCodeDatabase::GenCodeDatabase()
{
    self_print(GenCodeDatabase);
    codesets.clear();
    InitCodeSets();

}
void GenCodeDatabase::InitCodeSets()
{
    codestructSets set;
    char buf[512];
    int len = 0;

    for(unsigned i=0;i<ARRAY_SIZE(namesets);i++)
    {
        memset(&set,0,sizeof(codestructSets));
        memset(buf,0,sizeof(buf));
        /* name */
        len = sizeof(set.name) < strlen(namesets[i].name)?sizeof(set.name):strlen(namesets[i].name);
        memcpy(set.name,QString::fromLocal8Bit(namesets[i].name).toLower().toUtf8().data(),len);
        //        set.name = QString::fromLocal8Bit(namesets[i].name);
        /* table name*/
        snprintf(buf,sizeof(buf),"%s_table",namesets[i].name);
        memcpy(set.talbename,QString::fromLocal8Bit(buf).toLower().toLocal8Bit().data(),sizeof(set.talbename));
        /* language type */
        snprintf(buf,sizeof(buf),"languagetype_%s",namesets[i].name);
        md5.Md5Sum((unsigned char*)buf,set.langtype.con);
        /* databasename */
        memcpy(set.databasename,QString::fromLocal8Bit(DB_NAME).toLower().toLocal8Bit().data(),sizeof(set.databasename));
        /* create table express */
        CREATTABLE(set.talbename,
                          set.creat_table_express,
                          sizeof(set.creat_table_express));
        //////qDebug()<< "add list :=========="<<endl;
        //////qDebug()<< "name          :"<<set.name;
        //////qDebug()<< "tablename     :" <<set.talbename ;
        //        //////qDebug()<< "langu type    :";
        //        for(int i=0;i<16;i++)
        //        {
        //            //////qDebug() << set.langtype.con[i];
        ////            fprintf(stdout,"%02x",set.langtype.con[i]);
        //        }
        qDebug()<< "create express:"<<set.creat_table_express ;
        //////qDebug()<< "databasename  :"<<set.databasename ;
        addcodesets(set);
    }
}

codestructSets GenCodeDatabase::get_table_sets_bytype(LanguageType type)
{
    int len = sizeof(LanguageType);
    int succnum = 0;
    //    //////qDebug() << "LanguageType len::"<<len;
    for(unsigned i=0;i<codesets.count();i++)
    {
        succnum = 0;
        for(int j=0;j<16;j++)
        {
            if(type.con,codesets.at(i).langtype.con[j] == type.con[j])
            {
                succnum++;
            }

        }
        //        //////qDebug() << "succnum ::"<<succnum;
        if(16 == succnum)
            return (codesets.at(i));
    }
    //////qDebug() << "getDefaultcodestructSets ::";
    return getDefaultcodestructSets();
}
const char * GenCodeDatabase::get_tablename_bytype(LanguageType type)
{
    for(unsigned i=0;i<codesets.count();i++)
        if(!memcmp(&type,&codesets.at(i).langtype,sizeof(LanguageType)))
            return codesets[i].talbename;
    return NULL;
}
int GenCodeDatabase::opendatabase(const char *databases_name,
                                  const char *createtableexpress)
{
    sqlite3 * db = 0;
    char * pErrMsg = 0;
    int ret = 0;
    //////qDebug() << "databases_name:"<<databases_name;
    // 连接数据库
    ret = sqlite3_open(databases_name, &db);
    if ( ret != SQLITE_OK ){
        fprintf(stderr, "无法打开数据库: %s", sqlite3_errmsg(db));
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



    //关闭数据库，释放内存
    sqlite3_close(db);



    return(0);
}
int GenCodeDatabase::insertdatabase(const char *databases_name,
                                    char *inserttableexpress)
{
    sqlite3 * db = 0;
    char * pErrMsg = 0;
    int ret = 0;
    //////qDebug() << "databases_name:"<<databases_name;
    // 连接数据库
    ret = sqlite3_open(databases_name, &db);
    if ( ret != SQLITE_OK ){
        fprintf(stderr, "无法打开数据库: %s", sqlite3_errmsg(db));
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




    //关闭数据库，释放内存
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
    char **dbResult; //是 char ** 类型，两个*号
    int index;
    ////qDebug() << "databases_name:"<<databases_name;
    // 连接数据库
    ret = sqlite3_open(databases_name, &db);
    if ( ret != SQLITE_OK ){
        fprintf(stderr, "无法打开数据库: %s", sqlite3_errmsg(db));
        return(1);
    }
    //    str_print(databases_name);
    //    printf("connect database success!\n");


    //    fprintf(stdout,"express:%s\n",selecttableexpress);
    result = sqlite3_get_table( db, selecttableexpress, &dbResult, &nRow, &nColumn, &pErrMsg );
    if( SQLITE_OK == result )
    {
        //查询成功

        index = nColumn; //前面说过 dbResult 前面第一行数据是字段名称，从 nColumn 索引开始才是真正的数据

        //        printf( "查到%d条记录\n", nRow );
        for(  int i = 0; i < nRow ; i++ )

        {
            //           printf( "第 %d 条记录\n", i+1 );
            for( int j = 0 ; j < nColumn; j++ )
            {
                if(ASPECT_HAVE == aspeactflag){
                    selectres.aspect_list <<QString::fromUtf8(dbResult [index]);
                }else{
#if 0
                    //                printf( "字段名:%s  ?> 字段值:%s\n",  dbResult[j], dbResult [index] );
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
    //关闭数据库，释放内存
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
    char **dbResult; //是 char ** 类型，两个*号
    int index;
    ////qDebug() << "databases_name:"<<databases_name;
    // 连接数据库
    ret = sqlite3_open(databases_name, &db);
    if ( ret != SQLITE_OK ){
        fprintf(stderr, "无法打开数据库: %s", sqlite3_errmsg(db));
        return(1);
    }
    //    str_print(databases_name);
    //    printf("connect database success!\n");


    //    fprintf(stdout,"express:%s\n",selecttableexpress);
    result = sqlite3_get_table( db, selecttableexpress, &dbResult, &nRow, &nColumn, &pErrMsg );
    if( SQLITE_OK == result )
    {
        //查询成功

        index = nColumn; //前面说过 dbResult 前面第一行数据是字段名称，从 nColumn 索引开始才是真正的数据
        int searchflag = 0;
        //        printf( "查到%d条记录\n", nRow );
        for(  int i = 0; i < nRow ; i++ )

        {
            //           printf( "第 %d 条记录\n", i+1 );
            for( int j = 0 ; j < nColumn; j++ )
            {//查询顺序表
                /* lowercase_keyworks,keywords,content,lantype,note,vartype */
                if(0==j){
                    if(QString::fromUtf8(dbResult [index]).contains(searchtext)){
                        searchflag = 1;
                    }
                }else if(1==j){
                    if(searchflag){
                        selectres.keyword_list << QString::fromUtf8(dbResult [index]);

                    }
                    else if(QString::fromUtf8(dbResult [index]).contains(searchtext)){
                        searchflag = 1;
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
    //关闭数据库，释放内存
    sqlite3_close(db);


    return(0);
}



void GenCodeDatabase::creatable(InsertCon *cont)
{
    self_print(creatable);
    codestructSets sets = get_table_sets_bytype(cont->languagetype);
    if(!memcmp(getDefaultcodestructSets().name,sets.name,sizeof(sets.name)))
    {
        ////qDebug() << "bad err sets!!!!!";
        return;
    }
    //    self_print(tablename);
    str_print(sets.talbename);
    if(CODE_TRUE == IsLanguageTypeInScope(cont->languagetype))
    {
        opendatabase(sets.databasename,sets.creat_table_express);
    }


    //    switch(cont->languagetype)
    //    {
    //    case    languagetype_C_:
    //    case    languagetype_Cpp_:
    //    case    languagetype_Qt_:
    //    case    languagetype_Python_:
    //    case    languagetype_Jave_:
    //    case    languagetype_Shell_:
    //    case    languagetype_Aspect_:
    //    case    languagetype_Oracle_:
    //    case    languagetype_Object_C_:
    //    case    languagetype_Qtquick_:
    //    case    languagetype_Php_:
    //    case    languagetype_Mysql_:
    //    case    languagetype_Sqlite3_:
    //    case    languagetype_JavaScript_:
    //    case    languagetype_Debug_:

    //        opendatabase(sets.databasename,sets.creat_table_express);
    //    default:
    //        break;
    //    }

}

void GenCodeDatabase::inserttable(InsertCon *cont)
{
    self_print(creatable);
    codestructSets sets = get_table_sets_bytype(cont->languagetype);
    if(!memcmp(getDefaultcodestructSets().name,sets.name,sizeof(sets.name)))
    {
        ////qDebug() << "bad err sets!!!!!";
        return;
    }
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

    //    if(getLanguageAspect() == cont->languagetype)
    if(IsLanguageTypeSame(getLanguageAspect(), cont->languagetype))
    {
        aspect = cont->aspect.replace("\'","\'\'");
        insertexpress = QString("insert into %1([aspect_field])  VALUES('%2');")
                .arg(sets.talbename)
                .arg(aspect);
    }else{
        content = cont->content.replace("\'","\'\'");
        keyword =  cont->keyword.replace("\'","\'\'");
        note    =  cont->note.replace("\'","\'\'");
        vartype    =  cont->vartype.replace("\'","\'\'");
        aspect = cont->aspect.replace("\'","\'\'");

#if 0
        insertexpress = QString("insert into %1([content],[lantype] ,[keywords] ,[note] ,[vartype], [aspect_field])  VALUES('%2','%3','%4','%5','%6','%7')")
                .arg(sets.talbename)
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
        insertexpress += sets.talbename;
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
    str_print(sets.talbename);

    if(CODE_TRUE == IsLanguageTypeInScope(cont->languagetype))
    {
        insertdatabase(sets.databasename,insertexpress.toUtf8().data());
    }

    //    switch(cont->languagetype)
    //    {
    //    case    languagetype_C_:
    //    case    languagetype_Cpp_:
    //    case    languagetype_Qt_:
    //    case    languagetype_Python_:
    //    case    languagetype_Jave_:
    //    case    languagetype_Shell_:
    //    case    languagetype_Aspect_:
    //    case    languagetype_Oracle_:
    //    case    languagetype_Object_C_:
    //    case    languagetype_Qtquick_:
    //    case    languagetype_Php_:
    //    case    languagetype_Mysql_:
    //    case    languagetype_Sqlite3_:
    //    case    languagetype_JavaScript_:
    //    case    languagetype_Debug_:


    //        break;

    //    default:
    //        break;
    //    }

}
void GenCodeDatabase::updatetable(LanguageType languagetype,QString &insertexpress)
{
    self_print(updatetable);
    codestructSets sets = get_table_sets_bytype(languagetype);
    if(!memcmp(getDefaultcodestructSets().name,sets.name,sizeof(sets.name)))
    {
        ////qDebug() << "bad err sets!!!!!";
        return;
    }
    str_print(insertexpress);
    str_print(sets.talbename);

    if(CODE_TRUE == IsLanguageTypeInScope(languagetype))
    {
        insertdatabase(sets.databasename,insertexpress.toUtf8().data());
    }

    //    switch(languagetype)
    //    {
    //    case    languagetype_C_:
    //    case    languagetype_Cpp_:
    //    case    languagetype_Qt_:
    //    case    languagetype_Python_:
    //    case    languagetype_Jave_:
    //    case    languagetype_Shell_:
    //    case    languagetype_Aspect_:
    //    case    languagetype_Oracle_:
    //    case    languagetype_Object_C_:
    //    case    languagetype_Qtquick_:
    //    case    languagetype_Php_:
    //    case    languagetype_Mysql_:
    //    case    languagetype_Sqlite3_:
    //    case    languagetype_JavaScript_:
    //    case    languagetype_Debug_:

    //        insertdatabase(sets.databasename,insertexpress.toUtf8().data());
    //        break;

    //    default:
    //        break;
    //    }

}


QString GenCodeDatabase::getLanguageStr(LanguageType type)
{

    for(int i=0;i<codesets.count();i++)
    {
        //        if(codesets[i].talbename == name)
        if(!memcmp(&type, &codesets.at(i).langtype,sizeof(LanguageType)))
            return codesets.at(i).name;
    }
    return "ERR";

    //    switch(type)
    //    {
    //    case languagetype_C_:
    //        return "C";
    //    case languagetype_Qt_:
    //        return "Qt";
    //    case languagetype_Python_:
    //        return "Python";
    //    case languagetype_Shell_:
    //        return "shell";
    //    case languagetype_Jave_:
    //        return "Jave";
    //    case languagetype_Cpp_:
    //        return "C++";
    //    case languagetype_Oracle_:
    //        return "Oracle";
    //    case    languagetype_Object_C_:
    //        return "Object_C";
    //    case    languagetype_Qtquick_:
    //        return "Qtquick";
    //    case    languagetype_Php_:
    //        return "Php";
    //    case    languagetype_Mysql_:
    //        return "Mysql";
    //    case    languagetype_Sqlite3_:
    //        return "Sqlite3";
    //    case    languagetype_JavaScript_:
    //        return "JavaScript";
    //    case    languagetype_Debug_:
    //        return "Debug";

    //    default:
    //        return "Err";
    //    }
}

qint8 GenCodeDatabase::addcodesets(codestructSets &elm)
{

    codesets.append(elm);
}
void GenCodeDatabase::getLanguageErr(LanguageType langtype)
{
    //    md5.Md5Sum((unsigned char *)"languagetype_Err",langtype.con);
    unsigned char temp[16];/*=986b7da7792a03ac8b9fe6b587ff6332;*/
    temp[0] = 0x98;
    temp[1] = 0x6b;
    temp[2] = 0x7d;
    temp[3] = 0xa7;
    temp[4] = 0x79;
    temp[5] = 0x2a;
    temp[6] = 0x03;
    temp[7] = 0xac;
    temp[8] = 0x8b;
    temp[9] = 0x9f;
    temp[10] = 0xe6;
    temp[11] = 0xb5;
    temp[12] = 0x87;
    temp[13] = 0xff;
    temp[14] = 0x63;
    temp[15] = 0x32;
    memcpy(langtype.con,temp,sizeof(temp));
}

LanguageType GenCodeDatabase::getLanguageType(QString &name)
{
    LanguageType lantype;
    getLanguageErr(lantype);
    //////qDebug() << "==========>>>>>>>>>>>";
    for(int i=0;i<codesets.count();i++)
    {
        //////qDebug() << "codesets.at(i).name:"<<codesets.at(i).name;
        //////qDebug() << "codesets.at(i).len:"<<strlen(codesets.at(i).name);
        if(name.toLower() == QString::fromLocal8Bit(codesets.at(i).name))
        {
            //////qDebug()<< "igotit::!" << name.toLower();
            return codesets[i].langtype;
        }
    }
    //////qDebug()<<"Err Language Type!!!!!!";
    return lantype;
}

codestructSets GenCodeDatabase::getDefaultcodestructSets()
{
    codestructSets set;
    memset(&set,0,sizeof(codestructSets));
    return set;
}

qint8 GenCodeDatabase::IsLanguageTypeInScope(LanguageType &type)
{
    for(int i=0;i<codesets.count();i++)
    {
        if(!memcmp(&type,&codesets.at(i).langtype,sizeof(LanguageType)))
            return CODE_TRUE;
    }
    return CODE_FALSE;
}

LanguageType GenCodeDatabase::getLanguageAspect()
{
    QString str;
    str.clear();
    str = STRSELF(Aspect);
    return getLanguageType(str);
}

qint8 GenCodeDatabase::IsLanguageTypeSame(LanguageType lan1,LanguageType lan2)
{
    if(!memcmp(&lan1,&lan2,sizeof(LanguageType)))
        return CODE_TRUE;

    return CODE_FALSE;
}

void GenCodeDatabase::GetLanList(QStringList &strlist)
{
    for(unsigned i=0;i<ARRAY_SIZE(namesets);i++)
    {
        if(QString("Aspect") == QString(namesets[i].name))
            continue;
        strlist<< namesets[i].name;
    }
}
