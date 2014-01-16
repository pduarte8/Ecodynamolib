// Macrophyte object CPP code file
// @ Pedro Duarte, Outubro de 1999


/**
 * NPI work
 * 	08 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <stdio.h>
	#include <vcl.h>
	#include <math.h>
	#include <stdlib.h>
	#include <time.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "macrobjt.h"
#include "iodll.h"




TGelidiumSesquipedale::TGelidiumSesquipedale(TEcoDynClass* APEcoDynClass, char* className)
								 : TMacrophytes(APEcoDynClass, className)

{
	int i, j, k, m, n, X, Y;
   char MyParameter[65];

   DTemperature = new double[NumberOfBoxes];
   ETemperature = new double[NumberOfBoxes];
   /*Generic      = new double[NumberOfBoxes];
   GenericFlux  = new double[NumberOfBoxes];*/

   for (n = 0; n < NumberOfBoxes; n++)
   {
   	DTemperature[n] = 0.0;
      ETemperature[n] = 0.0;
      /*Generic[n] = 0.0;
      GenericFlux[n] = 0.0;  */
   }

	ParameterCount = 0;
	// Read in the parameters
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("GelidiumSesquipedale");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "GelidiumSesquipedale parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TGelidiumSesquipedale::TGelidiumSesquipedale - parameters file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        double MyValue = 0;

        // Just read number of classes
        if (PReadWrite->FindString("Gelidium sesquipedale", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            for (i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "Number of classes") == 0)
                {
                    PReadWrite->ReadNumber(X+4, i, MyValue);
                    NumberOfClasses = MyValue;
                //Debugger(NumberOfClasses);
                }
            }

            //ClassMaxLimits  = new double[NumberOfClasses];
            //ClassMinLimits  = new double[NumberOfClasses];
          //PhytoInitialSize     = new double[NumberOfClasses];
            PhytoIndividualSize  = new double[NumberOfClasses];
          PhytoIndividualWeight = new double[NumberOfClasses];
          AssimptoticBiomass = new double[NumberOfClasses];

          for (m = 0; m < NumberOfClasses; m++)
          {
            PhytoIndividualSize[m]  = 0.0; PhytoIndividualWeight[m] = 0.0; AssimptoticBiomass[m] = 0.0;
          }

            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
             PReadWrite->ReadString(X+2, i, MyParameter);
            if (strcmp(MyParameter, "G. sesquipedale size") == 0)
             {
                for (k = 0; k < NumberOfClasses; k++)
                    {
                    PReadWrite->ReadNumber( X + 4, i, MyValue);
                   PhytoIndividualSize[k] = MyValue;
                   //Debugger(PhytoIndividualSize[k]);
                        i++;
                }
             }
            else if (strcmp(MyParameter, "Gelidium upper limit1") == 0)
             {
                for (k = 0; k < NumberOfClasses; k++)
                    {
                    PReadWrite->ReadNumber( X + 4, i, MyValue);
                   AssimptoticBiomass[k] = MyValue;
                   //Debugger(AssimptoticBiomass[k]);
                        i++;
                }
             }

            else if (strcmp(MyParameter, "Gelidium_min_prof") == 0)
             {
                    PReadWrite->ReadNumber(X+4, i, MyValue);
                Gelidium_min_prof = MyValue;
                //Debugger(Gelidium_min_prof);
             }
            else if (strcmp(MyParameter, "Gelidium_max_prof") == 0)
             {
                    PReadWrite->ReadNumber(X+4, i, MyValue);
                Gelidium_max_prof = MyValue;
                //Debugger(Gelidium_max_prof);
             }

            else if (strcmp(MyParameter, "DefaultAEiler") == 0)
             {
                    PReadWrite->ReadNumber(X+4, i, MyValue);
                //Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                        AEiler[j] = MyValue;
             }
            else if (strcmp(MyParameter, "BEiler") == 0)
             {
                    PReadWrite->ReadNumber(X+4, i, MyValue);
                //Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                        BEiler[j] = MyValue;
             }
            else if (strcmp(MyParameter, "CEiler") == 0)
             {
                    PReadWrite->ReadNumber(X+4, i, MyValue);
                //Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                        CEiler[j] = MyValue;
             }
            else if (strcmp(MyParameter, "DTemperature") == 0)
             {
                    PReadWrite->ReadNumber(X+4, i, MyValue);
                //Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                        DTemperature[j] = MyValue;
             }
            else if (strcmp(MyParameter, "ETemperature") == 0)
             {
                    PReadWrite->ReadNumber(X+4, i, MyValue);
                //Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                        ETemperature[j] = MyValue;
             }
            else if (strcmp(MyParameter, "KValue") == 0)
             {
                    PReadWrite->ReadNumber(X+4, i, MyValue);
                    KValue = MyValue;
                    //Debugger(KValue);
             }
            else if (strcmp(MyParameter, "Carbon content") == 0)
             {
                    PReadWrite->ReadNumber(X+4, i, MyValue);
                    CarbonContent = MyValue;
                    //Debugger(MyValue);
             }
            else if (strcmp(MyParameter, "Exudation rate") == 0)
             {
                    PReadWrite->ReadNumber(X+4, i, MyValue);
                    ExudationRate = MyValue;
                    //Debugger(MyValue);
             }
            else if (strcmp(MyParameter, "Death loss") == 0)
             {
                    PReadWrite->ReadNumber(X+4, i, MyValue);
                //Debugger(MyValue);
                DeathLoss = MyValue;
             }
            else if (strcmp(MyParameter, "Assimptotic biomass") == 0)
             {
                    PReadWrite->ReadNumber(X+4, i, MyValue);
                //Debugger(MyValue);
                BiomassCarryingCapacity = MyValue;
             }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Gelidium sesquipedale - Undefined object", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TGelidiumSesquipedale::TGelidiumSesquipedale - undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

   ClassIncrement = PhytoIndividualSize[1] - PhytoIndividualSize[0];

