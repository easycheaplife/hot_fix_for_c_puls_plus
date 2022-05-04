#include <iostream>
#include <signal.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "hot_fix.h"
using namespace std;

static int fix_func(const void* new_func, void *old_func) 
{
    cout << "begin fix func " << endl;

    //跳转指令
    char prefix[] = {'\x48', '\xb8'};   //MOV new_func %rax
    char postfix[] = {'\xff', '\xe0'};  //JMP %rax

    //开启代码可写权限
    size_t page_size= getpagesize();
    const int inst_len = sizeof(prefix) + sizeof(void *) + sizeof(postfix);
    char *align_point = (char *)old_func - ((uint64_t)(char *)old_func % page_size);
    if (0 != mprotect(align_point, (char *)old_func - align_point + inst_len, PROT_READ | PROT_WRITE | PROT_EXEC)) {
        return -1;
    }

    //将跳转指令写入原函数开头
    memcpy(old_func, prefix, sizeof(prefix));
    memcpy((char *)old_func + sizeof(prefix), &new_func, sizeof(void *));
    memcpy((char *)old_func + sizeof(prefix) + sizeof(void *), postfix, sizeof(postfix));

    //关闭代码可写权限
    if (0 != mprotect(align_point, (char *)old_func - align_point + inst_len, PROT_READ | PROT_EXEC)) {
        return -1;
    }
    return 0;
}

static void do_fix(int signum)
{
    cout << "do fix" << endl;

    //1. 调用dlopen加载so库
    char patch_patch[] = "./patch.so";
    void *lib = dlopen(patch_patch, RTLD_NOW);
    if (NULL == lib)
    {
        cout << "dlopen failed , patch " << patch_patch << " error "<< dlerror() << endl;
        return;
    }

    // 2. 查找函数符号表并且替换
    FIXTABLE *fix_item = (FIXTABLE *)dlsym(lib, "fix_table");
    if (NULL == fix_item) 
    {
        cout << "fix symbol failed" << endl;
        dlclose(lib);
        return;
    }
    
    void * result = dlopen(NULL, RTLD_NOW);
    if (NULL == result) 
    {
        cout << "result is null" << endl;
        dlclose(lib);
        return;
    }

    // 3. 执行更新
    int ret = fix_func(fix_item->new_func, fix_item->old_func);
    cout << "fix result ret " << ret << endl;
    return;
}

int init_hot_fix_signal() 
{
    if (signal(SIGUSR1, do_fix) == SIG_ERR) 
    {
        return -1;
    }
    return 0;
}
