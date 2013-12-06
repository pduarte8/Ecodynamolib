// Bidimensional hydrodynamic model object CPP code file
// 	@ Pedro Duarte
// Rev. A 95.4.23
/**
 * NPI work
 * 	14 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "hydrobjt.h"
#include "iodll.h"
#include "globals.h"

TBiDimensionalTidalTest::TBiDimensionalTidalTest(TEcoDynClass* PEcoDynClass, char* className)
														 :TBiDimensionalNormalTimeStep(PEcoDynClass, className)

{

   //Parameters file
   double MyValue;
   int X,Y;
	ParameterCount = 0;
	// Read in the parameters
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Transport");
	if (PReadWrite != NULL)
	{
        if (PReadWrite->FindString("Transport", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            // read in the parameter names
            char MyParameter[65];
    //		ParametersNameArray = new VNA[NumberOfParameters];

            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
    //            strcpy(ParametersNameArray[i-Y], MyParameter);
    //
    // List of parameters initialized in TOneDimensionalDITransport
    //
                if (strcmp(MyParameter, "Gravity") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Gravity);
                    //Debugger(Gravity);
                }
                else
                if (strcmp(MyParameter, "Coeficiente de Manning") == 0)
                {
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(X + 3, i + j, CoefManning[j]);
                        //Debugger(CoefManning[j]);
                    }
                }
                else
                if (strcmp(MyParameter, "Difusion coeficient") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, HorizontalDiffCoef);
                    //Debugger(HorizontalDiffCoef);
                }
             /*else
                if (strcmp(MyParameter, "Columns") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, GridColumns);
                    //Debugger(GridColumns);
                }
             else
                if (strcmp(MyParameter, "Lines") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, GridLines);
                    //Debugger(GridLines);
                }*/
             else
             if (strcmp(MyParameter, "Latitude") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, MyLatitude);
                //Debugger(MyLatitude);
             }
    /*         else
             if (strcmp(MyParameter, "Critical depth for land boundary") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, CriticalDepthForLandBoundary);
                //Debugger(CriticalDepthForLandBoundary);
             }*/
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TBiDimensionalTidalTest::TBiDimensionalTidalTest - parameters undefined object" << endl;
#endif  // __BORLANDC__
        }
       //End reading parameters file
        CloseDataFile((void*)PReadWrite);
    }

   GridLines = MyPEcoDynClass->GetNumberOfLines();
   GridColumns = MyPEcoDynClass->GetNumberOfColumns();

   if (NumberOfBoxes > 0)
	{
		UVelocity = new double[NumberOfBoxes + GridLines];
      VVelocity = new double[NumberOfBoxes + GridColumns];
      UVelocityPast = new double[NumberOfBoxes + GridLines];
      VVelocityPast = new double[NumberOfBoxes + GridColumns];
		UFlowBetweenBoxes = new double[NumberOfBoxes + GridLines];
      VFlowBetweenBoxes = new double[NumberOfBoxes + GridColumns];
/*      A = new double[NumberOfBoxes * NumberOfBoxes];
      B = new double[NumberOfBoxes];
      Indx = new int[NumberOfBoxes];*/
   }



   for (int i = 0; i < NumberOfBoxes + GridLines; i++)
   {
   	UVelocity[i] = 0;
      UVelocityPast[i] = 0;
      UFlowBetweenBoxes[i] = 0;
   }

   for (int i = 0; i < NumberOfBoxes + GridColumns; i++)
   {
   	VVelocity[i] = 0;
      VVelocityPast[i] = 0;
      VFlowBetweenBoxes[i] = 0;
   }

/*   for (int i = 0; i < NumberOfBoxes; i++)
   {
   	B[i] = 0;
   	Indx[i] = i;
   	for (int j = 0; j < NumberOfBoxes; j++)
      {
      	A[i * NumberOfBoxes + j] = 0.0;
      }
   }*/

	//Variables file
	char MyVariable[65];