/*   Generic                   = new double[NumberOfBoxes * NumberOfClasses];
	GenericFlux               = new double[NumberOfBoxes * NumberOfClasses];*/
	PhytoClassBiomass         = new double[NumberOfBoxes * NumberOfClasses];
	PhytoClassDensity         = new double[NumberOfBoxes * NumberOfClasses];
	PhytoClassBiomassFlux     = new double[NumberOfBoxes * NumberOfClasses];
	PhytoClassDensityFlux     = new double[NumberOfBoxes * NumberOfClasses];
   PhytoIndividualFlux       = new double[NumberOfBoxes * NumberOfClasses];
   //org_seston_mass           = new double[NumberOfBoxes * NumberOfClasses];
   //total_seston_mass         = new double[NumberOfBoxes * NumberOfClasses];
   GrowthRate                = new double[NumberOfBoxes * NumberOfClasses];

   ParticulateMass           = new double[NumberOfBoxes];
   ParticulateFlux           = new double[NumberOfBoxes];
   DissolvedMass             = new double[NumberOfBoxes];
   DissolvedFlux             = new double[NumberOfBoxes];
   RespiratedFlux            = new double[NumberOfBoxes];

   TransitionNumber         = new double[NumberOfClasses * NumberOfClasses];
   TransitionBiomass        = new double[NumberOfClasses * NumberOfClasses];
   TransitionWeight         = new double[NumberOfClasses * NumberOfClasses];

   for (n = 0; n < NumberOfBoxes; n++)
   {
   	ParticulateMass[n] = 0.0;
      ParticulateFlux[n] = 0.0;
      DissolvedMass[n]   = 0.0;
      DissolvedFlux[n]   = 0.0;
      RespiratedFlux[n]  = 0.0;
   }
   for (m = 0; m < NumberOfClasses; m++)
		for (n = 0; n < NumberOfBoxes; n++)
		{
/*      	Generic[m * NumberOfBoxes + n] 	              = 0.0;
         GenericFlux[m * NumberOfBoxes + n] 	           = 0.0;*/
			PhytoClassBiomass[m * NumberOfBoxes + n] 	     = 0.0;
			PhytoClassDensity[m * NumberOfBoxes + n]       = 0.0;
			PhytoClassBiomassFlux[m * NumberOfBoxes + n]   = 0.0;
			PhytoClassDensityFlux[m * NumberOfBoxes + n]   = 0.0;
			PhytoIndividualFlux[m * NumberOfBoxes + n]     = 0.0;
			//org_seston_mass[m * NumberOfBoxes + n]         = 0.0;
			//total_seston_mass[m * NumberOfBoxes + n]       = 0.0;
			GrowthRate[m * NumberOfBoxes + n] 	           = 0.0;
			//Mortality[m * NumberOfBoxes + n] 	     		  = 0.0;
		}


	for (m = 0; m < NumberOfClasses; m++)
		for (n = 0; n < NumberOfClasses; n++)
		{
			TransitionNumber[m * NumberOfClasses + n]  = 0;
			TransitionBiomass[m * NumberOfClasses + n] = 0;
			TransitionWeight[m * NumberOfClasses + n]  = 0;
		}

	// Read in the variables
	VariableCount = 0;
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("GelidiumSesquipedale");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "GelidiumSesquipedale variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TGelidiumSesquipedale::TGelidiumSesquipedale - variables file missing" << endl;
#endif  // __BORLANDC__
	}
    else {
        int XV, YV, offset;
        if (PReadWrite->FindString("Gelidium sesquipedale", X, Y))
        {

            PReadWrite->ReadNumber( X+1, Y, NumberOfVariables);
            // Create VNA array for variables
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names
    /*		for (i = Y; i < Y + NumberOfVariables; i++)
                PReadWrite->ReadString( X+2, i, VariableNameArray[i-Y]);
    */
          for (i = Y; i < Y + NumberOfVariables; i++)
          {
            PReadWrite->ReadString(X+2, i, MyParameter);
            strcpy(VariableNameArray[i-Y], MyParameter);
        
            if (strcmp(MyParameter, "G. sesquipedale biomass1") == 0)
            {
                if (PReadWrite->FindString("G. sesquipedale biomass1 values", XV, YV))
                {
                    for (k = 0; k < NumberOfClasses; k++)
                    {
                        offset = k * NumberOfBoxes;
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV + k, YV + 1 + j, PhytoClassBiomass[offset + j]);
                        }
                    }
                }
                else
                {
                    for (k = 0; k < NumberOfClasses; k++)
                    {
                        offset = k * NumberOfBoxes;
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber( X + 5 + j, i + k, PhytoClassBiomass[offset + j]);
                        }
                    }
                }
            }
          }
       }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object in G. sesquipedale", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TGelidiumSesquipedale::TGelidiumSesquipedale - undefined object in G. sesquipedale" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

   for (k = 0; k < NumberOfClasses; k++)
   {
   	for (int j = 0; j < NumberOfBoxes; j++)
      {
      	PhytoIndividualWeight[k * NumberOfBoxes + j] = WeightVsLenght(PhytoIndividualSize[k]);
         PhytoClassDensity[k * NumberOfBoxes + j] = PhytoClassBiomass[k * NumberOfBoxes + j] /
                                                    PhytoIndividualWeight[k * NumberOfBoxes + j];
         //Debugger(PhytoClassDensity[k * NumberOfBoxes + j]);

         //Debugger(PhytoIndividualWeight[k * NumberOfBoxes + j]);
      }
   }
}


TGelidiumSesquipedale::~TGelidiumSesquipedale()
{
    freeMemory();
}

