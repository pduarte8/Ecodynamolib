// Bidimensional hydrodynamic model object CPP code file
// 	@ Pedro Duarte
// Rev. A 00.1.15
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


//
// Constructors invoked outside EcoDyn shell...
//
TBiDimensional::TBiDimensional(char* className, float timeStep, char* morphologyFilename)
             :TOneDimensionalDITransport(className, timeStep, morphologyFilename)
{
    BuildBiDimensional();
}

TBiDimensional::TBiDimensional(char* className, float timeStep, int nLines, int nColumns,
        double aDepth[], double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[])
        :TOneDimensionalDITransport(className, timeStep, nLines, nColumns, aDepth,
                      aLength, aWidth, aElevation,
                      aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    BuildBiDimensional();
}
// ...

TBiDimensional::TBiDimensional(TEcoDynClass* PEcoDynClass, char* className)
                            :TOneDimensionalDITransport(PEcoDynClass, className)
{
    BuildBiDimensional();
}

void TBiDimensional::BuildBiDimensional()
{
	WesternBoundaryNumber = 4;
    SouthernBoundaryNumber = 3;
    EasternBoundaryNumber = 2;
    NorthernBoundaryNumber = 1;
	if (NumberOfBoxes > 0)
	{
        atemporary = new double[NumberOfBoxes];
        btemporary = new double[NumberOfBoxes];
        ctemporary = new double[NumberOfBoxes];
        rtemporary = new double[NumberOfBoxes];

        MeanUVelocity = new double[NumberOfBoxes];
        MeanVVelocity = new double[NumberOfBoxes];
        MeanUFlow = new double[NumberOfBoxes];
        MeanVFlow = new double[NumberOfBoxes];
	}
	else
#ifdef __BORLANDC__
		MessageBox(0,
					  "Hydrodynamic array not dimensioned",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TBiDimensional::BuildBiDimensional - Hydrodynamic array not dimensioned" << endl;
#endif  // __BORLANDC__
   for (int i = 0; i < NumberOfBoxes; i++)
   {
      //BoxArray[i].Dummybool1 = true;
      atemporary[i] = 0.0;
      btemporary[i] = 0.0;
      ctemporary[i] = 0.0;
      rtemporary[i] = 0.0;
   }
   ATimeStep = MyPEcoDynClass->GetTimeStep();
   CriticalDepthForLandBoundary = MyPEcoDynClass->GetCriticalDepth(); // AP, 050309
}

TBiDimensional::~TBiDimensional()
{
    freeMemory();
}

void TBiDimensional::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
        {
            delete [] atemporary;
            delete [] btemporary;
            delete [] ctemporary;
            delete [] rtemporary;

            delete [] MeanUVelocity;
            delete [] MeanVVelocity;
            delete [] MeanUFlow;
            delete [] MeanVFlow;
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
        cerr << "TBiDimensional::freeMemory - " << exc.what() << endl;
    }
#endif  // __BORLANDC__
}

