// Water temperature object CPP code file
// @ Pedro Duarte 11/09/2000
/**
 * NPI work
 * 	09 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "twatobjt.h"
#include "iodll.h"
#include "EcoDynClass.h"


// TWaterTemperature Class for 2D Sango model
//
// Constructors invoked outside EcoDyn shell...
//

TWaterTemperatureTwoDimensionalForSango::TWaterTemperatureTwoDimensionalForSango(
        char* className, float timeStep,
        int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aAverageTemperature[],
        double genericRiver, double relativeHumidity, double aDailyOceanTemperature[])
    :TWaterTemperatureTwoDimensional(className, timeStep, nLines, nColumns,
                aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                nVars, aVarsNames, aAverageTemperature,
                genericRiver, relativeHumidity)
{
    PreBuild2DimSango();
    DailyOceanTemperature  = new double[365];
    for (int i = 0; i < 365; i++)
    {
        DailyOceanTemperature[i] = aDailyOceanTemperature[i];
    }
}

TWaterTemperatureTwoDimensionalForSango::TWaterTemperatureTwoDimensionalForSango(
        char* className, float timeStep,
        char* morphologyFilename, char* variablesFilename)
              :TWaterTemperatureTwoDimensional(className, timeStep,
                            morphologyFilename, variablesFilename)
{
    Build2DimSango();
}
// ...


TWaterTemperatureTwoDimensionalForSango::TWaterTemperatureTwoDimensionalForSango(
        TEcoDynClass* APEcoDynClass, char* className)
                : TWaterTemperatureTwoDimensional(APEcoDynClass, className)
{
    Build2DimSango();
}

void TWaterTemperatureTwoDimensionalForSango::PreBuild2DimSango()
{
	VariableCount = 0;
	WesternBoundaryNumber = 4;
    SouthernBoundaryNumber = 3;
    EasternBoundaryNumber = 2;
    NorthernBoundaryNumber = 1;
    GridLines = GetNumberOfLines();
    GridColumns = GetNumberOfColumns();
	BoxArray = MyPEcoDynClass->BoxArray;
}

void TWaterTemperatureTwoDimensionalForSango::Build2DimSango()
{
    PreBuild2DimSango();

	DailyOceanTemperature  = new double[365];
    for (int i = 0; i < 365; i++)
   	    DailyOceanTemperature[i] = 0;

//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("WaterTemperature");
	if (PReadWrite != NULL)
	{
        int X,Y, XD, YD;

        if (PReadWrite->FindString("Water temperature", X, Y))
        {
            // Read the water temperature values
            char MyVariable[65];
            for (int i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + 2, i, MyVariable);
                if (strcmp(MyVariable, "Daily ocean temperature") == 0)
                {
                    if (!PReadWrite->FindString("Daily ocean temperature values", XD, YD))
                    {
                        XD = X + 4;
                        YD = i - 1;
                    }
                    for (int j = 0; j < 365; j++)
                    {
                        PReadWrite->ReadNumber(XD, YD + 1 +j, DailyOceanTemperature[j]);
                    }
                    break;
                }
            }
        }
        CloseDataFile((void*)PReadWrite);
    }
}


TWaterTemperatureTwoDimensionalForSango::~TWaterTemperatureTwoDimensionalForSango()
{
	freeMemory();
}

void TWaterTemperatureTwoDimensionalForSango::freeMemory()
{
    try {
        delete [] DailyOceanTemperature;
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
				cerr << "TWaterTemperatureTwoDimensionalForSango::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}


void TWaterTemperatureTwoDimensionalForSango::Inquiry(char* srcName, double &Value,
												int BoxNumber,
												char* ParameterName,
												int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Water temperature") == 0)
		Value = AverageWaterTemperature[MyBoxNumber];
	else if (strcmp(MyParameter, "Water irradiance") == 0)
		Value = WaterIrradiance[MyBoxNumber];
	else if (strcmp(MyParameter, "Net heat flux") == 0)
		Value = NetHeatFlux[MyBoxNumber];
	else if (strcmp(MyParameter, "Latent heat flux") == 0)
		Value = LatentHeatFlux[MyBoxNumber];
	else if (strcmp(MyParameter, "Sensible heat flux") == 0)
		Value = SensibleHeatFlux[MyBoxNumber];
    else if (strcmp(MyParameter, "Daily ocean temperature") == 0)
        Value = DailyOceanTemperature[JulianDay-1];
    else if (strcmp(MyParameter, "Relative humidity") == 0)
	    Value = RelativeHumidity;
    else if (strcmp(MyParameter, "Relative humidity time series") == 0)
    {
        int ADay, AHour, AMoment;
        if  (NumberOfDaysForTimeSeries >= 1)
        {
           ADay = MIN(MyPEcoDynClass->GetJulianDay() - 1,NumberOfDaysForTimeSeries - 1);
           AMoment = ADay;
        }
        else
        if (NumberOfHoursForTimeSeries >= 1)
        {
           ADay = MyPEcoDynClass->GetJulianDay() - 1;
           AHour = ADay * DAYSTOHOURS + floor(MyPEcoDynClass->GetCurrTime());
           AMoment = MIN(AHour, NumberOfHoursForTimeSeries - 1);
        }
#ifdef __BORLANDC__
           MessageBox(0, "TWaterTemperatureTwoDimensionalForSango::Inquiry", "Incorrect value in NumberOfDaysForTimeSeries or NumberOfHoursForTimeSeries", MB_OK);
#else  // __BORLANDC__
					 cerr << "TWaterTemperatureTwoDimensionalForSango::Inquiry  - Incorrect value in NumberOfDaysForTimeSeries or NumberOfHoursForTimeSeries" << endl;
#endif  // __BORLANDC__
        Value = RelativeHumidityTimeSeries[AMoment];
    }
    else
        Value = 0;
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

bool TWaterTemperatureTwoDimensionalForSango::SetVariableValue(char* srcName, double Value,
									      int BoxNumber,
									      char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Water temperature") == 0)
        AverageWaterTemperature[BoxNumber] = Value;
	else if (strcmp(VariableName, "Water irradiance") == 0)
		WaterIrradiance[BoxNumber] = Value;
	else if (strcmp(VariableName, "Net heat flux") == 0)
		NetHeatFlux[BoxNumber] = Value;
	else if (strcmp(VariableName, "Latent heat flux") == 0)
		LatentHeatFlux[BoxNumber] = Value;
	else if (strcmp(VariableName, "Sensible heat flux") == 0)
		SensibleHeatFlux[BoxNumber] = Value;
    else if (strcmp(VariableName, "Daily ocean temperature") == 0)
    {
        for (int i = 0; i < 365; i++)
    	    DailyOceanTemperature[i] = Value;
    }
    else
        rc = false;

    return rc;
}

bool TWaterTemperatureTwoDimensionalForSango::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("WaterTemperature");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Water temperature");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    PReadWrite->WriteSeparator();

    /*
     * save variables' names
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (i > 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteCell(VariableNameArray[i]);
        if (strcmp(VariableNameArray[i], "Relative humidity") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(RelativeHumidity, 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    /*
     * save variables' values
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (strcmp(VariableNameArray[i], "Water temperature") == 0)
        {
            PReadWrite->WriteCell("Water temperature values");
            PReadWrite->WriteSeparator(true);
            for (int j = 0; j < NumberOfBoxes; j++)
            {
                PReadWrite->WriteCell(AverageWaterTemperature[j], 8);
                PReadWrite->WriteSeparator(true);
            }
        }
        else if (strcmp(VariableNameArray[i], "Daily ocean temperature") == 0)
        {
            PReadWrite->WriteCell("Daily ocean temperature values");
            PReadWrite->WriteSeparator(true);
            for (int j = 0; j < 365; j++)
            {
                PReadWrite->WriteCell(DailyOceanTemperature[j], 9);
                PReadWrite->WriteSeparator(true);
            }
        }
    }

    CloseDataFile((void*)PReadWrite);

    return true;
}

void TWaterTemperatureTwoDimensionalForSango::Go()
{
// Active methods
	MyTime = MyPEcoDynClass->GetTime();
	CurrentTime = MyPEcoDynClass->GetCurrTime();
	JulianDay = MyPEcoDynClass->GetJulianDay();
	TimeBefore = MyTime;
	GetWaterTemperature();
}

void TWaterTemperatureTwoDimensionalForSango::Integrate()
{
	// Temporary
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
    {

   	    double ABoxDepth;

       	//Southern boundary
   	    int i,j;
       	for (j = 0; j < GridColumns; j++)
   	    {
            i = 0;
            MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,
                                             i * GridColumns + j,
                                             "Box depth",
                                             ObjectCode);
      	 	if ((ABoxDepth > 0.0) &&
                 (BoxArray[i * GridColumns + j].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2)  //Southern ocean boundary
       			)
          	{
      	    	AverageWaterTemperature[i * GridColumns + j] = DailyOceanTemperature[JulianDay-1];
          	}
	    }
       	for (j = 0; j < GridColumns; j++)
   	    {
       		i = GridLines - 1;
          	if (
   		    		(ABoxDepth > 0.0) &&
                	(BoxArray[i * GridColumns + j].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 2) //Northern ocean boundary
       			)
          	{
      	    	AverageWaterTemperature[i * GridColumns + j] = DailyOceanTemperature[JulianDay-1];
          	}
   	    }
       	for (i = 0; i < GridLines; i++)
   	    {
       		j = GridColumns - 1;
          	if (
   		    		(ABoxDepth > 0.0) &&
                	(BoxArray[i * GridColumns + j].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2) //Eastern ocean boundary
       			)
          	{
      	    	AverageWaterTemperature[i * GridColumns + j] = DailyOceanTemperature[JulianDay-1];
          	}
   	    }
        MyTransportPointer->Go(AverageWaterTemperature, AverageWaterTemperature, 0, 0);
	}
}

void TWaterTemperatureTwoDimensionalForSango::GetWaterTemperature()
{
	const double WaterSpecificHeat = 4100; // J/kg/k

	double MySalinity,
			 MyLightAtTop,
			 MyLightAtBottom,
			 ABottom,
			 MyAtmosphericIR,
			 MySVPAtWaterTemperature,
			 MySVPAtAirTemperature,
			 MyAirTemperature,
			 MyTemperatureVariation,
			 MyWindSpeed;

	TEcoDynClass* MyTAirTemperaturePointer = MyPEcoDynClass->GetAirTemperaturePointer();
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer();
	TEcoDynClass* MyWindPointer = MyPEcoDynClass->GetWindPointer();

	for (int i = 0; i < GridLines; i++)
    {
   	    for (int j = 0; j < GridColumns; j++)
        {
      	    if (MyTransportPointer != NULL)
            {
         	    MyTransportPointer->Inquiry(GetEcoDynClassName(), MySalinity,
			  									        i * GridColumns + j,
			 									        "Salinity",
												        ObjectCode);
                MyTransportPointer->Inquiry(GetEcoDynClassName(), ABottom,
			  											  i * GridColumns + j,
			 											  "Box depth",
														  ObjectCode);
                MyTransportPointer->Inquiry(GetEcoDynClassName(), MyWaterDensity,
			  											  i * GridColumns + j,
			 											  "Water density",
														  ObjectCode);
              }
            else
            {
         	    MySalinity = 37;      // local variable
                MixedLayerDepth = 15; // object variable
                MyWaterDensity = 1027.29; // seawater at 35psu and 10oC
                ABottom = MyPEcoDynClass->GetBoxDepth(i * GridColumns + j);
            }
       		if (((ABottom > 0.0) && (MyTransportPointer == NULL)) ||
                ((MyTransportPointer != NULL) &&
            		(ABottom > 0.0) &&
            		(BoxArray[i * GridColumns + j].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2)&& //No eastern ocean boundary
            		(BoxArray[i * GridColumns + j].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
            		(BoxArray[i * GridColumns + j].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2) //No northern ocean boundary
               ))
   		    {
				if (MyTAirTemperaturePointer != NULL)
					MyTAirTemperaturePointer->Inquiry(GetEcoDynClassName(), MyAirTemperature,
														  i * GridColumns + j,
														  "Air temperature",
														  ObjectCode);
				else
				{
					return;
				}

				if (MyLightPointer != NULL)
				{
					MyLightPointer->Inquiry(GetEcoDynClassName(), MyLightAtTop,
														 i * GridColumns + j,
														 "Total surface irradiance",
														 ObjectCode);
					MyLightPointer->Inquiry(GetEcoDynClassName(), MyLightAtBottom,
														 ABottom,
														 i * GridColumns + j,
														 "Sub-surface irradiance",
														 ObjectCode);
					MyLightPointer->Inquiry(GetEcoDynClassName(), MyAtmosphericIR,
														 i * GridColumns + j,
														 "Atmospheric IR",
														 ObjectCode);
				}
				else
				{
					return;
				}

				if (MyWindPointer != NULL)
					MyWindPointer->Inquiry(GetEcoDynClassName(), MyWindSpeed,
														 i * GridColumns + j,
														 "Wind speed",
														 ObjectCode);
				else
				{
					return;
				}



      		    WaterIrradiance[i * GridColumns + j] = GetWaterSurfaceRadiation(AverageWaterTemperature[i * GridColumns + j]);

      		    MySVPAtWaterTemperature = GetSaturationVapourPressure(AverageWaterTemperature[i * GridColumns + j]);

          		MySVPAtAirTemperature = GetSaturationVapourPressure(MyAirTemperature);

          		LatentHeatFlux[i * GridColumns + j] = GetLatentWaterHeatFlux(MySVPAtWaterTemperature,
																		 MySVPAtAirTemperature,
																		 MyWindSpeed);

      	    	SensibleHeatFlux[i * GridColumns + j] = GetSensibleWaterHeatFlux(AverageWaterTemperature[i * GridColumns + j],
																			  MyAirTemperature,
																			  MyWindSpeed);


      		    NetHeatFlux[i * GridColumns + j] = MyLightAtTop
									  - MyLightAtBottom
									  + MyAtmosphericIR
									  + WaterIrradiance[i * GridColumns + j]
									  + LatentHeatFlux[i * GridColumns + j]
									  + SensibleHeatFlux[i * GridColumns + j];

      		    if ((MyWaterDensity == 0.0) || (WaterSpecificHeat == 0.0) || (ABottom == 0.0))
                {
                    Debugger(MyWaterDensity);
                    Debugger(WaterSpecificHeat);
                    Debugger(ABottom);
                    Debugger(i * GridColumns + j);
                }
				MyTemperatureVariation = NetHeatFlux[i * GridColumns + j]
												 * TimeStep
												 / (MyWaterDensity
												 * WaterSpecificHeat
							  					 * ABottom);
				AverageWaterTemperature[i * GridColumns + j] = AverageWaterTemperature[i * GridColumns + j]
													  + MyTemperatureVariation;
      	    }
		}
	}
}
