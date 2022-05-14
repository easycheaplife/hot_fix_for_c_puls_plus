#include <iostream>
#include <unistd.h>
#include "app.h"
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
	FixClass fix_class;
    while(1)
    {
        sleep(2);
        need_fix_func();
		fix_class.fix_member_fun(99);
    }
    return 0;
}

FixClass::FixClass()
{

}

FixClass::~FixClass()
{

}

int FixClass::fix_member_fun(int param)
{
    cout << "before fix_member_fun addr : " << &FixClass::fix_member_fun << " param:"<< param << endl;
	return 0;
}
