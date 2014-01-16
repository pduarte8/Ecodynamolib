// SPM object CPP code file
// @ Pedro Duarte
// Rev. A 00.10.10

/**
 * NPI work
 * 	04 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <vcl.h>
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "spmobjt.h"
#include "iodll.h"

// TSangoResuspendDeposit Class
//
// Constructors invoked outside EcoDyn shell...
//

TSangoResuspendDeposit::TSangoResuspendDeposit(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double totalExtinctionCoefficient,
        double aSPMConcentration[], double aPOMConcentration[],
        double aBoundarySPMConcentration[], double aBoundaryPOMConcentration[],
        int nParams, double karmanConstant, double dragCoefficient,
        double criticalVelocity, double sinkingVelocity, double nContents,
        double detritusBreakdown, double bottomSedimentOrganicContents,
        double aErosionRate[], double aRoughness[])
    :TSPM(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    PreBuildSangoResuspendDeposit();

	NumberOfParameters = nParams;
	NumberOfVariables = nVars;
	VariableNameArray = new VNA[NumberOfVariables];
    for (int i = 0; i < NumberOfVariables; i++)
        strcpy(VariableNameArray[i], aVarsNames[i]);

	for (int j = 0; j < NumberOfBoxes; j++)
	{
		SPMConcentration[j] = aSPMConcentration[j];
		POMConcentration[j] = aPOMConcentration[j];
		ErosionRate[j] = aErosionRate[j];
		Roughness[j] = aRoughness[j];
		TotalExtinctionCoefficient[j] = totalExtinctionCoefficient;
		BottomSedimentOrganicContents[j] = bottomSedimentOrganicContents;
	}
	for (int j = 0; j < 365; j++)
	{
		BoundarySPMConcentration[j] = aBoundarySPMConcentration[j];
		BoundaryPOMConcentration[j] = aBoundaryPOMConcentration[j];
    }
	KarmanConstant = karmanConstant;
	DragCoefficient = dragCoefficient;
	CriticalVelocity = criticalVelocity;
	SinkingVelocity = sinkingVelocity;
	NContents = nContents;
	DetritusBreakDown = detritusBreakdown;
}

TSangoResuspendDeposit::TSangoResuspendDeposit(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
                        :TSPM(className, timeStep, morphologyFilename)
{
    strcpy(VarsFileName, variablesFilename);
    strcpy(ParsFileName, parametersFilename);
    BuildSangoResuspendDeposit();
}
// ...

TSangoResuspendDeposit::TSangoResuspendDeposit(TEcoDynClass* APEcoDynClass,
        char* className) : TSPM(APEcoDynClass, className)
{
    BuildSangoResuspendDeposit();
}

void TSangoResuspendDeposit::PreBuildSangoResuspendDeposit()
{
    WesternBoundaryNumber = 4;
    SouthernBoundaryNumber = 3;
    EasternBoundaryNumber = 2;
    NorthernBoundaryNumber = 1;
    BoxArray = MyPEcoDynClass->BoxArray;
    NumberOfBoxes = MyPEcoDynClass->GetGridSize();

    POMConcentration = new double[NumberOfBoxes];
    POMFlux = new double[NumberOfBoxes];
    Roughness = new double[NumberOfBoxes];
    ErosionRate = new double[NumberOfBoxes];
    BoundarySPMConcentration = new double[365];
    BoundaryPOMConcentration = new double[365];
    BottomSedimentOrganicContents = new double[NumberOfBoxes];

    for (int i = 0; i< 365; i++)
    {
   	    BoundarySPMConcentration[i] = 0.0;
        BoundaryPOMConcentration[i] = 0.0;
    }
    for (int i = 0; i < NumberOfBoxes; i++)
    {
   	    Roughness[i] = 0.0;
        POMConcentration[i] = 0.0;
        ErosionRate[i] = 0.0;
        POMFlux[i] = 0.0;
        BottomSedimentOrganicContents[i] = 0.0;
    }
    GridColumns = MyPEcoDynClass->GetNumberOfColumns();
    GridLines = MyPEcoDynClass->GetNumberOfLines();
}

void TSangoResuspendDeposit::BuildSangoResuspendDeposit()
{
    PreBuildSangoResuspendDeposit();

    int X, Y, XV, YV;
	// Read in the parameters
	ParameterCount = 0;
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("SuspendedMatter");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "SuspendedMatter parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TSangoResuspendDeposit::BuildSangoResuspendDeposit - Parameters file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        double MyValue;
        if (PReadWrite->FindString("Suspended matter", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "Karman constant") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KarmanConstant);
                }
                else if (strcmp(MyParameter, "Drag coefficient") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, DragCoefficient);
                }
                else if (strcmp(MyParameter, "Critical velocity") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, CriticalVelocity);
                }
                else if (strcmp(MyParameter, "Sinking velocity") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, SinkingVelocity);
                }
                else if (strcmp(MyParameter, "Erosion rate") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        ErosionRate[j] = MyValue;
                    }
                }
                else if (strcmp(MyParameter, "Bottom sediment organic contents") == 0)
                {
                    double MyValue;
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        BottomSedimentOrganicContents[j] = MyValue;
                    }
                }
                else if (strcmp(MyParameter, "Mineralization rate") == 0)
                {
                    double MyValue;
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    DetritusBreakDown = MyValue;
                }
                else if (strcmp(MyParameter, "N contents") == 0)
                {
                    double MyValue;
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    NContents = MyValue;
                }
                else if (strcmp(MyParameter, "Roughness") == 0)
                {
                    if (!PReadWrite->FindString("Roughness values", XV, YV))
                    {
                        XV = X + 3;
                        YV = i - 1;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, Roughness[j]);
                    }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Parameters: Undefined object - Suspended matter", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TSangoResuspendDeposit::BuildSangoResuspendDeposit - Parameters: Undefined object - Suspended matter" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("SuspendedMatter");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "SuspendedMatter variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TSangoResuspendDeposit::BuildSangoResuspendDeposit - Variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Suspended matter", X, Y))
        {
            char MyVariable[65];
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names

            for (int i = 0; i < NumberOfVariables; i++)
            {
                double MyValue;
                PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
                strcpy(VariableNameArray[i], MyVariable);
                if (strcmp(MyVariable, "TPM") == 0)
                {
                    if (!PReadWrite->FindString("TPM values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, SPMConcentration[j]);
                    }
                }
                else if (strcmp(MyVariable, "POM") == 0)
                {
                    if (!PReadWrite->FindString("POM values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, POMConcentration[j]);
                    }
                }
                else if (strcmp(MyVariable, "Boundary SPM concentration") == 0)
                {
                    if (!PReadWrite->FindString("Boundary SPM concentration values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < 365; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, BoundarySPMConcentration[j]);
                    }
                }
                else if (strcmp(MyVariable, "Boundary POM concentration") == 0)
                {
                    if (!PReadWrite->FindString("Boundary POM concentration values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < 365; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryPOMConcentration[j]);
                    }
                }
                else if (strcmp(MyVariable, "Extinction coefficient Ktot") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        TotalExtinctionCoefficient[j] = MyValue;
                    }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables: Undefined object - Suspended matter", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TSangoResuspendDeposit::BuildSangoResuspendDeposit - Variables: Undefined object - Suspended matter" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TSangoResuspendDeposit::~TSangoResuspendDeposit()
{
    freeMemory();
}

void TSangoResuspendDeposit::freeMemory()
{
    try {
        if (NumberOfBoxes > 0) {
            delete [] POMConcentration;
            delete [] POMFlux;
            delete [] Roughness;
            delete [] ErosionRate;
            delete [] BottomSedimentOrganicContents;
        }
        delete [] BoundarySPMConcentration;
        delete [] BoundaryPOMConcentration;
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
				cerr << "TSangoResuspendDeposit::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}


double TSangoResuspendDeposit::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "Karman constant") == 0)
    {
        value = KarmanConstant;
    }
    else if (strcmp(MyParameter, "Drag coefficient") == 0)
    {
        value = DragCoefficient;
    }
    else if (strcmp(MyParameter, "Critical velocity") == 0)
    {
        value = CriticalVelocity;
    }
    else if (strcmp(MyParameter, "Sinking velocity") == 0)
    {
        value = SinkingVelocity;
    }
    else if (strcmp(MyParameter, "Erosion rate") == 0)
    {
        value = ErosionRate[0];
    }
    else if (strcmp(MyParameter, "Bottom sediment organic contents") == 0)
    {
        value = BottomSedimentOrganicContents[0];
    }
    else if (strcmp(MyParameter, "Mineralization rate") == 0)
    {
        value = DetritusBreakDown;
    }
    else if (strcmp(MyParameter, "N contents") == 0)
    {
        value = NContents;
    }
    else if (strcmp(MyParameter, "Roughness") == 0)
    {
        value = Roughness[0];
    }
    else
        value = 0.0;
    return value;
}

bool TSangoResuspendDeposit::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int j;

    if (strcmp(MyParameter, "Karman constant") == 0)
    {
        KarmanConstant = value;
    }
    else if (strcmp(MyParameter, "Drag coefficient") == 0)
    {
        DragCoefficient = value;
    }
    else if (strcmp(MyParameter, "Critical velocity") == 0)
    {
        CriticalVelocity = value;
    }
    else if (strcmp(MyParameter, "Sinking velocity") == 0)
    {
        SinkingVelocity = value;
    }
    else if (strcmp(MyParameter, "Erosion rate") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            ErosionRate[j] = value;
    }
    else if (strcmp(MyParameter, "Bottom sediment organic contents") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            BottomSedimentOrganicContents[j] = value;
    }
    else if (strcmp(MyParameter, "Mineralization rate") == 0)
    {
        DetritusBreakDown = value;
    }
    else if (strcmp(MyParameter, "N contents") == 0)
    {
        NContents = value;
    }
    else if (strcmp(MyParameter, "Roughness") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            Roughness[j] = value;
    }
    else
        rc = false;

    return rc;
}

// AP, 2007.07.10
bool TSangoResuspendDeposit::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("SuspendedMatter");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Suspended matter");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    /*
     * save variables' names
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * save variables' values
     */
    PReadWrite->WriteSeparator();
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        if (strcmp(VariableNameArray[i], "Extinction coefficient Ktot") == 0)
        {
            PReadWrite->WriteCell(TotalExtinctionCoefficient[0], 8);
            break;
        }
    }
    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteSeparator(true);

    /*
     * save other variables' initial values
     */
    PReadWrite->WriteCell("Boundary SPM concentration values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Boundary POM concentration values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < 365; j++)
    {
        PReadWrite->WriteCell(BoundarySPMConcentration[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryPOMConcentration[j], 8);
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    PReadWrite->WriteCell("TPM values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("POM values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(SPMConcentration[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(POMConcentration[j], 8);
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TSangoResuspendDeposit::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("SuspendedMatter");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Suspended matter");
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
        if (strcmp(ParametersNameArray[i], "Karman constant") == 0)
        {
            PReadWrite->WriteCell(KarmanConstant, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Drag coefficient") == 0)
        {
            PReadWrite->WriteCell(DragCoefficient, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Critical velocity") == 0)
        {
            PReadWrite->WriteCell(CriticalVelocity, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Sinking velocity") == 0)
        {
            PReadWrite->WriteCell(SinkingVelocity, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Erosion rate") == 0)
        {
            PReadWrite->WriteCell(ErosionRate[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "Bottom sediment organic contents") == 0)
        {
            PReadWrite->WriteCell(BottomSedimentOrganicContents[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "Mineralization rate") == 0)
        {
            PReadWrite->WriteCell(DetritusBreakDown, 8);
        }
        else if (strcmp(ParametersNameArray[i], "N contents") == 0)
        {
            PReadWrite->WriteCell(NContents, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Roughness") == 0)
        {
            PReadWrite->WriteCell(Roughness[0], 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * save other parameters' initial values
     */
    PReadWrite->WriteCell("Roughness values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(Roughness[j], 8);
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}

void TSangoResuspendDeposit::Go()
{
	JulianDay = MyPEcoDynClass->GetJulianDay();
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    for (int i = 0; i < NumberOfBoxes; i++)
	{
   	if (MyTransportPointer != NULL)
		{
   		MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,
      											  i,
            	                          "Box depth",
               	                       ObjectCode);
   	}
      else
      	ABoxDepth = MyPEcoDynClass->GetBoxDepth(i);
      if (
      		(ABoxDepth > 0.0) &&
      	   (BoxArray[i].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2) && //No western ocean boundary
            (BoxArray[i].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) && //No eastern ocean boundary
            (BoxArray[i].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
            (BoxArray[i].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2) ||   //No northern ocean boundary
            (
               (ABoxDepth > 0.0) &&
            	(MyTransportPointer == NULL)
            )
         )
      {
   		Deposit(i);
   		Ressuspend(i);
         Mineralize(i);
   		ExtinctionCoefficientKTot(i);
      }
   }
}


void TSangoResuspendDeposit::Update(char* srcName, double Value,
						 int BoxNumber,
						 char* ParameterName,
						 int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);
    LogMessage("Update", srcName, MyParameter, Value, MyBoxNumber);

    if (strcmp(MyParameter, "TPM") == 0)
        SPMFlux[MyBoxNumber] = SPMFlux[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "POM") == 0)
        POMFlux[MyBoxNumber] = POMFlux[MyBoxNumber] + Value;
    else
    {
#ifdef __BORLANDC__
        char Caption[129];
        strcpy(Caption, MyParameter);
        strcat(Caption, " does not exist in ");
        strcat(Caption, GetEcoDynClassName());
        MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - Update 3",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TSangoResuspendDeposit::Update 3 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
    }
}


bool TSangoResuspendDeposit::SetVariableValue(char* srcName, double Value,
						 int BoxNumber,
						 char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "TPM") == 0)
		SPMConcentration[BoxNumber] = Value;
	else if (strcmp(VariableName, "POM") == 0)
		POMConcentration[BoxNumber] = Value;
	else if (strcmp(VariableName, "Extinction coefficient Ktot") == 0)
		TotalExtinctionCoefficient[BoxNumber] = Value;
    else if (strcmp(VariableName, "Boundary SPM concentration") == 0)
    {
        for (int j = 0; j < 365; j++)
    		BoundarySPMConcentration[j] = Value;
    }
    else if (strcmp(VariableName, "Boundary POM concentration") == 0)
    {
        for (int j = 0; j < 365; j++)
           BoundaryPOMConcentration[j] = Value;
    }
    else
        rc = false;

    return rc;
}


void TSangoResuspendDeposit::Inquiry(char* srcName, double &Value,
							                    int BoxNumber,
							                    char* ParameterName,
							                    int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "TPM") == 0)
		Value = SPMConcentration[MyBoxNumber];
	else if (strcmp(MyParameter, "POM") == 0)
		Value = POMConcentration[MyBoxNumber];
	else if (strcmp(MyParameter, "Extinction coefficient Ktot") == 0)
		Value = TotalExtinctionCoefficient[MyBoxNumber];
    else if (strcmp(MyParameter, "Boundary SPM concentration") == 0)
		Value = BoundarySPMConcentration[JulianDay - 1];
    else if (strcmp(MyParameter, "Boundary POM concentration") == 0)
        Value = BoundaryPOMConcentration[JulianDay - 1];
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
                      "EcoDynamo Alert - Inquiry 3",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TSangoResuspendDeposit::Inquiry 3 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

void TSangoResuspendDeposit::Integrate()
{

//	Generic = SPMConcentration; GenericFlux = SPMFlux;
	Integration(SPMConcentration, SPMFlux);
//	SPMConcentration = Generic; SPMFlux = GenericFlux;

//   Generic = POMConcentration; GenericFlux = POMFlux;
	Integration(POMConcentration, POMFlux);
//	POMConcentration = Generic; POMFlux = GenericFlux;

   for (int i = 0; i < NumberOfBoxes; i++)
   {
   	if (SPMConcentration[i] < 0.0) SPMConcentration[i] = 0.0;
   	if (POMConcentration[i] < 0.0) POMConcentration[i] = 0.0;
   }
   // Transport the SPM

   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
	{
		MyTransportPointer->Go(SPMConcentration, SPMConcentration, 0.0, 0.0);
      MyTransportPointer->Go(POMConcentration, POMConcentration, 0.0, 0.0);
      JulianDay = MyPEcoDynClass->GetJulianDay();

   	//Southern boundary
   	int i,j;
   	for (j = 0; j < GridColumns; j++)
   	{
   		i = 0;
         MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,
                                         i * GridColumns + j,
                                         "Box depth",
                                         ObjectCode);
  	 		if (
   				(ABoxDepth > 0.0) &&
            	(BoxArray[i * GridColumns + j].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2)  //Southern ocean boundary
   			)
      	{
      		SPMConcentration[i * GridColumns + j] = BoundarySPMConcentration[JulianDay-1];
            POMConcentration[i * GridColumns + j] = BoundaryPOMConcentration[JulianDay-1];
      	}
		}
      //Northern boundary
   	for (j = 0; j < GridColumns; j++)
   	{
   		i = GridLines - 1;
         MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,
                                         i * GridColumns + j,
                                         "Box depth",
                                         ObjectCode);
      	if (
   				(ABoxDepth > 0.0) &&
            	(BoxArray[i * GridColumns + j].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 2) //Northern ocean boundary
   			)
      	{
      		SPMConcentration[i * GridColumns + j] = BoundarySPMConcentration[JulianDay-1];
            POMConcentration[i * GridColumns + j] = BoundaryPOMConcentration[JulianDay-1];
      	}
   	}
      //Eastern boundary
   	for (i = 0; i < GridLines; i++)
   	{
   		j = GridColumns - 1;
         MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,
                                         i * GridColumns + j,
                                         "Box depth",
                                         ObjectCode);
      	if (
   				(ABoxDepth > 0.0) &&
            	(BoxArray[i * GridColumns + j].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2) //Eastern ocean boundary
   			)
      	{
      		SPMConcentration[i * GridColumns + j] = BoundarySPMConcentration[JulianDay-1];
            POMConcentration[i * GridColumns + j] = BoundaryPOMConcentration[JulianDay-1];
      	}
   	}
	}

   //This is to prevent Chl POM to be greater than overall POM. It should not be necessary in normal situations
   TEcoDynClass *MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer();
   if (MyPhytoPointer != NULL)
   {
   	double PHYORG, MyPhytoBiomass, MyChlorophyllToBiomass;
      for (int i = 0; i < NumberOfBoxes; i++)
      {
      	MyPhytoPointer->Inquiry(GetEcoDynClassName(), MyPhytoBiomass,
                                     i,
                                     "Phytoplankton biomass",
                                     ObjectCode);
         MyPhytoPointer->Inquiry(GetEcoDynClassName(), MyChlorophyllToBiomass,
            			                i,
                                     "Chlorophyll to biomass",
                                     ObjectCode);
         // Phytoplankton returns its mass as ug chl a l-1
         // convert to mg l-1 of organic dry matter
         PHYORG = MyPhytoBiomass / CUBIC * MyChlorophyllToBiomass; //mg/l
         if (PHYORG > POMConcentration[i])
         {
         	SPMConcentration[i] = SPMConcentration[i] + (PHYORG - POMConcentration[i]);
            POMConcentration[i] = POMConcentration[i] + (PHYORG - POMConcentration[i]);
         }
      }
   }
}

void TSangoResuspendDeposit::ExtinctionCoefficientKTot(int ABox)
{
	int MyBox = ABox;
   double MyPhytoBiomass, MyChlorophyllToCarbon;

   TEcoDynClass *MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer();
   if (MyPhytoPointer != NULL)
   {
   	MyPhytoPointer->Inquiry(GetEcoDynClassName(), MyPhytoBiomass,
            	                   MyBox,
               	                "Phytoplankton biomass",
                  	             ObjectCode);

      MyPhytoPointer->Inquiry(GetEcoDynClassName(), MyChlorophyllToCarbon,
            	                   MyBox,
               	                "Chlorophyll to Carbon",
                  	             ObjectCode);

      MyPhytoBiomass = MyPhytoBiomass * MyChlorophyllToCarbon / CUBIC; //convertion from ug Chl / l to mg C / l
   }
   else
   	MyPhytoBiomass = 0.0;

   //TotalExtinctionCoefficient[MyBox] = 0.4 + 0.00004 * SPMConcentration[MyBox] + 0.0000293 * MyPhytoBiomass;
   //m-1 - Esta rela??o ? baseada numa regress?o estabelecida por Baretta e utilizada na Tese de Mestrado do Fred Gazeau};

   TotalExtinctionCoefficient[MyBox] = 0.0484 * SPMConcentration[MyBox] + 0.0243;
   //Este regress?o ? baseada numa regress?o estabelecida pelo Cedric Bacher
   // a partir de dados de uma camapnha em Sango em Abril de 99
}


void TSangoResuspendDeposit::Deposit(int ABox)
{
	double TotalDeposition, OrganicDeposition;
   double MyPhytoBiomass, MyChlorophyllToBiomass, PHYORG;
   double MyDistance, MySinkingVelocity,LineNumber, ColumnNumber;
   int MyBox;
   MyBox = ABox;

   TEcoDynClass *MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer();
   if (MyPhytoPointer != NULL)
   {
   	MyPhytoPointer->Inquiry(GetEcoDynClassName(), MyPhytoBiomass,
            	                   MyBox,
               	                "Phytoplankton biomass",
                  	             ObjectCode);

      MyPhytoPointer->Inquiry(GetEcoDynClassName(), MyChlorophyllToBiomass,
            			             MyBox,
               	                "Chlorophyll to biomass",
                  	             ObjectCode);
      // Phytoplankton returns its mass as ug chl a l-1
      // convert to mg l-1 of organic dry matter
      PHYORG = MyPhytoBiomass / CUBIC * MyChlorophyllToBiomass; //mg/l
   }
   else
   	PHYORG = 0.0;

   LineNumber = floor(MyBox / GridColumns);
   ColumnNumber = MyBox - LineNumber * GridColumns;
   MyDistance = (GridColumns - 1 - ColumnNumber) * MyPEcoDynClass->GetBoxLength(MyBox);

   /*if ((MyBox > 100) && (MyBox < 120))
   {
   	Debugger(MyBox);
   	Debugger(LineNumber);
      Debugger(ColumnNumber);
      Debugger(MyDistance);
   }  */
   //Velocidade de deposi??o calculada em fun??o da dist?ncia ao mar.
   //Assume-se um decaimento exponencial da velocidade de deposi??o
   //do mar para o interior da ba?a.
   //O pressuposto ? de que os sedimentos junto ao mar poder?o ser mais grosseiros
   //e portanto a sua deposi??o mais r?pida. Este pressuposto pode justificar-se
   //pelo facto de haver um decr?scimo grande dos sedimentos na zona de contaco com
   //o mar, podendo depois a sua concentra??o n?o variar muito para o interior da ba?a
    
   MySinkingVelocity = 100.0 * exp(-0.000418 * 0.5 * MyDistance) / DAYSTOSECONDS;

   /*if ((MyBox > 100) && (MyBox < 120))
   	Debugger(MySinkingVelocity); */

   if ((SPMConcentration[MyBox] > PHYORG) && (POMConcentration[MyBox] > PHYORG))
   {
   	TotalDeposition = MySinkingVelocity * (SPMConcentration[MyBox] - PHYORG)  / ABoxDepth;
      OrganicDeposition = MySinkingVelocity * (POMConcentration[MyBox] - PHYORG) / ABoxDepth;
   }
   else
   {
   	TotalDeposition = 0.0;
      OrganicDeposition = 0.0;
   }


   //   TotalDeposition = SinkingVelocity * SPMConcentration[MyBox] / ABoxDepth;

   SPMFlux[MyBox] = SPMFlux[MyBox] - TotalDeposition;


   POMFlux[MyBox] = POMFlux[MyBox] - OrganicDeposition;

}


void TSangoResuspendDeposit::Ressuspend(int ABox)
{
	int MyBox;
   MyBox = ABox;
	double TotalResuspension, OrganicResuspension;
	// Ressuspension is added to the box flux - it must be a positive term in order to
	// reduce deposition: This term is simply added, because the material which is
	// ressuspended is not calculated from SPM in the water column, but has to do with
	// the bottom shear stress, sediment grain size and compaction - JGF - 94.04.11}
   if (ErosionRate[MyBox] > 0.0)
       TotalResuspension = ErosionRate[MyBox] * ShearStress(MyBox) /** DAYSTOSECONDS*/ / ABoxDepth; //Erosion rate in g / m2 / s,
                                                                                       //Resuspension in mg / L / s
   else
      TotalResuspension = 0.0;
   SPMFlux[MyBox] = SPMFlux[MyBox] + TotalResuspension;     //Ao retirar este fluxo estou a assumir que o SPM
                                                              //resuspendido inorganico se mantem junto ao fundo
                                                              //e ? imediatamente depositado

   if (SPMConcentration[MyBox] > 0)
   	OrganicResuspension = TotalResuspension * POMConcentration[MyBox] / SPMConcentration[MyBox];
   else
      OrganicResuspension = TotalResuspension * BottomSedimentOrganicContents[MyBox];

   POMFlux[MyBox] = POMFlux[MyBox] + OrganicResuspension;
   //SPMFlux[MyBox] = SPMFlux[MyBox] + OrganicResuspension;
}


void TSangoResuspendDeposit::Mineralize(int ABox)
{
	double POMBreakDown;
   int MyBox;

   MyBox = ABox;

	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();

   POMBreakDown = DetritusBreakDown * POMConcentration[MyBox] / DAYSTOSECONDS;

   POMFlux[MyBox] = POMFlux[MyBox] - POMBreakDown;
   SPMFlux[MyBox] = SPMFlux[MyBox] - POMBreakDown;

   if (MyNutrientPointer != NULL)
   {
      	/*MyNutrientPointer->Update(GetEcoDynClassName(), POCBreakDown * CUBIC /
                                    12 ,
												MyBox,
												"DOC",
												ObjectCode);*/

   	MyNutrientPointer->Update(GetEcoDynClassName(), POMBreakDown * NContents * CUBIC * CUBIC/
                                   NITROGENATOMICWEIGHT,
												MyBox,
												"Ammonia",
												ObjectCode);

   }
}

double TSangoResuspendDeposit::ShearStress(int ABox)
{
	double MyUVelocity, MyVVelocity, MyResultantVelocity, MyBottomDrag, Ustar;
	int MyBox;
   MyBox = ABox;
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
	{
   	MyTransportPointer->Inquiry(GetEcoDynClassName(), MyUVelocity,
                                      MyBox,
                                      "U Velocity",
                                      ObjectCode);
      MyTransportPointer->Inquiry(GetEcoDynClassName(), MyVVelocity,
                                      MyBox,
                                      "V Velocity",
                                      ObjectCode);
      MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBottomDrag,
                                      MyBox,
                                      "Drag coefficient",
                                      ObjectCode);
      MyVVelocity = - MyVVelocity; //This is because Bidiobjt returns the negative of VVelocity for graphical compatibility with Noesys
                                   //However, in this case this caution it is not necessary because in the calculation of
                                   //the resultant velocity (below), the squaring operation nulifies the minus sign

      MyResultantVelocity = sqrt(MyUVelocity * MyUVelocity + MyVVelocity * MyVVelocity);
   }
   else
   {
   	ABoxDepth = MyPEcoDynClass->GetBoxDepth(MyBox);
   	MyResultantVelocity = 0.0;
      MyBottomDrag = DragCoefficient;
   }
   Ustar = sqrt(MyBottomDrag) * MyResultantVelocity;

   //Ustar = KarmanConstant * fabs(MyResultantVelocity)/log(ABoxDepth/exp(1)/Roughness[MyBox]);
   //return MAX(pow(Ustar,2) / pow (CriticalVelocity,2),1.0);
   //return pow(Ustar,2) / pow (CriticalVelocity,2);

   //Ustar = MyResultantVelocity;
   if (Ustar > 0.02) Ustar = 0.02;

   if (Ustar < CriticalVelocity)
   	return 0.0;
   else
   	return (pow(Ustar,2) / pow (CriticalVelocity,2) - 1.0);
}