void TGelidiumSesquipedale::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
       {
        delete [] DTemperature;
          delete [] DTemperature;
          /*delete [] Generic;
          delete [] GenericFlux;*/
          delete [] PhytoClassBiomass;
          delete [] PhytoClassDensity;
          //delete [] PhytoInitialSize;
          delete [] PhytoIndividualSize;
          delete [] PhytoIndividualWeight;
          delete [] PhytoClassBiomassFlux;
          delete [] PhytoClassDensityFlux;
          delete [] PhytoIndividualFlux;
          //delete [] org_seston_mass;
            //delete [] total_seston_mass;
            delete [] GrowthRate;
            //delete [] Mortality;
          delete [] TransitionNumber;
          delete [] TransitionBiomass;
        delete [] TransitionWeight;
          delete [] ParticulateMass;
          delete [] ParticulateFlux;
          delete [] DissolvedMass;
          delete [] DissolvedFlux;
          delete [] RespiratedFlux;
          delete [] AssimptoticBiomass;
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
				cerr << "TGelidiumSesquipedale::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

double TGelidiumSesquipedale::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "Number of classes") == 0)
     {
        value = NumberOfClasses;
     }
    else if (strcmp(MyParameter, "G. sesquipedale size") == 0)
     {
        value = PhytoIndividualSize[0];
     }
    else if (strcmp(MyParameter, "Gelidium upper limit1") == 0)
     {
        value = AssimptoticBiomass[0];
     }

    else if (strcmp(MyParameter, "Gelidium_min_prof") == 0)
     {
        value = Gelidium_min_prof;
     }
    else if (strcmp(MyParameter, "Gelidium_max_prof") == 0)
     {
        value = Gelidium_max_prof;
     }
    else if (strcmp(MyParameter, "DefaultAEiler") == 0)
     {
        value = AEiler[0];
     }
    else if (strcmp(MyParameter, "BEiler") == 0)
     {
        value = BEiler[0];
     }
    else if (strcmp(MyParameter, "CEiler") == 0)
     {
        value = CEiler[0];
     }
    else if (strcmp(MyParameter, "DTemperature") == 0)
     {
        value = DTemperature[0];
     }
    else if (strcmp(MyParameter, "ETemperature") == 0)
     {
        value = ETemperature[0];
     }
    else if (strcmp(MyParameter, "KValue") == 0)
     {
        value = KValue;
     }
    else if (strcmp(MyParameter, "Carbon content") == 0)
     {
        value = CarbonContent;
     }
    else if (strcmp(MyParameter, "Exudation rate") == 0)
     {
        value = ExudationRate;
     }
    else if (strcmp(MyParameter, "Death loss") == 0)
     {
        value = DeathLoss;
     }
    else if (strcmp(MyParameter, "Assimptotic biomass") == 0)
     {
        value = BiomassCarryingCapacity;
     }
    else
        value = 0.0;
    return value;
}

bool TGelidiumSesquipedale::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int j;

    if (strcmp(MyParameter, "Number of classes") == 0)
     {
        NumberOfClasses = value;
     }
    else if (strcmp(MyParameter, "G. sesquipedale size") == 0)
     {
        for (j = 0; j < NumberOfClasses; j++)
            PhytoIndividualSize[j] = value;
     }
    else if (strcmp(MyParameter, "Gelidium upper limit1") == 0)
     {
        for (j = 0; j < NumberOfClasses; j++)
            AssimptoticBiomass[j] = value;
     }

    else if (strcmp(MyParameter, "Gelidium_min_prof") == 0)
     {
        Gelidium_min_prof = value;
     }
    else if (strcmp(MyParameter, "Gelidium_max_prof") == 0)
     {
        Gelidium_max_prof = value;
     }
    else if (strcmp(MyParameter, "DefaultAEiler") == 0)
     {
        for (j = 0; j < NumberOfBoxes; j++)
            AEiler[j] = value;
     }
    else if (strcmp(MyParameter, "BEiler") == 0)
     {
        for (j = 0; j < NumberOfBoxes; j++)
            BEiler[j] = value;
     }
    else if (strcmp(MyParameter, "CEiler") == 0)
     {
        for (j = 0; j < NumberOfBoxes; j++)
            CEiler[j] = value;
     }
    else if (strcmp(MyParameter, "DTemperature") == 0)
     {
        for (j = 0; j < NumberOfBoxes; j++)
            DTemperature[j] = value;
     }
    else if (strcmp(MyParameter, "ETemperature") == 0)
     {
        for (j = 0; j < NumberOfBoxes; j++)
            ETemperature[j] = value;
     }
    else if (strcmp(MyParameter, "KValue") == 0)
     {
        KValue = value;
     }
    else if (strcmp(MyParameter, "Carbon content") == 0)
     {
        CarbonContent = value;
     }
    else if (strcmp(MyParameter, "Exudation rate") == 0)
     {
        ExudationRate = value;
     }
    else if (strcmp(MyParameter, "Death loss") == 0)
     {
        DeathLoss = value;
     }
    else if (strcmp(MyParameter, "Assimptotic biomass") == 0)
     {
        BiomassCarryingCapacity = value;
     }
    else
        rc = false;

    return rc;
}

