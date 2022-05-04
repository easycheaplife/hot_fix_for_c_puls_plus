#include <iostream>
#include "app.h"
#include "../hot_fix/hot_fix.h"

using namespace std;

// 定义要热更新的函数
int fix_func()
{
    cout << "before fix_func addr : " << (void*)&need_fix_func << endl;
    cout << "after  fix_func addr : " << (void*)&fix_func <<endl;
    
    cout << "load new fix function" << endl;
    // fix here
    int times = 3;
    for (int i = 0; i < times; i++)
    {
        cout << "after fix cur times " << i << endl;
    }
    return 0;
}

// 定义替换的函数和更新后的函数
FIXTABLE fix_table = {(void *)&fix_func, (void *)&need_fix_func};
