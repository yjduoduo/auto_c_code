#if 0

/*  md5 test case */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

int main(int argc, char *argv[])
{
    MD5_CTX md5;
    Md5 obj;
    obj.MD5Init(&md5);
    int i;
    unsigned char encrypt[] ="admin";//21232f297a57a5a743894a0e4a801fc3
    unsigned char decrypt[16];
    obj.MD5Update(&md5,encrypt,strlen((char *)encrypt));
    obj.MD5Final(&md5,decrypt);
    printf("加密前:%s\t:加密后:\n",encrypt);
    for(i=0;i<16;i++)
    {
        printf("%02x",decrypt[i]);
    }

    getchar();

    return 0;
}

#endif
