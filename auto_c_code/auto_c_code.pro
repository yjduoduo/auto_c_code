#-------------------------------------------------
#
# Project created by QtCreator 2014-05-16T15:20:22
#
#-------------------------------------------------

QT       += core gui

TARGET = auto_c_code
TEMPLATE = app


SOURCES += main.cpp\
        autoccode.cpp \
    sqlite-3080403/sqlite3.c

HEADERS  += autoccode.h \
    sqlite-3080403/sqlite3ext.h \
    sqlite-3080403/sqlite3.h

FORMS    += autoccode.ui


INCLUDEPATH +=\
./sqlite-3080403

