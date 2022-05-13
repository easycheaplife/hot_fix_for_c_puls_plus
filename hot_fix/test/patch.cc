#include "../hot_fix/hot_fix.h"

using namespace std;

/*********************************************************************
 * 方式二:使用修饰后的名称直接替换，无需提前定义需要热更的函数
 ********************************************************************/
extern "C"
{
	int _Z13need_fix_funcv()
	{
		cout << "===load new fix function" << endl;
		// fix here
		int times = 3;
		for (int i = 0; i < times; i++)
		{
			cout << "===after fix cur times " << i << endl;
		}
		return 0;
	}
}