void TBiDimensional::Go()
{
   int k, index, indexS, indexW;
   double MyWaterTemperature;


   SemiTimeStep = 1;
   TideForcing();
   EquationOfMotion();
   k = 0;
   for (int i = 0; i < GridLines; i++)
   {
   	 for (int j = 0; j < GridColumns; j++)
     {
        index = i * GridColumns + j;
        indexW = index - 1;
   		if (
      			(j > 0) &&
            	(
               	(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 0) ||//No western boundary
                  (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)
               )  &&
               (BoxDepth[index] > CriticalDepthForLandBoundary) &&
               (BoxDepth[indexW] > CriticalDepthForLandBoundary) &&
               (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
               (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
               (BoxArray[indexW].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
               (BoxArray[indexW].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
               (BoxArray[index].Dummybool1)
            )
   		{
   			a[k] = atemporary[index];
      		b[k] = btemporary[index];
      		c[k] = ctemporary[index];
      		r[k] = rtemporary[index];
            k++;
      	}
	 }
   }
   IntegrationTridag(k);
   k = 0;
   for (int i = 0; i < GridLines; i++)
   {
     for (int j = 0; j < GridColumns; j++)
     {
        index = i * GridColumns + j;
        indexW = index - 1;
   		if (
      			(j > 0) &&
            	(
               	(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 0) ||//No western boundary
                  (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)
               )  &&
               (BoxDepth[index] > CriticalDepthForLandBoundary) &&
               (BoxDepth[indexW] > CriticalDepthForLandBoundary) &&
               (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
               (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
               (BoxArray[indexW].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
               (BoxArray[indexW].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
               (BoxArray[index].Dummybool1)
          	)
   		{
         	UVelocity[i * (GridColumns + 1) + j] = u[k];
            if (fabs(UVelocity[i * (GridColumns + 1) + j]) < 0.00000000001)
               UVelocity[i * (GridColumns + 1) + j] = 0.0;
            k++;
      	}
	 }
   }

   UpdateOfTidalBoundariesVelocities();

   for (int i = 0; i < NumberOfBoxes + GridLines; i++)
   {
   	  UVelocityPast[i] = UVelocity[i];
      if (
      		(i < NumberOfBoxes) &&
            (BoxArray[i].BoxToBoundary[1].TypeOfBoundary != 2) &&
            (BoxArray[i].BoxToBoundary[2].TypeOfBoundary != 2) &&
            (BoxArray[i].BoxToBoundary[3].TypeOfBoundary != 2) &&
            (BoxArray[i].BoxToBoundary[4].TypeOfBoundary != 2)
         )
         PastHeight[i] = DynamicHeight[i];
   }

   //Generic = Salt;
//   for (int i = 0; i < NumberOfBoxes; i++)
//   {
//   	Generic[i] = Salt[i];
//   	PastGeneric[i] = Generic[i];
//   }
   //PastGeneric = Salt;
   Continuity();
   //Salt = Generic;
//   for (int i = 0; i < NumberOfBoxes; i++)
//   	Salt[i] = Generic[i];

   for (int i = 0; i < NumberOfBoxes + GridColumns; i++)
   {
   	VVelocityPast[i] = VVelocity[i];
   }

   for (int i = 0; i < NumberOfBoxes; i++)
   	PastHeight[i] = DynamicHeight[i];


   if (SemiTimeStep == 1)
   {
      SemiTimeStep = 2;
      TideForcing();
      EquationOfMotion();
      k = 0;
      for (int j = 0; j < GridColumns; j++)
   	  {
      	for (int i = 0; i < GridLines; i++)
      	{
            index = i * GridColumns + j;
        	indexS = (i - 1) * GridColumns + j;
   			if (
      				(i > 0) &&
                  (
                  	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 0) ||//No southern boundary
                     (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)
                  ) &&
                  (BoxDepth[index] > CriticalDepthForLandBoundary) &&
                  (BoxDepth[indexS] > CriticalDepthForLandBoundary) &&
                  (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2)&& //No eastern ocean boundary
                  (BoxArray[indexS].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  (BoxArray[indexS].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2)&&   //No eastern ocean boundary
                  (BoxArray[index].Dummybool1)
          		)
   			{
   				a[k] = atemporary[index];
      			b[k] = btemporary[index];
      			c[k] = ctemporary[index];
      			r[k] = rtemporary[index];
            	k++;
      		}
		}
   	  }
   	  IntegrationTridag(k);
      k = 0;
      for (int j = 0; j < GridColumns; j++)
   	  {
      	for (int i = 0; i < GridLines; i++)
      	{
            index = i * GridColumns + j;
        	indexS = (i - 1) * GridColumns + j;
   			if (
      				(i > 0) &&
                  (
                  	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 0) ||//No southern boundary
                     (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)
                  ) &&
                  (BoxDepth[index] > CriticalDepthForLandBoundary) &&
                  (BoxDepth[indexS] > CriticalDepthForLandBoundary) &&
                  (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) && //No eastern ocean boundary
                  (BoxArray[indexS].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  (BoxArray[indexS].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) &&  //No eastern ocean boundary
                  (BoxArray[index].Dummybool1)
          		)
   			{
         		VVelocity[index] = u[k];
                if (fabs(VVelocity[index]) < 0.00000000001)
                    VVelocity[index] = 0.0;
            	k++;
      		}
	  	}
   	  }
   }
   for (int i = 0; i < NumberOfBoxes + GridLines; i++)
   {
      if (
      		(i < NumberOfBoxes) &&
            (BoxArray[i].BoxToBoundary[1].TypeOfBoundary != 2) &&
            (BoxArray[i].BoxToBoundary[2].TypeOfBoundary != 2) &&
            (BoxArray[i].BoxToBoundary[3].TypeOfBoundary != 2) &&
            (BoxArray[i].BoxToBoundary[4].TypeOfBoundary != 2)
         )
         PastHeight[i] = DynamicHeight[i];
   }

   UpdateOfTidalBoundariesVelocities();

   for (int i = 0; i < NumberOfBoxes + GridColumns; i++)
   {
   	VVelocityPast[i] = VVelocity[i];
   }

   for (int i = 0; i < NumberOfBoxes; i++)
   {
   	Generic[i] = Salt[i];
//   	PastGeneric[i] = Generic[i];
   }
   Continuity();

   for (int i = 0; i < NumberOfBoxes; i++)
   	Salt[i] = Generic[i];

   for (int i = 0; i < NumberOfBoxes + GridLines; i++)
   {
   	UVelocityPast[i] = UVelocity[i];
   }

   TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();

   for (int i = 0; i < NumberOfBoxes; i++)
   {
   	  PastHeight[i] = DynamicHeight[i];
      if (MyWaterTemperaturePointer != NULL)
      {
		MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,
											               i,
											               "Water temperature",
											               ObjectCode);

      	WaterDensity[i] = MyPEcoDynClass->GetDensity(Salt[i],MyWaterTemperature);
      }
      else
      	WaterDensity[i] = 1026.75; // seawater at 35psu and 10oC;
   }
}



void TBiDimensional::Go(double* AGeneric,
						double* ASecondGeneric,
                        double ARiver,
						double AOcean)

{
	Generic = AGeneric;
    GenericRiver = ARiver;
	GenericOcean = AOcean;
    GenericLoad = ASecondGeneric;
	AdvectDiffuse(AGeneric);
//    AGeneric = Generic;
}


void TBiDimensional::ClearMeanSamples()
{
  	samples = 0;
    for (int i = 0; i < NumberOfBoxes; i++)
    {
        MeanUVelocity[i] = 0;
        MeanVVelocity[i] = 0;
        MeanUFlow[i] = 0;
        MeanVFlow[i] = 0;
    }
}

void TBiDimensional::UpdateMeanSamples()
{
  	samples++;
    for (int i = 0; i < NumberOfBoxes; i++)
    {
        MeanUVelocity[i] += getUVelocity(i);
        MeanVVelocity[i] += getVVelocity(i);
        MeanUFlow[i] += getUFlow(i);
        MeanVFlow[i] += getVFlow(i);
    }
}


double TBiDimensional::getUFlow(int ABoxNumber)
{
    double Value, AVelocity;
    int LineIndex = GetLineIndex(ABoxNumber);
    int ColumnIndex= GetColumnIndex(ABoxNumber);
    if (SemiTimeStep == 1) AVelocity = UVelocity[LineIndex * (GridColumns + 1) + ColumnIndex];
    else if (SemiTimeStep == 2) AVelocity = UVelocityPast[LineIndex * (GridColumns + 1) + ColumnIndex];

    if ((ColumnIndex == 0) ||
        ((ColumnIndex > 0) && (BoxDepth[LineIndex * GridColumns + ColumnIndex - 1] <= 0.0))
       )
        Value = AVelocity * BoxDepth[LineIndex * GridColumns + ColumnIndex] * ASpatialStep;
    else
    {
        Value = AVelocity *
                  (0.5 * (BoxDepth[LineIndex * GridColumns + ColumnIndex - 1]
                    + BoxDepth[LineIndex * GridColumns + ColumnIndex])) *
                  ASpatialStep;
    }
    return Value;
}

double TBiDimensional::getVFlow(int ABoxNumber)
{
    double Value, AVelocity;
    int LineIndex = GetLineIndex(ABoxNumber);
    int ColumnIndex= GetColumnIndex(ABoxNumber);

    if (SemiTimeStep == 1) AVelocity = VVelocityPast[LineIndex * GridColumns + ColumnIndex];
    else if (SemiTimeStep == 2) AVelocity = VVelocity[LineIndex * GridColumns + ColumnIndex];

    if ((LineIndex == 0) ||
        ((LineIndex > 0) && (BoxDepth[(LineIndex - 1) * GridColumns + ColumnIndex] <= 0.0))
       )
        Value = -AVelocity * BoxDepth[LineIndex * GridColumns + ColumnIndex] * ASpatialStep;
    else
    {
        Value = -AVelocity *
                  (0.5 * (BoxDepth[(LineIndex - 1) * GridColumns + ColumnIndex]
                    + BoxDepth[LineIndex * GridColumns + ColumnIndex])) *
                  ASpatialStep;
    }
    return Value;
}

double TBiDimensional::getUVelocity(int ABoxNumber)
{
    int LineIndex = GetLineIndex(ABoxNumber);
    int ColumnIndex= GetColumnIndex(ABoxNumber);

    return UVelocity[LineIndex * (GridColumns + 1) + ColumnIndex]; //m / s
}

double TBiDimensional::getVVelocity(int ABoxNumber)
{
    int LineIndex = GetLineIndex(ABoxNumber);
    int ColumnIndex= GetColumnIndex(ABoxNumber);

    return -VVelocity[LineIndex * GridColumns + ColumnIndex]; //m / s A velocidade v é devolvida com sinal menos para que o Noesys
                                                                     // a entenda como de baixo para cima
}


void TBiDimensional::Inquiry(char* srcName, double &Value,
										   int BoxNumber,
										   char* ParameterName,
										   int AnObjectCode)
{
    char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
    int LineIndex = GetLineIndex(MyBoxNumber);
    int ColumnIndex= GetColumnIndex(MyBoxNumber);
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "U Velocity") == 0)
    {
        Value = getUVelocity(MyBoxNumber);
    }
    else if (strcmp(MyParameter, "V Velocity") == 0)
    {
        Value = getVVelocity(MyBoxNumber);
    }
	else if (strcmp(MyParameter, "U Flow") == 0)
    {
        Value = getUFlow(MyBoxNumber);
    }
    else if (strcmp(MyParameter, "V Flow") == 0)
    {
        Value = getVFlow(MyBoxNumber);
    }
	else if (strcmp(MyParameter, "Dynamic height") == 0)
		Value = DynamicHeight[MyBoxNumber];
	else if (strcmp(MyParameter, "Salinity") == 0)
		Value = Salt[MyBoxNumber];
    else if (strcmp(MyParameter, "Box depth") == 0)
		Value = BoxDepth[MyBoxNumber];
    else if (strcmp(MyParameter,"Water density") == 0)
       	Value = WaterDensity[MyBoxNumber];
    else if (strcmp(MyParameter,"Columns") == 0)
       	Value = float(ColumnIndex + 1.0); //A adição de 1 é para ter o número de colunas 1-based
    else if (strcmp(MyParameter,"Lines") == 0)
       	Value = float(GridLines - LineIndex);//A subtracção entre () destina-se a inverter o referencial vertical
                                              //para compatibilidade com o Noesys
    else if (strcmp(MyParameter, "Drag coefficient") == 0)
	    Value = CoefRugosidade(MyBoxNumber);

	else if (strcmp(MyParameter, "Mean U Velocity") == 0)
    {
        if (samples == 0)
            Value = 0;
        else
           	Value = MeanUVelocity[MyBoxNumber] / samples;
    }
    else if (strcmp(MyParameter, "Mean V Velocity") == 0)
    {
        if (samples == 0)
            Value = 0;
        else
		    Value = MeanVVelocity[MyBoxNumber] / samples;
    }
	else if (strcmp(MyParameter, "Mean U Flow") == 0)
    {
        if (samples == 0)
            Value = 0;
        else
	    	Value = MeanUFlow[MyBoxNumber] / samples;
    }
    else if (strcmp(MyParameter, "Mean V Flow") == 0)
    {
        if (samples == 0)
            Value = 0;
        else
    		Value = MeanVFlow[MyBoxNumber] / samples;
    }
    else
    {
        Value = 0;
        int j;
        for (j = 0; j < NumberOfVariables; j++)
        {
            if (strcmp(MyParameter, VariableNameArray[j]) == 0)
                break;
        }
        if (j == NumberOfVariables)
        {
#ifdef __BORLANDC__
            char Caption[129];
            strcpy(Caption, MyParameter);
            strcat(Caption, " does not exist in ");
            strcat(Caption, GetEcoDynClassName());
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - Inquiry 1",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
            cerr << "TBiDimensional::Inquiry 1 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
	}
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

//
// Inquiry if Mean variables exist...
//
VNA* TBiDimensional::Inquiry(char* srcName, int &TimeSeriesVariables)
{
    VNA* MyVarNameArray = new VNA[9];
    int n = 0;

    for (int i = 0; i < NumberOfVariables; i++)
    {
		if ((strcmp(VariableNameArray[i], "Mean U Velocity") == 0)
        	|| (strcmp(VariableNameArray[i], "Mean V Velocity") == 0)
        	|| (strcmp(VariableNameArray[i], "Mean U Flow") == 0)
        	|| (strcmp(VariableNameArray[i], "Mean V Flow") == 0)
           )
        {
            strcpy(MyVarNameArray[n], VariableNameArray[i]);
            n++;
        }
    }

    LogMessage("Inquiry", srcName, MyVarNameArray[0], n, 0);

    TimeSeriesVariables = n;
    return MyVarNameArray;
}

// AP, 2007.05.28
bool TBiDimensional::SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Dynamic height") == 0)
		DynamicHeight[BoxNumber] = Value;
	else if (strcmp(VariableName, "Salinity") == 0)
		Salt[BoxNumber] = Value;
    else if (strcmp(VariableName, "Box depth") == 0)
		BoxDepth[BoxNumber] = Value;
    else if (strcmp(VariableName,"Water density") == 0)
       	WaterDensity[BoxNumber] = Value;
    else if (strcmp(VariableName, "Flow discharge") == 0)
		FlowDischarge[BoxNumber] = Value;
    else if (strcmp(VariableName, "Flow uptake") == 0)
		FlowUptake[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}


void TBiDimensional::EquationOfMotion()    //Implicit scheme
{
   int i, j, k, index, indexN, indexS, indexW;
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
   //double aa, bb, cc, rr, rr1;
   if (SemiTimeStep == 1)
   {
/* AP, 2005.05.17...
 *
   	 for (int i = 0; i < GridLines; i++)
   	 {
   		for (int j = 0; j < GridColumns; j++)
      	{
 */
     for (i = pSubDomain->FirstLine;
          i <= pSubDomain->LastLine; i++)
     {
      for (j = pSubDomain->IColumn[i];
           j <= pSubDomain->FColumn[i]; j++)
        {
/* ...AP, 2005.05.17 */
            index = i * GridColumns + j;
            indexN = (i + 1) * GridColumns + j;
            indexS = (i + 1) * GridColumns + j;
            indexW = index - 1;

         	if (BoxDepth[index] > CriticalDepthForLandBoundary)
         	{
         		//Calculating intermediate depths and velocities

         		AUPStreamU = UPStreamUVelocity(i,j);
            	ADOWNStreamU = DOWNStreamUVelocity(i,j);
            	AUPStreamV = UPStreamVVelocity(i,j);
            	ADOWNStreamV = DOWNStreamVVelocity(i,j);
            	AUpperLeftDepth = UpperLeftCornerDepth(i,j);
            	ALowerLeftDepth = LowerLeftCornerDepth(i,j);
            	AUpperRightDepth = UpperRightCornerDepth(i,j);
            	ALowerRightDepth = LowerRightCornerDepth(i,j);


               if (
                     (j > 0) &&
            			(
                  		(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 0) ||//No western boundary
                    	(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)
                  	)  &&
                    (BoxDepth[index] > CriticalDepthForLandBoundary) &&
                    (BoxDepth[indexW] > CriticalDepthForLandBoundary) &&
                  	(BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
                  	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
                    (BoxArray[indexW].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
                    (BoxArray[indexW].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) //&&  //No southern ocean boundary
                  )
               {
               	//Cálculo do r - termo correspondente à velocidade na célula corrente no semi-passo anterior
                  if (
                  		(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1) ||
                        (BoxArray[indexW].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 1)
                      )
                  	rtemporary[index] = UVelocityPast[i * (GridColumns + 1) + j ];
                  else
               		rtemporary[index] =
                     	UVelocityPast[i * (GridColumns + 1) + j] +
                        VCoriolis(i,j) +
                        UVerticalDiffusion(i,j) -
                        UVAdvectionOfUEquation(i,j)+
                   		GravityOfRightSideOfUEquationOfMotion(i,j);


                  // Cálculo do a, b e c


               	  atemporary[index] = aUCalculation(i,j);
                  btemporary[index] = bUCalculation(i,j);
                  ctemporary[index] = cUCalculation(i,j);


                  if (BoxArray[indexW].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 2) //Left neighbour cell is a western ocean boundary
                  {
                  	btemporary[index] = btemporary[index] + atemporary[index] *
                                      (BoxDepth[indexW] +
                                       BoxDepth[index])
                                      / (2.0 * BoxDepth[indexW]);


                     double Parcel1;

                     Parcel1 = 2.0 * ASpatialStep * (DynamicHeight[indexW]  - PastHeight[indexW]) /
                              (ATimeStep * BoxDepth[indexW]);

                     if (
                        	(i < GridLines - 1) &&
                           (BoxDepth[indexN - 1] > 0.0)
                        )
                     	Parcel1 = Parcel1 + (BoxDepth[indexW] + BoxDepth[indexN - 1]) *
                     							VVelocityPast[indexN - 1] /
                           	            (2.0 * BoxDepth[indexW]);
                     else
                     	Parcel1 = Parcel1 + (BoxDepth[indexW] + BoxDepth[indexW]) *
                     							VVelocityPast[indexN - 1] /
                           	            (2.0 * BoxDepth[indexW]);
                     if (
                           (i > 0) &&
                           (BoxDepth[indexS - 1] > 0.0)
                        )

                        Parcel1 = Parcel1 - (BoxDepth[indexW] + BoxDepth[indexS - 1]) *
                                            VVelocityPast[indexW] /
                                            (2.0 * BoxDepth[indexW]);
                     else
                        Parcel1 = Parcel1 -  (BoxDepth[indexW] + BoxDepth[indexW]) *
                                            VVelocityPast[indexW] /
                                            (2.0 * BoxDepth[indexW]);

                     rtemporary[index] = rtemporary[index] - atemporary[index] * Parcel1;

                     atemporary[index] = 0.0;
                  }
                  else
                  if (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2) //Cell is a eastern ocean boundary
                  {

                  	btemporary[index] = btemporary[index] + ctemporary[index] *
                                        (BoxDepth[index] + BoxDepth[indexW]) /
                                        (2.0 * BoxDepth[index]);


                     double Parcel2;

                     Parcel2 = - 2.0 * ASpatialStep * (DynamicHeight[index] - PastHeight[index]) /
                              (ATimeStep * BoxDepth[index]);
                     if (
                        	(i < GridLines - 1) &&
                           (BoxDepth[indexN] > 0.0)
                        )
                        Parcel2 = Parcel2 - (BoxDepth[index] + BoxDepth[indexN]) *
                                             VVelocityPast[indexN] /
                                             (2.0 * BoxDepth[index]);
                     else
                        Parcel2 = Parcel2 - (BoxDepth[index] + BoxDepth[index]) *
                                             VVelocityPast[indexN] /
                                             (2.0 * BoxDepth[index]);
                     if (
                           (i > 0) &&
                           (BoxDepth[indexS] > 0.0)
                        )

                        Parcel2 = Parcel2 + (BoxDepth[index] + BoxDepth[indexS]) *
                        							VVelocityPast[index] /
                                             (2.0 * BoxDepth[index]);
                     else
                     	Parcel2 = Parcel2 + (BoxDepth[index] + BoxDepth[index]) *
                        							VVelocityPast[index] /
                                             (2.0 * BoxDepth[index]);


                     rtemporary[index] = rtemporary[index] - ctemporary[index] * Parcel2;


                     ctemporary[index] = 0.0;

                  }
               }
               else
               {
               	atemporary[index] = 0.0; btemporary[index] = 0.0;
                  ctemporary[index] = 0.0; rtemporary[index] = 0.0;
               }
            }
        } //j
   	 }    //i



             k = 0;
/* AP, 2005.05.17...
 *
             for (int i = 0; i < GridLines; i++)
             {
                for (int j = 0; j < GridColumns; j++)
                {
 */
             for (i = pSubDomain->FirstLine;
                  i <= pSubDomain->LastLine; i++)
             {
              for (j = pSubDomain->IColumn[i];
                   j <= pSubDomain->FColumn[i]; j++)
                {
/* ...AP, 2005.05.17 */
                    index = i * GridColumns + j;
                    indexW = index - 1;
   			      	if (
                  		(j > 0) &&
            				(
                  			(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 0) ||//No western boundary
                     		(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)
                  		)  &&
            				(BoxDepth[index] > CriticalDepthForLandBoundary) &&
            				(BoxDepth[indexW] > CriticalDepthForLandBoundary) &&
            				(BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
            				(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
            				(BoxArray[indexW].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
            				(BoxArray[indexW].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) //&&  //No southern ocean boundary
            				)
   						{

   							a[k] = atemporary[index];
      						b[k] = btemporary[index];
      						c[k] = ctemporary[index];
      						r[k] = rtemporary[index];

            				k++;
      					}
                 }
             }



   				IntegrationTridag(k);


               k = 0;
/* AP, 2005.05.17...
 *
               for (int i = 0; i < GridLines; i++)
                {
                    for (int j = 0; j < GridColumns; j++)
                    {
 */
               for (i = pSubDomain->FirstLine;
                    i <= pSubDomain->LastLine; i++)
                {
                    for (j = pSubDomain->IColumn[i];
                         j <= pSubDomain->FColumn[i]; j++)
                    {
/* ...AP, 2005.05.17 */
                        index = i * GridColumns + j;
                        indexN = i * (GridColumns + 1) + j;
   						if (
      					(j > 0) &&
            			(
                  		(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 0) ||//No western boundary
                     	(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)
                  	)  &&
            			(BoxDepth[index] > CriticalDepthForLandBoundary) &&
            			(BoxDepth[index - 1] > CriticalDepthForLandBoundary) &&
               			(BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
               			(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
               			(BoxArray[index - 1].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
            			(BoxArray[index - 1].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) //&&  //No southern ocean boundary
          				)
   						{
         					UVelocity[indexN] = u[k];
            				if (fabs(UVelocity[indexN]) < 0.00000000001)
            				UVelocity[indexN] = 0.0;
                        k++;
      					}
                    }
                }


/* AP, 2005.05.17...
 *
     for (int i = 0; i < GridLines; i++)
     {
        for (int j = 0; j < GridColumns; j++)
        {
 */
     for (i = pSubDomain->FirstLine;
          i <= pSubDomain->LastLine; i++)
     {
      for (j = pSubDomain->IColumn[i];
           j <= pSubDomain->FColumn[i]; j++)
        {
/* ...AP, 2005.05.17 */
            index = i * GridColumns + j;
            indexS = (i - 1) * GridColumns + j;
            indexN = (i + 1) * GridColumns + j;
         	if (BoxDepth[index] > CriticalDepthForLandBoundary)
         	{
         		//Calculating intermediate depths and velocities

         		AUPStreamU = UPStreamUVelocity(i,j);
            	ADOWNStreamU = DOWNStreamUVelocity(i,j);
            	AUPStreamV = UPStreamVVelocity(i,j);
            	ADOWNStreamV = DOWNStreamVVelocity(i,j);
            	AUpperLeftDepth = UpperLeftCornerDepth(i,j);
            	ALowerLeftDepth = LowerLeftCornerDepth(i,j);
            	AUpperRightDepth = UpperRightCornerDepth(i,j);
            	ALowerRightDepth = LowerRightCornerDepth(i,j);
                if (
               		(i > 0) &&
                     (
                     	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 0) ||//No southern boundary
                        (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)
                     ) &&
                     (BoxDepth[index] > CriticalDepthForLandBoundary) &&
                     (BoxDepth[indexS] > CriticalDepthForLandBoundary) &&
                   	 (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  	 (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) && //No eastern ocean boundary
                     (BoxArray[indexS].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                     (BoxArray[indexS].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) //&&  //No eastern ocean boundary

                     //De notar que este conjunto de condições lógicas não impede que seja calculada a velocidade
                     //explícita para uma célula que seja fronteira fluvial. No entanto, no final do passo de cálculo é imposta
                     //nessa célula a velocidade calculada a partir do caudal fluvial. Poderiam haver entretanto problemas no
                     //cálculo da continuidade e do transporte, mas não se passa nada de errado porque os referidos cálculos se baseiam
                     //nos termos explícitos passados (actualizados no final de cada passo de cálculo. Os implicitos são os presentes, mas quanto a esses
                     //não há problemas porque o a=0, o b=1 e o c=0 nas fronteiras fluviais pelo que o cálculo implicito da velocidade não
                     //muda o seu valor nas fronteiras fluviais.
                   )
                {

               	//vs  explicit
                  VVelocity[index] =
              			VVelocityPast[index] -
                     	ATimeStep / (2.0 * (BoxDepth[index] +
                     	BoxDepth[indexS])) *
                     	(
                        	BoxDepth[index] *
                          	(ADOWNStreamV + fabs(ADOWNStreamV)) / ASpatialStep *
                          	VVelocityPast[index] -
                          	BoxDepth[indexS] *
                          	(AUPStreamV + fabs(AUPStreamV)) / ASpatialStep *
                          	VVelocityPast[indexS] +
                          	BoxDepth[index] *
                          	(ADOWNStreamV - fabs(ADOWNStreamV)) / ASpatialStep *
                          	VVelocityPast[indexN] -
                          	BoxDepth[indexS] *
                          	(AUPStreamV - fabs(AUPStreamV)) / ASpatialStep *
                          	VVelocityPast[index]
                     	) -
                     	UVAdvectionOfVEquation(i,j) -
                       	ATimeStep / 2.0 * Gravity * (PastHeight[index] -
                       	PastHeight[indexS]) / ASpatialStep -
                       	ATimeStep / (2.0 * (BoxDepth[index] +
                        	                   BoxDepth[indexS])) *
                     	(
                       			CoefRugosidade(index) +
                   	 			CoefRugosidade(indexS)
                     	) *
                  	  	VForRugosity(i,j) *
                       	VVelocityPast[index] -
                        UCoriolis(i,j)
                       	+
                        VVerticalDiffusion(i,j) + VHorizontalDiffusion(i,j);
                }
            }

        } //j
    }    //i
   }       //First semi-timestep

            //Second semi-timestep

   else if (SemiTimeStep == 2)
   {
/* AP, 2005.05.17...
 *
     for (int i = 0; i < GridLines; i++)
     {
        for (int j = 0; j < GridColumns; j++)
        {
 */
     for (i = pSubDomain->FirstLine;
          i <= pSubDomain->LastLine; i++)
     {
      for (j = pSubDomain->IColumn[i];
           j <= pSubDomain->FColumn[i]; j++)
        {
/* ...AP, 2005.05.17 */
            index = i * GridColumns + j;
            indexS = (i - 1) * GridColumns + j;
         	if (BoxDepth[index] > CriticalDepthForLandBoundary)
         	{
         		//Calculating intermediate depths and velocities

         		AUPStreamU = UPStreamUVelocity(i,j);
            	//Debugger(AUPStreamU);
            	ADOWNStreamU = DOWNStreamUVelocity(i,j);
            	//Debugger(ADOWNStreamU);
            	AUPStreamV = UPStreamVVelocity(i,j);
            	//Debugger(AUPStreamV);
            	ADOWNStreamV = DOWNStreamVVelocity(i,j);
            	//Debugger(ADOWNStreamV);
            	AUpperLeftDepth = UpperLeftCornerDepth(i,j);
            	ALowerLeftDepth = LowerLeftCornerDepth(i,j);
            	AUpperRightDepth = UpperRightCornerDepth(i,j);
            	ALowerRightDepth = LowerRightCornerDepth(i,j);
               //vs  implicit
               if (
                     (i > 0) &&
                     (
                     	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 0) ||//No southern boundary
                        (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)
                     ) &&
                     (BoxDepth[index] > CriticalDepthForLandBoundary) &&
                     (BoxDepth[indexS] > CriticalDepthForLandBoundary) &&
                  	(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  	(BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) &&  //No eastern ocean boundary
                     (BoxArray[indexS].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                     (BoxArray[indexS].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) //&&  //No eastern ocean boundary
                     //(BoxArray[index].Dummybool1)
                  )
               {
               	//Cálculo do r - termo correspondente à velocidade na célula corrente no semi-passo anterior
                 if (
                 			(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1) ||
                        (BoxArray[indexS].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 1)
                     )
                  rtemporary[index] = VVelocityPast[index];
                 else
                 {
                 	rtemporary[index] =
                     	VVelocityPast[index] -
                        UCoriolis(i,j) +
                        VHorizontalDiffusion(i,j) -
                        UVAdvectionOfVEquation(i,j) +
                   		GravityOfRightSideOfVEquationOfMotion(i,j);

                 }
                 // Cálculo do a, b e c
                 atemporary[index] = aVCalculation(i,j);
                 btemporary[index] = bVCalculation(i,j);
                 ctemporary[index] = cVCalculation(i,j);

                 if (BoxArray[indexS].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2) //South neighbour cell is a southern ocean boundary
                 {
                  	btemporary[index] = btemporary[index] + atemporary[index] *
                                      (BoxDepth[indexS] +
                                       BoxDepth[index])
                                      / (2.0 * BoxDepth[indexS]);

                     double Parcel3;

                     Parcel3 = 2.0 * ASpatialStep * (DynamicHeight[indexS]  - PastHeight[indexS]) /
                              (ATimeStep * BoxDepth[indexS]);

                     if (
                     		(j > 0) &&
                           (BoxDepth[(i - 1) * GridColumns + j - 1] > 0.0)
                        )
                        Parcel3 = Parcel3 - (BoxDepth[indexS] + BoxDepth[indexS - 1]) *
                                             UVelocityPast[(i - 1) * (GridColumns + 1) + j] /
                                             (2.0 * BoxDepth[indexS]);
                     else
                     	Parcel3 = Parcel3 - (BoxDepth[indexS] + BoxDepth[indexS]) *
                                             UVelocityPast[(i - 1) * (GridColumns + 1) + j] /
                                             (2.0 * BoxDepth[indexS]);
                     if (
                     		(j < GridColumns - 1) &&
                           (BoxDepth[indexS + 1] > 0.0)
                        )
                        Parcel3 = Parcel3 + (BoxDepth[indexS] + BoxDepth[indexS + 1]) *
                                             UVelocityPast[(i - 1) * (GridColumns + 1) + j + 1] /
                                             (2.0 * BoxDepth[indexS]);
                     else
                     	Parcel3 = Parcel3 + (BoxDepth[indexS] + BoxDepth[indexS]) *
                                             UVelocityPast[(i - 1) * (GridColumns + 1) + j + 1] /
                                             (2.0 * BoxDepth[indexS]);

                     rtemporary[index] = rtemporary[index] - atemporary[index] *  Parcel3;



                     atemporary[index] = 0.0;
                  }
                  else
                  if (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 2) //Cell is a northern ocean boundary
                  {
                  	btemporary[index] = btemporary[index] + ctemporary[index] *
                                        (BoxDepth[index] + BoxDepth[indexS]) /
                                        (2.0 * BoxDepth[index]);

                     double Parcel4;

                     Parcel4 = - 2.0 * ASpatialStep * (DynamicHeight[index]  - PastHeight[index]) /
                              (ATimeStep * BoxDepth[index]);


                     if (
                     		(j > 0) &&
                           (BoxDepth[index - 1] > 0.0)
                        )
                        Parcel4 = Parcel4 + (BoxDepth[index] + BoxDepth[index - 1]) *
                                             UVelocityPast[i * (GridColumns + 1) + j] /
                                             (2.0 * BoxDepth[index]);
                     else
                     	Parcel4 = Parcel4 + (BoxDepth[index] + BoxDepth[index]) *
                                             UVelocityPast[i * (GridColumns + 1) + j] /
                                             (2.0 * BoxDepth[index]);
                     if (
                     		(j < GridColumns - 1) &&
                           (BoxDepth[index + 1] > 0.0)
                        )
                        Parcel4 = Parcel4 - (BoxDepth[index] + BoxDepth[index + 1]) *
                                             UVelocityPast[i * (GridColumns + 1) + j + 1] /
                                             (2.0 * BoxDepth[index]);
                     else
                        Parcel4 = Parcel4 - (BoxDepth[index] + BoxDepth[index]) *
                                             UVelocityPast[i * (GridColumns + 1) + j + 1] /
                                             (2.0 * BoxDepth[index]);

                     rtemporary[index] = rtemporary[index] - ctemporary[index] * Parcel4;



                     ctemporary[index] = 0.0;
                  }
               }
               else
               {
               	atemporary[index] = 0.0; btemporary[index] = 0.0;
                  ctemporary[index] = 0.0; rtemporary[index] = 0.0;
               }

            }
        } //j
    }    //i

               k = 0;
/* AP, 2005.05.17...
 *
                for (int i = 0; i < GridColumns; i++)
                {
                    for (int j = 0; j < GridLines; j++)
                    {
 */
                for (j = pSubDomain->FirstColumn;
                     j <= pSubDomain->LastColumn; j++)
                {
                    for (i = pSubDomain->ILine[j];
                         i <= pSubDomain->FLine[j]; i++)
                    {
/* ...AP, 2005.05.17 */
                        index = i * GridColumns + j;
                        indexS = (i - 1) * GridColumns + j;
   						if (
      					(i > 0) &&
                  	(
                  		(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 0) ||//No southern boundary
                     	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)
                  	) &&
                  	(BoxDepth[index] > CriticalDepthForLandBoundary) &&
                  	(BoxDepth[indexS] > CriticalDepthForLandBoundary) &&
                  	(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  	(BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2)&& //No eastern ocean boundary
                  	(BoxArray[indexS].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  	(BoxArray[indexS].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) //&&  //No eastern ocean boundary
          				)
   						{
   							a[k] = atemporary[index];
      						b[k] = btemporary[index];
      						c[k] = ctemporary[index];
      						r[k] = rtemporary[index];
            				k++;

      					}
                    }
   				}

   				IntegrationTridag(k);

      			k = 0;

/* AP, 2005.05.17...
 *
                for (int i = 0; i < GridColumns; i++)
                {
                    for (int j = 0; j < GridLines; j++)
                    {
 */
                for (j = pSubDomain->FirstColumn;
                     j <= pSubDomain->LastColumn; j++)
                {
                    for (i = pSubDomain->ILine[j];
                         i <= pSubDomain->FLine[j]; i++)
                    {
/* ...AP, 2005.05.17 */
                        index = i * GridColumns + j;
                        indexS = (i - 1) * GridColumns + j;
   						if (
      					(i > 0) &&
                  	(
                  		(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 0) ||//No southern boundary
                     	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)
                  	) &&
                  	(BoxDepth[index] > CriticalDepthForLandBoundary) &&
                  	(BoxDepth[indexS] > CriticalDepthForLandBoundary) &&
                  	(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  	(BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) && //No eastern ocean boundary
                  	(BoxArray[indexS].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  	(BoxArray[indexS].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2)//&&   //No eastern ocean boundary
          				)
   						{

         					VVelocity[index] = u[k];

                  		if (fabs(VVelocity[index]) < 0.00000000001)
               			VVelocity[index] = 0.0;
            				k++;
      					}
                    }
   				}
               //us explicit
/* AP, 2005.05.17...
 *
     for (int i = 0; i < GridLines; i++)
     {
   	     for (int j = 0; j < GridColumns; j++)
         {
 */

     for (i = pSubDomain->FirstLine;
          i <= pSubDomain->LastLine; i++)
     {
         for (j = pSubDomain->IColumn[i];
              j <= pSubDomain->FColumn[i]; j++)
         {
/* ...AP, 2005.05.17 */
            index = i * GridColumns + j;
            indexN = i * (GridColumns + 1) + j;
            if (BoxDepth[index] > CriticalDepthForLandBoundary)
            {
         	   //Calculating intermediate depths and velocities

         		AUPStreamU = UPStreamUVelocity(i,j);
            	//Debugger(AUPStreamU);
            	ADOWNStreamU = DOWNStreamUVelocity(i,j);
            	//Debugger(ADOWNStreamU);
            	AUPStreamV = UPStreamVVelocity(i,j);
            	//Debugger(AUPStreamV);
            	ADOWNStreamV = DOWNStreamVVelocity(i,j);
            	//Debugger(ADOWNStreamV);
            	AUpperLeftDepth = UpperLeftCornerDepth(i,j);
            	ALowerLeftDepth = LowerLeftCornerDepth(i,j);
            	AUpperRightDepth = UpperRightCornerDepth(i,j);
            	ALowerRightDepth = LowerRightCornerDepth(i,j);
               if (
               		(j > 0) &&
            			(
                  		(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 0) ||//No western boundary
                     	(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)
                  	)  &&
                     (BoxDepth[index] > CriticalDepthForLandBoundary) &&
                     (BoxDepth[index - 1] > CriticalDepthForLandBoundary) &&
                  	(BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
                  	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
                     (BoxArray[index - 1].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
                     (BoxArray[index - 1].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) //&&   //No southern ocean boundary
               	)
               {
                   	  	UVelocity[indexN] =
                       		UVelocityPast[indexN] -
                       		ATimeStep / (2.0 * (BoxDepth[index] +
                        		  BoxDepth[index - 1])
                       		) *
                       		(
                       		  	BoxDepth[index] *
                          		(ADOWNStreamU + fabs(ADOWNStreamU)) / ASpatialStep *
                          		UVelocityPast[indexN] -
                          		BoxDepth[index - 1] *
                          		(AUPStreamU + fabs(AUPStreamU)) / ASpatialStep *
                          		UVelocityPast[indexN - 1] +
                          		BoxDepth[index] *
                          		(ADOWNStreamU - fabs(ADOWNStreamU)) / ASpatialStep *
                          		UVelocityPast[indexN + 1] -
                          		BoxDepth[index - 1] *
                          		(AUPStreamU - fabs(AUPStreamU)) / ASpatialStep *
                          		UVelocityPast[indexN]
                           ) -
                           UVAdvectionOfUEquation(i,j) -
                       		ATimeStep / 2.0 * Gravity * (PastHeight[index] -
                       		PastHeight[index - 1]) / ASpatialStep -
                       		ATimeStep / (2.0 * (BoxDepth[index] +
                        		                   BoxDepth[index - 1])) *
                       		(
                       				CoefRugosidade(index) +
                   	 				CoefRugosidade(index - 1)
                       		) *
                  	  		UForRugosity(i,j) *
                       		UVelocityPast[indexN] +
                           VCoriolis(i,j)
                       	 	+
                           UHorizontalDiffusion(i,j) + UVerticalDiffusion(i,j);
               }
           	}
         }  //j
      } //i
	} // Second semi-timestep
}

void TBiDimensional::Continuity()
{
	double MyWestUFlow, MyEastUFlow, MyNorthVFlow, MySouthVFlow;
    int i, j, index;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();

/* AP, 2005.05.17...
 *
   //Debugger(SemiTimeStep);
   for (int i = 0; i < GridLines; i++)
   {
   	for (int j = 0; j < GridColumns; j++)
      {
 */

   for (i = pSubDomain->FirstLine;
        i <= pSubDomain->LastLine; i++)
   {
   	for (j = pSubDomain->IColumn[i];
         j <= pSubDomain->FColumn[i]; j++)
      {
/* ...AP, 2005.05.17 */
        index = i * GridColumns + j;
      	if (
         		(BoxDepth[index] > CriticalDepthForLandBoundary) &&
               (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 2) &&
               (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 2) &&
               (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 2) &&
               (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 2)
            )
         {
         	MyWestUFlow = WesternComponentOfContinuity(i,j);
            MyEastUFlow = EasternComponentOfContinuity(i,j);
            MyNorthVFlow = NorthernComponentOfContinuity(i,j);
            MySouthVFlow = SouthernComponentOfContinuity(i,j);


            DynamicHeight[i * GridColumns + j ] = (2.0 * ASpatialStep *
                                                   	PastHeight[index] +
                                                   	MyWestUFlow -
                                                   	MyEastUFlow -
                                                   	MyNorthVFlow +
                                                   	MySouthVFlow
                                                   ) /
                                                   (2.0 * ASpatialStep);
      	}
     }
	}
   GenericLoad = SaltLoad;
   AdvectDiffuse(Salt);

/* AP, 2005.05.17...
 *
   for (int i = 0; i < GridLines; i++)
   {
   	for (int j = 0; j < GridColumns; j++)
      {
 */

   for (i = pSubDomain->FirstLine;
        i <= pSubDomain->LastLine; i++)
   {
   	for (j = pSubDomain->IColumn[i];
         j <= pSubDomain->FColumn[i]; j++)
      {
/* ...AP, 2005.05.17 */
        index = i * GridColumns + j;
      	if (
         		(BoxDepth[index] > CriticalDepthForLandBoundary)
            )
         {
      		BoxDepth[index] = BoxDepth[index] +
                              (DynamicHeight[index] - PastHeight[index]);
         }
         else
         if (BoxDepth[index] <= CriticalDepthForLandBoundary)
         {
         	UVelocity[i * (GridColumns + 1) + j] = 0.0;
            VVelocity[index] = 0.0;
            UVelocity[i * (GridColumns + 1) + j + 1] = 0.0;
            VVelocity[(i + 1) * GridColumns + j] = 0.0;
         }
      }
	}
}


void TBiDimensional::AdvectDiffuse(double* Generic)
{
	double MyWestUFlow, MyEastUFlow, MyNorthVFlow, MySouthVFlow,
   		 MyWestDiffusion, MyEastDiffusion, MyNorthDiffusion, MySouthDiffusion;
    int i, j, index;
    double divider;

    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    double* PastGeneric = new double[NumberOfBoxes];

/* AP, 2005.05.17...
 *
   //Debugger(SemiTimeStep);
   for (int i = 0; i < NumberOfBoxes; i++)
        PastGeneric[i] = Generic[i];

   for (int i = 0; i < GridLines; i++)
   {
   	for (int j = 0; j < GridColumns; j++)
      {
 */

   for (i = 0; i < NumberOfBoxes; i++)
       PastGeneric[i] = Generic[i];

   for (i = pSubDomain->FirstLine;
        i <= pSubDomain->LastLine; i++)
   {
   	for (j = pSubDomain->IColumn[i];
         j <= pSubDomain->FColumn[i]; j++)
      {
/* ...AP, 2005.05.17 */

        index = i * GridColumns + j;
      	if (
         		(BoxDepth[index] > CriticalDepthForLandBoundary) &&
               (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 2) &&
               (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 2) &&
               (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 2) &&
               (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 2) &&
               (BoxArray[index].Dummybool1)
            )
         {
         	//Advective fluxes
         	MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGeneric);
            MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGeneric);
            MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGeneric);
            MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGeneric);

            //Diffusive fluxes
            MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGeneric);
            MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGeneric);
            MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGeneric);
            MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGeneric);

			divider = BoxDepth[index] + DynamicHeight[index] - PastHeight[index];

            Generic[index] = PastGeneric[index]  *
                                            BoxDepth[index] /
                                            (divider)
                                            +
                                            (
                                                MyWestUFlow -
                                                MyEastUFlow -
                                                MyNorthVFlow +
                                                MySouthVFlow
                                            ) /
                                            (2.0 * ASpatialStep) /
                                            (divider)
                                            +
                                            (
                                            		MyWestDiffusion +
                                                MyEastDiffusion +
                                                MyNorthDiffusion +
                                                MySouthDiffusion
                                            ) /
                                            (2.0 * ASpatialStep * ASpatialStep) /
                                            (divider);

      	}
     }
	}
    delete [] PastGeneric;
}


void TBiDimensional::TideForcing()
{
/*	MessageBox(0,
					  "Hydrodynamic bidimensional object",
					  "Always override Tide Forcing",
					  MB_OK | MB_ICONHAND); */
}


void TBiDimensional::RiverForcing()
{
/*	MessageBox(0,
					  "Hydrodynamic bidimensional object",
					  "Always override River Forcing",
					  MB_OK | MB_ICONHAND);*/
}






void TBiDimensional::UpdateOfTidalBoundariesVelocities()
{
    int index;

	for (int i = 0; i < GridLines; i++)
   {
   	for (int j = 0; j < GridColumns; j++)
      {
        index = i * GridColumns + j;
   		if (
         		(BoxArray[index].BoxToBoundary[1].TypeOfBoundary == 2) ||
               (BoxArray[index].BoxToBoundary[2].TypeOfBoundary == 2) ||
               (BoxArray[index].BoxToBoundary[3].TypeOfBoundary == 2) ||
               (BoxArray[index].BoxToBoundary[4].TypeOfBoundary == 2)
   			)
      	{

         	if ((BoxArray[index].BoxToBoundary[1].TypeOfBoundary == 2) && (i > 0) && (i < GridLines))  //Northern sea boundary
            	VVelocity[(i + 1) * GridColumns + j] = (
            			                               		VVelocity[index] *
                                                      	(BoxDepth[(i - 1) * GridColumns + j] + BoxDepth[index]) / 2.0 -
                                                      	(DynamicHeight[index] - PastHeight[index]) *
                                                         ASpatialStep / (ATimeStep/ 2.0)
                                                   	) / BoxDepth[index];

            else if ((BoxArray[index].BoxToBoundary[3].TypeOfBoundary == 2) && (i < GridLines))  //Southern sea boundary
               VVelocity[i * GridColumns + j] =       (
            			                               		VVelocity[(i + 1) * GridColumns + j] *
                                                      	(BoxDepth[(i + 1) * GridColumns + j] + BoxDepth[index]) / 2.0 +
                                                      	(DynamicHeight[index] - PastHeight[index]) *
                                                         ASpatialStep / (ATimeStep/ 2.0)
                                                   	) / BoxDepth[index];

            //Nesta versão das velocidades nas fronteiras já não há o problema da anterior, pois calculo as velocidades em t+1
            // e não centradas no tempo

            else if  ((BoxArray[index].BoxToBoundary[2].TypeOfBoundary == 2) &&  (j > 0) && (j < GridColumns)) //Eastern sea boundary
               UVelocity[i * (GridColumns + 1) + j + 1] =   (
            			                               				UVelocity[i * (GridColumns + 1) + j] *
                                                      			(BoxDepth[index - 1] + BoxDepth[index]) / 2.0 -
                                                      			(DynamicHeight[index] - PastHeight[index]) *
                                                         		ASpatialStep / (ATimeStep/ 2.0)
                                                   			) / BoxDepth[index];

            else if  ((BoxArray[index].BoxToBoundary[4].TypeOfBoundary == 2) && (j < GridColumns)) //Western sea boundary
               UVelocity[i * (GridColumns + 1) + j] =   		(
            			                               				UVelocity[i * (GridColumns + 1) + j + 1] *
                                                      			(BoxDepth[index + 1] + BoxDepth[index]) / 2.0 +
                                                      			(DynamicHeight[index] - PastHeight[index]) *
                                                         		ASpatialStep / (ATimeStep/ 2.0)
                                                   			) / BoxDepth[index];


         }
      }
   }
}


void TBiDimensional::TransportEquation()
{

}



double TBiDimensional::UPStreamUVelocity(int i, int j) //Calcula o Ui,j-1/2           ok
{
    int index = i * GridColumns + j;
	if ((i < 0) || (i >= GridLines) || (j < 0) || (j >= GridColumns))
#ifdef __BORLANDC__
       	MessageBox(0,
	    		  "Erro in TBiDimensional::UPStreamUVelocity",
				  "EcoDynamo alert",
				  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TBiDimensional::UPStreamUVelocity - erro" << endl;
#endif  // __BORLANDC__
	else if (j > 0) //Implica que a caixa n?o constitu? limite da malha a oeste
    {
		if (
      		(BoxDepth[index - 1] <= CriticalDepthForLandBoundary) ||
         	(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 4)
         ) //Ligação com terra a oeste
   		//return UVelocityPast[i * (GridColumns + 1) + j] / 2.0;
            return 0.0;
   	    else
   		    return (
         			UVelocityPast[i * (GridColumns + 1) + j - 1] +
                  UVelocityPast[i * (GridColumns + 1) + j]
                ) / 2.0;
   }
   else if (j == 0)  //Implica que a caixa constituí limite da malha a oeste
   {
   	if (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 4)
      	return 0.0;
      else
      	return UVelocityPast[i * (GridColumns + 1) + j];//No caso de a fronteira ser liquída devolver a velocidade da própria caixa
   }
    return 0.0;
}


double TBiDimensional::UPStreamVVelocity(int i, int j)  //Calcula o Vi-1/2,j         ok
{
	if ((i < 0) || (i >= GridLines) || (j < 0) || (j >= GridColumns))
#ifdef __BORLANDC__
   	MessageBox(0,
					  "Erro in TBiDimensional::UPStreamVVelocity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
    cerr << "TBiDimensional::UPStreamVVelocity - erro" << endl;
#endif  // __BORLANDC__
	else if (i > 0) //Implica que a caixa n?o constitu? limite da malha a sul
   {
		if(
      		(BoxDepth[(i - 1) * GridColumns + j] <= CriticalDepthForLandBoundary) || //Fronteira com terra
            (BoxArray[i * GridColumns + j].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 4)
        )
        //return VVelocityPast[i * GridColumns + j] / 2.0;
        return 0.0;
   	else
        return (
                  VVelocityPast[(i - 1) * GridColumns + j] +
         			VVelocityPast[i * GridColumns + j]
                ) / 2.0;
   }
   else if (i == 0)  //Implica que a caixa constituí limite da malha a sul
   {
   	if (BoxArray[i * GridColumns + j].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 4)
        return 0.0;
      else
        return VVelocityPast[i * GridColumns + j];//No caso de a fronteira ser liquída devolver a velocidade da própria caixa
   }
    return 0.0;
}


double TBiDimensional::DOWNStreamUVelocity(int i, int j)  //Calcula o Ui,j+1/2          ok
{
	if ((i < 0) || (i >= GridLines) || (j < 0) || (j >= GridColumns))
#ifdef __BORLANDC__
   	MessageBox(0,
					  "Erro in TBiDimensional::DOWNStreamUVelocity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
    cerr << "TBiDimensional::DOWNStreamUVelocity - erro" << endl;
#endif  // __BORLANDC__
	else if (j < GridColumns - 1)
   {
      if(
      		(BoxDepth[i * GridColumns + j + 1] <= CriticalDepthForLandBoundary) || //Fronteira com terra
            (BoxArray[i * GridColumns + j ].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 4)
        )
   		return UVelocityPast[i * (GridColumns + 1) + j] / 2.0;
   	else
   		return (
         			UVelocityPast[i * (GridColumns + 1) + j] +
                  UVelocityPast[i * (GridColumns + 1) + j + 1]
                ) / 2.0;
   }
   else if (j == GridColumns - 1)
   {

			return (
         			UVelocityPast[i * (GridColumns + 1) + j] +
                  UVelocityPast[i * (GridColumns + 1) + j + 1]
                ) / 2.0;
   }
    return 0.0;
}


double TBiDimensional::DOWNStreamVVelocity(int i, int j)  //Calcula o Vi+1/2,j            ok
{
	if ((i < 0) || (i >= GridLines) || (j < 0) || (j >= GridColumns))
#ifdef __BORLANDC__
   	MessageBox(0,
					  "Erro in TBiDimensional::DownStreamVVelocity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
    cerr << "TBiDimensional::DownStreamVVelocity - erro" << endl;
#endif  // __BORLANDC__
	else
   if (i < GridLines - 1)
   {
		if (
      		(BoxDepth[(i + 1) * GridColumns + j] <= CriticalDepthForLandBoundary) ||
            (BoxArray[i * GridColumns + j ].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 4)
         )
         return VVelocityPast[i * GridColumns + j] / 2.0;
   	else
   		return (
                  VVelocityPast[i * GridColumns + j] +
         			VVelocityPast[(i + 1) * GridColumns + j]
                ) / 2.0;
   }
   if (i == GridLines - 1)
   {

   		return (
      				VVelocityPast[i * GridColumns + j] +
               	VVelocityPast[(i + 1) * GridColumns + j]
             	 ) / 2.0;
   }
    return 0.0;
}


double TBiDimensional::UpperUVelocity(int i, int j)   //Calcula o Ui+1/2,j    ok
{
   if ((i < 0) || (i >= GridLines) || (j < 0) || (j > GridColumns))
#ifdef __BORLANDC__
   	MessageBox(0,
					  "Erro in TBiDimensional::UpperUVelocity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
    cerr << "TBiDimensional::UpperUVelocity- erro" << endl;
#endif  // __BORLANDC__
	else
   {
		if (
      		(i == GridLines - 1) ||
            (
            	(j < GridColumns) &&
            	(i < GridLines - 1) &&
               (BoxDepth[(i + 1) * GridColumns + j] <= CriticalDepthForLandBoundary)
            )                    ||
           (
            	(j == GridColumns) &&
            	(i < GridLines - 1) &&
               (BoxDepth[(i + 1) * GridColumns + j - 1] <= CriticalDepthForLandBoundary)
            )
         )
         return UVelocityPast[i * (GridColumns + 1) + j]; //Se a superfície lateral da parede for < 10% da superfície horizontal
                                                    //da célula o fluxo difusivo é comparável à tensão de corte junto ao fundo e
                                                    //pode ser desprezado (Tese do Ramiro, pág. 144).
                                                    //Se a superfície lateral for muito grande, então deve utilizar-se a
                                                    //condição de "non-glissment" ou "no slipping" (Tese do Ramiro, pág. 144).
   	else
   		return (
                  UVelocityPast[i * (GridColumns + 1) + j] +
         			UVelocityPast[(i + 1) * (GridColumns + 1) + j]
                ) / 2.0;
   }
    return 0.0;
}



double TBiDimensional::LowerUVelocity(int i, int j)  //Calcula o Ui-1/2,j      ok
{
	if ((i < 0) || (i >= GridLines) || (j < 0) || (j > GridColumns))
   {
#ifdef __BORLANDC__
   	MessageBox(0,
					  "Erro in TBiDimensional::LowerUVelocity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
    cerr << "TBiDimensional::LowerUVelocity - erro" << endl;
#endif  // __BORLANDC__
   	//Debugger(MyPIntegrate->GetCurrentTime());
      Debugger(i); Debugger(j);
   }
	else
   {
		if (
      		(i == 0) ||
            (
            	(j < GridColumns) &&
            	(i > 0) &&
               (BoxDepth[(i - 1) * GridColumns + j] <= CriticalDepthForLandBoundary) //Fronteira com terra
            )        ||
            (
            	(j == GridColumns)&&
            	(i > 0) &&
               (BoxDepth[(i - 1) * GridColumns + j - 1] <= CriticalDepthForLandBoundary) //Fronteira com terra
            )
         )

      		return UVelocityPast[i * (GridColumns + 1) + j];//Se a superfície lateral da parede for < 10% da superfície horizontal
                                                            //da célula o fluxo difusivo é comparável à tensão de corte junto ao fundo e
                                                            //pode ser desprezado (Tese do Ramiro, pág. 144).
                                                            //Se a superfície lateral for muito grande, então deve utilizar-se a
                                                            //condição de "non-glissment" ou "no slipping" (Tese do Ramiro, pág. 144).

   		else
        		return (
                  	UVelocityPast[(i - 1) * (GridColumns + 1) + j] +
         				UVelocityPast[i * (GridColumns + 1) + j]
                	  ) / 2.0;
   }
    return 0.0;
}


double TBiDimensional::LeftVVelocity(int i, int j)   //Calcula o Vi,j-1/2        ok
{
	if ((i < 0) || (i > GridLines) || (j < 0) || (j >= GridColumns))
   {
#ifdef __BORLANDC__
   	MessageBox(0,
					  "Erro in TBiDimensional::LeftVVelocity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
    cerr << "TBiDimensional::LeftVVelocity - erro" << endl;
#endif  // __BORLANDC__
      //Debugger(i); Debugger(j);
   }
	else
   {
		if (
      		(j == 0) ||
            (
            	(i < GridLines) &&
            	(j > 0) &&
               (BoxDepth[i * GridColumns + j - 1] <= CriticalDepthForLandBoundary)
            )        ||
            (
            	(i == GridLines) &&
            	(j > 0) &&
               (BoxDepth[(i - 1) * GridColumns + j - 1] <= CriticalDepthForLandBoundary)
            )

         ) //Ligação com terra a oeste
   		return VVelocityPast[i * GridColumns + j];//Se a superfície lateral da parede for < 10% da superfície horizontal
                                                    //da célula o fluxo difusivo é comparável à tensão de corte junto ao fundo e
                                                    //pode ser desprezado (Tese do Ramiro, pág. 144).
                                                    //Se a superfície lateral for muito grande, então deve utilizar-se a
                                                    //condição de "non-glissment" ou "no slipping" (Tese do Ramiro, pág. 144).

   	else
      {
      	/*if ((i * GridColumns + j > 23) && (MyPIntegrate->GetCurrentTime() > 0.068))
         {
         	Debugger(1000);
         	Debugger(VVelocityPast[i * GridColumns + j ]);
         	Debugger(VVelocityPast[i * GridColumns + j - 1]);

         }  */
   		return (
         			VVelocityPast[i * GridColumns + j - 1] +
                  VVelocityPast[i * GridColumns + j ]
                ) / 2.0;
      }
   }
    return 0.0;
}


double TBiDimensional::RightVVelocity(int i, int j) //Calcula o Vi,j+1/2          ok
{
	if ((i < 0) || (i > GridLines) || (j < 0) || (j >= GridColumns))
#ifdef __BORLANDC__
   	MessageBox(0,
					  "Erro in TBiDimensional::RightVVelocity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
    cerr << "TBiDimensional::RightVVelcocity - erro" << endl;
#endif  // __BORLANDC__
	else
   {
      if(
      		(j == GridColumns - 1) ||
            (
            	(i < GridLines) &&
      	   	(i < GridColumns - 1) &&
               (BoxDepth[i * GridColumns + j + 1] <= CriticalDepthForLandBoundary)
            )                      ||
            (
            	(i == GridLines) &&
      	   	(i < GridColumns - 1) &&
               (BoxDepth[(i - 1) * GridColumns + j + 1] <= CriticalDepthForLandBoundary)
            )
        )
   		return VVelocityPast[i * GridColumns + j ] ;//Se a superfície lateral da parede for < 10% da superfície horizontal
                                                     //da célula o fluxo difusivo é comparável à tensão de corte junto ao fundo e
                                                     //pode ser desprezado (Tese do Ramiro, pág. 144).
                                                     //Se a superfície lateral for muito grande, então deve utilizar-se a
                                                     //condição de "non-glissment" ou "no slipping" (Tese do Ramiro, pág. 144).

   	else
   		return (
         			VVelocityPast[i * GridColumns + j ] +
                  VVelocityPast[i * GridColumns + j + 1]
                ) / 2.0;
   }
    return 0.0;
}




double TBiDimensional::LowerLeftCornerUVelocity(int i, int j)  //Ui-1/2,j-1/2   ok
{
	//Debugger((LowerUVelocity(i,j-1) + LowerUVelocity(i,j)) / 2);
	return (LowerUVelocity(i,j-1) + LowerUVelocity(i,j)) / 2.0;
}

double TBiDimensional::UpperLeftCornerUVelocity(int i, int j)  //Ui+1/2,j-1/2    ok
{
	//Debugger((UpperUVelocity(i,j-1) + UpperUVelocity(i,j)) / 2);
	return (UpperUVelocity(i,j-1) + UpperUVelocity(i,j)) / 2.0;
}

double TBiDimensional::LowerRightCornerUVelocity(int i, int j) //Ui-1/2,j+1/2   ok
{
	//Debugger((LowerUVelocity(i,j) + LowerUVelocity(i,j+1)) / 2);
	return (LowerUVelocity(i,j) + LowerUVelocity(i,j+1)) / 2.0;
}

double TBiDimensional::UpperRightCornerUVelocity(int i, int j) //Ui+1/2,j+1/2     ok
{
	//Debugger((UpperUVelocity(i,j) + UpperUVelocity(i,j+1)) / 2);
	return (UpperUVelocity(i,j) + UpperUVelocity(i,j+1)) / 2.0;
}

double TBiDimensional::LowerLeftCornerVVelocity(int i, int j) //Vi-1/2,j-1/2       ok
{
	//Debugger((LeftVVelocity(i-1,j) + LeftVVelocity(i,j)) / 2);
	return (LeftVVelocity(i-1,j) + LeftVVelocity(i,j)) / 2.0;
}

double TBiDimensional::UpperLeftCornerVVelocity(int i, int j) //Vi+1/2,j-1/2         ok
{
	//Debugger((LeftVVelocity(i+1,j) + LeftVVelocity(i,j)) / 2.0);
	return (LeftVVelocity(i+1,j) + LeftVVelocity(i,j)) / 2.0;
}

double TBiDimensional::LowerRightCornerVVelocity(int i, int j) //Vi-1/2,j+1/2       ok
{
	//Debugger((RightVVelocity(i-1,j) + RightVVelocity(i,j)) / 2.0);
	return (RightVVelocity(i-1,j) + RightVVelocity(i,j)) / 2.0;
}

double TBiDimensional::UpperRightCornerVVelocity(int i, int j) //Vi-1/2,j+1/2        ok
{
	//Debugger((RightVVelocity(i,j) + RightVVelocity(i+1,j)) / 2.0);
	return (RightVVelocity(i,j) + RightVVelocity(i+1,j)) / 2.0;
}


double TBiDimensional::UpperLeftCornerDepth(int i, int j)      //ok
{
	double SumOfSurroundingDepths, NumberOfParcels;
   SumOfSurroundingDepths = 0;
   NumberOfParcels = 0;

   for (int Line = i; Line < i + 2; Line++)
   {
   	for (int Column = j - 1; Column < j + 1; Column++)
      {
      	if (
         	 (
            	(Line >= 0) && (Line < GridLines)
             )
             &&
             (
            	(Column >= 0) && (Column < GridColumns)
             )
             &&
             (
            	(BoxDepth[i * GridColumns + j] > CriticalDepthForLandBoundary)
             )
            )
            {
            	SumOfSurroundingDepths = SumOfSurroundingDepths + BoxDepth[Line * GridColumns + Column];
               NumberOfParcels++;
            }
      }
   }
   //Debugger(SumOfSurroundingDepths / NumberOfParcels);
   if (NumberOfParcels == 0.0)
   	return 0.0;
   else
   	return SumOfSurroundingDepths / NumberOfParcels;
}


double TBiDimensional::UpperRightCornerDepth(int i, int j)        //ok
{
	double SumOfSurroundingDepths, NumberOfParcels;
   SumOfSurroundingDepths = 0;
   NumberOfParcels = 0;

   for (int Line = i; Line < i + 2; Line++)
   {
   	for (int Column = j; Column < j + 2; Column++)
      {
      	if (
         		(
         			(Line >= 0) && (Line < GridLines)
               )
               	&&
               (
               	(Column >= 0) && (Column < GridColumns)
               )
               	&&
               (
                  (BoxDepth[i * GridColumns + j] > CriticalDepthForLandBoundary)
               )
            )
            {
          		SumOfSurroundingDepths = SumOfSurroundingDepths + BoxDepth[Line * GridColumns + Column];
               NumberOfParcels++;
            }
      }
   }
   if (NumberOfParcels == 0.0)
   	return 0.0;
   else
   //Debugger(SumOfSurroundingDepths / NumberOfParcels);
   	return SumOfSurroundingDepths / NumberOfParcels;
}


double TBiDimensional::LowerLeftCornerDepth(int i, int j)          //ok
{
	double SumOfSurroundingDepths, NumberOfParcels;
   SumOfSurroundingDepths = 0;
   NumberOfParcels = 0;

   for (int Line = i - 1; Line < i + 1; Line++)
   {
   	for (int Column = j - 1; Column < j + 1; Column++)
      {
      	if (
         		(
         			(Line >= 0) && (Line < GridLines)
               )
               	&&
               (
               	(Column >= 0) && (Column < GridColumns)
               )
               	&&
               (
                  (BoxDepth[i * GridColumns + j] > CriticalDepthForLandBoundary)
               )
            )
            {
          		SumOfSurroundingDepths = SumOfSurroundingDepths + BoxDepth[Line * GridColumns + Column];
               NumberOfParcels++;
            }
      }
   }
   if (NumberOfParcels == 0.0)
   	return 0.0;
   else
   //Debugger(SumOfSurroundingDepths / NumberOfParcels);
   	return SumOfSurroundingDepths / NumberOfParcels;
}


double TBiDimensional::LowerRightCornerDepth(int i, int j)     //ok
{
	double SumOfSurroundingDepths, NumberOfParcels;
   SumOfSurroundingDepths = 0;
   NumberOfParcels = 0;

   for (int Line = i - 1; Line < i + 1; Line++)
   {
   	for (int Column = j; Column < j + 2; Column++)
      {
      	if (
         		(
         			(Line >= 0) && (Line < GridLines)
               )
               	&&
               (
               	(Column >= 0) && (Column < GridColumns)
               )
               	&&
               (
                  (BoxDepth[i * GridColumns + j] > CriticalDepthForLandBoundary)
               )
            )
            {
          		SumOfSurroundingDepths = SumOfSurroundingDepths + BoxDepth[Line * GridColumns + Column];
               NumberOfParcels++;
            }
      }
   }
   if (NumberOfParcels == 0.0)
   	return 0.0;
   else
   {
   	//Debugger(SumOfSurroundingDepths / NumberOfParcels);
   	return SumOfSurroundingDepths / NumberOfParcels;
   }
}


double TBiDimensional::Coriolis(float ALatitude)
{
	const double EarthAngularSpeed = 0.0000727;  //7.27e-5; // s-1, ou seja, 2Pi em 24 horas}
	return 2.0
			 * EarthAngularSpeed
			 * sin (M_PI * ALatitude / 180.0);
}




double TBiDimensional::VCoriolis(int i, int j)
{
	double VVelocityComponent = 0.0, NumberOfParcels = 2.0;

   VVelocityComponent = VVelocityComponent + VVelocity[i * GridColumns + j ] + VVelocity[(i + 1) * GridColumns + j ];
   if ((j > 0) && (BoxDepth[i * GridColumns + j - 1] > CriticalDepthForLandBoundary))
   {
   	VVelocityComponent = VVelocityComponent + VVelocity[i * GridColumns + j - 1];
   	NumberOfParcels++;
   	VVelocityComponent = VVelocityComponent + VVelocity[(i + 1) * GridColumns + j - 1 ];
   	NumberOfParcels++;
   }

	return ATimeStep * Coriolis ( MyLatitude ) * VVelocityComponent / (2.0 * NumberOfParcels);
}


double TBiDimensional::UCoriolis(int i, int j)
{
	double UVelocityComponent = 0.0, NumberOfParcels = 2.0;

   UVelocityComponent = UVelocityComponent + UVelocity[i * (GridColumns + 1) + j ] + UVelocity[i * (GridColumns + 1) + j + 1];
   if ((i > 0) && (BoxDepth[(i - 1) * GridColumns + j] > CriticalDepthForLandBoundary))
   {
   	UVelocityComponent = UVelocityComponent + UVelocity[(i - 1) * (GridColumns + 1) + j];
   	NumberOfParcels++;
   	UVelocityComponent = UVelocityComponent + UVelocity[(i - 1) * (GridColumns + 1) + j + 1 ];
   	NumberOfParcels++;
   }
      
	return ATimeStep * Coriolis ( MyLatitude ) * UVelocityComponent / (2.0 * NumberOfParcels);
}

/*
double TBiDimensional::UCoriolis(int i, int j)
{
	double SumOfSurroundingSpeeds, NumberOfParcels;
   SumOfSurroundingSpeeds = 0;
   NumberOfParcels = 0;

   for (int Line = i; Line < i + 2; Line++)
   {
   	for (int Column = j - 1; Column < j + 1; Column++)
      {
      	if (
         		(
         			(Line >= 0) && (Line < GridLines)
               )
               	&&
               (
               	(Column >= 0) && (Column < GridColumns)
               )
               	&&
               (
                  (BoxDepth[Line * GridColumns + Column] > CriticalDepthForLandBoundary)
               )
            )
            {
          		SumOfSurroundingSpeeds = SumOfSurroundingSpeeds + VVelocityPast[Line * GridColumns + Column];
               NumberOfParcels++;
            }
      }
   }

   return ATimeStep * Coriolis ( MyLatitude ) * SumOfSurroundingSpeeds / (2.0 * NumberOfParcels);
} */


/*
double TBiDimensional::VCoriolis(int i, int j)
{
	double SumOfSurroundingSpeeds, NumberOfParcels;
   SumOfSurroundingSpeeds = 0;
   NumberOfParcels = 0;

   for (int Line = i - 1; Line < i + 1; Line++)
   {
   	for (int Column = j; Column < j + 2; Column++)
      {
      	if (
         		(
         			(Line >= 0) && (Line < GridLines)
               )
               	&&
               (
               	(Column >= 0) && (Column < GridColumns)
               )
               	&&
               (
                  (BoxDepth[Line * GridColumns + Column] > CriticalDepthForLandBoundary)
               )
            )
            {
          		SumOfSurroundingSpeeds = SumOfSurroundingSpeeds + UVelocityPast[Line * (GridColumns + 1) + Column];
               NumberOfParcels++;
            }
      }
   }
   return ATimeStep * Coriolis ( MyLatitude ) * SumOfSurroundingSpeeds / (2.0 * NumberOfParcels);
}
*/


double TBiDimensional::UVerticalDiffusion(int i, int j)
{
	double NorthernFlux, SouthernFlux;

   //Southern flux
   if (
   		(i > 0) &&
         (BoxDepth[(i - 1) * GridColumns + j] > CriticalDepthForLandBoundary) &&
      	(BoxArray[i * GridColumns + j].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 4)  &&
         (BoxArray[(i - 1) * GridColumns + j].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 4) &&
         (BoxDepth[(i - 1) * GridColumns + j - 1] > CriticalDepthForLandBoundary)               //Nova
 		)

      SouthernFlux = HorizontalDiffCoef *
                    (
             				UVelocityPast[i * (GridColumns + 1) + j] -
               			UVelocityPast[(i - 1) * (GridColumns + 1) + j]
                    ) / ASpatialStep;
   else
   	SouthernFlux = 0.0;

   //Northern flux
   if	 (
   	 	(i < GridLines -  1) &&
         (BoxDepth[(i + 1) * GridColumns + j] > CriticalDepthForLandBoundary) &&
         (BoxArray[i * GridColumns + j].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 4) &&
         (BoxArray[(i + 1) * GridColumns + j].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 4) &&
         (BoxDepth[(i + 1) * GridColumns + j - 1] > CriticalDepthForLandBoundary)                    //Nova
       )

       NorthernFlux = HorizontalDiffCoef *
                    (
             				UVelocityPast[(i + 1) * (GridColumns + 1) + j] -
               			UVelocityPast[i * (GridColumns + 1) + j]
                    ) / ASpatialStep;
   else
   	NorthernFlux = 0.0;

   return ATimeStep / 2.0 * (NorthernFlux - SouthernFlux) /  ASpatialStep;

}

double TBiDimensional::UHorizontalDiffusion(int i, int j)
{
	double WesternFlux, EasternFlux;

      //Western flux
      if (
      		(
      			(j == 1) && //(j > 0) &&
      			(BoxDepth[i * GridColumns + j - 1] > CriticalDepthForLandBoundary) &&
            	(BoxArray[i * GridColumns + j - 1].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 4)
         	) ||
            (
            	(j >= 2) &&
               (BoxDepth[i * GridColumns + j - 1] > CriticalDepthForLandBoundary) &&
      			(BoxArray[i * GridColumns + j].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 4) &&
         		(BoxDepth[i * GridColumns + j - 2] > CriticalDepthForLandBoundary) &&
         		(BoxArray[i * GridColumns + j - 1].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 4)
            )
         )
      	WesternFlux = HorizontalDiffCoef *
                       (
                       		UVelocityPast[i * (GridColumns + 1) + j] -
                           UVelocityPast[i * (GridColumns + 1) + j - 1]
                       ) / ASpatialStep;
      else
      	WesternFlux = 0.0;

      //EasternFlux

      if (
   			(
   				(j < GridColumns - 1) &&
         		(BoxDepth[i * GridColumns + j + 1] > CriticalDepthForLandBoundary) &&
      			(BoxArray[i * GridColumns + j].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 4)
      		)  ||
         	(
         		(j == GridColumns - 1) &&
            	(BoxArray[i * GridColumns + j].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 4)
         	)
      	)
      	EasternFlux = HorizontalDiffCoef *
                       (
                      		UVelocityPast[i * (GridColumns + 1) + j + 1] -
                        	UVelocityPast[i * (GridColumns + 1) + j]
                      	) / ASpatialStep;
      else
      	EasternFlux = 0.0;

      return  ATimeStep / 2.0 * (EasternFlux - WesternFlux) /  ASpatialStep;
}


double TBiDimensional::VVerticalDiffusion(int i, int j)
{
	double NorthernFlux, SouthernFlux;

   //Southern flux
   if (
   		(
   			(i == 1) &&    //(i > 0) &&
         	(BoxDepth[(i - 1) * GridColumns + j] > CriticalDepthForLandBoundary) &&
      		(BoxArray[(i - 1) * GridColumns + j].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 4)  //mudado i para i-1
 			)   ||
      	(
      		(i >= 2) &&
         	(BoxDepth[(i - 1) * GridColumns + j] > CriticalDepthForLandBoundary) &&
      		(BoxArray[i * GridColumns + j].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 4) &&
         	(BoxDepth[(i - 2) * GridColumns + j] > CriticalDepthForLandBoundary) &&
         	(BoxArray[(i - 1) * GridColumns + j].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 4)
      	)
   	)

      SouthernFlux = HorizontalDiffCoef *
                    (
             				VVelocityPast[i * GridColumns + j] -
               			VVelocityPast[(i - 1) * GridColumns + j]
                    ) / ASpatialStep;
   else
   	SouthernFlux = 0.0;

   //Northern flux
   if (
   		(
   			(i < GridLines - 1) &&
         	(BoxDepth[(i + 1) * GridColumns + j] > CriticalDepthForLandBoundary) &&
      		(BoxArray[i * GridColumns + j].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 4)
      	)  ||
         (
         	(i == GridLines - 1) &&
            (BoxArray[i * GridColumns + j].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 4)
         )
      )
      NorthernFlux = HorizontalDiffCoef *
                    (
             				VVelocityPast[(i + 1) * GridColumns + j] -
               			VVelocityPast[i * GridColumns + j]
                    ) / ASpatialStep;
   else
   	NorthernFlux = 0.0;

   return ATimeStep / 2.0 * (NorthernFlux - SouthernFlux) /  ASpatialStep;
}


double TBiDimensional::VHorizontalDiffusion(int i, int j)
{
    double WesternFlux, EasternFlux;

      //Western flux
      if (
      		(j > 0) &&
      		(BoxDepth[i * GridColumns + j - 1] > CriticalDepthForLandBoundary) &&
            (BoxArray[i * GridColumns + j].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 4) &&
            (BoxArray[i * GridColumns + j - 1].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 4) &&
            (BoxDepth[(i - 1) * GridColumns + j - 1] > CriticalDepthForLandBoundary)                        //Nova
         )
      	WesternFlux = HorizontalDiffCoef *
                       (
                       		VVelocityPast[i * GridColumns + j] -
                           VVelocityPast[i * GridColumns + j - 1]
                       ) / ASpatialStep;
      else
      	WesternFlux = 0.0;

      //EasternFlux
      if (
      		(j < GridColumns - 1) &&
      		(
            	(BoxDepth[i * GridColumns + j + 1] > CriticalDepthForLandBoundary) &&
            	(BoxArray[i * GridColumns + j].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 4) &&
               (BoxArray[i * GridColumns + j + 1].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 4) &&
               (BoxDepth[(i - 1) * GridColumns + j + 1] > CriticalDepthForLandBoundary)                       //Nova
             )
         )
         EasternFlux = HorizontalDiffCoef *
                       (
                      		VVelocityPast[i * GridColumns + j + 1] -
                        	VVelocityPast[i * GridColumns + j]
                      	) / ASpatialStep;

      else
      	EasternFlux = 0.0;

      return  ATimeStep / 2.0 * (EasternFlux - WesternFlux) /  ASpatialStep;
}


double TBiDimensional::UAdvection(int i, int j)
{
    return 0.0;
}

double TBiDimensional::VAdvection(int i, int j)
{
/*	if ((i < 0) || (i > GridLines - 1) || (j < 0) || (j > GridColumns - 1))
   	MessageBox(0,
					  "TBiDimensional::AdvectionOfRightSideOfUEquationOfMotion",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);

	double VelocityComponent;
   VelocityComponent = 0.0;

   VelocityComponent = VelocityComponent + (BoxDepth[i * GridColumns + j] *
                       (DOWNStreamVVelocity(i,j) + fabs(DOWNStreamVVelocity(i,j))) *
                       VVelocityPast[i * GridColumns + j]);
   if (i > 0)
   	VelocityComponent = VelocityComponent - (BoxDepth[(i - 1) * GridColumns + j] *
                          (UPStreamVVelocity(i,j) + fabs(UPStreamVVelocity(i,j))) *
                          VVelocityPast[(i - 1) * GridColumns + j]);
   if (i < GridLines)
   	VelocityComponent = VelocityComponent + (BoxDepth[i * GridColumns + j] *
                          (DOWNStreamVVelocity(i,j) - fabs(DOWNStreamVVelocity(i,j))) *
                          VVelocityPast[(i + 1) * GridColumns + j]);
   if (i > 0)
   	VelocityComponent = VelocityComponent - (BoxDepth[(i - 1) * GridColumns + j] *
                          (UPStreamVVelocity(i,j) - fabs(UPStreamVVelocity(i,j))) *
                          UVelocityPast[i * (GridColumns + 1) + j]);
   //Falta aqui mais um conjunto de parcelas
   if (j > 0)
   	return ATimeStep / (2.0 * (BoxDepth[i * GridColumns + j] +
                     		      BoxDepth[i * GridColumns + j - 1])
                         ) * VelocityComponent / ASpatialStep;
   else
   	return ATimeStep / (2.0 * BoxDepth[i * GridColumns + j]) * VelocityComponent / ASpatialStep;  */
    return 0.0;
}

double TBiDimensional::AdvectionOfRightSideOfUEquationOfMotion(int i, int j)
{
	if ((i < 0) || (i > GridLines - 1) || (j < 0) || (j > GridColumns - 1))
#ifdef __BORLANDC__
   	MessageBox(0,
					  "TBiDimensional::AdvectionOfRightSideOfUEquationOfMotion",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
    cerr << "TBiDimensional::AdvectionOfRightSideOfUEquationOfMotion - alert" << endl;
#endif  // __BORLANDC__

	double VelocityComponent;
   VelocityComponent = 0.0;

   VelocityComponent = VelocityComponent + (UpperLeftCornerDepth(i,j) *
                       (UpperLeftCornerVVelocity(i,j) + fabs(UpperLeftCornerVVelocity(i,j))) *
                       UVelocityPast[i * (GridColumns + 1) + j]);
   if (i > 0)
   	VelocityComponent = VelocityComponent - (LowerLeftCornerDepth(i,j) *
                          (LowerLeftCornerVVelocity(i,j) + fabs(LowerLeftCornerVVelocity(i,j))) *
                          UVelocityPast[(i - 1) * (GridColumns + 1) + j]);
   if (i < GridLines - 1)
   	VelocityComponent = VelocityComponent + (UpperLeftCornerDepth(i,j) *
                          (UpperLeftCornerVVelocity(i,j) - fabs(UpperLeftCornerVVelocity(i,j))) *
                          UVelocityPast[(i + 1) * (GridColumns + 1) + j]);
   if (i > 0)
   	VelocityComponent = VelocityComponent - (LowerLeftCornerDepth(i,j) *
                          (LowerLeftCornerVVelocity(i,j) - fabs(LowerLeftCornerVVelocity(i,j))) *
                          UVelocityPast[i * (GridColumns + 1) + j]);
   if (j > 0)
   	return ATimeStep / (2.0 * (BoxDepth[i * GridColumns + j] +
                     		      BoxDepth[i * GridColumns + j - 1])
                         ) * VelocityComponent / ASpatialStep;
   else
   	return ATimeStep / (2.0 * (BoxDepth[i * GridColumns + j] +
                                 BoxDepth[i * GridColumns + j])
                         ) * VelocityComponent / ASpatialStep;
}


double TBiDimensional::UVAdvectionOfUEquation(int i, int j)
{
	if ((i < 0) || (i > GridLines - 1) || (j < 0) || (j > GridColumns - 1))
#ifdef __BORLANDC__
   	MessageBox(0,
					  "TBiDimensional::UVAdvectionOfUEquation",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
    cerr << "TBiDimensional::UVAdvectionOfUEquation - alert" << endl;
#endif  // __BORLANDC__

	double VelocityComponent;
   VelocityComponent = 0.0;
   double MyUpperLeftCornerVVelocity, MLowerLeftCornerVVelocity;
	MyUpperLeftCornerVVelocity = (VVelocityPast[(i + 1) * GridColumns + j] + VVelocityPast[(i + 1) * GridColumns + j - 1]) / 2.0;
   MLowerLeftCornerVVelocity = (VVelocityPast[i * GridColumns + j] + VVelocityPast[i * GridColumns + j - 1]) / 2.0;

   //Nota - No intertidal uso sempre a ultima estimativa da velocidade V conforme nas duas linhas abaixo
   //embora não pareça fazer grande diferença nos resultados
   //MyUpperLeftCornerVVelocity = (VVelocity[(i + 1) * GridColumns + j] + VVelocity[(i + 1) * GridColumns + j - 1]) / 2.0;
   //MLowerLeftCornerVVelocity = (VVelocity[i * GridColumns + j] + VVelocity[i * GridColumns + j - 1]) / 2.0;
   	VelocityComponent = VelocityComponent + (UpperLeftCornerDepth(i,j) *
      	                 (MyUpperLeftCornerVVelocity + fabs(MyUpperLeftCornerVVelocity)) *
         	              UVelocityPast[i * (GridColumns + 1) + j]);

   if (i > 0)
   	VelocityComponent = VelocityComponent - (LowerLeftCornerDepth(i,j) *
                          (MLowerLeftCornerVVelocity + fabs(MLowerLeftCornerVVelocity)) *
                          UVelocityPast[(i - 1) * (GridColumns + 1) + j]);

   if (i < GridLines - 1)
   	VelocityComponent = VelocityComponent + (UpperLeftCornerDepth(i,j) *
                          (MyUpperLeftCornerVVelocity - fabs(MyUpperLeftCornerVVelocity)) *
                          UVelocityPast[(i + 1) * (GridColumns + 1) + j]);

   	VelocityComponent = VelocityComponent - (LowerLeftCornerDepth(i,j) *
                          (MLowerLeftCornerVVelocity - fabs(MLowerLeftCornerVVelocity)) *
                          UVelocityPast[i * (GridColumns + 1) + j]);

   return ATimeStep / (2.0 * (BoxDepth[i * GridColumns + j] +  BoxDepth[i * GridColumns + j - 1])) *
          VelocityComponent / ASpatialStep;
}

double TBiDimensional::VUAdvectionOfExplicitUEquation(int i, int j)
{
	if ((i < 0) || (i > GridLines - 1) || (j < 0) || (j > GridColumns - 1))
#ifdef __BORLANDC__
   	MessageBox(0,
					  "TBiDimensional::VUAdvectionOfExplicitUEquation",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
    cerr << "TBiDimensional::VUAdvectionOfExplicitUEquation - alert" << endl;
#endif  // __BORLANDC__

	double VelocityComponent;
   VelocityComponent = AUpperLeftDepth * (AUpperLeftVVelocity + fabs(AUpperLeftVVelocity)) /
                       ASpatialStep * UVelocityPast[i * (GridColumns + 1) + j ];
   if (i > 0)
   	VelocityComponent = VelocityComponent - ALowerLeftDepth * (ALowerLeftVVelocity + fabs(ALowerLeftVVelocity)) /
                       		ASpatialStep * UVelocityPast[(i - 1) * (GridColumns + 1) + j ];
   if (i < GridLines - 1)
   	VelocityComponent = VelocityComponent + AUpperLeftDepth * (AUpperLeftVVelocity - fabs(AUpperLeftVVelocity)) /
                          ASpatialStep * UVelocityPast[(i + 1) * (GridColumns + 1) + j ];

   VelocityComponent = VelocityComponent - ALowerLeftDepth * (ALowerLeftVVelocity - fabs(ALowerLeftVVelocity)) /
                       ASpatialStep * UVelocityPast[i * (GridColumns + 1) + j ];
   return VelocityComponent;
}




double TBiDimensional::GravityOfRightSideOfUEquationOfMotion(int i, int j)
{
	if ((i < 0) || (i > GridLines - 1) || (j < 0) || (j > GridColumns - 1))
#ifdef __BORLANDC__
   	MessageBox(0,
					  "TBiDimensional::GravityOfRightSideOfUEquationOfMotion",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
    cerr << "TBiDimensional::GravityOfRightSideOfUEquationOfMotion - alert" << endl;
#endif  // __BORLANDC__

	double VelocityComponent1, VelocityComponent2, VelocityComponent3, VelocityComponent4,
          NumberOfparcels;
    int index = i * GridColumns + j;

   VelocityComponent2 = 0.0;
   VelocityComponent3 = 0.0;
   VelocityComponent4 = 0.0;
   NumberOfparcels = 0.0;

   //VelocityComponent1
   VelocityComponent1 = - Gravity * PastHeight[index] *
                        ATimeStep / (2.0 * ASpatialStep);

   //VelocityComponent2
   VelocityComponent2 = VelocityComponent2 +
                        (-BoxDepth[index] * VVelocityPast[index]);


   NumberOfparcels++;
   VelocityComponent2 = VelocityComponent2 +
                        (BoxDepth[index] * VVelocityPast[(i + 1) * GridColumns + j]);


   NumberOfparcels++;
   if ((i < GridLines - 1) && (BoxDepth[(i + 1) * GridColumns + j] > CriticalDepthForLandBoundary))
   {
   	VelocityComponent2 = VelocityComponent2 +
                           (BoxDepth[(i + 1) * GridColumns + j] * VVelocityPast[(i + 1) * GridColumns + j]);

      NumberOfparcels++;
   }
   if ((i > 0) && (BoxDepth[(i - 1) * GridColumns + j] > CriticalDepthForLandBoundary))
   {
   	VelocityComponent2 = VelocityComponent2 +
                           (-BoxDepth[(i - 1) * GridColumns + j] * VVelocityPast[index]);

      NumberOfparcels++;
   }
   VelocityComponent2 =  Gravity * (ATimeStep * ATimeStep) * VelocityComponent2 /
                        (2.0 * ASpatialStep * ASpatialStep * NumberOfparcels);

   //VelocityComponent3
   if ((j > 0) && (BoxDepth[index - 1] > CriticalDepthForLandBoundary))
   	VelocityComponent3 = Gravity * PastHeight[index - 1] *
                           ATimeStep / (2.0 * ASpatialStep);

   //VelocityComponent4
   NumberOfparcels = 0.0;
   if ((j > 0) && (BoxDepth[index - 1] > CriticalDepthForLandBoundary))
   {
   	VelocityComponent4 = VelocityComponent4 +
      	                  (-BoxDepth[index - 1] * VVelocityPast[(i + 1) * GridColumns + j - 1]);


   	NumberOfparcels++;
      VelocityComponent4 = VelocityComponent4 +
      	                  (BoxDepth[index - 1] * VVelocityPast[index - 1]);


      NumberOfparcels++;
   }
   if ((i < GridLines - 1) && (j > 0) && (BoxDepth[(i + 1) * GridColumns + j - 1] > CriticalDepthForLandBoundary))
   {
   	VelocityComponent4 = VelocityComponent4 +
      	                  (-BoxDepth[(i + 1) * GridColumns + j - 1] * VVelocityPast[(i + 1) * GridColumns + j - 1]);


   	NumberOfparcels++;
   }
   if ((i > 0) && (j > 0) && (BoxDepth[(i - 1) * GridColumns + j - 1] > CriticalDepthForLandBoundary))
   {
   	VelocityComponent4 = VelocityComponent4 +
                           (BoxDepth[(i - 1) * GridColumns + j - 1] * VVelocityPast[index - 1]);

      NumberOfparcels++;
   }
   VelocityComponent4 = Gravity * (ATimeStep * ATimeStep) * VelocityComponent4 /
                        (2.0 * ASpatialStep * ASpatialStep * NumberOfparcels);

   return VelocityComponent1 + VelocityComponent2 + VelocityComponent3 + VelocityComponent4;
}



double TBiDimensional::AdvectionOfRightSideOfVEquationOfMotion(int i, int j)
{
	if ((i < 0) || (i > GridLines - 1) || (j < 0) || (j > GridColumns - 1))
#ifdef __BORLANDC__
   	MessageBox(0,
					  "TBiDimensional::AdvectionOfRightSideOfVEquationOfMotion",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
    cerr << "TBiDimensional::AdvectionOfRightSideOfVEquationOfMotion - alert" << endl;
#endif  // __BORLANDC__

	double VelocityComponent = 0.0;
    int index = i * GridColumns + j;

   VelocityComponent = VelocityComponent + (LowerRightCornerDepth(i,j) *
                       (LowerRightCornerUVelocity(i,j) + fabs(LowerRightCornerUVelocity(i,j))) *
                       VVelocityPast[index]);
   if (j > 0)
   	VelocityComponent = VelocityComponent - (LowerLeftCornerDepth(i,j) *
                          (LowerLeftCornerUVelocity(i,j) + fabs(LowerLeftCornerUVelocity(i,j))) *
                          VVelocityPast[index - 1]);

   VelocityComponent = VelocityComponent + (LowerRightCornerDepth(i,j) *
                       (LowerRightCornerUVelocity(i,j) - fabs(LowerRightCornerUVelocity(i,j))) *
                        VVelocityPast[index + 1]);

   if ((j > 0) && (j < GridColumns))
   	VelocityComponent = VelocityComponent - (LowerLeftCornerDepth(i,j) *
                          (LowerLeftCornerUVelocity(i,j) - fabs(LowerLeftCornerUVelocity(i,j))) *
                          VVelocityPast[index]);

   if (i > 0)
   	return ATimeStep / (2.0 * (BoxDepth[index] + BoxDepth[(i - 1) * GridColumns + j]))
             * VelocityComponent / ASpatialStep;
   else
   	return ATimeStep / (2.0 * (BoxDepth[index] + BoxDepth[index]))
             * VelocityComponent / ASpatialStep;

}

double TBiDimensional::UVAdvectionOfVEquation(int i, int j)
{
	if ((i < 0) || (i > GridLines - 1) || (j < 0) || (j > GridColumns - 1))
#ifdef __BORLANDC__
   	MessageBox(0,
					  "TBiDimensional::UVAdvectionOfVEquation",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
    cerr << "TBiDimensional::UVAdvectionOfVEquation - alert" << endl;
#endif  // __BORLANDC__
	double VelocityComponent, MyLowerRightCornerUVelocity, MyLowerLeftCornerUVelocity;
    int index = i * GridColumns + j;

   VelocityComponent = 0.0;

   MyLowerRightCornerUVelocity = (UVelocityPast[i * (GridColumns + 1) + j + 1] + UVelocityPast[(i - 1) * (GridColumns + 1) + j + 1]) / 2.0;
   MyLowerLeftCornerUVelocity =  (UVelocityPast[i * (GridColumns + 1) + j] + UVelocityPast[(i - 1) * (GridColumns + 1) + j]) / 2.0;

   //Nota - No intertidal uso sempre a ultima estimativa da velocidade U conforme nas duas linhas abaixo
   //embora não pareça fazer grande diferença nos resultados
   //MyLowerRightCornerUVelocity = (UVelocity[i * (GridColumns + 1) + j + 1] + UVelocity[(i - 1) * (GridColumns + 1) + j + 1]) / 2.0;
   //MyLowerLeftCornerUVelocity =  (UVelocity[i * (GridColumns + 1) + j] + UVelocity[(i - 1) * (GridColumns + 1) + j]) / 2.0;

   VelocityComponent = VelocityComponent + (LowerRightCornerDepth(i,j) *
                       (MyLowerRightCornerUVelocity + fabs(MyLowerRightCornerUVelocity)) *
                       VVelocityPast[index]);

   if (j > 0)
   	VelocityComponent = VelocityComponent - (LowerLeftCornerDepth(i,j) *
                          (MyLowerLeftCornerUVelocity + fabs(MyLowerLeftCornerUVelocity)) *
                          VVelocityPast[index - 1]);

   if (j < GridColumns - 1)
   	VelocityComponent = VelocityComponent + (LowerRightCornerDepth(i,j) *
                       		(MyLowerRightCornerUVelocity - fabs(MyLowerRightCornerUVelocity)) *
                        	VVelocityPast[index + 1]);

   	VelocityComponent = VelocityComponent - (LowerLeftCornerDepth(i,j) *
                          (MyLowerLeftCornerUVelocity - fabs(MyLowerLeftCornerUVelocity)) *
                          VVelocityPast[index]);

   return ATimeStep / (2.0 * (BoxDepth[index] + BoxDepth[(i - 1) * GridColumns + j])) *
          VelocityComponent / ASpatialStep;
}


double TBiDimensional::UVAdvectionOfExplicitVEquation(int i, int j)
{
	if ((i < 0) || (i > GridLines - 1) || (j < 0) || (j > GridColumns - 1))
#ifdef __BORLANDC__
   	MessageBox(0,
					  "TBiDimensional::UVAdvectionOfExplicitVEquation",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
    cerr << "TBiDimensional::UVAdvectionOfExplicitVEquation - alert" << endl;
#endif  // __BORLANDC__

	double VelocityComponent;
    int index = i * GridColumns + j;

   VelocityComponent = ALowerRightDepth * (ALowerRightUVelocity + fabs(ALowerRightUVelocity)) /
                       ASpatialStep * VVelocityPast[index];
   if (j > 0)
   	VelocityComponent = VelocityComponent - ALowerLeftDepth * (ALowerLeftUVelocity + fabs(ALowerLeftUVelocity)) /
                       	  ASpatialStep * VVelocityPast[index - 1];

   if (j < GridColumns - 1)
      VelocityComponent = VelocityComponent + ALowerRightDepth * (ALowerRightUVelocity - fabs(ALowerRightUVelocity)) /
                          ASpatialStep * VVelocityPast[index + 1];

   VelocityComponent = VelocityComponent - ALowerLeftDepth * (ALowerLeftUVelocity - fabs(ALowerLeftUVelocity)) /
                        ASpatialStep * VVelocityPast[index];

   return VelocityComponent;
}


double TBiDimensional::GravityOfRightSideOfVEquationOfMotion(int i, int j)
{
	if ((i < 0) || (i > GridLines - 1) || (j < 0) || (j > GridColumns - 1))
#ifdef __BORLANDC__
   	MessageBox(0,
					  "TBiDimensional::GravityOfRightSideOfVEquationOfMotion",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
    cerr << "TBiDimensional::GravityOfRightSideOfVEquationOfMotion - alert" << endl;
#endif  // __BORLANDC__
	double VelocityComponent1, VelocityComponent2, VelocityComponent3, VelocityComponent4,
          NumberOfparcels;
    int index = i * GridColumns + j;
    int indexS = (i - 1) * GridColumns + j;

   VelocityComponent2 = 0.0;
   VelocityComponent3 = 0.0;
   VelocityComponent4 = 0.0;
   NumberOfparcels = 0.0;

   //VelocityComponent1
   VelocityComponent1 = - Gravity * PastHeight[index] *
                        ATimeStep / (2.0 * ASpatialStep);

   //VelocityComponent2
   VelocityComponent2 = VelocityComponent2 +
                        (-BoxDepth[index] * UVelocityPast[i * (GridColumns + 1) + j]);

   NumberOfparcels++;
   VelocityComponent2 = VelocityComponent2 +
                        (BoxDepth[index] * UVelocityPast[i * (GridColumns + 1) + j + 1]);


   NumberOfparcels++;
   if ((j < GridColumns - 1) && (BoxDepth[index + 1] > CriticalDepthForLandBoundary))
   {
   	VelocityComponent2 = VelocityComponent2 +
                           (BoxDepth[index + 1] * UVelocityPast[i * (GridColumns + 1) + j + 1]);
      NumberOfparcels++;
   }
   if ((j > 0) && (BoxDepth[index - 1] > CriticalDepthForLandBoundary))
   {
   	VelocityComponent2 = VelocityComponent2 +
                           (-BoxDepth[index - 1] * UVelocityPast[i * (GridColumns + 1) + j]);
      NumberOfparcels++;
   }

   VelocityComponent2 = Gravity * (ATimeStep * ATimeStep) * VelocityComponent2 /
                        (2.0 * ASpatialStep * ASpatialStep * NumberOfparcels);

   //VelocityComponent3
   if ((i > 0) && (BoxDepth[indexS] > CriticalDepthForLandBoundary))
   {
   	VelocityComponent3 = Gravity * PastHeight[indexS] *
                           ATimeStep / (2.0 * ASpatialStep);

   }

   //VelocityComponent4
   NumberOfparcels = 0.0;
   if ((i > 0) && (BoxDepth[indexS] > CriticalDepthForLandBoundary))
   {
   	VelocityComponent4 = VelocityComponent4 +
      	                  (-BoxDepth[indexS] * UVelocityPast[(i - 1) * (GridColumns + 1) + j + 1]);

   	NumberOfparcels++;
      VelocityComponent4 = VelocityComponent4 +
      	                  (BoxDepth[indexS] * UVelocityPast[(i - 1) * (GridColumns + 1) + j]);

      NumberOfparcels++;

   }
   if ((i > 0) && (j < GridColumns - 1) && (BoxDepth[indexS + 1] > CriticalDepthForLandBoundary))
   {
   	VelocityComponent4 = VelocityComponent4 +
                           (-BoxDepth[indexS + 1] * UVelocityPast[(i - 1) * (GridColumns + 1) + j + 1]);
      NumberOfparcels++;
   }
   if ((i > 0) && (j > 0) && (BoxDepth[indexS - 1] > CriticalDepthForLandBoundary))
   {
   	VelocityComponent4 = VelocityComponent4 +
      	                  (BoxDepth[indexS - 1] * UVelocityPast[(i - 1) * (GridColumns + 1) + j]);
   	NumberOfparcels++;
   }

   VelocityComponent4 = Gravity * (ATimeStep * ATimeStep) * VelocityComponent4 /
                        (2.0 * ASpatialStep * ASpatialStep * NumberOfparcels);


   return VelocityComponent1 + VelocityComponent2 + VelocityComponent3 + VelocityComponent4;
}


double TBiDimensional::UForRugosity(int i, int j)
{
	double VVelocityComponent = 0.0, NumberOfParcels = 2.0;
    int index = i * GridColumns + j;

   VVelocityComponent = VVelocityComponent + VVelocity[index] + VVelocity[(i + 1) * GridColumns + j ];
   if ((j > 0) && (BoxDepth[index - 1] > CriticalDepthForLandBoundary))
   {
   	VVelocityComponent = VVelocityComponent + VVelocity[index - 1];
   	NumberOfParcels++;
   	VVelocityComponent = VVelocityComponent + VVelocity[(i + 1) * GridColumns + j - 1 ];
   	NumberOfParcels++;
   }

	return sqrt(
   					sqr(UVelocityPast[i * (GridColumns + 1) + j ]) +
   		 			sqr(
                  		VVelocityComponent / NumberOfParcels
                  	)
          		);
}

double TBiDimensional::VForRugosity(int i, int j)
{
	double UVelocityComponent = 0.0, NumberOfParcels = 2.0;

   UVelocityComponent = UVelocityComponent + UVelocity[i * (GridColumns + 1) + j ] + UVelocity[i * (GridColumns + 1) + j + 1];
   if ((i > 0) && (BoxDepth[(i - 1) * GridColumns + j] > CriticalDepthForLandBoundary))
   {
   	UVelocityComponent = UVelocityComponent + UVelocity[(i - 1) * (GridColumns + 1) + j];
   	NumberOfParcels++;
   	UVelocityComponent = UVelocityComponent + UVelocity[(i - 1) * (GridColumns + 1) + j + 1 ];
   	NumberOfParcels++;
   }
      
	return sqrt(
   					sqr(VVelocityPast[i * GridColumns + j ]) +
   		 			sqr(
                  		UVelocityComponent / NumberOfParcels
                  	)
          		);
}


double TBiDimensional::aUCalculation (int i, int j)
{
    int index = i * GridColumns + j;

	// Cálculo do a
   if (
   		//(BoxArray[index - 1].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1) ||  //Left neighbour cell is a western river boundary
         //(BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 1)
   		(BoxArray[index - 1].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 4) ||//Left neighbour cell is a western land boundary
         (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1) ||
         (BoxArray[index - 1].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 1) ||    //Cell is a river boundary
         (
         	(j >= 2) &&
            (BoxDepth[index - 2] <= CriticalDepthForLandBoundary)
         )
   	)
      	return (0.0);
      else
      if (
      		(BoxArray[index - 1].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 2) || //Left neighbour cell is a western ocean boundary
            (BoxArray[index - 1].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)    //Left neighbour cell is a western river boundary

         )
      {
      	return (- ATimeStep / (2.0 * (BoxDepth[index] +
                                       BoxDepth[index - 1])
                                ) *
                    BoxDepth[index - 1] *
                    (AUPStreamU + fabs(AUPStreamU)) / ASpatialStep -
                    HorizontalDiffCoef / (ASpatialStep * ASpatialStep) * ATimeStep / 2.0 -
                    Gravity * (BoxDepth[index - 1]) *
                    (ATimeStep * ATimeStep) /
                    (4.0 * (ASpatialStep * ASpatialStep)));
      }
      else  //Left neighbour cell is a "normal" cell
      {
      	return (- ATimeStep / (2.0 * (BoxDepth[index] +
                                       BoxDepth[index - 1])
                                ) *
                   BoxDepth[index - 1] *
                   (AUPStreamU + fabs(AUPStreamU)) / ASpatialStep -
                   HorizontalDiffCoef / (ASpatialStep * ASpatialStep) * ATimeStep / 2.0 -
                   Gravity * (BoxDepth[index - 1] +
                              BoxDepth[index - 2]) *
                             (ATimeStep * ATimeStep) /
                             (8.0 * (ASpatialStep * ASpatialStep)));
      }
}

double TBiDimensional::bUCalculation(int i, int j)
{
    int index = i * GridColumns + j;

	// Cálculo do b

	if (
   		//(BoxArray[index - 1].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1) ||//Left neighbour cell is a western river boundary
         (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1) ||
         (BoxArray[index - 1].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 1) //||
      	/*(BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 1) */
      )
   	return (1.0);
   else
   	return(1 + ATimeStep / (2.0 * (BoxDepth[index] +
                                     BoxDepth[index - 1])
             ) *
            (
            	BoxDepth[index] *
               (ADOWNStreamU + fabs(ADOWNStreamU)) / ASpatialStep -
               BoxDepth[index - 1] *
               (AUPStreamU - fabs(AUPStreamU)) / ASpatialStep +
            	(
            		CoefRugosidade(index) +
               	CoefRugosidade(index - 1)
            	) *
            	UForRugosity(i,j)
            ) +
            HorizontalDiffCoef / (ASpatialStep * ASpatialStep) * ATimeStep +
            Gravity * (
            				BoxDepth[index] +
                        BoxDepth[index - 1]
                       ) *
            (ATimeStep * ATimeStep) /
            (4.0 * (ASpatialStep * ASpatialStep)));
}

double TBiDimensional::cUCalculation(int i, int j)
{
    int index = i * GridColumns + j;

	//Cálculo do c - termo correspondente à velocidade na célula seguinte

	if (
   		//(BoxArray[index - 1].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1) || //Left neighbour cell is a western river boundary                                                                //Grid limit
         //(BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 1) ||     //Cell is a eastern river boundary
         (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1) ||
         (BoxArray[index - 1].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 1) ||
         (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 4) ||
         (
         	(j < GridColumns - 1) &&
            (BoxDepth[index + 1] <= CriticalDepthForLandBoundary)
         )
   	)
   	return(0.0);
   else if (
   		(BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2) ||//Cell is a eastern ocean boundary
         (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 1)   //Cell is a eastern river boundary
   	)
      	return(ATimeStep / (2.0 * (BoxDepth[index] +
                                BoxDepth[index - 1])
                         ) *
            BoxDepth[index] *
            (ADOWNStreamU - fabs(ADOWNStreamU)) / ASpatialStep -
            HorizontalDiffCoef / (ASpatialStep * ASpatialStep) * ATimeStep / 2.0 -
            Gravity * (BoxDepth[index]) *
            (ATimeStep * ATimeStep) /
            (4.0 * (ASpatialStep * ASpatialStep)));
         else
         if (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 0)    //"Normal" cell
         	return (ATimeStep / (2.0 * (BoxDepth[index] +
                                   BoxDepth[index - 1])
                                 ) *
                    BoxDepth[index] *
                   (ADOWNStreamU - fabs(ADOWNStreamU)) / ASpatialStep -
                    HorizontalDiffCoef / (ASpatialStep * ASpatialStep) * ATimeStep / 2.0 -
                    Gravity * (BoxDepth[index] +
                    BoxDepth[index + 1]) *
                   (ATimeStep * ATimeStep) /
                   (8.0 * (ASpatialStep * ASpatialStep)));
    return 0.0;
}

double TBiDimensional::aVCalculation(int i, int j)
{
    int index = i * GridColumns + j;
    int indexS = (i - 1) * GridColumns + j;
	// Cálculo do a

   if (
   		//(BoxArray[indexS].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1) || //South neighbour cell is a southern river boundary
         //(BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 1) ||       //Cell is a northern boundary
   		(BoxArray[indexS].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 4) || //South neighbour cell is a southern land boundary
         (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1) ||
         (BoxArray[indexS].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 1) ||
         (
         	(i >= 2) &&
            (BoxDepth[(i - 2) * GridColumns + j] <= CriticalDepthForLandBoundary)
         )
   	)
      	return (0.0);
      else
      if (
      		(BoxArray[indexS].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2) ||//South neighbour cell is a southern ocean boundary
            (BoxArray[indexS].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)   //South neighbour cell is a southern river boundary
         )
      {
      	return - ATimeStep / (2.0 * (BoxDepth[index] +
                                      BoxDepth[indexS])
                               ) *
                BoxDepth[indexS] *
                (AUPStreamV + fabs(AUPStreamV)) / ASpatialStep -
                HorizontalDiffCoef / (ASpatialStep * ASpatialStep) * ATimeStep / 2.0 -
                Gravity * (BoxDepth[indexS]) *
                (ATimeStep * ATimeStep) /
                (4.0 * (ASpatialStep * ASpatialStep));
      }
      else  //South neighbour cells are "normal" cells
         return - ATimeStep / (2.0 * (BoxDepth[index] +
                                      BoxDepth[indexS])
                               ) *
                BoxDepth[indexS] *
                (AUPStreamV + fabs(AUPStreamV)) / ASpatialStep -
                HorizontalDiffCoef / (ASpatialStep * ASpatialStep) * ATimeStep / 2.0 -
                Gravity * (BoxDepth[indexS] +
                           BoxDepth[(i - 2) * GridColumns + j]) *
                          (ATimeStep * ATimeStep) /
                          (8.0 * (ASpatialStep * ASpatialStep));

}


double TBiDimensional::bVCalculation (int i, int j)
{
    int index = i * GridColumns + j;
    int indexS = (i - 1) * GridColumns + j;
	// Cálculo do b

	if (
   		//(BoxArray[indexS].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1) || //South neighbour cell is a southern river boundary
         (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1) ||
         (BoxArray[indexS].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 1) /*||
         (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 1) *///Cell is a northern river boundary
      )
   	return (1.0);
   else
   	return(1 + ATimeStep / (2.0 * (BoxDepth[index] +
                                     BoxDepth[indexS])
                              ) *
            (
            	BoxDepth[index] *
               (ADOWNStreamV + fabs(ADOWNStreamV)) / ASpatialStep -
               BoxDepth[indexS] *
               (AUPStreamV - fabs(AUPStreamV)) / ASpatialStep +
            	(
               	CoefRugosidade(index) +
               	CoefRugosidade(indexS)
            	) *
            	VForRugosity(i,j)
            ) +
            HorizontalDiffCoef / (ASpatialStep * ASpatialStep) * ATimeStep +
            Gravity * (
                      	BoxDepth[index] +
                        BoxDepth[indexS]
                       ) *
            (ATimeStep * ATimeStep) /
            (4.0 * (ASpatialStep * ASpatialStep)));
}


double TBiDimensional::cVCalculation(int i, int j)
{
    int index = i * GridColumns + j;
    int indexS = (i - 1) * GridColumns + j;
    int indexN = (i + 1) * GridColumns + j;

	//Cálculo do c - termo correspondente à velocidade na célula seguinte
	if (
   		//(BoxArray[indexS].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1) ||
         //(BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 1) ||    //Cell is connected to a northern river boundary
         (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1) ||
         (BoxArray[indexS].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 1) ||
         (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 4) ||   //Cell is connected to a northern land boundary
      	(
      		(i < GridLines - 1) &&
         	(BoxDepth[indexN] <= CriticalDepthForLandBoundary)
      	)
      )
   {
   	return(0.0);
   }
   else
   if (
   		(BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 2) ||//Cell is a northern ocean boundary
         (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 1)   //Cell is a northern river boundary

         /*(i == GridLines - 1) ||                                                       //Grid limit
			(
         	(i < GridLines - 1) &&
            (BoxDepth[(i + 1) * GridColumns + j] <= CriticalDepthForLandBoundary) //Cell is connected to a northern land node
         )    */
   	)
      	return(ATimeStep / (2.0 * (BoxDepth[index] +
                                    BoxDepth[indexS])
                             ) *
                BoxDepth[index] *
                (ADOWNStreamV - fabs(ADOWNStreamV)) / ASpatialStep -
                HorizontalDiffCoef / (ASpatialStep * ASpatialStep) * ATimeStep / 2.0 -
                Gravity * (BoxDepth[index]) *
                (ATimeStep * ATimeStep) /
                (4.0 * (ASpatialStep * ASpatialStep)));

   else
   if (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 0)    //"Normal" cell
   		return(ATimeStep / (2.0 * (BoxDepth[index] +
                                    BoxDepth[indexS])
                             ) *
                BoxDepth[index] *
                (ADOWNStreamV - fabs(ADOWNStreamV)) / ASpatialStep -
                HorizontalDiffCoef / (ASpatialStep * ASpatialStep) * ATimeStep / 2.0 -
                Gravity * (BoxDepth[index] +
                BoxDepth[indexN]) *
                (ATimeStep * ATimeStep) /
                (8.0 * (ASpatialStep * ASpatialStep)));
    return 0.0;
}



double TBiDimensional::WesternComponentOfContinuity(int i, int j)
{
    int index = i * GridColumns + j;

	//Western flow
   if (SemiTimeStep == 1) //First semi timestep
   {
		if ((j == 0) ||
   		((j > 0) && (BoxDepth[index - 1] <= CriticalDepthForLandBoundary))
      	)
   	{
   		return (ATimeStep * BoxDepth[index] *
         	     UVelocity[i * (GridColumns + 1) + j]);
   	}
   	else
   	{
   		return (0.5 * ATimeStep *
         	            (
            	         	BoxDepth[index] +
               	         BoxDepth[index - 1]
                  	   ) *
                     	UVelocity[i * (GridColumns + 1) + j]);
   	}
   }
   else
   if (SemiTimeStep == 2) //Second semi timestep
   {
   	if ((j == 0) ||
      	((j > 0) && (BoxDepth[index - 1] <= CriticalDepthForLandBoundary))
      	)
      {
      	return (ATimeStep * BoxDepth[index] *
                  UVelocityPast[i * (GridColumns + 1) + j ]);
      }
      else
      {
      	return (0.5 * ATimeStep *
                           	  (
                                		BoxDepth[index] +
                        	     	 	BoxDepth[index - 1]
                     	         ) *
                  	            UVelocityPast[i * (GridColumns + 1) + j ]);
      }
	}
    return 0.0;
}

double TBiDimensional::EasternComponentOfContinuity(int i, int j)
{
    int index = i * GridColumns + j;

	//Eastern flow
   if (SemiTimeStep == 1) //First semi timestep
   {
   	if ((j == GridColumns - 1)  ||
   		((j < GridColumns - 1) && (BoxDepth[index + 1] <= CriticalDepthForLandBoundary))
   		)
   	{
   		return (ATimeStep * BoxDepth[index] *
         	     UVelocity[i * (GridColumns + 1) + j + 1]);
   	}
   	else
   	{
   		return (0.5 * ATimeStep *
         	           (
            	        		BoxDepth[index] +
               	         BoxDepth[index + 1]
                  	  ) *
                    	UVelocity[i * (GridColumns + 1) + j + 1]);
   	}
    }
    else
    {
    	if ((j == GridColumns - 1)  ||
      	((j < GridColumns - 1) && (BoxDepth[index + 1] <= CriticalDepthForLandBoundary))
         )
      {
      	return (ATimeStep * BoxDepth[index] *
                 UVelocityPast[i * (GridColumns + 1) + j + 1]);
      }
      else
      {
      	return (0.5 * ATimeStep *
                           	  (
                                		BoxDepth[index] +
                        	      	BoxDepth[index + 1]
                     	         ) *
                  	            UVelocityPast[i * (GridColumns + 1) + j + 1]);
      }
    }
}

double TBiDimensional::NorthernComponentOfContinuity(int i, int j)
{
    int index = i * GridColumns + j;
    int indexN = (i + 1) * GridColumns + j;

	//Northern flow
   if (SemiTimeStep == 1) //First semi timestep
   {
   	if ((i == GridLines - 1)  ||
   		((i < GridLines - 1) && (BoxDepth[indexN] <= CriticalDepthForLandBoundary))
      	)
   	{
   		return (ATimeStep * BoxDepth[index] *
                     VVelocityPast[indexN]);
   	}
   	else
   	{
   		return (0.5 * ATimeStep *
                     (
                     	BoxDepth[index] +
                        BoxDepth[indexN]
                     ) *
                     VVelocityPast[indexN]);
   	}
   }
   else
   {
   	if ((i == GridLines - 1)  ||
   		((i < GridLines - 1) && (BoxDepth[indexN] <= CriticalDepthForLandBoundary))
      	)
   	{
   		return (ATimeStep * BoxDepth[index] *
                     VVelocity[indexN]);
   	}
   	else
   	{
   		return (0.5 * ATimeStep *
                     (
                     	BoxDepth[index] +
                        BoxDepth[indexN]
                     ) *
                     VVelocity[indexN]);
   	}

   }
}

double TBiDimensional::SouthernComponentOfContinuity(int i, int j)
{
    int index = i * GridColumns + j;
    int indexS = (i - 1) * GridColumns + j;

	//Southern flow
   if (SemiTimeStep == 1) //First semi timestep
   {
		if ((i == 0) ||
      	((i > 0) && (BoxDepth[indexS] <= CriticalDepthForLandBoundary))
      	)
   	{
   		return (ATimeStep * BoxDepth[index] *
                     VVelocityPast[index]);

   	}
   	else
   	{
   		return (0.5 * ATimeStep *
                     (
                     	BoxDepth[indexS] +
                        BoxDepth[index]
                     ) *
                     VVelocityPast[index]);
   	}
   }
   else
   if (SemiTimeStep == 2) //Second semi timestep
   {
   	if ((i == 0) ||
      	((i > 0) && (BoxDepth[indexS] <= CriticalDepthForLandBoundary))
      	)
   	{
   		return (ATimeStep * BoxDepth[index] *
                     VVelocity[index]);

   	}
   	else
   	{
   		return (0.5 * ATimeStep *
                     (
                     	BoxDepth[indexS] +
                        BoxDepth[index]
                     ) *
                     VVelocity[index]);
   	}
   }
    return 0.0;
}




double TBiDimensional::WesternComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric)
{
    int index = i * GridColumns + j;

	//Western flow
   if (SemiTimeStep == 1) //First semi timestep
   {
		if ((j == 0) ||
   		((j > 0) && (BoxDepth[index - 1] <= CriticalDepthForLandBoundary))
      	)
   	{
   		return (ATimeStep * BoxDepth[index] *
         	     UVelocity[i * (GridColumns + 1) + j] *
                 PastGeneric[index]);
   	}
   	else
   	{
      	if (UVelocity[i * (GridColumns + 1) + j] > 0.0)
   			return (0.5 * ATimeStep *
         	            (
            	         	BoxDepth[index] +
               	         BoxDepth[index - 1]
                  	   ) *
                     	UVelocity[i * (GridColumns + 1) + j] *
                        PastGeneric[index - 1]);
         else
           return (0.5 * ATimeStep *
         	            (
            	         	BoxDepth[index] +
               	         BoxDepth[index - 1]
                  	   ) *
                     	UVelocity[i * (GridColumns + 1) + j] *
                        PastGeneric[index]);
   	}
   }
   else
   if (SemiTimeStep == 2) //Second semi timestep
   {
   	if ((j == 0) ||
      	((j > 0) && (BoxDepth[index - 1] <= CriticalDepthForLandBoundary))
      	)
      {
      	return (ATimeStep * BoxDepth[index] *
                  UVelocityPast[i * (GridColumns + 1) + j ] *
                 PastGeneric[index]);
      }
      else
      {
      	if (UVelocityPast[i * (GridColumns + 1) + j] > 0.0)
      		return (0.5 * ATimeStep *
                       (
                       		BoxDepth[index] +
                           BoxDepth[index - 1]
                       ) *
                       UVelocityPast[i * (GridColumns + 1) + j ]*
                       PastGeneric[index - 1]);
         else
           return (0.5 * ATimeStep *
                       (
                       		BoxDepth[index] +
                           BoxDepth[index - 1]
                       ) *
                       UVelocityPast[i * (GridColumns + 1) + j ]*
                       PastGeneric[index]);
      }
	}
    return 0.0;
}

double TBiDimensional::EasternComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric)
{
    int index = i * GridColumns + j;

	//Eastern flow
   if (SemiTimeStep == 1) //First semi timestep
   {
   	if ((j == GridColumns - 1)  ||
   		((j < GridColumns - 1) && (BoxDepth[index + 1] <= CriticalDepthForLandBoundary))
   		)
   	{
   		return (ATimeStep * BoxDepth[index] *
         	     UVelocity[i * (GridColumns + 1) + j + 1] *
                 PastGeneric[index]);
   	}
   	else
   	{
      	if (UVelocity[i * (GridColumns + 1) + j + 1] > 0.0)
   			return (0.5 * ATimeStep *
         	           (
            	        		BoxDepth[index] +
               	         BoxDepth[index + 1]
                  	  ) *
                    	UVelocity[i * (GridColumns + 1) + j + 1]*
                 		PastGeneric[index]);
         else
         	return (0.5 * ATimeStep *
         	           (
            	        		BoxDepth[index] +
               	         BoxDepth[index + 1]
                  	  ) *
                    	UVelocity[i * (GridColumns + 1) + j + 1]*
                 		PastGeneric[index + 1]);
   	}
   }
   else
   {
    	if ((j == GridColumns - 1)  ||
      	((j < GridColumns - 1) && (BoxDepth[index + 1] <= CriticalDepthForLandBoundary))
         )
      {
      	return (ATimeStep * BoxDepth[index] *
                 UVelocityPast[i * (GridColumns + 1) + j + 1]*
                 PastGeneric[index]);
      }
      else
      {
      	if (UVelocityPast[i * (GridColumns + 1) + j + 1] > 0.0)
      		return (0.5 * ATimeStep *
                           	  (
                                		BoxDepth[index] +
                        	      	BoxDepth[index + 1]
                     	         ) *
                  	            UVelocityPast[i * (GridColumns + 1) + j + 1]*
                 		            PastGeneric[index]);
         else
         	return (0.5 * ATimeStep *
                           	  (
                                		BoxDepth[index] +
                        	      	BoxDepth[index + 1]
                     	         ) *
                  	            UVelocityPast[i * (GridColumns + 1) + j + 1]*
                 		            PastGeneric[index + 1]);
      }
   }
}

double TBiDimensional::NorthernComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric)
{
    int index = i * GridColumns + j;
    int indexN = (i + 1) * GridColumns + j;

	//Northern flow
   if (SemiTimeStep == 1) //First semi timestep
   {
   	if ((i == GridLines - 1)  ||
   		((i < GridLines - 1) && (BoxDepth[indexN] <= CriticalDepthForLandBoundary))
      	)
   	{
   		return (ATimeStep * BoxDepth[index] *
                 VVelocityPast[indexN] *
                 PastGeneric[index]);
   	}
   	else
   	{
      	if (VVelocityPast[indexN] > 0.0)
   			return (0.5 * ATimeStep *
                     (
                     	BoxDepth[index] +
                        BoxDepth[indexN]
                     ) *
                     VVelocityPast[indexN] *
                     PastGeneric[index]);
         else
         	return (0.5 * ATimeStep *
                     (
                     	BoxDepth[index] +
                        BoxDepth[indexN]
                     ) *
                     VVelocityPast[indexN] *
                     PastGeneric[indexN]);
   	}
   }
   else
   {
   	if ((i == GridLines - 1)  ||
   		((i < GridLines - 1) && (BoxDepth[indexN] <= CriticalDepthForLandBoundary))
      	)
   	{
   		return (ATimeStep * BoxDepth[index] *
                 VVelocity[indexN]*
                 PastGeneric[index]);
   	}
   	else
   	{
      	if (VVelocity[indexN] > 0.0)
   			return (0.5 * ATimeStep *
                     (
                     	BoxDepth[index] +
                        BoxDepth[indexN]
                     ) *
                     VVelocity[indexN] *
                     PastGeneric[index]);
         else
            return (0.5 * ATimeStep *
                     (
                     	BoxDepth[index] +
                        BoxDepth[indexN]
                     ) *
                     VVelocity[indexN] *
                     PastGeneric[indexN]);
   	}

   }
}

double TBiDimensional::SouthernComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric)
{
    int index = i * GridColumns + j;
    int indexS = (i - 1) * GridColumns + j;

	//Southern flow
   if (SemiTimeStep == 1) //First semi timestep
   {
		if ((i == 0) ||
      	((i > 0) && (BoxDepth[indexS] <= CriticalDepthForLandBoundary))
      	)
   	{
   		return (ATimeStep * BoxDepth[index] *
                 VVelocityPast[index] *
                 PastGeneric[index]);

   	}
   	else
   	{
      	if (VVelocityPast[index] > 0.0)
   			return (0.5 * ATimeStep *
                     (
                     	BoxDepth[indexS] +
                        BoxDepth[index]
                     ) *
                     VVelocityPast[index] *
                     PastGeneric[indexS]);
         else
         	return (0.5 * ATimeStep *
                     (
                     	BoxDepth[indexS] +
                        BoxDepth[index]
                     ) *
                     VVelocityPast[index] *
                     PastGeneric[index]);
   	}
   }
   else
   if (SemiTimeStep == 2) //Second semi timestep
   {
   	if ((i == 0) ||
      	((i > 0) && (BoxDepth[indexS] <= CriticalDepthForLandBoundary))
      	)
   	{
   		return (ATimeStep * BoxDepth[index] *
                 VVelocity[index] *
                 PastGeneric[index]);

   	}
   	else
   	{
      	if (VVelocity[index] > 0.0)
   			return (0.5 * ATimeStep *
                     (
                     	BoxDepth[indexS] +
                        BoxDepth[index]
                     ) *
                     VVelocity[index]*
                     PastGeneric[indexS]);
         else
         	return (0.5 * ATimeStep *
                     (
                     	BoxDepth[indexS] +
                        BoxDepth[index]
                     ) *
                     VVelocity[index]*
                     PastGeneric[index]);
   	}
   }
    return 0.0;
}


double TBiDimensional::SaltWesternHorizontalDiffusion(int i, int j, double* PastGeneric)
{
    int index = i * GridColumns + j;

   if (
   		(j > 0) &&
         (BoxDepth[index] > CriticalDepthForLandBoundary) &&
         (BoxDepth[index - 1] > CriticalDepthForLandBoundary) &&
         (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 1)
   	)
   	return //-ATimeStep / (2.0 * ASpatialStep * ASpatialStep) *
             -0.5 * ATimeStep *
      		 (
             	HorizontalDiffCoef *
      		 	(
             		BoxDepth[index] +
               	BoxDepth[index - 1]
             	) *
               PastGeneric[index ] -
             	//MyUpStreamPastConcentration -
               HorizontalDiffCoef *
      		 	(
             		BoxDepth[index] +
               	BoxDepth[index - 1]
             	) *
               PastGeneric[index - 1]
               //MyCurrentBoxPastConcentration
             );
   else
   	return 0.0;

}

double TBiDimensional::SaltEasternHorizontalDiffusion(int i, int j, double* PastGeneric)
{
    int index = i * GridColumns + j;

	if (
   		(j < GridColumns - 1) &&
         (BoxDepth[index] > CriticalDepthForLandBoundary) &&
         (BoxDepth[index + 1] > CriticalDepthForLandBoundary) &&
         (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 1)
   	)
   {

   	return //ATimeStep / (2.0 * ASpatialStep * ASpatialStep) *
              0.5 * ATimeStep *
      		 (
             	HorizontalDiffCoef *
      		 	(
             		BoxDepth[index + 1] +
               	BoxDepth[index]
             	) *
               PastGeneric[index + 1] -
               HorizontalDiffCoef *
      		 	(
             		BoxDepth[index + 1] +
               	BoxDepth[index]
             	) *
               PastGeneric[index]
             );
   }
   else
   	return 0.0;
}

double TBiDimensional::SaltNorthernVerticalDiffusion(int i, int j, double* PastGeneric)
{
    int index = i * GridColumns + j;
    int indexN = (i + 1) * GridColumns + j;

	if (
   		(i < GridLines - 1) &&
         (BoxDepth[index] > CriticalDepthForLandBoundary) &&
         (BoxDepth[indexN] > CriticalDepthForLandBoundary)&&
         (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 1)
   	)
   	return //ATimeStep / (2.0 * ASpatialStep * ASpatialStep) *
              0.5 * ATimeStep *
      		 (
             	HorizontalDiffCoef *
      		 	(
             		BoxDepth[indexN] +
               	BoxDepth[index]
             	) *
               PastGeneric[indexN] -
             	//MyDownStreamPastConcentration -
               HorizontalDiffCoef *
      		 	(
             		BoxDepth[indexN] +
               	BoxDepth[index]
             	) *
               PastGeneric[index]
               //MyCurrentBoxPastConcentration
             );
   else
   	return 0.0;

}

double TBiDimensional::SaltSouthernVerticalDiffusion(int i, int j, double* PastGeneric)
{
    int index = i * GridColumns + j;
    int indexS = (i - 1) * GridColumns + j;

	if (
   		(i > 0) &&
         (BoxDepth[index] > CriticalDepthForLandBoundary) &&
         (BoxDepth[indexS] > CriticalDepthForLandBoundary) &&
         (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 1)
   	)
   	return //-ATimeStep / (2.0 * ASpatialStep * ASpatialStep) *
             -0.5 * ATimeStep *
      		 (
             	HorizontalDiffCoef *
      		 	(
             		BoxDepth[index] +
               	BoxDepth[indexS]
             	) *
               PastGeneric[index] -
             	//MyDownStreamPastConcentration -
               HorizontalDiffCoef *
      		 	(
             		BoxDepth[index] +
               	BoxDepth[indexS]
             	) *
               PastGeneric[indexS]
               //MyCurrentBoxPastConcentration
             );
   else
   	return 0.0;
}


double TBiDimensional::CoefRugosidade(int ABoxNumber)
{
   int MyBoxNumber;
   double MyDepth;
   MyBoxNumber = ABoxNumber;
   if (BoxDepth[MyBoxNumber] < 0.5) MyDepth = 0.5;
   else MyDepth = BoxDepth[MyBoxNumber];
   return(Gravity * pow(CoefManning[MyBoxNumber],2.0)) / pow(MyDepth,0.333333);
   //return 0.004;
}


double TBiDimensional::CorrectionOfHorizontalDiffCoef(double ADepth, double AVelocityComponent)
{
    double const ReferenceSpeed = 1.0; //ms-1
    double const ReferenceDepth = 10.0; //m
    double CorrectionFactor;
    CorrectionFactor =  pow(AVelocityComponent/ReferenceSpeed,2) * ADepth / ReferenceDepth;
    //CorrectionFactor = 0.0;
    return MAX(1.0, CorrectionFactor);
}