//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Transport");
	if (PReadWrite != NULL)
	{
        if (PReadWrite->FindString("Transport", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Array for state variable names created in TOneDimensionalDITransport

            for (int i = 0; i < NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
                strcpy(VariableNameArray[i], MyVariable);

                if (strcmp(MyVariable, "U Velocity") == 0)
                {
                    for (int j = 0; j < NumberOfBoxes + GridLines; j++)
                    {
                        PReadWrite->ReadNumber(X + 2 + i , Y + 1 + j, UVelocity[j]);
                   UVelocityPast[j] = UVelocity[j];
                        //Debugger(UVelocity[j]);
                    }
                }
             //MessageBox(0, VariableNameArray[i], "EcoDynamo alert", MB_OK);

             if (strcmp(MyVariable, "V Velocity") == 0)
                {
                    for (int j = 0; j < NumberOfBoxes + GridColumns; j++)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1 + j, VVelocity[j]);
                   VVelocityPast[j] = VVelocity[j];
                        //Debugger(VVelocity[j]);
                    }
                }

                else if (strcmp(MyVariable, "Dynamic height") == 0)
                {
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1 + j, DynamicHeight[j]);
                        //Debugger(DynamicHeight[j]);
                        PastHeight[j] = DynamicHeight[j];
                    }
                }
         
                else if (strcmp(MyVariable, "U Flow") == 0)
                {
                PReadWrite->ReadNumber(X + 2 + i, Y + 1, MyValue);
                    for (int j = 0; j < NumberOfBoxes + GridLines; j++)
                    {
                   UFlowBetweenBoxes[j] = MyValue;
                        //Debugger(UFlowBetweenBoxes[j]);
                    }
                }
             else if (strcmp(MyVariable, "V Flow") == 0)
                {
                PReadWrite->ReadNumber(X + 2 + i, Y + 1, MyValue);
                    for (int j = 0; j < NumberOfBoxes + GridColumns; j++)
                    {
                   VFlowBetweenBoxes[j] = MyValue;
                        //Debugger(VFlowBetweenBoxes[j]);
                    }
                }
                else if (strcmp(MyVariable, "Salinity") == 0)
                {
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1 + j, Salt[j]);
                        //Debugger(Salt[j]);
                    }
                }
             else if (strcmp(MyVariable, "Box depth") == 0)
                {
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1 + j, BoxDepth[j]);
                        //Debugger(BoxDepth[j]);
                    }
                }
             else if (strcmp(MyVariable, "Water density") == 0)
                {
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1 + j, WaterDensity[j]);
                        //Debugger(WaterDensity[j]);
                    }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TBiDimensionalTidalTest::TBiDimensionalTidalTest - variables undefined object" << endl;
#endif  // __BORLANDC__
        }
        //End reading variables file
        CloseDataFile((void*)PReadWrite);
    }


   ASpatialStep = MyPEcoDynClass->GetBoxLength(1);
   Debugger(ASpatialStep);



  for (int i = 0; i < GridLines; i++)
   {
   	for (int j = 0; j < GridColumns; j++)
      {
         //BoxDepth[i * GridColumns + j] = BoxDepth[i * GridColumns + j] + DynamicHeight[i * GridColumns + j];

         if (j == 0)
         	BoxArray[i * GridColumns + j].BoxToBoundary[4].TypeOfBoundary = 4;
         if (j == GridColumns - 1)
         	BoxArray[i * GridColumns + j].BoxToBoundary[2].TypeOfBoundary = 4;
         if (i == 0)
         	BoxArray[i * GridColumns + j].BoxToBoundary[3].TypeOfBoundary = 4;
         if (i == GridLines - 1)
         	BoxArray[i * GridColumns + j].BoxToBoundary[1].TypeOfBoundary = 4;
      }
   }


   //for (int j = 5; j < GridColumns; j++)
   //for (int j = 0; j < GridColumns; j++)
   //for (int j = 1; j < GridColumns-1; j++)
   //for (int j = 0; j < 2; j++)
   //{
   	/*if (j == 25)
      {
      	BoxArray[(GridLines - 1) * GridColumns + j].BoxToBoundary[4].TypeOfBoundary = 4;
      	BoxArray[j].BoxToBoundary[4].TypeOfBoundary = 4;
      } */
      //BoxArray[(GridLines - 1) * GridColumns + j].BoxToBoundary[1].TypeOfBoundary = 2;  //Northern sea boundary
      //BoxArray[j].BoxToBoundary[3].TypeOfBoundary = 2;  //Southern sea boundary
   //}
   //for (int i = 0; i < GridLines; i++)
   for (int i = 0; i < GridLines; i++)
   //for (int i = 5; i < GridLines; i++)
   //for (int i = 0; i < 5; i++)
   {
      //BoxArray[i * GridColumns + GridColumns - 1].BoxToBoundary[2].TypeOfBoundary = 2;  //Eastern sea boundary
      BoxArray[i * GridColumns].BoxToBoundary[4].TypeOfBoundary = 2;  //Western sea boundary
   }
   /*for (int i = 4; i < 6; i++)
   {
   	BoxArray[i * GridColumns].Dummybool1 = false;
      BoxArray[i * GridColumns + 1].BoxToBoundary[4].TypeOfBoundary = 1;  //Western river boundary
   } */
   for (int i = 17; i < 18; i++)
   {
   	BoxArray[i * GridColumns + GridColumns - 1].Dummybool1 = false;
      BoxArray[i * GridColumns + GridColumns - 2].BoxToBoundary[2].TypeOfBoundary = 1;  //Eastern river boundary
   }
   /*for (int j = 4; j < 6; j++)
   {
   	BoxArray[(GridLines - 1) * GridColumns + j].Dummybool1 = false;
      BoxArray[(GridLines - 2) * GridColumns + j].BoxToBoundary[1].TypeOfBoundary = 1;  //Northern river boundary
   }*/
   /*for (int j = 4; j < 6; j++)
   {
   	BoxArray[j].Dummybool1 = false;
      BoxArray[GridColumns + j].BoxToBoundary[3].TypeOfBoundary = 1;  //Southern river boundary
   }*/
   //BoxArray[2 * GridColumns].Dummybool1 = false;
   //BoxArray[2 * GridColumns + 1].BoxToBoundary[4].TypeOfBoundary = 1;  //Western river boundary
   caudal = -1000.0; //-1000.0 m3 / s
   for (int i = 0; i < NumberOfBoxes; i++)
   {
      if (BoxDepth[i] > CriticalDepthForLandBoundary)
      {
         BoxArray[i].Dummybool2 = true;
         BoxArray[i].Dummybool3 = true;
         //BoxArray[i].Dummybool4 = true;
      }
      else
      {
         BoxArray[i].Dummybool2 = false;
         BoxArray[i].Dummybool3 = false;
         //BoxArray[i].Dummybool4 = false;
      }
   }
}



