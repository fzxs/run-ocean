
/* 获取字符编码 */

#include <stdio.h>
#include <stdlib.h>  /* calloc()函数头文件 */
#include <string.h>
#include <assert.h>
#include "uchardet.h"

void test()
{
    uchardet_t ud;
    const char *p = "一开始 金星实际上 和 地球 是姊妹星球 。";
    const char *pcCode = NULL;

    do 
    {
        //1.初始化
        ud = uchardet_new();

        //2.分析样本字符串
        if (uchardet_handle_data(ud, p, strlen(p)))
        {
            printf("---uchardet_handle_data() failed----\n");
            break;
        }

        //3.关闭样本分析
        uchardet_data_end(ud);

        //4.打印当前字符串的编码
        pcCode = uchardet_get_charset(ud);
        if (pcCode)
        {
            printf("------current code is [%s]------------\n", pcCode);
        }
        else
        {
            printf("---uchardet_get_charset() failed----\n");
        }

    } while (0);

    //5.关闭资源
    uchardet_delete(ud);

}

int main()
{
    test();
    return 0;
}


