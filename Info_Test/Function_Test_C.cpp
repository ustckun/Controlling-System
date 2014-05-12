//using the library of PvAPI.lib before compiling this program
/*
Copyright (C) 2014 Chenkun Wang
University of Science and Technology of China
All Rights Reserved.
*/
//This program is used to test the I/O of AVT Camera and read out the Camera information.
#include <stdio.h>
#include <windows.h>
//#include <stdlib.h>
#include <string>
//#include <stdlib>
#include "PvAPI.h"

using namespace std;

int main()
{
	string String_test;
	char Char_test;
	char *Char_point_test;
	tPvCameraInfoEx Tested_Camera;
	tPvHandle handle_test;
	unsigned long Camera_number;
	if(PvInitialize() != ePvErrSuccess)
    { 
		printf("PvInitialize err!\n");
	}
	else
	{
		Camera_number=PvCameraListEx(&Tested_Camera,1,NULL,sizeof(tPvCameraInfoEx));
		PvCameraOpen(Tested_Camera.UniqueId,ePvAccessMaster,&handle_test);
		PvCaptureStart(handle_test);
		if((PvAttrEnumSet(handle_test,"FrameSrartTriggerMode","Software")!=ePvErrSuccess)||
		(PvAttrEnumSet(handle_test,"AcquisitionMode","Continuous") != ePvErrSuccess) ||
		(PvCommandRun(handle_test,"AcquisitionStart") != ePvErrSuccess))
		{
			printf("Software Source failed!\n");
		}
		//This place is used to capture by software triggers
		
		printf("Software Trigger Set Done! Please check the demo.\n");
		printf("If you have checked, press \"s\"to continue...\n");
		while(getchar()=='s')
			Sleep(500);

		//
		if((PvAttrEnumSet(handle_test,"FrameStartTriggerMode","Freerun") != ePvErrSuccess) ||
		(PvAttrEnumSet(handle_test,"AcquisitionMode","Continuous") != ePvErrSuccess) ||
		(PvCommandRun(handle_test,"AcquisitionStart") != ePvErrSuccess))
		{
			printf("Setting failed!!!!!\n");
			printf("Version of this structure: %d\n",Tested_Camera.StructVer);
			printf("UniqueID: %u\n",Tested_Camera.UniqueId);
			printf("Camera name: %s\n",Tested_Camera.CameraName);
			printf("Model name: %s\n",Tested_Camera.ModelName);
			PvCaptureQueueClear(handle_test);
			// stop driver capture stream
			PvCaptureEnd(handle_test);
			return false;
		}
		printf("Waiting for the triggers!!!\n");
		//This place is used to capture the frame ans waiting for triggers


		printf("Hardware continue trigger Set Done! Please check the demo.\n");
		printf("If you have checked, press \"s\"to continue...\n");
		while(getchar()=='s')
			Sleep(500);


		//
		PvCommandRun(handle_test,"AcquisitionStop");
		Sleep(200);
		printf("Calling PvCaptureQueueClear...\n");
		if (PvCaptureQueueClear(handle_test) != ePvErrSuccess)
			printf("PvCaptureQueueClear err!\n");
		else
			printf("...Queue cleared.\n");  

		//stop driver stream
		if (PvCaptureEnd(handle_test) != ePvErrSuccess)
			printf("PvCaptureEnd err!\n");
		else
			printf("Driver stream stopped.\n");
		PvCameraClose(handle_test);
		printf("Camera has been closed!\n");
		handle_test=NULL;
	}
	PvUnInitialize();
}