TBiDimensionalTidalTest::~TBiDimensionalTidalTest()
{
    try {
        delete [] A;
       delete [] B;
       delete [] Indx;
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
        cerr << "TBiDimensionalTidalTest::~TBiDimensionalTidalTest - " << exc.what() << endl;
    }
#endif  // __BORLANDC__
}


double TBiDimensionalTidalTest::GetParameterValue(char* MyParameter)
{
  double value;


    // parameters from TOneDimensionalDITransport
    if (strcmp(MyParameter, "Gravity") == 0)
    {
        value = Gravity;
    }
    else if (strcmp(MyParameter, "Coeficiente de Manning") == 0)
    {
        value = CoefManning[0];
    }
    else if (strcmp(MyParameter, "Difusion coeficient") == 0)
    {
        value = HorizontalDiffCoef;
    }
    else if (strcmp(MyParameter, "Box Width") == 0)
    {
        value = BoxWidth[0];
    }
    else if (strcmp(MyParameter, "Start Year") == 0)
    {
        value = StartYear;
    }
    else if (strcmp(MyParameter, "kdiffusion") == 0)
    {
        value = kdiffusion;
    }

    // parameters from TBiDimensionalTidalTest
    else if (strcmp(MyParameter, "Latitude") == 0)
    {
        value = MyLatitude;
    }
    else
        value = 0.0;
    return value;
}

bool TBiDimensionalTidalTest::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int j;

    // parameters from TOneDimensionalDITransport
    if (strcmp(MyParameter, "Gravity") == 0)
    {
        Gravity = value;
    }
    else if (strcmp(MyParameter, "Coeficiente de Manning") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
        {
            CoefManning[j] = value;
        }
    }
    else if (strcmp(MyParameter, "Difusion coeficient") == 0)
    {
        HorizontalDiffCoef = value;
    }
    else if (strcmp(MyParameter, "Box Width") == 0)
    {
        for (j = 0; j < NumberOfBoxes + 1; j++)
        {
            BoxWidth[j] = value;
        }
    }
    else if (strcmp(MyParameter, "Start Year") == 0)
    {
        StartYear = value;
    }
    else if (strcmp(MyParameter, "kdiffusion") == 0)
    {
        kdiffusion = value;
    }

    // parameters from TBiDimensionalTidaltest
    else if (strcmp(MyParameter, "Latitude") == 0)
    {
        MyLatitude = value;
    }
    else
        rc = false;

    return rc;
}

