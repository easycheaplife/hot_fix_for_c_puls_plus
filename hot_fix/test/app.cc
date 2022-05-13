#include <iostream>
#include <unistd.h>
using namespace std;

// need fix here
int need_fix_func()
{
    cout << "before fix_func addr : " << (void*)&need_fix_func <<endl;

    int times = 5;
    for (int i = 0; i < times; i++) 
    {
        cout << "before fix cur times " << i << endl;
    }
    return 0;
}

int business_logic()
{
    // do something
    while(1)
    {
        sleep(2);
        need_fix_func();
    }
    return 0;
}