// AP, 2007.07.13
bool TGelidiumSesquipedale::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("GelidiumSesquipedale");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Gelidium sesquipedale");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    /*
     * save variables' names and values
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * gelidium sesquipedale biomass initial values
     */
    PReadWrite->WriteCell("G. sesquipedale biomass1 values");
    /*
     * only information header
     */
    char classText[16];
    for (int k = 1; k < NumberOfClasses; k++)
    {
        PReadWrite->WriteSeparator();
        sprintf(classText, "Class %-d", k + 1);
        PReadWrite->WriteCell(classText);
    }
    PReadWrite->WriteSeparator(true);

    for (int j = 0; j < NumberOfBoxes; j++)
    {
        for (int k = 0; k < NumberOfClasses; k++)
        {
            PReadWrite->WriteCell(PhytoClassBiomass[k * NumberOfBoxes + j], 8);
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TGelidiumSesquipedale::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("GelidiumSesquipedale");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Gelidium sesquipedale");
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
        PReadWrite->WriteSeparator(); // empty column

        if (strcmp(ParametersNameArray[i], "Number of classes") == 0)
        {
            PReadWrite->WriteCell(NumberOfClasses, 0);
        }
        else if (strcmp(ParametersNameArray[i], "G. sesquipedale size") == 0)
        {
            PReadWrite->WriteCell(PhytoIndividualSize[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "Gelidium upper limit1") == 0)
        {
            PReadWrite->WriteCell(AssimptoticBiomass[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "Gelidium_min_prof") == 0)
        {
            PReadWrite->WriteCell(Gelidium_min_prof, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Gelidium_max_prof") == 0)
        {
            PReadWrite->WriteCell(Gelidium_max_prof, 8);
        }
        else if (strcmp(ParametersNameArray[i], "DefaultAEiler") == 0)
        {
            PReadWrite->WriteCell(AEiler[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "BEiler") == 0)
        {
            PReadWrite->WriteCell(BEiler[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "CEiler") == 0)
        {
            PReadWrite->WriteCell(CEiler[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "DTemperature") == 0)
        {
            PReadWrite->WriteCell(DTemperature[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "ETemperature") == 0)
        {
            PReadWrite->WriteCell(ETemperature[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "KValue") == 0)
        {
            PReadWrite->WriteCell(KValue, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Carbon content") == 0)
        {
            PReadWrite->WriteCell(CarbonContent, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Exudation rate") == 0)
        {
            PReadWrite->WriteCell(ExudationRate, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Death loss") == 0)
        {
            PReadWrite->WriteCell(DeathLoss, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Assimptotic biomass") == 0)
        {
            PReadWrite->WriteCell(BiomassCarryingCapacity, 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}

void TGelidiumSesquipedale::Go()
{
	JulianDay = MyPEcoDynClass->GetJulianDay();
	for (int i = 0; i < NumberOfBoxes; i++)
	{
   	//Generic[i] = 0.0;
      //GenericFlux[i] = 0.0;
   	GPP[i] = 0.0;
      NPP[i] = 0.0;
      PhytoProd[i] = 0.0;
      RespiratedFlux[i] = 0.0;
      DissolvedFlux[i] = 0.0;
      ParticulateFlux[i] = 0.0;
		for (int k = 0; k < NumberOfClasses; k++)
		{
      	PhytoClassBiomassFlux[k * NumberOfBoxes + i] = 0.0;
         PhytoClassDensityFlux[k * NumberOfBoxes + i] = 0.0;
         PhytoIndividualFlux[k * NumberOfBoxes + i] = 0.0;
		}
	}
	Production();
   Respiration();
   Exudation();
   for (int i = 0; i < NumberOfBoxes; i++)
	{
   	TotalBiomass = 0.0;
      for (int kk = 0; kk < NumberOfClasses; kk++)
      	TotalBiomass = TotalBiomass + PhytoClassBiomass[kk * NumberOfBoxes + i];
		for (int k = 0; k < NumberOfClasses; k++)
		{

      	if (PhytoIndividualWeight[k * NumberOfBoxes + i] + PhytoIndividualFlux[k * NumberOfBoxes + i] >= 0.0)
      		GrowthRate[k * NumberOfBoxes + i] = LengthVsWeight(PhytoIndividualWeight[k * NumberOfBoxes + i] +
                                                	            PhytoIndividualFlux[k * NumberOfBoxes + i]) -
                                             	LengthVsWeight(PhytoIndividualWeight[k * NumberOfBoxes + i]);
         else
            GrowthRate[k * NumberOfBoxes + i] = - LengthVsWeight(PhytoIndividualWeight[k * NumberOfBoxes + i]);

         if (TotalBiomass >= BiomassCarryingCapacity)
         	GrowthRate[k * NumberOfBoxes + i] = GrowthRate[k * NumberOfBoxes + i] *
            	                                 (BiomassCarryingCapacity - TotalBiomass)/
               	                              BiomassCarryingCapacity;

         //Debugger(GrowthRate[k * NumberOfBoxes + i]);
		}
	}
   FrondBreakage();
   Mortality();
}

void TGelidiumSesquipedale::Update(char* srcName, double Value,
                                    int BoxNumber,
									         char* ParameterName,
									         int AnObjectCode)
{
    LogMessage("Update", srcName, ParameterName, Value, BoxNumber);
}

void TGelidiumSesquipedale::Update(char* srcName, double Value,
                                    double HarvestableSize,
                                    int BoxNumber,
                                    int AnObjectCode)
{
   int FirstHarvestedClass = 0, k = 0;
   while (HarvestableSize >
          (PhytoIndividualSize[k * NumberOfBoxes + BoxNumber] - ClassIncrement / 2.0)
         )
   {
   	k++;
   }
   FirstHarvestedClass = k;
   //Debugger(FirstHarvestedClass);
   LogMessage("Update", srcName, "Phyto Class Density", Value, BoxNumber);

   for (int k = FirstHarvestedClass; k < NumberOfClasses; k++)
   {
   	PhytoClassDensityFlux[k * NumberOfBoxes + BoxNumber] =
         	PhytoClassDensityFlux[k * NumberOfBoxes + BoxNumber] -
            PhytoClassDensity[k * NumberOfBoxes + BoxNumber];
      if (FirstHarvestedClass > 0)
      	PhytoClassDensityFlux[(FirstHarvestedClass - 1) * NumberOfBoxes + BoxNumber] =
            	PhytoClassDensityFlux[(FirstHarvestedClass - 1) * NumberOfBoxes + BoxNumber] +
               PhytoClassDensity[k * NumberOfBoxes + BoxNumber];
	}
}


void TGelidiumSesquipedale::Inquiry(char* srcName, double &Value,
                                        int BoxNumber,
                                        char* ParameterName,
                                        int AnObjectCode)
{
	int i,j;
    double MyBiomass = 0.0, MyDensity = 0.0;
	char MyParameter[65], // 64 characters (max) for parameter name
					Myjstring[5],
					MyName[65];
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	for (i = 0; i < NumberOfVariables; i++)
		if (strcmp(MyParameter, VariableNameArray[i]) == 0)
		{

			for (j = 0; j < NumberOfClasses; j++)
			{
				strcpy( MyName, "G. sesquipedale biomass" );
				sprintf( Myjstring , "%i", j + 1 );
				strcat( MyName, Myjstring );
				if (strcmp(MyParameter, MyName) == 0)
					Value = PhytoClassBiomass[NumberOfBoxes * j + BoxNumber];
			}
			for (j = 0; j < NumberOfClasses; j++)
			{
				strcpy( MyName, "G. sesquipedale density" );
				sprintf( Myjstring , "%i", j + 1 );
				strcat( MyName, Myjstring );
				if (strcmp(MyParameter, MyName) == 0)
					Value = PhytoClassDensity[NumberOfBoxes * j + BoxNumber];
			}

			for (j = 0; j < NumberOfClasses; j++)
			{
				strcpy( MyName, "G. sesquipedale growth" );
				sprintf( Myjstring , "%i", j + 1 );
				strcat( MyName, Myjstring );
				if (strcmp(MyParameter, MyName) == 0)
					Value = GrowthRate[NumberOfBoxes * j + BoxNumber];
			}

			if (strcmp(MyParameter, "GPP") == 0)
				Value = GPP[BoxNumber];
         else
			if (strcmp(MyParameter, "NPP") == 0)
				Value = NPP[BoxNumber];
         else
			if (strcmp(MyParameter, "POM production") == 0)
				Value = ParticulateFlux[BoxNumber];
         else
         if (strcmp(MyParameter, "Respiration total") == 0)
         {
				Value = RespiratedFlux[BoxNumber]; //Debugger(2);Debugger(RespiratedFlux[BoxNumber]);
         }
			else
         if (strcmp(MyParameter, "DOM production") == 0)
         {
				Value = DissolvedFlux[BoxNumber]; //Debugger(1);Debugger(DissolvedFlux[BoxNumber]);
         }
         else
         if (strcmp(MyParameter, "Total biomass") == 0)
         {
         	TotalBiomass = 0.0;
         	for (int j = 0; j < NumberOfClasses; j++)
   				TotalBiomass = TotalBiomass + PhytoClassBiomass[NumberOfBoxes * j + BoxNumber];
            Value = TotalBiomass;
         }
         else
         if (strcmp(MyParameter, "G. sesquipedale class biomass 0-5") == 0)
         {
         	for (int j = 0; j < 5; j++)
   				MyBiomass = MyBiomass + PhytoClassBiomass[NumberOfBoxes * j + BoxNumber];
            Value = MyBiomass;
         }
         else
         if (strcmp(MyParameter, "G. sesquipedale class biomass 5-10") == 0)
         {
         	for (int j = 5; j < 10; j++)
   				MyBiomass = MyBiomass + PhytoClassBiomass[NumberOfBoxes * j + BoxNumber];
            Value = MyBiomass;
         }
         else
         if (strcmp(MyParameter, "G. sesquipedale class biomass 10-15") == 0)
         {
         	for (int j = 10; j < 15; j++)
   				MyBiomass = MyBiomass + PhytoClassBiomass[NumberOfBoxes * j + BoxNumber];
            Value = MyBiomass;
         }
         else
         if (strcmp(MyParameter, "G. sesquipedale class biomass >15") == 0)
         {
         	for (int j = 15; j < NumberOfClasses; j++)
   				MyBiomass = MyBiomass + PhytoClassBiomass[NumberOfBoxes * j + BoxNumber];
            Value = MyBiomass;
         }
         else
         if (strcmp(MyParameter, "G. sesquipedale class density 0-5") == 0)
         {
         	for (int j = 0; j < 5; j++)
   				MyDensity = MyDensity + PhytoClassDensity[NumberOfBoxes * j + BoxNumber];
            Value = MyDensity;
         }
         else
         if (strcmp(MyParameter, "G. sesquipedale class density 5-10") == 0)
         {
         	for (int j = 5; j < 10; j++)
   				MyDensity = MyDensity + PhytoClassDensity[NumberOfBoxes * j + BoxNumber];
            Value = MyDensity;
         }
         else
         if (strcmp(MyParameter, "G. sesquipedale class density 10-15") == 0)
         {
         	for (int j = 10; j < 15; j++)
   				MyDensity = MyDensity + PhytoClassDensity[NumberOfBoxes * j + BoxNumber];
            Value = MyDensity;
         }
         else
         if (strcmp(MyParameter, "G. sesquipedale class density >15") == 0)
         {
         	for (int j = 15; j < NumberOfClasses; j++)
   				MyDensity = MyDensity + PhytoClassDensity[NumberOfBoxes * j + BoxNumber];
            Value = MyDensity;
         }
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
			return;

		}

#ifdef __BORLANDC__
    char Caption[129];
    strcpy(Caption, MyParameter);
    strcat(Caption, " does not exist in ");
    strcat(Caption, GetEcoDynClassName());
    MessageBox(0,
              Caption,
              "EcoDynamo Alert - Inquiry 2",
              MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
		cerr << "TGelidiumSesquipedale::Inquiry 2 - " << MyParameter << " does not exits in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
    Value = 0;
}

bool TGelidiumSesquipedale::SetVariableValue(char* srcName, double Value,
									      int BoxNumber,
									     char* VariableName)
{
    bool rc = true;
	int j;
	char MyName[65];

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strstr(VariableName, "G. sesquipedale biomass") != NULL)
	{
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "G. sesquipedale biomass%i", j + 1 );
            if (strcmp(VariableName, MyName) == 0) {
                PhytoClassBiomass[NumberOfBoxes * j + BoxNumber] = Value;
                break;
            }
        }
    }
    else if (strstr(VariableName, "G. sesquipedale density") != NULL)
	{
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "G. sesquipedale density%i", j + 1 );
            if (strcmp(VariableName, MyName) == 0) {
                PhytoClassDensity[NumberOfBoxes * j + BoxNumber] = Value;
                break;
            }
        }
    }
    else if (strstr(VariableName, "G. sesquipedale growth") != NULL)
	{
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "G. sesquipedale growth%i", j + 1 );
            if (strcmp(VariableName, MyName) == 0) {
                GrowthRate[NumberOfBoxes * j + BoxNumber] = Value;
                break;
            }
        }
    }
    else if (strcmp(VariableName, "GPP") == 0)
        GPP[BoxNumber] = Value;
    else if (strcmp(VariableName, "NPP") == 0)
        NPP[BoxNumber] = Value;
    else if (strcmp(VariableName, "POM production") == 0)
        ParticulateFlux[BoxNumber] = Value;
    else if (strcmp(VariableName, "Respiration total") == 0)
        RespiratedFlux[BoxNumber] = Value;
    else if (strcmp(VariableName, "DOM production") == 0)
        DissolvedFlux[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}


void TGelidiumSesquipedale::Inquiry(char* srcName, double &Value,
                                        int BoxNumber,
                                        double HarvestSize,
                                        int AnObjectCode)
{
	double HarvestableBiomass = 0.0;
    for (int k = 0; k < NumberOfClasses; k++)
    {
   	    if (HarvestSize >
              (PhytoIndividualSize[k * NumberOfBoxes + BoxNumber] - ClassIncrement / 2.0)
           )
        {
      	    HarvestableBiomass = HarvestableBiomass +
                              PhytoClassBiomass[k * NumberOfBoxes + BoxNumber];
        }
    }
    Value = HarvestableBiomass;
    LogMessage("Inquiry", srcName, "Harvestable Biomass", Value, BoxNumber);
}



void TGelidiumSesquipedale::DemographicUpwind()
//Para que este algoritmo funcione importa que num passo de c?lculo
//os individuos de uma classe possam transitar somente para uma das classes
//adjacentes.
{
	int i,k;
	for (i = 0; i < NumberOfBoxes; i++)
	{
		//Inicializa??o das transfer?ncias entre classes
		for (k = 0; k < NumberOfClasses * NumberOfClasses; k++)
			TransitionNumber[k] = 0;

		for (k = 0; k < NumberOfClasses; k++)
		{
			if (
				 (GrowthRate[k * NumberOfBoxes + i] > 0.0) &&
				 (k < NumberOfClasses - 1)
				)
			{ //Situa??o "normal", em que a taxa de crescimento ? positiva
			  //A ?ltima classe n?o deve exibir crescimento positivo.
				TransitionNumber[k * NumberOfClasses + (k + 1)] =
					GrowthRate[k * NumberOfBoxes + i] *
					PhytoClassDensity[k * NumberOfBoxes + i] /
					ClassIncrement;//Ind?viduos perdidos pela classe k
										//para a classe k+1


			}
			if(
				(GrowthRate[k * NumberOfBoxes + i] < 0.0) &&
				(k > 0)
			  )
			{ //Situa??o "anormal", em que a taxa de crescimento ? negativa.
			  //A primeira classe n?o deve exibir crescimento negativo.


				TransitionNumber[k * NumberOfClasses + (k - 1)] =
						-GrowthRate[k * NumberOfBoxes + i] *          // - because negative growth rate
						PhytoClassDensity[k * NumberOfBoxes + i] /
						ClassIncrement;//Ind?viduos perdidos pela classe k
											//para a classe k-1
			}
		}
		for (k = 0; k < NumberOfClasses; k++)
		{
			for (int kk = 0; kk < NumberOfClasses; kk++)
			{

				PhytoClassDensityFlux[k * NumberOfBoxes + i] =
					PhytoClassDensityFlux[k * NumberOfBoxes + i] -
					TransitionNumber[k * NumberOfClasses + kk] +
					TransitionNumber[kk * NumberOfClasses + k];
			}
		}
	} //End of boxes

	//Integra??o dos fluxos de individuos
//	Generic = PhytoClassDensity;
//	GenericFlux = PhytoClassDensityFlux;
	Integration(NumberOfClasses, PhytoClassDensity, PhytoClassDensityFlux);
///	PhytoClassDensity = Generic; PhytoClassDensityFlux = GenericFlux;

	for (i = 0; i < NumberOfBoxes; i++)
	{
		for (int k = 0; k < NumberOfClasses; k++)
		{
      	//Debugger(PhytoClassDensity[k * NumberOfBoxes + i]);
         PhytoClassBiomass[k * NumberOfBoxes + i] = PhytoIndividualWeight[k] *
                                                    PhytoClassDensity[k * NumberOfBoxes + i];
         //Debugger(PhytoIndividualWeight[k]);
         PhytoClassBiomassFlux[k * NumberOfBoxes + i] = 0.0;
			if (PhytoClassBiomass[k * NumberOfBoxes + i] < 0.0)
				 PhytoClassBiomass[k * NumberOfBoxes + i] = 0.0;
			if (PhytoClassDensity[k * NumberOfBoxes + i] < 0.0)
				 PhytoClassDensity[k * NumberOfBoxes + i] = 0.0;
		}
	}
}

void TGelidiumSesquipedale::Integrate()
{
	DemographicUpwind();
}


void TGelidiumSesquipedale::Production()
{
	//const double KValue = 0.25;
	double ATop, ABottom, /*Productivity,*/ LightAtTop, LightAtBottom,
			 D, B1, B2, MyWaterTemperature, MyTidalHeight;

	TEcoDynClass *MyLightPointer = MyPEcoDynClass->GetLightPointer(),
				  *MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
              *MyTidePointer = MyPEcoDynClass->GetTidePointer(),
              *MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		ATop = Gelidium_min_prof;
      ABottom = Gelidium_max_prof;
      if (MyTidePointer != NULL)
      {
      	MyTidePointer->Inquiry(GetEcoDynClassName(), MyTidalHeight,
                                    i,
                                    "Tidal height",
                                    ObjectCode);
         ATop = ATop + MyTidalHeight / CUBIC;
         ABottom = ABottom + MyTidalHeight / CUBIC;
      }

		if (MySPMPointer != NULL)
			MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,
											  i,
											  "Extinction coefficient Ktot",
											  ObjectCode);
		if (MyLightPointer != NULL)
		{
      	MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
                                     ATop,
                                     i,
                                     "Sub-surface PAR irradiance",
                                     ObjectCode);
         //Debugger(LightAtTop);
			MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom,
                                     ABottom,
												 i,
												 "Sub-surface PAR irradiance",
												 ObjectCode);
         //Debugger(LightAtBottom);
			LightAtTop = LightAtTop * WATTSTOMICROEINSTEINS;
			LightAtBottom = LightAtBottom * WATTSTOMICROEINSTEINS;

         if (LightAtTop == 0)
         	Productivity = 0.0;
         else
         {
         	if (AEiler[i] != 0.0)
            {
            	D = BEiler[i] * BEiler[i] - 4 * AEiler[i] * CEiler[i];
               B1 = 2*AEiler[i] * LightAtTop + BEiler[i];
               B2 = 2*AEiler[i] * LightAtBottom + BEiler[i];
               if (D < 0.0)
               {
               	Productivity = 2.0 / (KValue * sqrt(-D)) *
					  						(atan(B1/sqrt(-D))- atan(B2/sqrt(-D)))
					 						/ (ABottom - ATop)
											* DAYSTOHOURS;
               }
               else if (D == 0.0)
               {
               	Productivity = 2.0 / KValue *
					 						(1.0 / B2 - 1.0 / B1)
											/ (ABottom - ATop)
				  							* DAYSTOHOURS;
               }
               else if (D > 0.0)
               {
				  		Productivity = 1.0 / (KValue*sqrt(D)) *
				 							log (((B1 - sqrt(D))*(B2 + sqrt(D)))/
											((B1 + sqrt(D)) * (B2 - sqrt(D))))
			  								/ (ABottom - ATop)
			 								* DAYSTOHOURS;
               }
            }
            if (AEiler[i] == 0)
            	Productivity = 1.0 / (KValue * BEiler[i]) *
				  						log (fabs(BEiler[i] * LightAtTop + CEiler[i])/
				 						fabs(BEiler[i] * LightAtBottom + CEiler[i]))
										/ (ABottom - ATop)
			  							* DAYSTOHOURS;
         }
      }
      if (MyWaterTemperaturePointer != NULL)
      {
      	MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,
                                                i,
                                                "Water temperature",
                                                ObjectCode);
         //Debugger(WaterTemperature);
         Productivity = Productivity * ArrheniusTemperatureLimitation(i, MyWaterTemperature);

      }

   	for (int k = 0; k < NumberOfClasses; k++)
   	{

         GPP[i] = GPP[i] + PhytoClassBiomass[k * NumberOfBoxes + i] * Productivity;
         PhytoProd[i] = PhytoProd[i] + GPP[i];
         PhytoIndividualFlux[k * NumberOfBoxes + i] = PhytoIndividualFlux[k * NumberOfBoxes + i] +
                                                      PhytoIndividualWeight[k * NumberOfBoxes + i] * Productivity /
                                                      CUBIC /
                                                      CarbonContent;
         /*if (Productivity > 0.0)
         {
            Debugger(k);
            Debugger(PhytoIndividualFlux[k * NumberOfBoxes + i]);
         } */
      }
	}
}


void TGelidiumSesquipedale::Respiration()
{
	double Resp, MyWaterTemperature;
	TEcoDynClass *MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   if (MyWaterTemperaturePointer != NULL)
   {
   	for (int i = 0; i < NumberOfBoxes; i++)
		{
   		MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,
         	                                    i,
            	                                 "Water temperature",
               	                              ObjectCode);
      	//Debugger(WaterTemperature);
      	Resp = exp(-0.95 - 27.6 / MyWaterTemperature);
      	//Debugger(Resp);
         for (int k = 0; k < NumberOfClasses; k++)
   		{
         	PhytoProd[i] = PhytoProd[i] - PhytoClassBiomass[k * NumberOfBoxes + i] * Resp;
            RespiratedFlux[i] = RespiratedFlux[i] + PhytoClassBiomass[k * NumberOfBoxes + i] * Resp /
                                                         CUBIC /
                                                         CarbonContent;

         	PhytoIndividualFlux[k * NumberOfBoxes + i] = PhytoIndividualFlux[k * NumberOfBoxes + i] -
                                                         PhytoIndividualWeight[k * NumberOfBoxes + i] * Resp /
                                                         CUBIC /
                                                         CarbonContent;

         }
         //Debugger(RespiratedFlux[i]);
      }
   }
}


void TGelidiumSesquipedale::Exudation()
{
	double Exud;
   for (int i = 0; i < NumberOfBoxes; i++)
   {
      for (int k = 0; k < NumberOfClasses; k++)
      {
      	DissolvedFlux[i] = DissolvedFlux[i] + PhytoClassBiomass[k * NumberOfBoxes + i] * ExudationRate /
                            CUBIC /
                            CarbonContent;

         PhytoProd[i] = PhytoProd[i] - DissolvedFlux[i];
         NPP[i] = PhytoProd[i];                   
         PhytoIndividualFlux[k * NumberOfBoxes + i] = PhytoIndividualFlux[k * NumberOfBoxes + i] -
                                                      PhytoIndividualWeight[k * NumberOfBoxes + i] * ExudationRate /
                                                      CUBIC /
                                                      CarbonContent;
      }
      //Debugger(DissolvedFlux[i]);
   }
}


void TGelidiumSesquipedale::FrondBreakage()
{
	int  i, k, DestinationClass;
   double BreakageProbability, /*dimensao_da_quebra,*/ BrokenBiomass;

   for (i = 0; i < NumberOfBoxes; i++)
   {
   	for (k = 1; k < NumberOfClasses; k++)
      {
      	if (
         		(PhytoClassBiomass[k * NumberOfBoxes + i] > 0) &&
               (PhytoClassDensity[k * NumberOfBoxes + i] > 0)
            )
         {
         	BreakageProbability = StochasticFrondBreakage(PhytoIndividualSize[k]);
            //Debugger(k);
            DestinationClass = k;
            while (DestinationClass >= k)
            {
            	DestinationClass = random(NumberOfClasses);
               //Debugger(DestinationClass);
            }


            BrokenBiomass = WeightVsLenght(PhytoIndividualSize[k])-
                            WeightVsLenght(PhytoIndividualSize[DestinationClass]);

            ParticulateFlux[i] = ParticulateFlux[i] + BrokenBiomass *
                                                      PhytoClassDensity[k * NumberOfBoxes + i]*
                                                      BreakageProbability;

            PhytoClassDensityFlux[k * NumberOfBoxes + i] = PhytoClassDensityFlux[k * NumberOfBoxes + i] -
                                                           PhytoClassDensity[k * NumberOfBoxes + i] *
                                                           BreakageProbability;
            PhytoClassDensityFlux[DestinationClass * NumberOfBoxes + i] =
                                                           PhytoClassDensityFlux[DestinationClass * NumberOfBoxes + i] +
                                                           PhytoClassDensity[k * NumberOfBoxes + i] *
                                                           BreakageProbability;

         }
      }
   }
}


void TGelidiumSesquipedale::Mortality()
{
   int i,k;
   double BrokenBiomass;
   
   for (i = 0; i < NumberOfBoxes; i++)
   {
      for (k = 1; k < NumberOfClasses; k++)
      {
      	if (
         		(PhytoClassBiomass[k * NumberOfBoxes + i] > 0.0) &&
               (PhytoClassDensity[k * NumberOfBoxes + i] > 0.0)
            )
         {

            PhytoClassDensityFlux[k * NumberOfBoxes + i] = PhytoClassDensityFlux[k * NumberOfBoxes + i]-
                                                       DeathLoss *
                                                       PhytoClassDensity[k * NumberOfBoxes + i];

            PhytoClassDensityFlux[0 * NumberOfBoxes + i] = PhytoClassDensityFlux[0 * NumberOfBoxes + i]+
                                                       DeathLoss *
                                                       PhytoClassDensity[k * NumberOfBoxes + i];
            BrokenBiomass = WeightVsLenght(PhytoIndividualSize[k])-
                            WeightVsLenght(PhytoIndividualSize[0]);
            ParticulateFlux[i] = ParticulateFlux[i] + BrokenBiomass *
                                                      DeathLoss *
                                                      PhytoClassDensity[k * NumberOfBoxes + i];

         }
      }
	}
}


double TGelidiumSesquipedale::WeightVsLenght(double Size)
{
   if (Size > 1.92)
   	return -0.008631 + 0.003773 * Size + 0.0003799 * Size * Size;
   else
   	return 0.0000136 * Size / 1.92;
	//Rela??o emp?rica obtida com todos os dados do Pedro Duarte
}


double TGelidiumSesquipedale::LengthVsWeight(double Weight)
{
   if (Weight > 0.0000136)
   	return (-0.003773 + sqrt((0.003773 * 0.003773) - 4 * 0.0003799 * (-0.008631 - Weight))) /
             (2 * 0.0003799);
   else
   	return Weight * 1.92 / 0.0000136;
   //Rela??o emp?rica obtida com todos os dados do Pedro Duarte
}


double TGelidiumSesquipedale::ArrheniusTemperatureLimitation(int ABoxNumber, double AWaterTemperature)
{
	return exp(DTemperature[ABoxNumber] -  ETemperature[ABoxNumber] / AWaterTemperature);
}



double TGelidiumSesquipedale::StochasticFrondBreakage(double size)
{
  /*if (size <= 15.0)
  	  return 0.0;
  /*else */
  if (size <= 15.0)
  	  return (0.003274 + 0.000409 * size);     //m?dia di?ria
  //Rela??o emp?rica obtida com os dados do Rui Santos e do Pedro Duarte
  else
     return (0.003274 + 0.000409 * size + 0.0004 * size * size);  
}


double TGelidiumSesquipedale::SizeOfBrokenPortion (double size)
{
   if (size < 0.403)
   	return 0.0;
   else
   	return -0.00098 + 0.002433 * size;   //m?dia di?ria
   //Rela??o emp?rica obtida com os dados do Rui Santos e do Pedro Duarte}
}    