void TBiDimensionalTidalTest::RiverForcing()
{
	//MyTime = MyPEcoDynClass->GetCurrentTime();
   //caudal = - (MyTime * 10.0);
	/*UVelocityPast[2 * (GridColumns + 1) + 1] = caudal / (BoxDepth[2 * GridColumns + 1] * ASpatialStep);
   UVelocity[2 * (GridColumns + 1) + 1] = UVelocityPast[2 * (GridColumns + 1) + 1];
   BoxDepth[2 * GridColumns] = BoxDepth[2 * GridColumns + 1]; */
   //DynamicHeight[2 * GridColumns] = DynamicHeight[2 * GridColumns + 1];
   //PastHeight[2 * GridColumns] = PastHeight[2 * GridColumns + 1];
   /*for (int i = 4; i < 6; i++)
   {
   	UVelocityPast[i * (GridColumns + 1) + 1] = caudal / (BoxDepth[i * GridColumns + 1] * ASpatialStep);     // Western river boundary
   	UVelocity[i * (GridColumns + 1) + 1] = UVelocityPast[i * (GridColumns + 1) + 1];
   	BoxDepth[i * GridColumns] = BoxDepth[i * GridColumns + 1];
      VVelocityPast[i * GridColumns] = 0.0;
      VVelocity[i * GridColumns] = 0.0;
   } */
   //for (int i = 4; i < 6; i++)

   for (int i = 17; i < 18; i++)
   {
      /*if ((MyPEcoDynClass->GetJulianDay() == 1) && (MyPEcoDynClass->GetCurrentTime() < 2))
      { */
   	UVelocityPast[i * (GridColumns + 1) + GridColumns - 1] = caudal / (BoxDepth[i * GridColumns + GridColumns - 2] * ASpatialStep);  //Eastern river boundary
   	UVelocity[i * (GridColumns + 1) + GridColumns - 1] = UVelocityPast[i * (GridColumns + 1) + GridColumns - 1];
   	BoxDepth[i * GridColumns + GridColumns - 1] = BoxDepth[i * GridColumns + GridColumns - 2];
      //VVelocityPast[i * GridColumns + GridColumns - 1] = 0.0;
      //VVelocity[i * GridColumns + GridColumns - 1] = 0.0;
      VVelocity[i * GridColumns + GridColumns - 1] = VVelocityPast[i * GridColumns + GridColumns - 1];
      /*}
      else
      {
      UVelocityPast[i * (GridColumns + 1) + GridColumns - 1] = 0.0;  //Eastern river boundary
   	UVelocity[i * (GridColumns + 1) + GridColumns - 1] = UVelocityPast[i * (GridColumns + 1) + GridColumns - 1];
   	BoxDepth[i * GridColumns + GridColumns - 1] = BoxDepth[i * GridColumns + GridColumns - 2];
      //VVelocityPast[i * GridColumns + GridColumns - 1] = 0.0;
      //VVelocity[i * GridColumns + GridColumns - 1] = 0.0;
      VVelocity[i * GridColumns + GridColumns - 1] = VVelocityPast[i * GridColumns + GridColumns - 1];

      }
   } */
   /*for (int j = 4; j < 6; j++)
   {
   	VVelocityPast[(GridLines - 1) * GridColumns + j] = caudal / (BoxDepth[(GridLines - 2) * GridColumns + j] * ASpatialStep);
   	VVelocity[(GridLines - 1) * GridColumns + j] = VVelocityPast[(GridLines - 1) * GridColumns + j];                         //Northern river boundary
   	BoxDepth[(GridLines - 1) * GridColumns + j] = BoxDepth[(GridLines - 2) * GridColumns + j];
      UVelocityPast[(GridLines-1) * (GridColumns + 1) + j] = 0.0;
      UVelocity[(GridLines-1) * (GridColumns + 1) + j] = 0.0;
   }*/
   /*for (int j = 4; j < 6; j++)
   {
   	VVelocityPast[GridColumns + j] = caudal / (BoxDepth[GridColumns + j] * ASpatialStep);             //Southern river boundary
   	VVelocity[GridColumns + j] = VVelocityPast[GridColumns + j];
   	BoxDepth[j] = BoxDepth[GridColumns + j];
      //UVelocityPast[j] = 0.0;
      //UVelocity[j] = 0.0;
      UVelocity[j] = UVelocityPast[j]; */
   }
}


void TBiDimensionalTidalTest::TideForcing()
{
	double MyCurrentTime;

   MyTime = MyPEcoDynClass->GetCurrentTime();
   SurfaceHeight = cos(2.0 * M_PI / 12 * MyTime) * 0.5 + 0.5 /*+ 10*/;

   //for (int i = 5; i < GridLines; i++)
   //for (int i = 0; i < GridLines; i++)
   for (int i = 0; i < GridLines; i++)
   //for (int i = 0; i < 5; i++)
   {
      //DynamicHeight[i * GridColumns + GridColumns - 1] = SurfaceHeight;  //Eastern sea boundary
      DynamicHeight[i * GridColumns] = SurfaceHeight;  //Western sea boundary
   }

   //for (int j = 5; j < GridColumns; j++)
   //for (int j = 1; j < GridColumns-1; j++)
   //for (int j = 0; j < GridColumns; j++)
   /*for (int j = 0; j < 2; j++)
   {
      DynamicHeight[(GridLines - 1) * GridColumns + j] = SurfaceHeight;  //Northern sea boundary
      DynamicHeight[j] = SurfaceHeight;  //Southern sea boundary
   }  */
}




