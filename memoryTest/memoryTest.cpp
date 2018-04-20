// memoryTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "HOOK.h"
#include "windows.h"




//原本执行函数
void a()
{
	printf("a\n");
}

//挂钩后执行函数
__declspec(naked) void b()
{

	void *v1;
	__asm
	{
		sub[esp], 5
			mov[v1], esp
	}



	printf("b\n");

	//MessageBoxA(0,"666","666",0);


	_EndHOOK(v1);
	
	__asm
	{
		ret
	}
}

void c()
{
	printf("c\n");
}



int _tmain(int argc, _TCHAR* argv[])
{
	_BeginHOOK(a, b);
	a();
	_BeginHOOK(c, b);
	c();

	getchar();
	return 0;
}

