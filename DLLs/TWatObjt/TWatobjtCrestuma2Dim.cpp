// Water temperature object CPP code file
// @ Pedro Duarte
// Translated by J. Gomes Ferreira
// Rev. A 96.02.09
/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	#include <cstring>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "twatobjt.h"
#include "iodll.h"

// Two dimensional vertical model for Crestuma Lever
//
// Constructors invoked outside EcoDyn shell...
//

TWaterTemperatureCrestumaTwoDimensional::TWaterTemperatureCrestumaTwoDimensional(
        char* className, float timeStep,
        int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aAverageTemperature[],
        double genericRiver, double relativeHumidity,
        double aTemperatureOutflow[], int boxWithThermicEffluent)
    :TWaterTemperatureTwoDimensional(className, timeStep, nLines, nColumns,
                aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                nVars, aVarsNames, aAverageTemperature,
                genericRiver, relativeHumidity)
{
    PreBuild2DimCrestuma();
	TemperatureOutflow  = new double[NumberOfBoxes];
    for (int i = 0; i < NumberOfBoxes; i++)
        TemperatureOutflow[i] = aTemperatureOutflow[i];
	BoxWithThermicEffluent = boxWithThermicEffluent;
}

TWaterTemperatureCrestumaTwoDimensional::TWaterTemperatureCrestumaTwoDimensional(
        char* className, float timeStep,
        char* morphologyFilename, char* variablesFilename)
              :TWaterTemperatureTwoDimensional(className, timeStep,
                            morphologyFilename, variablesFilename)
{
    Build2DimCrestuma();
}
// ...


TWaterTemperatureCrestumaTwoDimensional::TWaterTemperatureCrestumaTwoDimensional(
        TEcoDynClass* APEcoDynClass, char* className)
			: TWaterTemperatureTwoDimensional(APEcoDynClass, className)
{
    Build2DimCrestuma();
}


void TWaterTemperatureCrestumaTwoDimensional::PreBuild2DimCrestuma()
{
	VariableCount = 0;
    GridLines = GetNumberOfLines();
    GridColumns = GetNumberOfColumns();
	BoxArray = MyPEcoDynClass->BoxArray;
    UpperLayer = GridLines - 1;
    NumberOfDaysForRiverLoads = floor(MyPEcoDynClass->GetNumberOfHoursForFlowTimeSeries() / DAYSTOHOURS);
    DailyOceanTemperature = new double[NumberOfDaysForRiverLoads];
}

void TWaterTemperatureCrestumaTwoDimensional::Build2DimCrestuma()
{
	PreBuild2DimCrestuma();

    if (NumberOfBoxes > 0)                       // Initialise arrays for variables
	{
		TemperatureOutflow  = new double[NumberOfBoxes];
		for (int i = 0; i < NumberOfBoxes; i++)
		{
			TemperatureOutflow[i] = 0;
            if (i < NumberOfDaysForRiverLoads)
                DailyOceanTemperature[i] = 0;
		}
	}
	// Get forcing function file name
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("WaterTemperature");
	if (PReadWrite != NULL)
	{
        int X,Y, XO, YO, XD, YD;
        if (PReadWrite->FindString("Water temperature", X, Y))
        {
    /*
     * already read in TWaterTemperature...
     *
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            for (int i = Y; i < Y + NumberOfVariables; i++)
                PReadWrite->ReadString(X+2, i, VariableNameArray[i-Y]);
     */
            // Read the water temperature values
            char MyVariable[65];
            for (int i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + 2, i, MyVariable);
    /*
     * already read in TWaterTemperature...
     *
                if (strcmp(MyVariable, "Water temperature") == 0)
                {
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(X+3, i+j, AverageWaterTemperature[j]);
                    }
                }
                else
     */
                if (strcmp(MyVariable, "Outflow temperature") == 0)
                {
                    if (!PReadWrite->FindString("Outflow temperature values", XO, YO))
                    {
                        XO = X + 4;
                        YO = i - 1;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XO, YO + 1+j, TemperatureOutflow[j]);
                    }
                }
                else if (strcmp(MyVariable, "Box with thermic effluent") == 0)
                {

                    PReadWrite->ReadNumber(X+3, i, BoxWithThermicEffluent);
                }
                else if (strcmp(MyVariable, "Daily ocean temperature") == 0)
                {
                    if (!PReadWrite->FindString("Daily ocean temperature values", XD, YD))
                    {
                        XD = X + 5;
                        YD = i - 1;
                    }
                    for (int j = 0; j < NumberOfDaysForRiverLoads; j++)
                    {
                        PReadWrite->ReadNumber(XD, YD + 1+j, DailyOceanTemperature[j]);
                        //if (j == 364) Debugger(DailyOceanTemperature[j]);
                    }
                }
            }
        }
        CloseDataFile((void*)PReadWrite);
     }
}


