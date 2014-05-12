// CameraInfo.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "PvAPI.h"

extern "C" __declspec(dllexport)char CameraInfo();

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	return TRUE;
}

char CameraInfo()
{
	tPvCameraInfoEx List[5];
	unsigned long numCamera;
	numCamera=PvCameraListEx(List,5,NULL,sizeof(tPvCameraInfoEx));
	if(numCamera==1)
		return List[0].CameraName[6];
	else
		return List[0].ModelName[6];
}