TBiDimensionalTidalTestIntertidal::TBiDimensionalTidalTestIntertidal(TEcoDynClass* PEcoDynClass, char* className)
														 :TBiDimensionalIntertidal(PEcoDynClass, className)

{
   //Parameters file
   double MyValue;
   int X,Y;
	ParameterCount = 0;
	// Read in the parameters
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Transport");
	if (PReadWrite != NULL)
	{
        if (PReadWrite->FindString("Transport", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            // read in the parameter names
            char MyParameter[65];
    //		ParametersNameArray = new VNA[NumberOfParameters];

            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
    //            strcpy(ParametersNameArray[i-Y], MyParameter);
    //
    // List of parameters initialized in TOneDimensionalDITransport
    //
                if (strcmp(MyParameter, "Gravity") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Gravity);
                    //Debugger(Gravity);
                }
                else
                if (strcmp(MyParameter, "Coeficiente de Manning") == 0)
                {
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(X + 3, i + j, CoefManning[j]);
                        //Debugger(CoefManning[j]);
                    }
                }
                else
                if (strcmp(MyParameter, "Difusion coeficient") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, HorizontalDiffCoef);
                    //Debugger(HorizontalDiffCoef);
                }
                else
                if (strcmp(MyParameter, "Latitude") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, MyLatitude);
                //Debugger(MyLatitude);
             }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TBiDimensionalTidalTestIntertidal::TBiDimensionalTidalTestIntertidal - parameters undefined object" << endl;
#endif  // __BORLANDC__
        }
       //End reading parameters file
        CloseDataFile((void*)PReadWrite);
    }

   if (NumberOfBoxes > 0)
	{
		UVelocity = new double[NumberOfBoxes + GridLines];
      VVelocity = new double[NumberOfBoxes + GridColumns];
      UVelocityPast = new double[NumberOfBoxes + GridLines];
      VVelocityPast = new double[NumberOfBoxes + GridColumns];
		UFlowBetweenBoxes = new double[NumberOfBoxes + GridLines];
      VFlowBetweenBoxes = new double[NumberOfBoxes + GridColumns];

   }



   for (int i = 0; i < NumberOfBoxes + GridLines; i++)
   {
   	UVelocity[i] = 0;
      UVelocityPast[i] = 0;
      UFlowBetweenBoxes[i] = 0;
   }

   for (int i = 0; i < NumberOfBoxes + GridColumns; i++)
   {
   	VVelocity[i] = 0;
      VVelocityPast[i] = 0;
      VFlowBetweenBoxes[i] = 0;
   }

   ASpatialStep = MyPEcoDynClass->GetBoxLength(1);
    //Debugger(ASpatialStep);
    GridLines = MyPEcoDynClass->GetNumberOfLines();
    GridColumns = MyPEcoDynClass->GetNumberOfColumns();
    

	//Variables file
	char MyVariable[65];

