// TestFunction.cpp : Defines the entry point for the DLL application.
//

#include "StdAfx.h"

extern "C" __declspec(dllexport)int IntAdd(int a, int b);

extern "C" __declspec(dllexport)int IntMutiply(int d, int f);

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#include "stdio.h"

int IntAdd(int a,int b)
{
	int c=a+b;
	return c;
}

int IntMutiply(int d,int f)
{
	int g=IntAdd(d,f);
	g=g*g;
	return g;
}