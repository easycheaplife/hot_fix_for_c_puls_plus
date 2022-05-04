#include <iostream>
#include "app.h"
#include "../hot_fix/hot_fix.h"
using namespace std;

int main()
{
    init_hot_fix_signal();

    business_logic();

    return 0;
}