//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Transport");
	if (PReadWrite != NULL)
	{
        if (PReadWrite->FindString("Transport", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Array for state variable names created in TOneDimensionalDITransport

            for (int i = 0; i < NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
                strcpy(VariableNameArray[i], MyVariable);
                if (strcmp(MyVariable, "U Velocity") == 0)
                {
                    for (int j = 0; j < NumberOfBoxes + GridLines; j++)
                    {
                        PReadWrite->ReadNumber(X + 2 + i , Y + 1 + j, UVelocity[j]);
                   UVelocityPast[j] = UVelocity[j];
                        //Debugger(UVelocity[j]);
                    }
                }
             //MessageBox(0, VariableNameArray[i], "EcoDynamo alert", MB_OK);
         
             if (strcmp(MyVariable, "V Velocity") == 0)
                {
                    for (int j = 0; j < NumberOfBoxes + GridColumns; j++)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1 + j, VVelocity[j]);
                   VVelocityPast[j] = VVelocity[j];
                        //Debugger(VVelocity[j]);
                    }
                }

                else if (strcmp(MyVariable, "Dynamic height") == 0)
                {
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1 + j, DynamicHeight[j]);
                        //Debugger(DynamicHeight[j]);
                        PastHeight[j] = DynamicHeight[j];
                    }
                }
         
                else if (strcmp(MyVariable, "U Flow") == 0)
                {
                PReadWrite->ReadNumber(X + 2 + i, Y + 1, MyValue);
                    for (int j = 0; j < NumberOfBoxes + GridLines; j++)
                    {
                   UFlowBetweenBoxes[j] = MyValue;
                        //Debugger(UFlowBetweenBoxes[j]);
                    }
                }
             else if (strcmp(MyVariable, "V Flow") == 0)
                {
                PReadWrite->ReadNumber(X + 2 + i, Y + 1, MyValue);
                    for (int j = 0; j < NumberOfBoxes + GridColumns; j++)
                    {
                   VFlowBetweenBoxes[j] = MyValue;
                        //Debugger(VFlowBetweenBoxes[j]);
                    }
                }
                else if (strcmp(MyVariable, "Salinity") == 0)
                {
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1 + j, Salt[j]);
                        //Debugger(Salt[j]);
                    }
                }
             else if (strcmp(MyVariable, "Box depth") == 0)
                {
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1 + j, BoxDepth[j]);
                        //Debugger(BoxDepth[j]);
                    }
                }
             else if (strcmp(MyVariable, "Water density") == 0)
                {
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1 + j, WaterDensity[j]);
                        //Debugger(WaterDensity[j]);
                    }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TBiDimensionalTidalTestIntertidal::TBiDimensionalTidalTestIntertidal - variables undefined object" << endl;
#endif  // __BORLANDC__
        }
        //End reading variables file
        CloseDataFile((void*)PReadWrite);
    }

   ASpatialStep = MyPEcoDynClass->GetBoxLength(1);

  for (int i = 0; i < GridLines; i++)
   {
   	for (int j = 0; j < GridColumns; j++)
      {
         //BoxDepth[i * GridColumns + j] = BoxDepth[i * GridColumns + j] + DynamicHeight[i * GridColumns + j];
         
         if (j == 0)
         	BoxArray[i * GridColumns + j].BoxToBoundary[4].TypeOfBoundary = 4;
         if (j == GridColumns - 1)
         	BoxArray[i * GridColumns + j].BoxToBoundary[2].TypeOfBoundary = 4;
         if (i == 0)
         	BoxArray[i * GridColumns + j].BoxToBoundary[3].TypeOfBoundary = 4;
         if (i == GridLines - 1)
         	BoxArray[i * GridColumns + j].BoxToBoundary[1].TypeOfBoundary = 4;
      }
   }


   //for (int j = 5; j < GridColumns; j++)
   //for (int j = 0; j < GridColumns; j++)
   //for (int j = 1; j < GridColumns-1; j++)
   //for (int j = 0; j < 2; j++)
   //{
   	/*if (j == 25)
      {
      	BoxArray[(GridLines - 1) * GridColumns + j].BoxToBoundary[4].TypeOfBoundary = 4;
      	BoxArray[j].BoxToBoundary[4].TypeOfBoundary = 4;
      } */
      //BoxArray[(GridLines - 1) * GridColumns + j].BoxToBoundary[1].TypeOfBoundary = 2;  //Northern sea boundary
      //BoxArray[j].BoxToBoundary[3].TypeOfBoundary = 2;  //Southern sea boundary
   //}
   //for (int i = 0; i < GridLines; i++)
   for (int i = 0; i < GridLines; i++)
   //for (int i = 5; i < GridLines; i++)
   //for (int i = 0; i < 5; i++)
   {
      //BoxArray[i * GridColumns + GridColumns - 1].BoxToBoundary[2].TypeOfBoundary = 2;  //Eastern sea boundary
      BoxArray[i * GridColumns].BoxToBoundary[4].TypeOfBoundary = 2;  //Western sea boundary
      //if (i == 2) Debugger(BoxArray[i * GridColumns].BoxToBoundary[4].TypeOfBoundary);
   }
   /*for (int i = 4; i < 6; i++)
   {
   	BoxArray[i * GridColumns].Dummybool1 = false;
      BoxArray[i * GridColumns + 1].BoxToBoundary[4].TypeOfBoundary = 1;  //Western river boundary
   } */
   for (int i = 17; i < 18; i++)
   {
   	BoxArray[i * GridColumns + GridColumns - 1].Dummybool1 = false;
      BoxArray[i * GridColumns + GridColumns - 2].BoxToBoundary[2].TypeOfBoundary = 1;  //Eastern river boundary
   }
   /*for (int j = 4; j < 6; j++)
   {
   	BoxArray[(GridLines - 1) * GridColumns + j].Dummybool1 = false;
      BoxArray[(GridLines - 2) * GridColumns + j].BoxToBoundary[1].TypeOfBoundary = 1;  //Northern river boundary
   }*/
   /*for (int j = 4; j < 6; j++)
   {
   	BoxArray[j].Dummybool1 = false;
      BoxArray[GridColumns + j].BoxToBoundary[3].TypeOfBoundary = 1;  //Southern river boundary
   }*/
   //BoxArray[2 * GridColumns].Dummybool1 = false;
   //BoxArray[2 * GridColumns + 1].BoxToBoundary[4].TypeOfBoundary = 1;  //Western river boundary
   caudal = -1000.0; //-1000.0 m3 / s
   for (int i = 0; i < NumberOfBoxes; i++)
   {
      if (BoxDepth[i] > CriticalDepthForLandBoundary)
      {
         BoxArray[i].Dummybool2 = true;
         BoxArray[i].Dummybool3 = true;
         //BoxArray[i].Dummybool4 = true;
      }
      else
      {
         BoxArray[i].Dummybool2 = false;
         BoxArray[i].Dummybool3 = false;
         //BoxArray[i].Dummybool4 = false;
      }
   } 
}



