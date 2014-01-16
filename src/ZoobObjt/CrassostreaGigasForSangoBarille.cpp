// Zoobenthos object CPP code file
// @ Pedro Duarte
// Rev. A 12 Dez 2002
/**
 * NPI work
 * 	07 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
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
#include "zoobobjt.h"
#include "iodll.h"

// TCrassostreaGigas7 Class
//
// Constructors invoked outside EcoDyn shell...
//

TCrassostreaGigas7::TCrassostreaGigas7(char* className, float timeStep,
        int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aZooShellMass[],
        double aZooIndividualWeight[], double aZooClassDensity[],
        int nParams, double dryWeightOfAStandardAnimal, double allometricCoefficientForFeeding,
        double allometricCoefficientForRespiration, double meatEnergeticContents,
        double shellEnergeticContents, double intrinsicRateOfMortality,
        double respirationSlope, double respirationAtZero,
        double proportionOfEnergyAllocatedToShell,
        double proportionOfEnergyAllocatedToSoftTissue, double ammoniaToJoules,
        double numberOfClasses, double totalFWToMeatDW,
        double standardClearanceRate, double saturationTPM, double c1o,
        double c1m, double c2, double pFmaxo, double pFmaxm, double Xkpo,
        double Xkpm, double Xkp2o, double Xkp2m, double oxygenToJoules,
        double softTissueEnergyToTotalEnergy, double Fileseuil1,
        double pFmaxCHL, double XkpCHL)
    :TCrassostreaGigas6(className, timeStep, nLines, nColumns,
            aDepth, aLength, aWidth, aElevation,
            aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
            nVars, aVarsNames, aZooShellMass, aZooIndividualWeight,
            aZooClassDensity, nParams, dryWeightOfAStandardAnimal,
            allometricCoefficientForFeeding, allometricCoefficientForRespiration,
            meatEnergeticContents, shellEnergeticContents, intrinsicRateOfMortality,
            respirationSlope, respirationAtZero, proportionOfEnergyAllocatedToShell,
            proportionOfEnergyAllocatedToSoftTissue, ammoniaToJoules,
            numberOfClasses, totalFWToMeatDW, standardClearanceRate,
            saturationTPM, c1o, c1m, c2, pFmaxo, pFmaxm, Xkpo, Xkpm, Xkp2o,
            Xkp2m, oxygenToJoules, softTissueEnergyToTotalEnergy)
{
    xkpm = Xkpm;
    xkp2o = Xkp2o;
    xkp2m = Xkp2m;
}

TCrassostreaGigas7::TCrassostreaGigas7(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
        :TCrassostreaGigas6(className, timeStep, morphologyFilename,
                variablesFilename, parametersFilename)
{
    BuildCrassostreaGigas7();
}
// ...

TCrassostreaGigas7::TCrassostreaGigas7(TEcoDynClass* APEcoDynClass, char* className)
								 :	TCrassostreaGigas6(APEcoDynClass, className)
{
    BuildCrassostreaGigas7();
}

void TCrassostreaGigas7::BuildCrassostreaGigas7()
{
    double MyValue;
    int X, Y;
   // Read in the parameters
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("CrassostreaGigas");
	if (PReadWrite != NULL)
	{
        if (PReadWrite->FindString("Crassostrea gigas", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            //ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                //strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "filseuil1") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, filseuil1);
                //Debugger(filseuil1);
                }
                else if (strcmp(MyParameter, "PFmaxCHL") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, PFmaxCHL);
                //Debugger(PFmaxCHL);
                }
                else if (strcmp(MyParameter, "xkpCHL") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, xkpCHL);
                //Debugger(xkpCHL);
                }
                else if (strcmp(MyParameter, "Number of classes") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, NumberOfClasses);
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined parameters in C. gigas parameters file", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TCrassostreaGigas7::BuildCrassotreaGigas7 - Unknown Undefined parameters in C. gigas parameters file." << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}


TCrassostreaGigas7::~TCrassostreaGigas7()
{
    freeMemory();
}

void TCrassostreaGigas7::freeMemory()
{
}


double TCrassostreaGigas7::GetParameterValue(char* MyParameter)
{
  double value;

    // parameters from TCrassostreaGigas6
    if (strcmp(MyParameter, "Dry weight of a standard animal") == 0)
    {
        value = DryWeightOfAStandardAnimal;
    }
    else if (strcmp(MyParameter, "Allometric coefficient for feeding") == 0)
    {
        value = AllometricCoefficientForFeeding;
    }
    else if (strcmp(MyParameter, "Allometric coefficient for respiration") == 0)
    {
        value = AllometricCoefficientForRespiration;
    }
    else if (strcmp(MyParameter, "Meat energetic contents") == 0)
    {
        value = MeatEnergeticContents;
    }
    else if (strcmp(MyParameter, "Shell energetic contents") == 0)
    {
        value = ShellEnergeticContents;
    }
    else if (strcmp(MyParameter, "Intrinsic rate of mortality") == 0)
    {
        value = IntrinsicRateOfMortality;
    }
    else if (strcmp(MyParameter, "Respiration slope") == 0)
    {
        value = RespirationSlope;
    }
    else if (strcmp(MyParameter, "Respiration at 0") == 0)
    {
        value = RespirationAtZero;
    }
    else if (strcmp(MyParameter, "Proportion of energy allocated to shell") == 0)
    {
        value = ProportionOfEnergyAllocatedToShell;
    }
    else if (strcmp(MyParameter, "Proportion of energy allocated to soft tissue") == 0)
    {
        value = ProportionOfEnergyAllocatedToSoftTissue;
    }
    else if (strcmp(MyParameter, "umol NH4 to Joules") == 0)
    {
        value = AmmoniaToJoules;
    }
    else if (strcmp(MyParameter, "Number of classes") == 0)
    {
        value = NumberOfClasses;
    }
    else if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
    {
        value = TotalFWToMeatDW;
    }
    else if (strcmp(MyParameter, "Standard clearance rate") == 0)
    {
        value = StandardClearanceRate;
    }
    else if (strcmp(MyParameter, "TPM saturation threshold") == 0)
    {
        value = SaturationTPM;
    }
    else if (strcmp(MyParameter, "C1o") == 0)
    {
        value = C1o;
    }
    else if (strcmp(MyParameter, "C1m") == 0)
    {
        value = C1m;
    }
    else if (strcmp(MyParameter, "C2") == 0)
    {
        value = C2;
    }
    else if (strcmp(MyParameter, "PFmaxo") == 0)
    {
        value = PFmaxo;
    }
    else if (strcmp(MyParameter, "PFmaxm") == 0)
    {
        value = PFmaxm;
    }
    else if (strcmp(MyParameter, "xkpo") == 0)
    {
        value = xkpo;
    }
    else if (strcmp(MyParameter, "xkpm") == 0)
    {
        value = xkpm;
    }
    else if (strcmp(MyParameter, "xkp2o") == 0)
    {
        value = xkp2o;
    }
    else if (strcmp(MyParameter, "xkp2m") == 0)
    {
        value = xkp2m;
    }
    else if (strcmp(MyParameter, "Oxygen To Joules") == 0)
    {
        value = OxygenToJoules;
    }
    else if (strcmp(MyParameter, "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
    {
        value = SoftTissueEnergyToTotalEnergy;
    }

    // parameters from TCrassotreaGigas7
    else if (strcmp(MyParameter, "filseuil1") == 0)
    {
        value = filseuil1;
    }
    else if (strcmp(MyParameter, "PFmaxCHL") == 0)
    {
        value = PFmaxCHL;
    }
    else if (strcmp(MyParameter, "xkpCHL") == 0)
    {
        value = xkpCHL;
    }
    else
        value = 0.0;
    return value;
}

bool TCrassostreaGigas7::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;

    // parameters from TCrassostreaGigas6
    if (strcmp(MyParameter, "Dry weight of a standard animal") == 0)
    {
        DryWeightOfAStandardAnimal = value;
    }
    else if (strcmp(MyParameter, "Allometric coefficient for feeding") == 0)
    {
        AllometricCoefficientForFeeding = value;
    }
    else if (strcmp(MyParameter, "Allometric coefficient for respiration") == 0)
    {
        AllometricCoefficientForRespiration = value;
    }
    else if (strcmp(MyParameter, "Meat energetic contents") == 0)
    {
        MeatEnergeticContents = value;
    }
    else if (strcmp(MyParameter, "Shell energetic contents") == 0)
    {
        ShellEnergeticContents = value;
    }
    else if (strcmp(MyParameter, "Intrinsic rate of mortality") == 0)
    {
        IntrinsicRateOfMortality = value;
    }
    else if (strcmp(MyParameter, "Respiration slope") == 0)
    {
        RespirationSlope = value;
    }
    else if (strcmp(MyParameter, "Respiration at 0") == 0)
    {
        RespirationAtZero = value;
    }
    else if (strcmp(MyParameter, "Proportion of energy allocated to shell") == 0)
    {
        ProportionOfEnergyAllocatedToShell = value;
    }
    else if (strcmp(MyParameter, "Proportion of energy allocated to soft tissue") == 0)
    {
        ProportionOfEnergyAllocatedToSoftTissue = value;
    }
    else if (strcmp(MyParameter, "umol NH4 to Joules") == 0)
    {
        AmmoniaToJoules = value;
    }
    else if (strcmp(MyParameter, "Number of classes") == 0)
    {
        NumberOfClasses = value;
    }
    else if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
    {
        TotalFWToMeatDW = value;
    }
    else if (strcmp(MyParameter, "Standard clearance rate") == 0)
    {
        StandardClearanceRate = value;
    }
    else if (strcmp(MyParameter, "TPM saturation threshold") == 0)
    {
        SaturationTPM = value;
    }
    else if (strcmp(MyParameter, "C1o") == 0)
    {
        C1o = value;
    }
    else if (strcmp(MyParameter, "C1m") == 0)
    {
        C1m = value;
    }
    else if (strcmp(MyParameter, "C2") == 0)
    {
        C2 = value;
    }
    else if (strcmp(MyParameter, "PFmaxo") == 0)
    {
        PFmaxo = value;
    }
    else if (strcmp(MyParameter, "PFmaxm") == 0)
    {
        PFmaxm = value;
    }
    else if (strcmp(MyParameter, "xkpo") == 0)
    {
        xkpo = value;
    }
    else if (strcmp(MyParameter, "xkpm") == 0)
    {
        xkpm = value;
    }
    else if (strcmp(MyParameter, "xkp2o") == 0)
    {
        xkp2o = value;
    }
    else if (strcmp(MyParameter, "xkp2m") == 0)
    {
        xkp2m = value;
    }
    else if (strcmp(MyParameter, "Oxygen To Joules") == 0)
    {
        OxygenToJoules = value;
    }
    else if (strcmp(MyParameter, "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
    {
        SoftTissueEnergyToTotalEnergy = value;
    }

    // parameters from TCrassostreaGigas7
    else if (strcmp(MyParameter, "filseuil1") == 0)
    {
        filseuil1 = value;
    }
    else if (strcmp(MyParameter, "PFmaxCHL") == 0)
    {
        PFmaxCHL = value;
    }
    else if (strcmp(MyParameter, "xkpCHL") == 0)
    {
        xkpCHL = value;
    }
    else
        rc = false;

    return rc;
}

// AP, 2007.07.16
bool TCrassostreaGigas7::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("CrassostreaGigas");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Crassostrea gigas");
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

        // parameters from TCrassostreaGiags6
        if (strcmp(ParametersNameArray[i], "Number of classes") == 0)
        {
            PReadWrite->WriteCell(NumberOfClasses, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Dry weight of a standard animal") == 0)
        {
            PReadWrite->WriteCell(DryWeightOfAStandardAnimal, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Allometric coefficient for feeding") == 0)
        {
            PReadWrite->WriteCell(AllometricCoefficientForFeeding, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Allometric coefficient for respiration") == 0)
        {
            PReadWrite->WriteCell(AllometricCoefficientForRespiration, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Meat energetic contents") == 0)
        {
            PReadWrite->WriteCell(MeatEnergeticContents, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Shell energetic contents") == 0)
        {
            PReadWrite->WriteCell(ShellEnergeticContents, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Intrinsic rate of mortality") == 0)
        {
            PReadWrite->WriteCell(IntrinsicRateOfMortality, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Respiration slope") == 0)
        {
            PReadWrite->WriteCell(RespirationSlope, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Respiration at 0") == 0)
        {
            PReadWrite->WriteCell(RespirationAtZero, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Proportion of energy allocated to shell") == 0)
        {
            PReadWrite->WriteCell(ProportionOfEnergyAllocatedToShell, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Proportion of energy allocated to soft tissue") == 0)
        {
            PReadWrite->WriteCell(ProportionOfEnergyAllocatedToSoftTissue, 8);
        }
        else if (strcmp(ParametersNameArray[i], "umol NH4 to Joules") == 0)
        {
            PReadWrite->WriteCell(AmmoniaToJoules, 8);
        }
        else if (strcmp(ParametersNameArray[i], "TotalFW to meatDW") == 0)
        {
            PReadWrite->WriteCell(TotalFWToMeatDW, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Standard clearance rate") == 0)
        {
            PReadWrite->WriteCell(StandardClearanceRate, 8);
        }
        else if (strcmp(ParametersNameArray[i], "TPM saturation threshold") == 0)
        {
            PReadWrite->WriteCell(SaturationTPM, 8);
        }
        else if (strcmp(ParametersNameArray[i], "C1o") == 0)
        {
            PReadWrite->WriteCell(C1o, 8);
        }
        else if (strcmp(ParametersNameArray[i], "C1m") == 0)
        {
            PReadWrite->WriteCell(C1m, 8);
        }
        else if (strcmp(ParametersNameArray[i], "C2") == 0)
        {
            PReadWrite->WriteCell(C2, 8);
        }
        else if (strcmp(ParametersNameArray[i], "PFmaxo") == 0)
        {
            PReadWrite->WriteCell(PFmaxo, 8);
        }
        else if (strcmp(ParametersNameArray[i], "PFmaxm") == 0)
        {
            PReadWrite->WriteCell(PFmaxm, 8);
        }
        else if (strcmp(ParametersNameArray[i], "xkpo") == 0)
        {
            PReadWrite->WriteCell(xkpo, 8);
        }
        else if (strcmp(ParametersNameArray[i], "xkpm") == 0)
        {
            PReadWrite->WriteCell(xkpm, 8);
        }
        else if (strcmp(ParametersNameArray[i], "xkp2o") == 0)
        {
            PReadWrite->WriteCell(xkp2o, 8);
        }
        else if (strcmp(ParametersNameArray[i], "xkp2m") == 0)
        {
            PReadWrite->WriteCell(xkp2m, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Oxygen To Joules") == 0)
        {
            PReadWrite->WriteCell(OxygenToJoules, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
        {
            PReadWrite->WriteCell(SoftTissueEnergyToTotalEnergy, 8);
        }

        // parameters from TCrassostreaGigas7
        else if (strcmp(ParametersNameArray[i], "filseuil1") == 0)
        {
            PReadWrite->WriteCell(filseuil1, 8);
        }
        else if (strcmp(ParametersNameArray[i], "PFmaxCHL") == 0)
        {
            PReadWrite->WriteCell(PFmaxCHL, 8);
        }
        else if (strcmp(ParametersNameArray[i], "xkpCHL") == 0)
        {
            PReadWrite->WriteCell(xkpCHL, 8);
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

void TCrassostreaGigas7::Feed(int ABox, int AClass)
{
	int MyBox, MyClass;
   double Delta, Delta1, Delta2, FRDET, PFo, PFm, PFCHL /*Organic, chlorophyll and inorganic pseudofaeces rate*/;
   double EFFETTEMP, FILTMES, FILT, WtStandNEA;
   double const NEAThreshold = 500.0 / DAYSTOSECONDS; //J/ind/s  Calibrated in Stella
   MyBox = ABox;
   MyClass = AClass;


   if ((TPM > 0.001) && (POM > 0.0001) && (PHYORG > 0.0001))
   {
    double MyDepth;
    TEcoDynClass * MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    if (MyTransportPointer != NULL)
      MyTransportPointer->Inquiry(GetEcoDynClassName(), MyDepth,
			  											  MyBox,
			 											  "Box depth",
														  ObjectCode);
    else
      MyDepth = MyPEcoDynClass->GetBoxDepth(MyBox);

   	Delta = SaturationTPM - TPM;
      if (Delta > 0.0) Delta = 0.0;

      EFFETTEMP = TemperatureEffectsOnFeedingRate(MyBox);
      if (TPM > filseuil1)
      	FILTMES = TPMEffectsOnFeedingRate(TPM);
      else
      	FILTMES = StandardClearanceRate;
      FILT = FILTMES - EFFETTEMP;



      if (FILT < 0.0) FILT = 0.0;
      ClearanceRate[MyClass * NumberOfBoxes + MyBox] = FILT *
         AllometricCorrection (ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox],AllometricCoefficientForFeeding)*
         exp(0.07 * Delta);  //{L/h/individuo}


      FRTPM[MyBox] = ClearanceRate[MyClass * NumberOfBoxes + MyBox] * TPM / CUBIC / HOURSTOSECONDS /** DAYSTOHOURS*/ * Retention(TPM);  //g/d/individual


      if (FRTPM[MyBox] * CUBIC * ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / MyDepth / CUBIC * ATimeStep > TPM)

      {

        //Debugger(4);

        FRTPM[MyBox]= TPM / ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / MyDepth / CUBIC / ATimeStep / CUBIC;

      }


      FRPOM[MyBox] = FRTPM[MyBox] * POM / TPM;

      if (FRPOM[MyBox] * CUBIC * ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / MyDepth / CUBIC * ATimeStep > POM)

      {

        //Debugger(5);

        FRPOM[MyBox]= POM / ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / MyDepth / CUBIC / ATimeStep / CUBIC;

      }

      if (PIM / TPM < 1.0)

      {

      	FRPIM[MyBox] = FRTPM[MyBox]* PIM / TPM;
         if (FRPIM[MyBox] * CUBIC * ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / MyDepth / CUBIC* ATimeStep > PIM)
         {
            //Debugger(6);
      	    FRPIM[MyBox]= PIM / ZooClassDensity[MyClass * NumberOfBoxes + MyBox]/ MyDepth / CUBIC / ATimeStep / CUBIC;
         }
      }
      else
         FRPIM[MyBox] = FRTPM[MyBox];

      if(PHYORG / POM < 1.0)
      {
   		FRPHY[MyBox] = FRTPM[MyBox] * PHYORG / TPM;
         if (FRPHY[MyBox] * CUBIC * ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / MyDepth / CUBIC * ATimeStep > PHYORG)
         {
            //Debugger(7);
      	    FRPHY[MyBox]= PHYORG / ZooClassDensity[MyClass * NumberOfBoxes + MyBox]/ MyDepth  / CUBIC / ATimeStep / CUBIC;
         }
      }
      else
         FRPHY[MyBox] = FRPOM[MyBox];

      FRDET = FRPOM[MyBox] - FRPHY[MyBox];
      if (FRDET < 0.0) FRDET = 0.0;



      //Inorganic pseudofaeces production rate
      Delta1 = C1m - TPM;
      if (Delta1 > 0.0) Delta1 = 0.0;

      PFm = PFmaxm * (1.0 - exp(xkpm * Delta1));


      


      //Organic and Chl pseudofaeces production rate

      Delta2 = C1o - TPM;

      if (Delta2 > 0.0) Delta2 = 0.0;


      PFo = PFmaxo * (1.0 - exp(xkpo * Delta1))+ ((PFmaxm - PFmaxo) * (1.0 - exp(xkp2o * Delta2)));


      PFCHL = PFmaxCHL * (1.0 - exp(xkpCHL * Delta1))+ ((PFmaxm - PFmaxCHL) * (1.0 - exp(xkp2o * Delta2)));

      //end of organic pseudofaeces rate calculation


      IRPIM[MyBox] = FRPIM[MyBox] * (1.0 - PFm);



      IRPHY[MyBox] = (1.0 - PFCHL) * FRPHY[MyBox];


      IRDET[MyBox] = (1.0 - PFo) * FRDET;

      IRTPM[MyBox] = IRPIM[MyBox] + IRPHY[MyBox] + IRDET[MyBox];
      if (FRTPM[MyBox] < IRTPM[MyBox])
      	FRTPM[MyBox] = IRTPM[MyBox];

      if (IRPIM[MyBox] < 0.0)
      IRPIM[MyBox] = 0.0;
   }
   else
   {
   	FRTPM[MyBox] = 0.0;
      IRTPM[MyBox] = 0.0;
      RRTPM[MyBox] = 0.0;
      ClearanceRate[MyClass * NumberOfBoxes + MyBox] = 0.0;
      FRPIM[MyBox] = 0.0;
      IRPIM[MyBox] = 0.0;
      RRPIM[MyBox] = 0.0;
      FRPHY[MyBox] = 0.0;
      IRPHY[MyBox] = 0.0;
      FRDET = 0.0;
      IRDET[MyBox] = 0.0;
      FRPOM[MyBox] = 0.0;
   }


   /*if (MyBox == 228)
   {
   	Debugger(PFm);
      Debugger(PFo);
      Debugger(PFCHL);
   	Debugger(IRTPM[MyBox]);
      Debugger(IRPIM[MyBox]);
      Debugger(IRPHY[MyBox]);
      Debugger(IRDET[MyBox]);
   }*/

   RRTPM[MyBox] = FRTPM[MyBox] - IRTPM[MyBox];
   RRPIM[MyBox] = FRPIM[MyBox] - IRPIM[MyBox];

   if (IRPHY[MyBox] + IRDET[MyBox] + IRPIM[MyBox] > 0.0000000000000001)
   	OCI[MyBox] = (IRPHY[MyBox] + IRDET[MyBox]) / (IRPHY[MyBox] + IRDET[MyBox] + IRPIM[MyBox]); //Organic content of ingested matter; fraction
   else
      OCI[MyBox] = 0.0;

   if (OCI[MyBox] > 0.0000000000000001)
   	NAEIO[MyBox] = 0.74 - (0.084 * (1.0 / OCI[MyBox])); //Net absorption efficiency from ingested organics; fraction
   else
      NAEIO[MyBox] = 0.0;
   if (NAEIO[MyBox] < 0.0)
   NAEIO[MyBox] = 0.0;

   NEA[MyBox] = NAEIO[MyBox] * (23500.0 * IRPHY[MyBox] + 6100.0 * IRDET[MyBox]); //Net energy absorption rate; J/d

   WtStandNEA = NEA[MyBox] /
   				 AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox],AllometricCoefficientForFeeding);

   if (WtStandNEA > NEAThreshold)
   	NEA[MyBox] = NEAThreshold * AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox],AllometricCoefficientForFeeding);


   FRTPM[MyBox] = FRTPM[MyBox] * CUBIC; //convers?o para mg/d/indivividual

   FRPIM[MyBox] = FRPIM[MyBox] * CUBIC; //convers?o para mg/d/indivividual


   /*if ((MyPEcoDynClass->GetJulianDay() == 3) && (MyPEcoDynClass->GetCurrentTime() > 3.4)
                && (MyBox >= 195))
                Debugger(57);  */

   FRPOM[MyBox] = FRPOM[MyBox] * CUBIC; //convers?o para mg/d/indivividual

   /*if ((MyPEcoDynClass->GetJulianDay() == 3) && (MyPEcoDynClass->GetCurrentTime() > 3.4)
                && (MyBox >= 195))
                Debugger(58);   */

   FRPHY[MyBox] = FRPHY[MyBox] * CUBIC; //convers?o para mg/d/indivividual
   IRTPM[MyBox] = IRTPM[MyBox] * CUBIC; //convers?o para mg/d/indivividual
   IRPHY[MyBox] = IRPHY[MyBox] * CUBIC; //convers?o para mg/d/indivividual
   IRDET[MyBox] = IRDET[MyBox] * CUBIC; //convers?o para mg/d/indivividual
   IRPIM[MyBox] = IRPIM[MyBox] * CUBIC; //convers?o para mg/d/indivividual
   RRTPM[MyBox] = RRTPM[MyBox] * CUBIC; //convers?o para mg/d/indivividual
   RRPIM[MyBox] = RRPIM[MyBox] * CUBIC; //convers?o para mg/d/indivividual
   OIR[MyBox] = IRPHY[MyBox] + IRDET[MyBox]; //mg/d/individual
}

