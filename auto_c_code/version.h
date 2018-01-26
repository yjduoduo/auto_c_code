#ifndef VERSION_H
#define VERSION_H

#include "sqlite3.h"
//发布正式版，定义它
#if 1
#define RELEASE_VERSION
#else
#define DEBUG_V
#endif

const char version_autoccode[]="v2.8.1\n(sqlite:"SQLITE_VERSION")";

#endif // VERSION_H