TBiDimensionalTidalTestIntertidal::~TBiDimensionalTidalTestIntertidal()
{
}

double TBiDimensionalTidalTestIntertidal::GetParameterValue(char* MyParameter)
{
  double value;


    // parameters from TOneDimensionalDITransport
    if (strcmp(MyParameter, "Gravity") == 0)
    {
        value = Gravity;
    }
    else if (strcmp(MyParameter, "Coeficiente de Manning") == 0)
    {
        value = CoefManning[0];
    }
    else if (strcmp(MyParameter, "Difusion coeficient") == 0)
    {
        value = HorizontalDiffCoef;
    }
    else if (strcmp(MyParameter, "Box Width") == 0)
    {
        value = BoxWidth[0];
    }
    else if (strcmp(MyParameter, "Start Year") == 0)
    {
        value = StartYear;
    }
    else if (strcmp(MyParameter, "kdiffusion") == 0)
    {
        value = kdiffusion;
    }

    // parameters from TBiDimensionalTidalTestIntertidal
    else if (strcmp(MyParameter, "Latitude") == 0)
    {
        value = MyLatitude;
    }
    else
        value = 0.0;
    return value;
}

bool TBiDimensionalTidalTestIntertidal::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int j;

    // parameters from TOneDimensionalDITransport
    if (strcmp(MyParameter, "Gravity") == 0)
    {
        Gravity = value;
    }
    else if (strcmp(MyParameter, "Coeficiente de Manning") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
        {
            CoefManning[j] = value;
        }
    }
    else if (strcmp(MyParameter, "Difusion coeficient") == 0)
    {
        HorizontalDiffCoef = value;
    }
    else if (strcmp(MyParameter, "Box Width") == 0)
    {
        for (j = 0; j < NumberOfBoxes + 1; j++)
        {
            BoxWidth[j] = value;
        }
    }
    else if (strcmp(MyParameter, "Start Year") == 0)
    {
        StartYear = value;
    }
    else if (strcmp(MyParameter, "kdiffusion") == 0)
    {
        kdiffusion = value;
    }

    // parameters from TBiDimensionalTidalTestIntertidal
    else if (strcmp(MyParameter, "Latitude") == 0)
    {
        MyLatitude = value;
    }
    else
        rc = false;

    return rc;
}

