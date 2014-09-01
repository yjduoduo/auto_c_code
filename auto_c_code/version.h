#ifndef VERSION_H
#define VERSION_H

//发布正式版，定义它
#if 2
#define RELEASE_VERSION
#else
#define DEBUG_V
#endif

const char version_autoccode[]="v1.3.2";

#endif // VERSION_H
