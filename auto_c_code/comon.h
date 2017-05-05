#ifndef COMON_H
#define COMON_H

#include <QString>


#if 1
//±àÂëºº×Ö
#define str_china(A)     QString::fromLocal8Bit(#A)
//#define str_china(A)     QString::fromUtf8(#A)
#else
#define str_china(A)     codec->toUnicode(#A)
#endif


#endif // COMON_H
