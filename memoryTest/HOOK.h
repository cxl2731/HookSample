#include "BaseHead.h"
#define _GetAdessFucntion(CodeAdress,val)  (CodeAdress+val+5)
#define _GetValFuncntion(AdressFunction,CodeAdress) (AdressFunction-CodeAdress-5)
#define _CALL 0xe8
#define DEBUG(); __asm int 3


//ȡ������ַ ->new
//fn ->jmp ��ַ
int _getFunctionAdress(void * fn)
{

	int fnA = (int)fn;
	int v1 = *(int*)(((char*)fn) + 1);
	int v2 = _GetAdessFucntion(fnA, v1);//A������ַ
	return v2;
}
//ȡ���������� ->new 
//codeAdress->�����ַ   functionAdress->������ַ
int _getFunctionJiQiCode(int codeAdress, int functionAdress)
{
	return _GetValFuncntion(functionAdress, codeAdress);
}


//ȡ���к���������
int _getFunctionValAdress(void *fn)
{
	return (int)((char *)fn + 1);
}


//д�ڴ�
//A->edi B->esi C->count
SIZE_T* _WriteProcessMemory(void *A, void *B, int C)
{
	SIZE_T *R = (SIZE_T *)malloc(4);
	WriteProcessMemory(GetCurrentProcess(), A, B, C, (SIZE_T *)R);
	return R;
}

//�����ڴ�
//v1 -> size
LPVOID _VirtualAlloc(DWORD v1)
{
	return VirtualAlloc(NULL, v1, MEM_COMMIT, PAGE_READWRITE);
}

//a->hook��ַ  d->����
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

	//����5�ֽ��ڴ�
	aD = (char *)_VirtualAlloc(10);//����call

	//����5�ֽ�
	char *v3 = (char *)v2;//ѭ������5�ֽڵ�ָ��
	char *vv = aD;//edi
	for (size_t i = 0; i < 5; i++)
	{
		//esi aD
		//edi v3
		*vv = *v3;
		vv++;
		v3++;
	}
	//5�ֽ��޸�Ϊcall b
	char *v4 = (char *)malloc(5); //esi
	*v4 = _CALL;
	*((int *)(v4 + 1)) = _getFunctionJiQiCode((int)v2, _getFunctionAdress(b));
	char *v5 = ((char *)v2);//edi
	_setFunctionHOOK(v5, v4);



}


void  _EndHOOK(void *a)
{
	int v2 = *(int *)a;//A������ַ

	char *v3 = (char *)v2;
	char *v4 = (char *)aD;
	//��ԭ5�ֽ�
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