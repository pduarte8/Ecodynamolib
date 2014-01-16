/**
 * NPI work
 * 	15 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
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
#include "globals.h"
#include "iodll.h"

TMixedReactor::TMixedReactor(TEcoDynClass* APEcoDynClass, char* className)
							  : TTransport(APEcoDynClass, className)
{
   BuildMixedReactor();
}

void TMixedReactor::BuildMixedReactor()
{
     if (NumberOfBoxes > 0)
     {
          DynamicHeight = new double[NumberOfBoxes];
          PastHeight = new double[NumberOfBoxes];
          BoxDepth = new double[NumberOfBoxes];
          PastBoxDepth = new double[NumberOfBoxes];
          BoxLength = new double[NumberOfBoxes];
          BoxVolume = new double[NumberOfBoxes];
          PastBoxVolume = new double[NumberOfBoxes];
          WaterDensity = new double[NumberOfBoxes];
          FlowDischarge = new double[NumberOfBoxes];
          FlowUptake = new double[NumberOfBoxes];
          BoxWidth = new double[NumberOfBoxes];
          BoxCrossSectionalArea = new double[NumberOfBoxes];
          PastBoxCrossSectionalArea = new double[NumberOfBoxes];
	}
	else {
#ifdef __BORLANDC__
		MessageBox(0,"Hydrodynamic array not dimensioned",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TMixedLReactor::BuildMixedReactor - Hydrodynamic array not dimensioned" << endl;
#endif  // __BORLANDC__
        return;
    }

    for (int i = 0; i < NumberOfBoxes; i++)
    {
        Salt[i] = 0.0;
        SaltLoad[i] = 0.0;
        DynamicHeight[i] = 0.0;
        PastHeight[i] = 0.0;
        BoxDepth[i] = MyPEcoDynClass->GetBoxDepth(i);
        PastBoxDepth[i] = 0.0;
        BoxLength[i] = 0.0;
        BoxVolume[i] = 0.0;
        PastBoxVolume[i] = 0.0;
        WaterDensity[i] = 0.0;
        FlowDischarge[i] = 0.0;
        FlowUptake[i] = 0.0;
        BoxWidth[i] = 0.0;
        BoxCrossSectionalArea[i] = 0.0;
        PastBoxCrossSectionalArea[i] = 0.0;

    }

    //Variables file

    int X,Y, XV, YV;
    char MyVariable[65];
    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Transport");
    if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
            MessageBox(0, "Transport variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TMixedLReactor::BuildMixedReactor - variables file missing" << endl;
#endif  // __BORLANDC__
    }
    else {
        if (PReadWrite->FindString("Transport", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];

            // read in the variable names
            for (int i = 0; i < NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
                strcpy(VariableNameArray[i], MyVariable);
                if (strcmp(MyVariable, "Salinity") == 0)
                {
                    if (!PReadWrite->FindString("Salinity values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                       PReadWrite->ReadNumber(XV , YV + 1 + j, Salt[j]);
                       //Debugger(Salt[j]);
                    }
                }
                if (strcmp(MyVariable, "Dynamic height") == 0)
                {
                    if (!PReadWrite->FindString("Dynamic height values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                       PReadWrite->ReadNumber(XV , YV + 1 + j, DynamicHeight[j]);
                       PastHeight[j] = DynamicHeight[j];
                    }
                }
                if (strcmp(MyVariable, "Water density") == 0)
                {
                    if (!PReadWrite->FindString("Water density values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                       PReadWrite->ReadNumber(XV , YV + 1 + j, WaterDensity[j]);
                    }
                }
                if (strcmp(MyVariable, "Salt load") == 0)
                {
                    if (!PReadWrite->FindString("Salt load values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                       PReadWrite->ReadNumber(XV , YV + 1 + j, SaltLoad[j]);
                    }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TMixedLReactor::BuildMixedReactor variables undefined object" << endl;
#endif  // __BORLANDC__
        }
        //End reading variables file
        CloseDataFile((void*)PReadWrite);
    }
    for (int i = 0; i < NumberOfBoxes; i++)
    {
        BoxDepth[i] = MyPEcoDynClass->GetBoxDepth(i) + DynamicHeight[i];
        PastBoxDepth[i] = BoxDepth[i];
        BoxLength[i] = MyPEcoDynClass->GetBoxLength(i);
        BoxVolume[i] = MyPEcoDynClass->GetBoxVolume(i);
        PastBoxVolume[i] = BoxVolume[i];
        BoxWidth[i] = MyPEcoDynClass->GetChannelWidth(i);
        BoxCrossSectionalArea[i] = BoxWidth[i] * BoxDepth[i];
        PastBoxCrossSectionalArea[i] = BoxCrossSectionalArea[i];
    }
    ATimeStep = MyPEcoDynClass->GetTimeStep();
}

TMixedReactor::~TMixedReactor()
{
    freeMemory();
}

void TMixedReactor::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
        {
            delete [] DynamicHeight;
            delete [] PastHeight;
            delete [] BoxDepth;
            delete [] PastBoxDepth;
            delete [] BoxLength;
            delete [] BoxVolume;
            delete [] PastBoxVolume;
            delete [] WaterDensity;
            delete [] FlowDischarge;
            delete [] FlowUptake;
            delete [] BoxWidth;
            delete [] BoxCrossSectionalArea;
            delete [] PastBoxCrossSectionalArea;
        }
        if (NumberOfVariables > 0) {
            delete [] VariableNameArray;
            NumberOfVariables = 0;
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
        cerr << "TMixedReactor::freeMemory - " << exc.what() << endl;
    }
#endif  // __BORLANDC__
}

void TMixedReactor::Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode)
{
    char MyParameter[65]; // 64 characters (max) for parameter name
    int MyBoxNumber = BoxNumber;
    strcpy(MyParameter, ParameterName);

    if (strcmp(MyParameter, "Dynamic height") == 0)
            Value = DynamicHeight[MyBoxNumber];
    else if (strcmp(MyParameter, "Box cross sectional area") == 0)
            Value = BoxCrossSectionalArea[MyBoxNumber];
    else if (strcmp(MyParameter, "Salinity") == 0)
            Value = Salt[MyBoxNumber];
    else if (strcmp(MyParameter, "Water density") == 0)
		Value = WaterDensity[MyBoxNumber];
    else if (strcmp(MyParameter, "Box depth") == 0)
		Value = BoxDepth[MyBoxNumber];
    else if (strcmp(MyParameter, "Flow discharge") == 0)
		Value = FlowDischarge[MyBoxNumber];
    else if (strcmp(MyParameter, "Flow uptake") == 0)
		Value = FlowUptake[MyBoxNumber];
    else if (strcmp(MyParameter, "Box Volume") == 0)
		Value = BoxVolume[MyBoxNumber];
    else if (strcmp(MyParameter, "Salt load") == 0)
		Value = SaltLoad[MyBoxNumber];
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
                      "EcoDynamo Alert - Inquiry 7",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
            cerr << "TMixedLReactor::Inquiry 7 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}


bool TMixedReactor::SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName)
{
    bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strcmp(VariableName, "Dynamic height") == 0)
		DynamicHeight[BoxNumber] = Value;
    else if (strcmp(VariableName, "Box cross sectional area") == 0)
            BoxCrossSectionalArea[BoxNumber] = Value;
    else if (strcmp(VariableName, "Salinity") == 0)
            Salt[BoxNumber] = Value;
    else if (strcmp(VariableName, "Water density") == 0)
		WaterDensity[BoxNumber] = Value;
    else if (strcmp(VariableName, "Box depth") == 0)
		BoxDepth[BoxNumber] = Value;
    else if (strcmp(VariableName, "Flow discharge") == 0)
		FlowDischarge[BoxNumber] = Value;
    else if (strcmp(VariableName, "Flow uptake") == 0)
		FlowUptake[BoxNumber] = Value;

    else
        rc = false;

    return rc;
}

bool TMixedReactor::SaveVariables()
{
    TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Transport");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Transport");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);

    /*
     * save variables' names
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (i > 0)
        {
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
        if (strcmp(VariableNameArray[i], "Salinity") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(RiverSalt, 6);
        }
        PReadWrite->WriteSeparator(true);
    }

    /*
     * save variables' initial values
     */
    PReadWrite->WriteCell("Salinity values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Water density values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Dynamic height values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Flow discharge values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Flow uptake values");
    PReadWrite->WriteSeparator(true);

    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(Salt[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(WaterDensity[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DynamicHeight[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(FlowDischarge[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(FlowUptake[j], 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

void TMixedReactor::Go()
{
   int ADay, AHour, AMoment;
   NumberOfDaysForLoads = MyPEcoDynClass->GetNumberOfDaysForLoads();
   NumberOfHoursForLoads = MyPEcoDynClass->GetNumberOfHoursForLoads();
   if (NumberOfDaysForLoads <= 366) //Nestes casos assume-se que a série começa no dia 1 e acaba no ultimo dia do ano
   {
       if  (NumberOfDaysForLoads >= 1)
       {
           ADay = MIN(MyPEcoDynClass->GetJulianDay() - 1,NumberOfDaysForLoads - 1);
           AMoment = ADay;
       }
       else
       if (NumberOfHoursForLoads >= 1)
       {
           ADay = MyPEcoDynClass->GetJulianDay() - 1;
           AHour = ADay * DAYSTOHOURS + floor(MyPEcoDynClass->GetCurrTime());
           AMoment = MIN(AHour, NumberOfHoursForLoads - 1);
       }
       else
#ifdef __BORLANDC__
           MessageBox(0, "TMixedReactor::Go", "Incorrect value in NumberOfDaysForLoads or NumberOfHoursForLoads", MB_OK);
#else  // __BORLANDC__
            cerr << "TMixedLReactor::Go - Incorrect value in NumberOfDaysForLoads or NumberOfHoursForLoads" << endl;
#endif  // __BORLANDC__
   }
   else
   {
      ATime = ATime + MyPEcoDynClass->GetTimeStep();  //Debugger(ATime);
      ADay = (int) ((ATime - AStartTime) / DAYSTOSECONDS + 1); //Debugger(ADay);
      AMoment = ADay - 1;
   }

   //if ((ADay > 452) && (MyPEcoDynClass->GetCurrTime() > 17.0)) Debugger(1);
   DischargeForcing(AMoment);
   //if ((ADay > 452) && (MyPEcoDynClass->GetCurrTime() > 17.0)) Debugger(2);
   UptakeForcing(AMoment);
   //if ((ADay > 452) && (MyPEcoDynClass->GetCurrTime() > 17.0)) Debugger(3);
   for (int index = 0; index < NumberOfBoxes; index++) {
      PastHeight[index] = DynamicHeight[index];
   }
   //if ((ADay > 452) && (MyPEcoDynClass->GetCurrTime() > 17.0)) Debugger(4);
   Continuity();
   //if ((ADay > 452) && (MyPEcoDynClass->GetCurrTime() > 17.0)) Debugger(5);
   GenericLoad = SaltLoad;
   //if ((ADay > 452) && (MyPEcoDynClass->GetCurrTime() > 17.0)) Debugger(6);
   Mix(Salt);
   //if ((ADay > 452) && (MyPEcoDynClass->GetCurrTime() > 17.0)) Debugger(7);
}

void TMixedReactor::Go(double* AGeneric,double* ASecondGeneric,
                                  double ARiver, double AOcean)
{
    Generic = AGeneric;
    GenericLoad = ASecondGeneric;
    Mix(Generic);
}

void TMixedReactor::Continuity()
{
   double PastDepth;
   for (int index = 0; index < NumberOfBoxes; index++)
   {
      PastDepth = BoxDepth[index];
      BoxVolume[index] = BoxVolume[index] + ATimeStep * (FlowDischarge[index] - FlowUptake[index]);
      if (BoxLength[index] * BoxWidth[index] > 0.0)
         BoxDepth[index] = BoxVolume[index] / (BoxLength[index] * BoxWidth[index]);
      else
         BoxDepth[index] = 0.0;
      DynamicHeight[index] = DynamicHeight[index] + BoxDepth[index] - PastDepth;
   }
}

void TMixedReactor::Mix(double* Generic)
{
   double* PastGeneric = new double[NumberOfBoxes];
   for (int i = 0; i < NumberOfBoxes; i++)
   	PastGeneric[i] = Generic[i];
   int index;
   if (MyLoadRecord != NULL)
   {
       for (int index = 0; index < NumberOfBoxes; index++)
       {
          if (BoxDepth[index] > 0.0)
          {
             Generic[index] = Generic[index ] + ATimeStep * GenericLoad[index] *
                                                         FlowDischarge[index] /
                                                         (
                                                            BoxDepth[index] *
                                                            (BoxLength[index] * BoxWidth[index])
                                                         );
          }
       }
   }
   if (MyLossesRecord != NULL)
   {
       for (int index = 0; index < NumberOfBoxes; index++)
       {
          if (BoxDepth[index] > 0.0)
          {
             Generic[index] = Generic[index] - ATimeStep * PastGeneric[index] *
                                                         FlowUptake[index] /
                                                         (
                                                            BoxDepth[index] *
                                                            (BoxLength[index] * BoxWidth[index])
                                                         );
             //Debugger(ATimeStep);
             //Debugger(FlowDischarge[index]); Debugger(FlowUptake[index]);
             //Debugger(Generic[index]);
          }
       }
   }
   delete [] PastGeneric;
}


void TMixedReactor::DischargeForcing(int AMoment)
{
   if (MyLoadRecord != NULL)
   {
       for (int k = 0; k < MyPEcoDynClass->GetNumberOfLoads(); k++)
       {
          int i  = MyLoadRecord[k].LineCoordinate;
          if (BoxDepth[i] > 0.0)
          {
            FlowDischarge[i] = MyLoadRecord[k].LoadFlow[AMoment];  //m3/s
            //Debugger(AMoment);
            //Debugger(FlowDischarge[i]);
          }
          else FlowDischarge[i] = 0.0;
       }
   }
}

void TMixedReactor::UptakeForcing(int AMoment)
{
   if (MyLossesRecord != NULL)
   {
       for (int k = 0; k < MyPEcoDynClass->GetNumberOfLosses(); k++)
       {
          int i = MyLossesRecord[k].LineCoordinate;
          if (BoxDepth[i] > 0.0)
          {
            FlowUptake[i] = MyLossesRecord[k].LossFlow[AMoment];  //m3/s
            //Debugger(FlowUptake[i]);
          }
          else FlowUptake[i] = 0.0;
       }
   }
}
