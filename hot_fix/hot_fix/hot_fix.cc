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

static void* find_func_symbol(const char* patch_patch, const char* symbol_name)
{
    //1. 调用dlopen加载so库
    void *lib = dlopen(patch_patch, RTLD_NOW|RTLD_GLOBAL);
    if (NULL == lib)
    {
        cout << "dlopen failed , patch " << patch_patch << " error "<< dlerror() << endl;
        return nullptr;
    }

    // 2. 查找函数符号表并且替换
    void* address = dlsym(lib, symbol_name);
    if (NULL == address) 
    {
        cout << "fix symbol failed:" << dlerror() << endl;
        dlclose(lib);
        return nullptr;
    }
    
    void * result = dlopen(NULL, RTLD_NOW);
    if (NULL == result) 
    {
        cout << "result is null" << endl;
        dlclose(lib);
        return nullptr;
    }
	return address;
}

static void* symbol_name_to_addr(const char* symbol_name)
{
    void* symbol_addr = dlsym(RTLD_DEFAULT, symbol_name);
	cout << "symbol_name:" << symbol_name << " symbol_addr:" << symbol_addr << endl;
	return symbol_addr;
}

static int fix_func(const void* new_func, void *old_func) 
{
    cout << "begin fix func, new_func: " << new_func << ", old_func:" << old_func << endl;
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
    char patch_path[] = "./patch.so";
    char need_patch_path[] = "./libapp.so";
	char name_mangling[] = "_Z13need_fix_funcv";
	void* fix_fun_address = find_func_symbol(patch_path, name_mangling);
	void* need_fix_fun_address = find_func_symbol(need_patch_path, name_mangling);   
	cout << "fix_fun_address:" << fix_fun_address << "need_fix_fun_address:" << need_fix_fun_address << endl;
	int ret = fix_func(fix_fun_address, need_fix_fun_address);  
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
