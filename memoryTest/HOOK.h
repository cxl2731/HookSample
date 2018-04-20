#include "BaseHead.h"
#define _GetAdessFucntion(CodeAdress,val)  (CodeAdress+val+5)
#define _GetValFuncntion(AdressFunction,CodeAdress) (AdressFunction-CodeAdress-5)
#define _CALL 0xe8
#define DEBUG(); __asm int 3


//取函数地址 ->new
//fn ->jmp 地址
int _getFunctionAdress(void * fn)
{

	int fnA = (int)fn;
	int v1 = *(int*)(((char*)fn) + 1);
	int v2 = _GetAdessFucntion(fnA, v1);//A函数地址
	return v2;
}
//取函数机器码 ->new 
//codeAdress->代码地址   functionAdress->函数地址
int _getFunctionJiQiCode(int codeAdress, int functionAdress)
{
	return _GetValFuncntion(functionAdress, codeAdress);
}


//取现有函数机器码
int _getFunctionValAdress(void *fn)
{
	return (int)((char *)fn + 1);
}


//写内存
//A->edi B->esi C->count
SIZE_T* _WriteProcessMemory(void *A, void *B, int C)
{
	SIZE_T *R = (SIZE_T *)malloc(4);
	WriteProcessMemory(GetCurrentProcess(), A, B, C, (SIZE_T *)R);
	return R;
}

//分配内存
//v1 -> size
LPVOID _VirtualAlloc(DWORD v1)
{
	return VirtualAlloc(NULL, v1, MEM_COMMIT, PAGE_READWRITE);
}

//a->hook地址  d->数据
void _setFunctionHOOK(void *a, void *d)
{
	char *_a = (char *)a, *_d = (char *)d;


	char *v5 = ((char *)_a);//edi
	char *v4 = _d;
	for (size_t i = 0; i < 5; i++)
	{
		//esi v4
		//edi v5
		//*v5 = *v4;

		_WriteProcessMemory(v5, v4, 1);
		v4++;
		v5++;
	}
}
char *aD;
//a->esi   b->edi   
void  _BeginHOOK(void *a, void *b)
{
	int v2 = _getFunctionAdress(a);

	//申请5字节内存
	aD = (char *)_VirtualAlloc(10);//两个call

	//保存5字节
	char *v3 = (char *)v2;//循环保存5字节的指针
	char *vv = aD;//edi
	for (size_t i = 0; i < 5; i++)
	{
		//esi aD
		//edi v3
		*vv = *v3;
		vv++;
		v3++;
	}
	//5字节修改为call b
	char *v4 = (char *)malloc(5); //esi
	*v4 = _CALL;
	*((int *)(v4 + 1)) = _getFunctionJiQiCode((int)v2, _getFunctionAdress(b));
	char *v5 = ((char *)v2);//edi
	_setFunctionHOOK(v5, v4);



}


void  _EndHOOK(void *a)
{
	int v2 = *(int *)a;//A函数地址

	char *v3 = (char *)v2;
	char *v4 = (char *)aD;
	//还原5字节
	for (size_t i = 0; i < 5; i++)
	{
		//esi v4
		//edi v3
		//*v3 = *v4;
		_WriteProcessMemory(v3, v4, 1);
		v4++;
		v3++;
	}
}


//
void _HOOK()
{


}