void TCrassostreaGigas7::Respire(int ABox, int AClass)
{
   int MyBox, MyClass;

   MyBox = ABox;
   MyClass = AClass;

   TotalRespiration = /*DAYSTOHOURS **/ TemperatureEffectsOnMaintenanceHeatLosses(MyBox)/ HOURSTOSECONDS * OxygenToJoules *
                     AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox], AllometricCoefficientForRespiration); // J s-1
   if (TotalRespiration < 0.0) TotalRespiration = 0.0;
}




double TCrassostreaGigas7::TemperatureEffectsOnFeedingRate(int ABoxNumber)
{
	TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   if (MyWaterTemperaturePointer != NULL)
   {
   	double WaterTemperature;
   	int MyBoxNumber;
      MyBoxNumber = ABoxNumber;
   	MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterTemperature,
                                             MyBoxNumber,
                                             "Water temperature",
                                             ObjectCode);
      return 0.013 * pow((WaterTemperature-18.954),2);
      //Equation is from Barill? et al.(1997) Aquat. Living Resources, 10: 31-48
   }
   else
#ifdef __BORLANDC__
      MessageBox(0, "Unknown temperature in TCrassostreaGigas7 Feeding", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TCrassostreaGigas7::TemperatureEffectsOnFeedingRate - Unknown temperature in TCrassostreaGigas7 Feeding." << endl;
#endif  // __BORLANDC__

    return 1.0;  // is this the default value??????? AP, 2006.07.27
}

