// Function_Set.cpp : Defines the entry point for the DLL application.
//
/*
| ==============================================================================
| Copyright (C) 2006-2011 Allied Vision Technologies.  All Rights Reserved.
|
| This code may be used in part, or in whole for your application development.
|
|==============================================================================
| ==============================================================================
| Copyright (C) 2014-2015 Chenkun Wang.  All Rights Reserved.
|
| This code may be used in part, or in whole for your application development.
|
|==============================================================================
*/
#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "PvApi.h"

using namespace std;

extern "C" __declspec(dllexport)void CameraStart();

extern "C" __declspec(dllexport)void CameraStop();

extern "C" __declspec(dllexport)string CameraModeSet(int EM, int EV, int AM, int FSTM);

extern "C" __declspec(dllexport)string CameraModeGet(char* EM, unsigned long EV, char* AM, char* FSTM);


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}



#define _STDCALL __stdcall

#define FRAMESCOUNT 15

typedef struct 
{
    unsigned long   UID;
    tPvHandle       Handle;
    tPvFrame        Frames[FRAMESCOUNT];
    bool            Abort;

} tCamera;

tCamera GCamera;

bool CameraGet(string SerialNo)
{
    tPvUint32 count,connected;
    tPvCameraInfoEx list;
	count = PvCameraListEx(&list,1,&connected, sizeof(tPvCameraInfoEx));
    if(count == 1)
    {
        GCamera.UID = list.UniqueId;
        SerialNo=list.SerialNumber;
    }
	return true;
}

bool CameraSetup()
{
	bool failed = false;
	unsigned long FrameSize = 0;
	PvCameraOpen(GCamera.UID,ePvAccessMaster,&(GCamera.Handle));
	PvAttrUint32Get(GCamera.Handle,"TotalBytesPerFrame",&FrameSize);
	for(int i=0;i<FRAMESCOUNT && !failed;i++)
	{
		GCamera.Frames[i].ImageBuffer = new char[FrameSize];
		GCamera.Frames[i].ImageBufferSize = FrameSize;
	}
	return !failed;
}

void CameraUnsetup()
{
	PvCameraClose(GCamera.Handle);

    for(int i=0;i<FRAMESCOUNT;i++)
        delete [] (char*)GCamera.Frames[i].ImageBuffer;

    GCamera.Handle = NULL;
}

bool CameraSet()
{
	bool failed = false;
	PvCaptureAdjustPacketSize(GCamera.Handle,8228);
	PvCaptureStart(GCamera.Handle);

	for(int i=0;i<FRAMESCOUNT && !failed;i++)
	{
		PvCaptureQueueFrame(GCamera.Handle,&(GCamera.Frames[i]),NULL);
	}
	if (failed)
		return false;

	return true;
}

void CameraStart()
{
	PvCommandRun(GCamera.Handle,"AcquisitionStart");
}

void CameraStop()
{
	PvCommandRun(GCamera.Handle,"AcquisitionStop");
	Sleep(200);
	PvCaptureQueueClear(GCamera.Handle);
	PvCaptureEnd(GCamera.Handle);
}


string CameraModeSet(int EM, int EV, int AM, int FSTM)
{
	string SerialNo;
	PvInitialize();
	memset(&GCamera,0,sizeof(tCamera));
	CameraGet(SerialNo);
	CameraSetup();
	CameraSet();
	switch(EM)
	{
		case 1: PvAttrEnumSet(GCamera.Handle,"ExposureMode","Manual");
		case 2: PvAttrEnumSet(GCamera.Handle,"ExposureMode","Auto");
		case 3: PvAttrEnumSet(GCamera.Handle,"ExposureMode","AutoOnece");
		case 4: PvAttrEnumSet(GCamera.Handle,"ExposureMode","External");
	}
	PvAttrUint32Set(GCamera.Handle,"ExposureValue",EV);
	switch(AM)
	{
		case 1: PvAttrEnumSet(GCamera.Handle,"AcquisitionMode","Continuous");
		case 2: PvAttrEnumSet(GCamera.Handle,"AcquisitionMode","SingleFrame");
		case 3: PvAttrEnumSet(GCamera.Handle,"AcquisitionMode","MultiFrame");
		case 4: PvAttrEnumSet(GCamera.Handle,"AcquisitionMode","Recorder");
	}
	switch(FSTM)
	{
		case 1: PvAttrEnumSet(GCamera.Handle,"FrameStartTriggerMode","Freerun");
		case 2: PvAttrEnumSet(GCamera.Handle,"FrameStartTriggerMode","SyncIn1");
		case 3: PvAttrEnumSet(GCamera.Handle,"FrameStartTriggerMode","SyncIn2");
		case 4: PvAttrEnumSet(GCamera.Handle,"FrameStartTriggerMode","FixedRate");
		case 5: PvAttrEnumSet(GCamera.Handle,"FrameStartTriggerMode","Software");
	}
	/*This place is used for other functions



	*/
	return SerialNo;
}


string CameraModeGet(char* EM, unsigned long EV, char* AM, char* FSTM)
{
	string SerialNo;
	PvInitialize();
	memset(&GCamera,0,sizeof(tCamera));
	CameraGet(SerialNo);
	CameraSetup();
	CameraSet();
	PvAttrEnumGet(GCamera.Handle,"ExposureMode",EM,NULL,NULL);
	PvAttrUint32Get(GCamera.Handle,"ExposureValue",&EV);
	PvAttrEnumGet(GCamera.Handle,"AcquisitionMode",AM,NULL,NULL);
	PvAttrEnumGet(GCamera.Handle,"FrameStartTriggerMode",FSTM,NULL,NULL);
	/*This place is used for other functions



	*/
	return SerialNo;
}