TWaterTemperatureCrestumaTwoDimensional::~TWaterTemperatureCrestumaTwoDimensional()
{
    freeMemory();
}

void TWaterTemperatureCrestumaTwoDimensional::freeMemory()
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
				cerr << "TWaterTemperatureCrestumaTwoDimensional::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}



void TWaterTemperatureCrestumaTwoDimensional::Inquiry(char* srcName, double &Value,
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
    else if (strcmp(MyParameter, "Outflow temperature") == 0)
		Value = TemperatureOutflow[MyBoxNumber];
    else if (strcmp(MyParameter, "Relative humidity") == 0)
		Value = RelativeHumidity;
    else if (strcmp(MyParameter, "River temperature") == 0)
		Value = GenericRiver;
    else
        Value = 0;
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

bool TWaterTemperatureCrestumaTwoDimensional::SetVariableValue(char* srcName, double Value,
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
    else if (strcmp(VariableName, "Outflow temperature") == 0)
		TemperatureOutflow[BoxNumber] = Value;
    else if (strcmp(VariableName, "Relative humidity") == 0)
		RelativeHumidity = Value;
    else if (strcmp(VariableName, "River temperature") == 0)
		GenericRiver = Value;
    else
        rc = false;

    return rc;
}

bool TWaterTemperatureCrestumaTwoDimensional::SaveVariables()
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
     * save variables' initial names
     */
    bool dailyValues = false;
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (i > 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteCell(VariableNameArray[i]);
        if (strcmp(VariableNameArray[i], "Box with thermic effluent") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(BoxWithThermicEffluent, 0);
        }
        else if (strcmp(VariableNameArray[i], "Relative humidity") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(RelativeHumidity, 8);
        }
        else if (strcmp(VariableNameArray[i], "Daily ocean temperature") == 0)
            dailyValues = true;
        PReadWrite->WriteSeparator(true);
    }
    /*
     * save variables' values
     */
    if (dailyValues) {
        PReadWrite->WriteCell("Daily ocean temperature values");
        PReadWrite->WriteSeparator(true);
        for (int j = 0; j < 365; j++)
        {
            PReadWrite->WriteCell(DailyOceanTemperature[j], 9);
            PReadWrite->WriteSeparator(true);
        }
    }
    PReadWrite->WriteCell("Water temperature values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Outflow temperature values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(AverageWaterTemperature[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(TemperatureOutflow[j], 9);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);

    return true;
}

void TWaterTemperatureCrestumaTwoDimensional::Go()
{
// Active methods

    MyTime = MyPEcoDynClass->GetTime();
    CurrentTime = MyPEcoDynClass->GetCurrTime();
    JulianDay = MyPEcoDynClass->GetJulianDay();
    TimeBefore = MyTime;
    GetWaterTemperature();
    int MyDay;
    MyDay = floor(MyPEcoDynClass->GetTime() + 1);
    //if (JulianDay > 365)
    if (MyDay > NumberOfDaysForRiverLoads)
        GenericRiver = DailyOceanTemperature[0];
    else
        GenericRiver = DailyOceanTemperature[MyDay - 1];
    
    for (int i = 0; i <= UpperLayer; i++)
    {
        TemperatureOutflow[i * GridColumns + BoxWithThermicEffluent - 1] = 1.4 * AverageWaterTemperature[BoxWithThermicEffluent - 1];
        //Debugger(TemperatureOutflow[i * GridColumns + BoxWithThermicEffluent - 1]);
    }
    //Debugger(GenericRiver);
}

void TWaterTemperatureCrestumaTwoDimensional::Integrate()
{
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    if (MyTransportPointer != NULL)
   	    MyTransportPointer->Go(AverageWaterTemperature, TemperatureOutflow, GenericRiver, 0);
    /*else
    {
		MessageBox(0, "Transport object is undefined in TWaterTemperatureTwoDimensionalForCrestuma", "EcoDynamo alert", MB_OK);
        return;
    }*/
    //Debugger(4);
}


void TWaterTemperatureCrestumaTwoDimensional::GetWaterTemperature()
{
    int MyUpperLayer; double ALayer, MyPreviousSurfVolume, MyCurrentSurfVolume;
    const double WaterSpecificHeat = 4100; // J/kg/k

    double MySalinity,
           MyLightAtTop,
           MyLightAtBottom,
           ATop,ABottom, AValue,
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
    if (MyTransportPointer != NULL)
    {
   	    MyTransportPointer->Inquiry(GetEcoDynClassName(), ALayer,
                                      0,
                                      "Upper layer",
                                      ObjectCode);

        MyUpperLayer = floor(ALayer);
        //Debugger(MyUpperLayer);
        if (MyUpperLayer != UpperLayer)
        {
      	    for (int j = 0; j < GridColumns; j++)
            {
                if (MyUpperLayer < UpperLayer)
                {
      			    MyTransportPointer->Inquiry(GetEcoDynClassName(), MyPreviousSurfVolume,
			  											     UpperLayer * GridColumns + j,
               		                          "Past box volume",
                  		                       ObjectCode);

            	    MyTransportPointer->Inquiry(GetEcoDynClassName(), MyCurrentSurfVolume,
			  											     MyUpperLayer * GridColumns + j,
               		                          "Past box volume",
                  		                       ObjectCode);
            	    AverageWaterTemperature[MyUpperLayer * GridColumns + j] =
            		    (
            			AverageWaterTemperature[UpperLayer * GridColumns + j] * MyPreviousSurfVolume +
               		    AverageWaterTemperature[MyUpperLayer * GridColumns + j] * MyCurrentSurfVolume
            		    ) / (MyPreviousSurfVolume + MyCurrentSurfVolume);
                }
                else if (MyUpperLayer > UpperLayer)
            	    AverageWaterTemperature[MyUpperLayer * GridColumns + j] =
                        AverageWaterTemperature[UpperLayer * GridColumns + j];
      	    }
      	    UpperLayer = MyUpperLayer;
        }
    }
    else
   	    MyUpperLayer = GridLines - 1;
    //Debugger(MyUpperLayer);
    for (int i = 0; i <= MyUpperLayer; i++)
    {
   	    for (int j = 0; j < GridColumns; j++)
        {
      	    if (MyTransportPointer != NULL)
            {
         	    MyTransportPointer->Inquiry(GetEcoDynClassName(), MySalinity,
			  									i * GridColumns + j,
                                                "Salinity",
                                                ObjectCode);
                MyTransportPointer->Inquiry(GetEcoDynClassName(), MyWaterDensity,
			  									i * GridColumns + j,
                                                "Water density",
                                                ObjectCode);
                if (i < MyUpperLayer)
                    MyTransportPointer->Inquiry(GetEcoDynClassName(), ATop,
			  										(i + 1) * GridColumns + j,
                                                    "Total box depth",
                                                    ObjectCode);
                else
                   ATop = 0;
                MyTransportPointer->Inquiry(GetEcoDynClassName(), ABottom,     // Depth at bottom surface
			  									i * GridColumns + j,
                                                "Total box depth",
                                                ObjectCode);
            }
            else
            {
#ifdef __BORLANDC__
         	    MessageBox(0, "Transport object is undefined in GetWaterTemperature -", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TWaterTemperatureCrestumaTwoDimensional::GetWaterTemperature - Transport object is undefined" << endl;
#endif  // __BORLANDC__
                return;
            }
            //Debugger(ATop); Debugger(ABottom);
            if ((ATop >= 0.0) && (ABottom > 0.0) && (ABottom - ATop > 0.0))
            {
                if (i == MyUpperLayer)
                {
            	    if (MyTAirTemperaturePointer != NULL)
						MyTAirTemperaturePointer->Inquiry(GetEcoDynClassName(), MyAirTemperature,
																		  i * GridColumns + j,
																		  "Air temperature",
																		  ObjectCode);
					else
					{
#ifdef __BORLANDC__
						MessageBox(0, "Air temperature object is undefined", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TWaterTemperatureCrestumaTwoDimensional::GetWaterTemperature - Air temperature object is undefined" << endl;
#endif  // __BORLANDC__
						return;
					}
                }
				if (MyLightPointer != NULL)
				{
                    if (i == MyUpperLayer)
                    {
               	        MyLightPointer->Inquiry(GetEcoDynClassName(), MyLightAtTop,
                                              i * GridColumns + j,
                                              "Total surface irradiance",
                                              ObjectCode);
                        MyLightPointer->Inquiry(GetEcoDynClassName(), MyAtmosphericIR,
															 i * GridColumns + j,
															 "Atmospheric IR",
															 ObjectCode);
                    }
                    else if (i < MyUpperLayer)
                    {
						MyLightPointer->Inquiry(GetEcoDynClassName(), MyLightAtTop,
                                              ATop,
                                              (i + 1) * GridColumns + j,
                                              "Sub-surface irradiance",
                                              ObjectCode);
                    }
					MyLightPointer->Inquiry(GetEcoDynClassName(), MyLightAtBottom,
														 ABottom,
														 i * GridColumns + j,
														 "Sub-surface irradiance",
														 ObjectCode);
				}
				else
				{
#ifdef __BORLANDC__
					MessageBox(0, "Light object is undefined", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
					cerr << "TWaterTemperatureCrestumaTwoDimensional::GetWaterTemperature - Light object is undefined" << endl;
#endif  // __BORLANDC__
					return;
				}
                if (i == MyUpperLayer)
                {
					if (MyWindPointer != NULL)
						MyWindPointer->Inquiry(GetEcoDynClassName(), MyWindSpeed,
															 i * GridColumns + j,
															 "Wind speed",
															 ObjectCode);
					else
					{
#ifdef __BORLANDC__
						MessageBox(0, "Wind object is undefined", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TWaterTemperatureCrestumaTwoDimensional::GetWaterTemperature - Wind object is undefined" << endl;
#endif  // __BORLANDC__
						return;
					}
                }
                //Debugger(i);
                if (i == MyUpperLayer)
                {
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
                }
                else if (i < MyUpperLayer)
                {
            	    NetHeatFlux[i * GridColumns + j] = MyLightAtTop - MyLightAtBottom;
                }
      		    if ((MyWaterDensity == 0.0) || (WaterSpecificHeat == 0.0) || (ABottom == 0.0))
                {
#ifdef __BORLANDC__
                    MessageBox(0,"Division by zero in TWaterTemperatureTwoDimensionalSango::GetWaterTemperature()","Atention!",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
										cerr << "Division by zero in TWaterTemperatureTwoDimensionalSango::GetWaterTemperature()" << endl;
#endif  // __BORLANDC__
                    Debugger(MyWaterDensity);
                    Debugger(WaterSpecificHeat);
                    Debugger(ABottom);
                    Debugger(i * GridColumns + j);
                }

				MyTemperatureVariation = NetHeatFlux[i * GridColumns + j]
                                                         * TimeStep               // Is already in days
                                                         /** DAYSTOSECONDS */         // Convert it to seconds
                                                         / (MyWaterDensity
                                                         * WaterSpecificHeat
                                                         * (ABottom - ATop));

				AverageWaterTemperature[i * GridColumns + j] = AverageWaterTemperature[i * GridColumns + j]
													  + MyTemperatureVariation;
      	    }
		}  //j
	} //i
}