double TCrassostreaGigas7::TPMEffectsOnFeedingRate(double ATPM)
{
	double MyTPM;
   double const fo1 = 5.4; //L/h/g, Ordenada da lei de influ?ncia negativa do seston na filtra??o,
                          //Barill? & Prou, 1993 in Barill? et al., 1997, Aq. Living Resour., 10: 31-48 }
   double const fo2 = -0.0102; //declive da influ?ncia negativa do seston na filtra??o,
                              //Barill? & Prou, 1993 in Barill? et al., 1997, Aq. Living Resour., 10: 31-48 }
   MyTPM = ATPM;
   return (fo2 * MyTPM + fo1);
}

double TCrassostreaGigas7::Retention(double ATPM)
{
	double MyTPM;
   double const retseuil = 70.0; //mg/L
   double const retmin = 0.7; //Barill? et al, 1993  in Barill? et al., 1997, Aq. Living Resour., 10: 31-43
   double const aret = -0.0043; // Barill? et al, 1993  in Barill? et al., 1997, Aq. Living Resour., 10: 31-43
   double const bret = 1.0; // Barill? et al, 1993  in Barill? et al., 1997, Aq. Living Resour., 10: 31-43
   MyTPM = ATPM;
   
   if (MyTPM < retseuil)
   	return aret * MyTPM + bret;
   else
      return retmin;
}


