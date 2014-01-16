// Macrophyte object CPP code file
// @ Pedro Duarte, Outubro de 2005

/**
 * NPI work
 * 	08 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <stdio.h>
	#include <vcl.h>
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__

#include "ecodyn.rh"
#include "macrobjt.h"
#include "iodll.h"


TSaltMarshForcing::TSaltMarshForcing(TEcoDynClass* APEcoDynClass, char* className): TMacrophytes(APEcoDynClass, className)
{
        NumberOfInterfaceBoxes = 0;
        BoxArray = MyPEcoDynClass->BoxArray;
        CriticalWaterDepth = MyPEcoDynClass->GetCriticalDepth();
        //Debugger(NumberOfBoxesWithBenthicSpecies);
        for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
        {
                if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "SaltMarsh") == 0)
                        NumberOfInterfaceBoxes++;

        }
        SaltMarshInterfaceBoxesRecord = new BenthicSpeciesRecord[NumberOfInterfaceBoxes];
        int AIndex = 0;
        for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
        {
                if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "SaltMarsh") == 0)
                {
                        strcpy(SaltMarshInterfaceBoxesRecord[AIndex].SpeciesName, MyBenthicSpeciesRecord[i].SpeciesName);
                        SaltMarshInterfaceBoxesRecord[AIndex].LineCoordinate = MyBenthicSpeciesRecord[i].LineCoordinate;
                        SaltMarshInterfaceBoxesRecord[AIndex].ColumnCoordinate = MyBenthicSpeciesRecord[i].ColumnCoordinate;
                        AIndex++;
                }
        }
        OrganicRelease = new double[NumberOfInterfaceBoxes];
        NitrateConsumption = new double[NumberOfInterfaceBoxes];
        AmmoniumRelease = new double[NumberOfInterfaceBoxes];
        for (int i = 0; i < NumberOfInterfaceBoxes; i++) {
           OrganicRelease[i] = 0.0; NitrateConsumption[i] = 0.0; AmmoniumRelease[i] = 0.0;
        }
	ParameterCount = 0;
	// Read in the parameters
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("SaltMarsh");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "SaltMarsh parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TSaltMarshForcing::TSaltMarshForcing - parameters file missing." << endl;
#endif  // __BORLANDC__
	}
    else {

        double MyValue;
        int X,Y;
        if (PReadWrite->FindString("SaltMarsh", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "OrganicRelease") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);   /*mg / m2 / s*/
                                    Debugger(MyValue);
                    for (int j = 0; j < NumberOfInterfaceBoxes; j++)
                        OrganicRelease[j] = MyValue;
                }
                else if (strcmp(MyParameter, "NitrateConsumption") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);   //micro mol N / m3 / s
                                    Debugger(MyValue);
                    for (int j = 0; j < NumberOfInterfaceBoxes; j++)
                        NitrateConsumption[j] = MyValue;
                }
                else if (strcmp(MyParameter, "AmmoniumRelease") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);   //micro mol N / m3 / s
                                    Debugger(MyValue);
                    for (int j = 0; j < NumberOfInterfaceBoxes; j++)
                        AmmoniumRelease[j] = MyValue;
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object parameters - SaltMarshForcing", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TSaltMarshForcing::TSaltMarshForcing - Parameters: undefined object SaltMarshForcing" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TSaltMarshForcing::~TSaltMarshForcing()
{
    freeMemory();
}

void TSaltMarshForcing::freeMemory()
{
    try {
       if (NumberOfInterfaceBoxes > 0)
       {
          delete [] OrganicRelease;
          delete [] NitrateConsumption;
          delete [] AmmoniumRelease;
          delete [] SaltMarshInterfaceBoxesRecord;
       }
       if (NumberOfParameters > 0) {
          delete [] ParametersNameArray;
          NumberOfParameters = 0;
       }
    }
#ifdef __BORLANDC__
    catch (EAccessViolation &e) {
        //Application->ShowException(&e);
    }
    catch (Exception &exception) {
        //Application->ShowException(&exception);
    }
#else  // __BORLANDC__
		catch (exception &exc) {
				cerr << "TSaltMarshForcing::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

void TSaltMarshForcing::Integrate()
{

}


double TSaltMarshForcing::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "OrganicRelease") == 0)
    {
        value = OrganicRelease[0];
    }
    else if (strcmp(MyParameter, "NitrateConsumption") == 0)
    {
        value = NitrateConsumption[0];
    }
    else if (strcmp(MyParameter, "AmmoniumRelease") == 0)
    {
        value = AmmoniumRelease[0];
    }
    else
        value = 0.0;
    return value;
}

bool TSaltMarshForcing::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int j;

    if (strcmp(MyParameter, "OrganicRelease") == 0)
    {
        for (j = 0; j < NumberOfInterfaceBoxes; j++)
            OrganicRelease[j] = value;
    }
    else if (strcmp(MyParameter, "NitrateConsumption") == 0)
    {
        for (j = 0; j < NumberOfInterfaceBoxes; j++)
            NitrateConsumption[j] = value;
    }
    else if (strcmp(MyParameter, "AmmoniumRelease") == 0)
    {
        for (j = 0; j < NumberOfInterfaceBoxes; j++)
            AmmoniumRelease[j] = value;
    }
    else
        rc = false;

    return rc;
}

