
/* 命令行参数解析 */

#include <stdio.h>
#include <stdlib.h>  /* calloc()函数头文件 */
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>

/*
知识补充:
    A.命令行参数说明
        命令行参数可以分为两类，一类是短选项，一类是长选项，短选项在参数前加一杠"-"，长选项在参数前连续加两杠"--"，
        例如 -a，-A,-b都表示短选项，--all,--almost-all, --author都表示长选项。
        他们两者后面都可选择性添加额外参数。例如--block-size=SIZE，SIZE便是额外的参数。

    B.getopt_long函数介绍

        函数原型
        int getopt_long(int argc, char * const argv[], const char *optstring, const struct option *longopts, int *longindex);

        功能说明:getopt_long函数既可以处理短选项也可以处理长选项

        参数说明
        argc: 同main()函数中的argc
        argv: 同main()函数中的argv
        optstring: 表示短选项字符串。
            形式如“a:b::cd:“，分别表示程序支持的命令行短选项有-a、-b、-c、-d，冒号含义如下：
                (1)只有一个字符，不带冒号——只表示选项， 如-c 
                (2)一个字符，后接一个冒号——表示选项后面带一个参数，如-a 100
                (3)一个字符，后接两个冒号——表示选项后面带一个可选参数，即参数可有可无，如果带参数，则选项与参数直接不能有空格形式应该如-b200
        longopts:表示长选项结构体

        struct option
        {
            const char * name;
            int has_arg;
            int *flag;
            int val;
        };

        eg:
        static struct option longopts[] = {
              { "daemon", no_argument, NULL, 'D' },
              { "dir", required_argument, NULL, 'd' },
              { "out", required_argument, NULL, 'o' },
              { "log", required_argument, NULL, 'l' },
              { "split", required_argument, NULL, 's' },
              { "http-proxy", required_argument, &lopt, 1 },
              { "http-user", required_argument, &lopt, 2 },
              { "http-passwd", required_argument, &lopt, 3 },
              { "http-proxy-user", required_argument, &lopt, 4 },
              { "http-proxy-passwd", required_argument, &lopt, 5 },
              { "http-auth-scheme", required_argument, &lopt, 6 },
              { "version", no_argument, NULL, 'v' },
              { "help", no_argument, NULL, 'h' },
              { 0, 0, 0, 0 }
        };
        name:表示选项的名称,比如daemon,dir,out等。

        has_arg:表示选项后面是否携带参数。
        该参数有三个不同值，如下：
        a: no_argument(或者是0)时，参数后面不跟参数值，eg: --version,--help
        b: required_argument(或者是1)时，参数输入格式为：(参数 值) 或者 (参数=值)。eg:--dir=/home
        c: optional_argument(或者是2)时，参数输入格式只能为：参数=值

        flag:这个参数有两个值，空或者非空。
        a:如果参数为空NULL，那么当选中某个长选项的时候，getopt_long将返回val值。eg，可执行程序 --help，getopt_long的返回值为h.     
        b:如果参数不为空，那么当选中某个长选项的时候，getopt_long将返回0，并且将flag指针参数指向val值。eg: 可执行程序 --http-proxy=127.0.0.1:80 那么getopt_long返回值为0，并且lopt值为1。

        val：表示指定函数找到该选项时的返回值，或者当flag非空时指定flag指向的数据的值val。

        longindex:longindex非空，它指向的变量将记录当前找到参数符合 longopts 里的第几个元素的描述，即是 longopts 的下标值。


        返回值说明:
        (1)如果短选项找到，那么将返回短选项对应的字符。
        (2)如果长选项找到，如果flag为NULL，返回val。如果flag不为空，返回0
        (3)如果遇到一个选项没有在短字符、长字符里面。或者在长字符里面存在二义性的，返回'?'
        (4)如果解析完所有字符没有找到（一般是输入命令参数格式错误，eg： 连斜杠都没有加的选项），返回'-1'
        (5)如果选项需要参数，忘了添加参数。返回值取决于optstring，如果其第一个字符是':'，则返回':'，否则返回'?'

        注意
        longopts 的最后一个元素必须是全0填充，否则会报段错误
        短选项中每个选项都是唯一的。而长选项如果简写，也需要保持唯一性。

    C.全局变量说明
        optarg:表示当前选项对应的参数值。
        optind:表示的是下一个将被处理到的参数在argv中的下标值。
        opterr:如果opterr = 0，在getopt、getopt_long、getopt_long_only遇到错误将不会输出错误信息到标准输出流。opterr在非0时，向屏幕输出错误。
        optopt:表示没有被未标识的选项。
*/

int lopt = 0;
static const struct option long_options[] = {
             { "daemon", no_argument, NULL, 'D' },
             { "log", required_argument, NULL, 'l' },
             { "http-proxy", required_argument, NULL, 'p' },
             { "version", no_argument, NULL, 'V' },
             { "help", no_argument, NULL, 'h' },
             { 0, 0, 0, 0 }
};

static void usage(void)
{
    fprintf(stderr,
        "test [option]... URL\n"
        "  -2                       kill program .\n"
        "  --daemon                 Use GET request daemon.\n"
        "  --log                    Use Log option.\n"
        "  -p|--http-proxy          Use Http Proxy.\n"
        "  -h|--help                This information.\n"
        "  -V|--version             Display program version.\n"
    );
};


void show_version()
{
    printf("====version[1.1.0]====\n");
}

void show_help()
{
    printf("i will help you, but i am not home now . ^_^ \n");
}

void test(int argc, char **argv)
{
    int opt = 0;
    int options_index = 0;

    if (1 == argc)
    {
        usage();
        return;
    }

    while ((opt = getopt_long(argc, argv, "2Vhp:", long_options, &options_index)) != EOF)
    {
        //短选项处理
        switch (opt)
        {
        case  0: 
            break;
        case '2': 
            printf(" i am right now exit .\n");
            break;
        case 'V': 
            show_version();
            break;
        case 'l': 
            printf(" i am open log option [%s].\n", optarg);
            break;
        case 'h':
            show_help();
            break;
        case 'p':
            printf("http proxy is [%s] .\n", optarg);
            break;
        default:
            usage();
            break;
        }
    }

    if (optind < argc)
    {
        usage();
        printf("getopt_long() error !\n");
    }
}

int main(int argc, char *argv[])
{
    test(argc, argv);
    return 0;
}