double TCrassostreaGigas7::TemperatureEffectsOnMaintenanceHeatLosses(int ABoxNumber)
{
	double const pmg2ml =  0.7; //ml O2 / mgO2
   double const resp1 = 0.432; //mg O2 / h /g - Bougrier et al, 1995 in Barill? et al., 1997, Aq. Living Resour., 10: 31-48
   double const resp2 = 0.613; //mg O2 / h /g - Bougrier et al, 1995 in Barill? et al., 1997, Aq. Living Resour., 10: 31-48
   double const resp3 = 1.042; //Bougrier et al, 1995 in Barill? et al., 1997, Aq. Living Resour., 10: 31-48
	TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   if (MyWaterTemperaturePointer != NULL)
   {
   	double WaterTemperature;
   	int MyBoxNumber;
      MyBoxNumber = ABoxNumber;
   	MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterTemperature,
                                             MyBoxNumber,
                                             "Water temperature",
                                             ObjectCode);

      return pmg2ml * (-resp1 + (resp2 * pow(resp3, WaterTemperature)));
      //ml O2 / h /g - Bougrier et al, 1995 in Barill? et al., 1997, Aq. Living Resour., 10: 31-48
   }
   else
#ifdef __BORLANDC__
   	MessageBox(0, "Unknown temperature in TCrassostreaGigas7 Respiration", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TCrassostreaGigas7::TemperatureEffectsOnMaintenanceHeatLosses - Unknown temperature in TCrassostreaGigas7 Respiration." << endl;
#endif  // __BORLANDC__

    return 1.0;  // is this the default value??????? AP, 2006.07.27
}

double TCrassostreaGigas7::OxygenToNitrogenRatio(int ABoxNumber, int AClass)
{
	int MyBoxNumber, MyClass;
   double WtStandNEA;
   MyBoxNumber = ABoxNumber;
   MyClass = AClass;
   WtStandNEA = NEA[MyBoxNumber] * DAYSTOSECONDS /
   				 AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + MyBoxNumber],AllometricCoefficientForFeeding);

 	if ((10.0 + 0.08 * WtStandNEA) > 50.0)
   	return 50.0;
   else
	if ((10.0 + 0.08 * WtStandNEA) < 10.0)
   	return 10.0;
   else
 		return (10.0 + 0.08 * WtStandNEA);
   //Pode variar entre 10 e 50, na propor??o de NEA }
}