// AP, 2007.07.13
bool TSaltMarshForcing::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("SaltMarsh");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("SaltMarsh");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfParameters, 0);
    /*
     * save parameters' names and values
     */
    for (int i = 0; i < NumberOfParameters; i++)
    {
        if (i > 0) {
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ParametersNameArray[i]);
        PReadWrite->WriteSeparator();

        if (strcmp(ParametersNameArray[i], "OrganicRelease") == 0)
        {
            PReadWrite->WriteCell(OrganicRelease[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "NitrateConsumption") == 0)
        {
            PReadWrite->WriteCell(NitrateConsumption[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "AmmoniumRelease") == 0)
        {
            PReadWrite->WriteCell(AmmoniumRelease[0], 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}

void TSaltMarshForcing::Go()
{
      int MyBox; double ABoxDepth;
   TEcoDynClass/** MySedimentPointer = MyPEcoDynClass->GetSedimentPointer(),*/
               * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
               * MyTransportPointer = MyPEcoDynClass->GetTransportPointer(),
               * MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   for (int i = 0; i < NumberOfInterfaceBoxes; i++)
   {
        MyBox = SaltMarshInterfaceBoxesRecord[i].LineCoordinate * NumberOfColumns + SaltMarshInterfaceBoxesRecord[i].ColumnCoordinate;

        if (MySPMPointer != NULL)
        {
                if (MyTransportPointer != NULL)
                {
                   MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,MyBox,"Box depth",ObjectCode);
                }
                else
                   ABoxDepth = MyPEcoDynClass->GetBoxDepth(MyBox);
                if (
                      (ABoxDepth > CriticalWaterDepth) &&
                      (BoxArray[MyBox].Dummybool1) &&
      	              (BoxArray[MyBox].BoxToBoundary[4].TypeOfBoundary != 2) && //No western ocean boundary
                      (BoxArray[MyBox].BoxToBoundary[2].TypeOfBoundary != 2) && //No eastern ocean boundary
                      (BoxArray[MyBox].BoxToBoundary[3].TypeOfBoundary != 2) &&  //No southern ocean boundary
                      (BoxArray[MyBox].BoxToBoundary[1].TypeOfBoundary != 2) ||   //No northern ocean boundary
                      (
                         (ABoxDepth > 0.0) &&
                         (MyTransportPointer == NULL)
                      )
                   )
                {
                   MySPMPointer->Update(GetEcoDynClassName(), OrganicRelease[i] / CUBIC / ABoxDepth, MyBox,"POM",ObjectCode);   //mg DW / L / s
                   MySPMPointer->Update(GetEcoDynClassName(), OrganicRelease[i] / CUBIC / ABoxDepth, MyBox,"TPM",ObjectCode);   //mg DW / L / s
                   MySPMPointer->Update(GetEcoDynClassName(), OrganicRelease[i] / CUBIC / 2.0 / ABoxDepth, MyBox,"DetritalC",ObjectCode);    //mg C / L / s
                   MySPMPointer->Update(GetEcoDynClassName(), OrganicRelease[i] / CUBIC / 2.0 * 0.18 / ABoxDepth, MyBox,"DetritalN",ObjectCode); //mg N / L / s
                   MySPMPointer->Update(GetEcoDynClassName(), OrganicRelease[i] / CUBIC / 2.0 * 0.02 / ABoxDepth, MyBox,"DetritalP",ObjectCode);//mg N / L / s
                }
        }
        if (MyNutrientPointer != NULL)
        {
                if (MyTransportPointer != NULL)
                {
                   MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,MyBox,"Box depth",ObjectCode);
                }
                else
                   ABoxDepth = MyPEcoDynClass->GetBoxDepth(MyBox);
                if (
                      (ABoxDepth > CriticalWaterDepth) &&
                      (BoxArray[MyBox].Dummybool1) &&
      	              (BoxArray[MyBox].BoxToBoundary[4].TypeOfBoundary != 2) && //No western ocean boundary
                      (BoxArray[MyBox].BoxToBoundary[2].TypeOfBoundary != 2) && //No eastern ocean boundary
                      (BoxArray[MyBox].BoxToBoundary[3].TypeOfBoundary != 2) &&  //No southern ocean boundary
                      (BoxArray[MyBox].BoxToBoundary[1].TypeOfBoundary != 2) ||   //No northern ocean boundary
                      (
                         (ABoxDepth > 0.0) &&
                         (MyTransportPointer == NULL)
                      )
                   )
                {
                   MyNutrientPointer->Update(GetEcoDynClassName(), -NitrateConsumption[i] / ABoxDepth, MyBox,"Nitrate",ObjectCode); //micro mol N / m3 / s
                   MyNutrientPointer->Update(GetEcoDynClassName(), AmmoniumRelease[i] / ABoxDepth, MyBox,"Ammonia",ObjectCode);   //micro mol N / m3 / s
                }
        }

   }
}


