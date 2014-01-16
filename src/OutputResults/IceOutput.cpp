/*
 * IceOutput.cpp
 * 
 * - implement interface functions to output results from Ice objects
 * 
 *   Public:
 *     void SetupIceOutputFile()
 *     void UpdateIceFile(double utcTime, double ATime)
 * 
 * Copyright 2013 - A.Pereira
 */


#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

#include "outputResults.h"

//---------------------------------------------------------------------------
void OutputResults::SetupIceFile()
{
     // builds header file if ice exist
    if (iceFile) {
    	TReadWrite* MyPReadWrite = new TReadWrite(MyFileIce);
	    if (!MyPReadWrite->SetupFile(WRITENEWFILE, false)) {
#ifdef __BORLANDC__
			MessageBox(0, "Output Ice file error.\nPlease reinitialise model again",
                   MyFileIce, MB_OK);
#else  // __BORLANDC__
            cerr << "Output Ice file error: "<< MyFileIce
				<< ".\nPlease select output file again" << endl;
#endif  // __BORLANDC__
            return;
        }

        MyPReadWrite->WriteCell("Time(UTC)");
        MyPReadWrite->WriteSeparator();
        MyPReadWrite->WriteCell("Time(hours)");
        MyPReadWrite->WriteSeparator();
        MyPReadWrite->WriteCell("TimeStep");
        MyPReadWrite->WriteSeparator();
        MyPReadWrite->WriteCell("GridColumn");
        MyPReadWrite->WriteSeparator();
        MyPReadWrite->WriteCell("GridLine");
        MyPReadWrite->WriteSeparator();
        MyPReadWrite->WriteCell("IceGridLayer");
        MyPReadWrite->WriteSeparator();
        MyPReadWrite->WriteCell("BoxNumber");

        for (int i = 0; i < myRegister->defaultsRecord->nrVariablesSelectedFileOutput; i++)
        {
            if (IsIceVariable(myRegister->defaultsRecord->SelectedVariablesFileOutput[i]))
            {
                MyPReadWrite->WriteSeparator();
                MyPReadWrite->WriteCell(myRegister->defaultsRecord->SelectedVariablesFileOutput[i]);
            }
        }
        MyPReadWrite->WriteSeparator(true);
        MyPReadWrite->CloseFile();
    }
}
//---------------------------------------------------------------------------
void OutputResults::UpdateIceFile(double utcTime, double ATime)
{
    int MyLayer, MyColumn, MyLine, Layers;

    if (!iceFile)
        return;

    TReadWrite* MyPReadWrite = new TReadWrite(MyFileIce);
	if (!MyPReadWrite->SetupFile(WRITEAPPENDFILE)) {
#ifdef __BORLANDC__
		MessageBox(0, "Update Ice file error.\nPlease reinitialise model again",
                   MyFileIce, MB_OK);
#else  // __BORLANDC__
        cerr << "Update Ice file error: "<< MyFileIce
				<< ".\nPlease select output file again" << endl;
#endif  // __BORLANDC__
        return;
    }

    for (int c = 0; c < myRegister->defaultsRecord->nrClassesSelected; c++)
    {
		Layers = myRegister->varsRecord[c].nrIceLayers;
		if (Layers != 0)
			break;
    }
    int n = myRegister->MyPEcoDynClass->GetNumberOfColumns() 
			* myRegister->MyPEcoDynClass->GetNumberOfLines() * Layers;

    if (pBoxFileQueue->size() > 0)
        n = pBoxFileQueue->size();

    for (int j = 0; j < n; j++)
    {
		int boxNumber = j;
		if (pBoxFileQueue->size() > 0) {
			RegBox* pBox = (RegBox*)pBoxFileQueue->RetrieveObject(j);
			boxNumber = pBox->number;
		}

		MyLayer = myRegister->MyPEcoDynClass->GetLayerIndex(boxNumber);
		MyColumn = myRegister->MyPEcoDynClass->GetColumnIndex3D(boxNumber, MyLayer);
		MyLine = myRegister->MyPEcoDynClass->GetLineIndex3D(boxNumber, MyLayer);

		MyPReadWrite->WriteCell(utcTime,0);
		MyPReadWrite->WriteSeparator();

		MyPReadWrite->WriteCell(ATime,6);
		MyPReadWrite->WriteSeparator();

		MyPReadWrite->WriteCell(MyNumberOfRecordedTimeSteps);
		MyPReadWrite->WriteSeparator();

		MyPReadWrite->WriteCell(MyColumn + 1);
		MyPReadWrite->WriteSeparator();

		MyPReadWrite->WriteCell(myRegister->MyPEcoDynClass->GetNumberOfLines() - MyLine);
		MyPReadWrite->WriteSeparator();

		MyPReadWrite->WriteCell(Layers - MyLayer);
		MyPReadWrite->WriteSeparator();
       
		MyPReadWrite->WriteCell(boxNumber);	

		for (int v = 0; v < myRegister->defaultsRecord->nrVariablesSelectedFileOutput; v++)
		{
			bool found = false;
	        char* variableName = myRegister->defaultsRecord->SelectedVariablesFileOutput[v];
            for (int i = 0; (i < myRegister->defaultsRecord->nrClassesSelected) && !found; i++)
            {
                if (!IsIceVariable(&myRegister->varsRecord[i], variableName))
					continue;

                myRegister->varsRecord[i].ObjectPointer->Inquiry((char*)NULL,
									  myRegister->varsRecord[i].Value,
									  boxNumber,
									  variableName,
									  SAVEDISKCODE);
                MyPReadWrite->WriteSeparator();
                MyPReadWrite->WriteCell(myRegister->varsRecord[i].Value,8);
                found = true;
                break;
            }
        }
		MyPReadWrite->WriteSeparator(true);
    }
    MyPReadWrite->CloseFile();
}
//---------------------------------------------------------------------------
