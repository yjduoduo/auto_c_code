#-------------------------------------------------
#
# Project created by QtCreator 2014-05-16T15:20:22
#
#-------------------------------------------------

QT       += core gui
QT       += widgets
QT       += network
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
    sqlite-autoconf/sqlite3.c \
    codeEditor/codeeditor.cpp \
    toolstabwidget.cpp \
    uithread.cpp \
    debugsets.cpp \
    globalapplication.cpp \
    udppkg.cpp \
    sockthread.cpp \
    msgtipsautoshut.cpp \
    calender/setbirthday.cpp \
    calender/qrc_resource.cpp \
    calender/displaybutton.cpp \
    calender/clock.cpp \
    calender/window_calender.cpp \
    netthings.cpp \
    codesophia.cpp

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
    sqlite-autoconf/sqlite3.h \
    codeEditor/linenumberarea.h \
    codeEditor/codeeditor.h \
    toolstabwidget.h \
    uithread.h \
    debugsets.h \
    globalapplication.h \
    udppkg.h \
    sockthread.h \
    msgtipsautoshut.h \
    calender/setbirthday.h \
    calender/displaybutton.h \
    calender/clock.h \
    calender/window_calender.h \
    zeromq/include/zmq_utils.h \
    zeromq/include/zmq.h \
    netthings.h \
    codesophia.h \
    comon.h

FORMS    += autoccode.ui \
    PushDbdialog.ui \
    dialog_select_database.ui \
    autoindb.ui \
    setup1.ui \
    toolstabwidget.ui \
    udppkgdialog.ui \
    codetools.ui \
    codesophia.ui


INCLUDEPATH +=\
./sqlite-autoconf\
./codeEditor\
./calender\
./zeromq\include \
./zeromq\

LIBS += -lpthread
LIBS += -L$$PWD/zeromq/lib  -lzmq

RC_FILE = autoapp.rc

RESOURCES += \
    imgsrc.qrc

OTHER_FILES += \
    qxtglobalshortcut/qxtglobalshortcut.pri \
    zeromq/lib/libzmq.dll

TRANSLATIONS += autoccode.ts


#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/zeromq/lib/ -llibzmq
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/zeromq/lib/ -llibzmqd
#else:symbian: LIBS += -llibzmq
#else:unix: LIBS += -L$$PWD/zeromq/lib/ -llibzmq

INCLUDEPATH += $$PWD/zeromq/lib
DEPENDPATH += $$PWD/zeromq/lib