void TBiDimensionalTidalTestIntertidal::RiverForcing()
{
	//MyTime = MyPEcoDynClass->GetCurrentTime();
   //caudal = - (MyTime * 10.0);
	/*UVelocityPast[2 * (GridColumns + 1) + 1] = caudal / (BoxDepth[2 * GridColumns + 1] * ASpatialStep);
   UVelocity[2 * (GridColumns + 1) + 1] = UVelocityPast[2 * (GridColumns + 1) + 1];
   BoxDepth[2 * GridColumns] = BoxDepth[2 * GridColumns + 1]; */
   //DynamicHeight[2 * GridColumns] = DynamicHeight[2 * GridColumns + 1];
   //PastHeight[2 * GridColumns] = PastHeight[2 * GridColumns + 1];
   /*for (int i = 4; i < 6; i++)
   {
   	UVelocityPast[i * (GridColumns + 1) + 1] = caudal / (BoxDepth[i * GridColumns + 1] * ASpatialStep);     // Western river boundary
   	UVelocity[i * (GridColumns + 1) + 1] = UVelocityPast[i * (GridColumns + 1) + 1];
   	BoxDepth[i * GridColumns] = BoxDepth[i * GridColumns + 1];
      VVelocityPast[i * GridColumns] = 0.0;
      VVelocity[i * GridColumns] = 0.0;
   } */
   //for (int i = 4; i < 6; i++)
   for (int i = 17; i < 18; i++)
   {
      /*if ((MyPEcoDynClass->GetJulianDay() == 1) && (MyPEcoDynClass->GetCurrentTime() < 2))
      { */
   	UVelocityPast[i * (GridColumns + 1) + GridColumns - 1] = caudal / (BoxDepth[i * GridColumns + GridColumns - 2] * ASpatialStep);  //Eastern river boundary
   	UVelocity[i * (GridColumns + 1) + GridColumns - 1] = UVelocityPast[i * (GridColumns + 1) + GridColumns - 1];
   	BoxDepth[i * GridColumns + GridColumns - 1] = BoxDepth[i * GridColumns + GridColumns - 2];
      //VVelocityPast[i * GridColumns + GridColumns - 1] = 0.0;
      //VVelocity[i * GridColumns + GridColumns - 1] = 0.0;
      VVelocity[i * GridColumns + GridColumns - 1] = VVelocityPast[i * GridColumns + GridColumns - 1];
      /*}
      else
      {
      UVelocityPast[i * (GridColumns + 1) + GridColumns - 1] = 0.0;  //Eastern river boundary
   	UVelocity[i * (GridColumns + 1) + GridColumns - 1] = UVelocityPast[i * (GridColumns + 1) + GridColumns - 1];
   	BoxDepth[i * GridColumns + GridColumns - 1] = BoxDepth[i * GridColumns + GridColumns - 2];
      //VVelocityPast[i * GridColumns + GridColumns - 1] = 0.0;
      //VVelocity[i * GridColumns + GridColumns - 1] = 0.0;
      VVelocity[i * GridColumns + GridColumns - 1] = VVelocityPast[i * GridColumns + GridColumns - 1];

      }
   } */
   /*for (int j = 4; j < 6; j++)
   {
   	VVelocityPast[(GridLines - 1) * GridColumns + j] = caudal / (BoxDepth[(GridLines - 2) * GridColumns + j] * ASpatialStep);
   	VVelocity[(GridLines - 1) * GridColumns + j] = VVelocityPast[(GridLines - 1) * GridColumns + j];                         //Northern river boundary
   	BoxDepth[(GridLines - 1) * GridColumns + j] = BoxDepth[(GridLines - 2) * GridColumns + j];
      UVelocityPast[(GridLines-1) * (GridColumns + 1) + j] = 0.0;
      UVelocity[(GridLines-1) * (GridColumns + 1) + j] = 0.0;
   }*/
   /*for (int j = 4; j < 6; j++)
   {
   	VVelocityPast[GridColumns + j] = caudal / (BoxDepth[GridColumns + j] * ASpatialStep);             //Southern river boundary
   	VVelocity[GridColumns + j] = VVelocityPast[GridColumns + j];
   	BoxDepth[j] = BoxDepth[GridColumns + j];
      //UVelocityPast[j] = 0.0;
      //UVelocity[j] = 0.0;
      UVelocity[j] = UVelocityPast[j]; */
   }
}


void TBiDimensionalTidalTestIntertidal::TideForcing()
{
	double MyCurrentTime;

   MyTime = MyPEcoDynClass->GetCurrentTime();
   SurfaceHeight = cos(2.0 * M_PI / 12 * MyTime) * 0.5 + 0.5 /*+ 10*/;

   //for (int i = 5; i < GridLines; i++)
   //for (int i = 0; i < GridLines; i++)
   for (int i = 0; i < GridLines; i++)
   //for (int i = 0; i < 5; i++)
   {
      //DynamicHeight[i * GridColumns + GridColumns - 1] = SurfaceHeight;  //Eastern sea boundary
      if (BoxArray[i * GridColumns].Dummybool2)
      DynamicHeight[i * GridColumns] = SurfaceHeight;  //Western sea boundary
   } 
   
   //for (int j = 5; j < GridColumns; j++)
   //for (int j = 1; j < GridColumns-1; j++)
   //for (int j = 0; j < GridColumns; j++)
   /*for (int j = 0; j < 2; j++)
   {
      DynamicHeight[(GridLines - 1) * GridColumns + j] = SurfaceHeight;  //Northern sea boundary
      DynamicHeight[j] = SurfaceHeight;  //Southern sea boundary
   }  */
}
