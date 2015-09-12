#-------------------------------------------------
#
# Project created by QtCreator 2014-05-16T15:20:22
#
#-------------------------------------------------

QT       += core gui
QT       += widgets
TARGET = auto_c_code
TEMPLATE = app


SOURCES += main.cpp\
        autoccode.cpp \
    prefix_string.cpp \
    gencodedatabase.cpp \
    dialog_select_database.cpp \
    qxtglobalshortcut/qxtglobalshortcut_win.cpp \
    qxtglobalshortcut/qxtglobalshortcut.cpp \
    autoindb.cpp \
    sqlite-autoconf/sqlite3.c

HEADERS  += autoccode.h \
    prefix_string.h \
    version.h \
    gencodedatabase.h \
    dialog_select_database.h \
    qxtglobalshortcut/qxtglobalshortcut_p.h \
    qxtglobalshortcut/qxtglobalshortcut.h \
    qxtglobalshortcut/qxtglobal.h \
    autoindb.h \
    sqlite-autoconf/sqlite3ext.h \
    sqlite-autoconf/sqlite3.h

FORMS    += autoccode.ui \
    PushDbdialog.ui \
    dialog_select_database.ui \
    autoindb.ui \
    setup1.ui


INCLUDEPATH +=\
./sqlite-autoconf

RC_FILE = autoapp.rc

RESOURCES += \
    imgsrc.qrc

OTHER_FILES += \
    qxtglobalshortcut/qxtglobalshortcut.pri

