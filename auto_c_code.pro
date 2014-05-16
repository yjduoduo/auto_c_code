QT       += core gui
TARGET = targettest
TEMPLATE = app
HEADERS += \
./sqlite-3080403/sqlite3.h\
./sqlite-3080403/sqlite3ext.h
SOURCES += \
./sqlite-3080403/shell.c\
./sqlite-3080403/sqlite3.c
OTHER_FILES += \
./README.md\
./auto_c_code.pro\
./findcandcpptolist.R.exe\
./sqlite-amalgamation-3080403.zip\
./数据库索引规则.xlsx\
./生成库的规则.docx


INCLUDEPATH +=\
./sqlite-3080403
