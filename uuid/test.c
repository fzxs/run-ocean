
/* uuid生成 */

#include <stdio.h>
#include <stdlib.h>  /* calloc()函数头文件 */
#include <assert.h>
#include "uuid.h"

void test()
{
    uuid_t uuid;
    char *pcOut = NULL;

    /*
    设计说明:
        uuid一般是36个字节
    */

    //1.分配内存
    pcOut = (char *)calloc(37, sizeof(char));
    assert(pcOut);

    //2.创建uuid
    uuid_generate(uuid);

    //3.转化成字符串
    uuid_unparse(uuid, pcOut);

    //4.打印uuid
    printf("====uuid [%s]====\n", pcOut);

    //5.释放内存
    free(pcOut);
    pcOut = NULL;

}

int main()
{
    test();
    return 0;
}


