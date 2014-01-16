/**
 * NPI work
 * 	07 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <vcl.h>
	#include <math.h>
	#include <stdlib.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	#include <cstdlib>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "zoobobjt.h"
#include "iodll.h"
#include "EcoDynClass.h"

TMytilusGalloprovincialisIndividual::TMytilusGalloprovincialisIndividual(TEcoDynClass* APEcoDynClass, char* className)
							 :	TChlamysFarreriIndividual(APEcoDynClass, className)
{
    NumberOfBoxesWithMytilusGalloprovincialis = 0;
    NumberOfBoxesWithBenthicSpecies = MyPEcoDynClass->GetNumberOfCellsWithBenthicSpecies();
    AsymptoticLength = 200.0;
    //Debugger(NumberOfBoxesWithBenthicSpecies);
    for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
    {
      if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "Mytilus galloprovincialis") == 0)
              NumberOfBoxesWithMytilusGalloprovincialis++;
    }
    //Debugger(NumberOfBoxesWithMytilusGalloprovincialis);
    MytilusGalloprovincialisRecord = new BenthicSpeciesRecord[NumberOfBoxesWithMytilusGalloprovincialis];
    int AIndex = 0;
    for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
    {
        if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "Mytilus galloprovincialis") == 0)
        {
            strcpy(MytilusGalloprovincialisRecord[AIndex].SpeciesName, MyBenthicSpeciesRecord[i].SpeciesName);
            MytilusGalloprovincialisRecord[AIndex].LineCoordinate = MyBenthicSpeciesRecord[i].LineCoordinate;
            MytilusGalloprovincialisRecord[AIndex].ColumnCoordinate = MyBenthicSpeciesRecord[i].ColumnCoordinate;
            AIndex++;
        }
    }
    BuildMytilusGalloprovincialisIndividual();
}


void TMytilusGalloprovincialisIndividual::BuildMytilusGalloprovincialisIndividual()
{
    ReadParameters();
    SomC = 0.0; SomN = 0.0; StorC = 0.0; OSW = 0.0; Age = 0.0;
    ReadVariables();
    PreBuildMytilusGalloprovincialisIndividual();
    PostBuildMytilusGalloprovincialisIndividual();
}

void TMytilusGalloprovincialisIndividual::ReadParameters()
{
    // Read in the parameters
    NumberOfParameterRecords = 0;
    MyPReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Mytilus");
    if (MyPReadWrite == NULL)
    {
            return;
    }

    double MyValue;
    int X, Y;
    if (MyPReadWrite->FindString("Mytilus galloprovincialis", X, Y))
    {
        MyPReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
        //Debugger(NumberOfParameters);
        MyParsRecord = new ParsRecord[NumberOfParameters-1];
        // read in the parameter names
        char MyParameter[65];
        int index;
        index = -1;
        for (int i = Y; i < Y + NumberOfParameters; i++)
        {
            MyPReadWrite->ReadString(X+2, i, MyParameter);
            if (strcmp(MyParameter, "Allometric coefficient for feeding") == 0)
            {
                index++;
                //Debugger(index);
                strcpy(MyParsRecord[index].ParameterName,"Allometric coefficient for feeding");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/

            }
            else if (strcmp(MyParameter, "Allometric coefficient for respiration") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Allometric coefficient for respiration");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment); */
            }
            else if (strcmp(MyParameter, "Meat energetic contents") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Meat energetic contents");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                MeatEnergeticContents = MyParsRecord[index].Value;
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment); */
            }
            else if (strcmp(MyParameter, "Shell energetic contents") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Shell energetic contents");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                ShellEnergeticContents = MyParsRecord[index].Value;
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment); */
            }
            else if (strcmp(MyParameter, "Intrinsic rate of mortality") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Intrinsic rate of mortality");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "Summer Intrinsic rate of mortality") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Summer Intrinsic rate of mortality");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment); */
            }
            else if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"TotalFW to meatDW");
				MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "Dry weight of a standard animal") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Dry weight of a standard animal");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "OSWFrac") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"OSWFrac");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "CDWFrac") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"CDWFrac");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "NDWFracInGametes") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"NDWFracInGametes");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "CDWFood") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"CDWFood");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "NDWFood") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"NDWFood");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "Cnatmin") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Cnatmin");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "Agc") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Agc");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "Bgc") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Bgc");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "GPTMin") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"GPTMin");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "GPTMax") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"GPTMax");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "PHYORGDigestibility") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"PHYORGDigestibility");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "DETORGDigestibility") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"DETORGDigestibility");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "Length_of_a_standard_mussel") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Length_of_a_standard_mussel");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "Option_Ingestion_Rate") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Option_Ingestion_Rate");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                Option_Ingestion_Rate = MyParsRecord[index].Value;
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "Option_Clearance_Rate") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Option_Clearance_Rate");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                Option_Clearance_Rate = MyParsRecord[index].Value;
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);
                Debugger(Option_Clearance_Rate); */
            }
            else if (strcmp(MyParameter, "KStorExp") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"KStorExp");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "KStor") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"KStor");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "OSWperSOM") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"OSWperSOM");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "mL_O2_to_Joules") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"mL_O2_to_Joules");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "Option_Respiration") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Option_Respiration");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                Option_Respiration = MyParsRecord[index].Value;
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "Ares") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Ares");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "Bres") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Bres");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "Q10Res") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Q10Res");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "Reference temperature") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Reference temperature");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "ResAbsC") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"ResAbsC");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "Aexcr") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Aexcr");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "Bexcr") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Bexcr");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "ReproductionDate") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"ReproductionDate");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "ReproductionExpenditure") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"ReproductionExpenditure");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            /*else if (strcmp(MyParameter, "Number of classes") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Number of classes");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
            } */
            else if (strcmp(MyParameter, "OSWtoDSW") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"OSWtoDSW");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment); */
            }
            else if (strcmp(MyParameter, "AsymptoticLength") == 0)
            {
                MyPReadWrite->ReadNumber(X+3, i, AsymptoticLength);
            }
            else if (strcmp(MyParameter, "IndividualBasedVariables") == 0)
            {
               MyPReadWrite->ReadNumber(X+3, i, IndividualBasedVariables);
            }
        }
        NumberOfParameterRecords = index + 1;
        //Debugger(NumberOfParameterRecords);
    }
    else
#ifdef __BORLANDC__
       MessageBox(0, "Undefined parameters in M. galloprovincialis parameters file", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
			 cerr << "TMytilusGalloprovincialisIndividual::ReadParameters - undefined parameters in M. galloprovincialis parameters file" << endl;
#endif  // __BORLANDC__

}

void TMytilusGalloprovincialisIndividual::ReadVariables()
{
    // Read in the variables
    char MyVariable[65];
    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Mytilus");
    if (PReadWrite == NULL)
#ifdef __BORLANDC__
       MessageBox(0, "MytilusGalloprovinvialis variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
			 cerr << "TMytilusGalloprovincialisIndividual::ReadVariables - variables file missing." << endl;
#endif  // __BORLANDC__
    else {
        double MyValue;
        int X, Y;

        if (PReadWrite->FindString("Mytilus galloprovincialis", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names

            for (int i = 0; i < NumberOfVariables; i++)
            {
               PReadWrite->ReadString(X + 2 + i, Y, VariableNameArray[i]);
            }
            int k, j, RecordNumber, Index, offset, XV, YV;
            for (int i = 0; i < NumberOfVariables; i++)
            {
               PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
               if (strcmp(MyVariable, "M. galloprovincialis shell mass 1") == 0)         //g Shell dry weight / individual
               {
                  if (PReadWrite->FindString("M. galloprovincialis shell mass 1 values", XV, YV))
                  {
                      for (k = 0; k < NumberOfClasses; k++)
                      {
                          offset = k * NumberOfBoxes;
                          for (j = 0; j < NumberOfBoxesWithMytilusGalloprovincialis; j++)
                          {
                              Index = MytilusGalloprovincialisRecord[j].LineCoordinate * NumberOfColumns +
                                      MytilusGalloprovincialisRecord[j].ColumnCoordinate;
                              PReadWrite->ReadNumber(XV + k, YV + 1 + j, MyValue);
                              ZooShellMass[offset + Index] = MyValue;
                              ShellEnergeticTotal[offset + Index] = MyValue * ShellEnergeticContents;
                          }
                      }
                  }
                  else // old file format
                  {
                      for (k = 0; k < NumberOfClasses; k++)
                      {
                          offset = k * NumberOfBoxes;
                          RecordNumber = 0;
                          for (j = k * NumberOfBoxesWithMytilusGalloprovincialis; j < k * NumberOfBoxesWithMytilusGalloprovincialis + NumberOfBoxesWithMytilusGalloprovincialis; j++)
                          {
                            Index = MytilusGalloprovincialisRecord[RecordNumber].LineCoordinate * NumberOfColumns +
                                    MytilusGalloprovincialisRecord[RecordNumber].ColumnCoordinate;
                            PReadWrite->ReadNumber( X + 2 + i, Y + 1 + j, MyValue);
                            ZooShellMass[offset + Index] = MyValue;
                            ShellEnergeticTotal[offset + Index] = MyValue * ShellEnergeticContents;
                            RecordNumber++;
                          }
                      }
                  }
               }
               else if (strcmp(MyVariable, "M. galloprovincialis meat weight 1") == 0)        //g Soft tissue dry weight / individual
               {
                  if (PReadWrite->FindString("M. galloprovincialis meat weight 1 values", XV, YV))
                  {
                      for (k = 0; k < NumberOfClasses; k++)
                      {
                          offset = k * NumberOfBoxes;
                          for (j = 0; j < NumberOfBoxesWithMytilusGalloprovincialis; j++)
                          {
                              Index = MytilusGalloprovincialisRecord[j].LineCoordinate * NumberOfColumns +
                                      MytilusGalloprovincialisRecord[j].ColumnCoordinate;
                              PReadWrite->ReadNumber(XV + k, YV + 1 + j, MyValue);
                              ZooIndividualWeight[offset + Index] = MyValue;
                              SoftTissueEnergeticTotal[offset + Index] = MyValue * MeatEnergeticContents;
                          }
                      }
                  }
                  else // old file format
                  {
                      for (k = 0; k < NumberOfClasses; k++)
                      {
                          offset = k * NumberOfBoxes;
                          RecordNumber = 0;
                          for (j = k * NumberOfBoxesWithMytilusGalloprovincialis; j < k * NumberOfBoxesWithMytilusGalloprovincialis + NumberOfBoxesWithMytilusGalloprovincialis; j++)
                          {
                            Index = MytilusGalloprovincialisRecord[RecordNumber].LineCoordinate * NumberOfColumns +
                                    MytilusGalloprovincialisRecord[RecordNumber].ColumnCoordinate;
                            PReadWrite->ReadNumber( X + 2 + i, Y + 1 + j, MyValue);
                            ZooIndividualWeight[offset + Index] = MyValue;
                            SoftTissueEnergeticTotal[offset + Index] = MyValue * MeatEnergeticContents;
                            RecordNumber++;
                          }
                      }
                  }
               }
               else if (strcmp(MyVariable, "M. galloprovincialis density 1") == 0)           //g soft + shell tissue DW / m2
               {
                  if (PReadWrite->FindString("M. galloprovincialis density 1 values", XV, YV))
                  {
                      for (k = 0; k < NumberOfClasses; k++)
                      {
                          offset = k * NumberOfBoxes;
                          for (j = 0; j < NumberOfBoxesWithMytilusGalloprovincialis; j++)
                          {
                              Index = MytilusGalloprovincialisRecord[j].LineCoordinate * NumberOfColumns +
                                      MytilusGalloprovincialisRecord[j].ColumnCoordinate;
                              PReadWrite->ReadNumber(XV + k, YV + 1 + j, MyValue);
                              ZooClassDensity[offset + Index] = MyValue;
                          }
                      }
                  }
                  else // old file format
                  {
                      for (k = 0; k < NumberOfClasses; k++)
                      {
                          offset = k * NumberOfBoxes;
                          RecordNumber = 0;
                          for (j = k * NumberOfBoxesWithMytilusGalloprovincialis; j < k * NumberOfBoxesWithMytilusGalloprovincialis + NumberOfBoxesWithMytilusGalloprovincialis; j++)
                          {
                            Index = MytilusGalloprovincialisRecord[RecordNumber].LineCoordinate * NumberOfColumns +
                                    MytilusGalloprovincialisRecord[RecordNumber].ColumnCoordinate;
                            PReadWrite->ReadNumber( X + 2 + i, Y + 1 + j, MyValue);
                            ZooClassDensity[offset + Index] = MyValue;
                            /*if (ZooClassDensity[k * NumberOfBoxes + Index] != 0.0) */
                            //Debugger(ZooClassDensity[k * NumberOfBoxes + Index]); 
                            RecordNumber++;
                          }
                      }
                  }
               }
               else if (strcmp(MyVariable, "SomC") == 0)
               {
                  PReadWrite->ReadNumber( X + 2 + i, Y + 1, MyValue);
                  SomC = MyValue;
               }
               else if (strcmp(MyVariable, "SomN") == 0)
               {
                  PReadWrite->ReadNumber( X + 2 + i, Y + 1, MyValue);
                  SomN = MyValue;
               }
               else if (strcmp(MyVariable, "StorC") == 0)
               {
                  PReadWrite->ReadNumber( X + 2 + i, Y + 1, MyValue);
                  StorC = MyValue;
               }
               else if (strcmp(MyVariable, "OSW") == 0)
               {
                  PReadWrite->ReadNumber( X + 2 + i, Y + 1, MyValue);
                  OSW = MyValue;
               }
               else if (strcmp(MyVariable, "Age") == 0)
               {
                  PReadWrite->ReadNumber( X + 2 + i, Y + 1, MyValue);
                  Age = MyValue;
               }
            }
        }
        else 
#ifdef __BORLANDC__
					MessageBox(0, "Variables: Undefined object in Mytilus", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
					cerr << "TMytilusGalloprovincialisIndividual::ReadVariables - undefined object in Mytilus" << endl;
#endif  // __BORLANDC__

        CloseDataFile((void*)PReadWrite);
    }
}

void TMytilusGalloprovincialisIndividual::PreBuildMytilusGalloprovincialisIndividual()
{
   NumberOfIndividuals = (int)ZooClassDensity[0];
   MyIndividualRecord = new IndividualRecord[NumberOfIndividuals * NumberOfBoxesWithMytilusGalloprovincialis];
   //SomC = new double[NumberOfBoxes]; SomN = new double[NumberOfBoxes];
   //StorC = new double[NumberOfBoxes]; OSW = new double[NumberOfBoxes]; GPT = new double[NumberOfBoxes];
   IRFlux = new double[NumberOfBoxes]; Egestion = new double[NumberOfBoxes];
   //IR = new double[NumberOfBoxes];
   for (int i = 0; i< NumberOfBoxes; i++){
      IRFlux[i] = 0.0; Egestion[i] = 0.0;
   }
   //Este modelo ser? em princ?pio 0D
   //por isso posso preencher o individual record s? para uma "caixa"
}



void TMytilusGalloprovincialisIndividual::PostBuildMytilusGalloprovincialisIndividual()
{
    int index;
    int offset;

    randomize();
    for (int k = 0; k < NumberOfClasses; k++)
    {
        offset = k * NumberOfBoxes;
        for (int j = 0; j < NumberOfBoxesWithMytilusGalloprovincialis; j++)
        {
          index = MytilusGalloprovincialisRecord[j].LineCoordinate * NumberOfColumns +
                  MytilusGalloprovincialisRecord[j].ColumnCoordinate;
          ZooClassBiomass[offset + index] = ZooClassDensity[offset + index] *
                                            (ZooIndividualWeight[offset + index]  + ZooShellMass[offset + index]);
          /*Debugger(ZooClassDensity[offset + index]);
          Debugger(ZooIndividualWeight[offset + index]);
          Debugger(ZooClassBiomass[offset + index]);  */
        }
    }

    for (int k = 0; k < NumberOfBoxesWithMytilusGalloprovincialis; k++)
    {
      for (int  i = 0; i < NumberOfIndividuals; i++)
      {
          index = i * NumberOfBoxesWithMytilusGalloprovincialis + k;
          //Debugger(i);
          MyIndividualRecord[index].IRPHY = 0.0; MyIndividualRecord[index].IRDET = 0.0;
          MyIndividualRecord[index].IRPIM = 0.0; MyIndividualRecord[index].IRTPM = 0.0;
          MyIndividualRecord[index].FRTPM = 0.0; MyIndividualRecord[index].FRPIM = 0.0;
          MyIndividualRecord[index].RRTPM = 0.0; MyIndividualRecord[index].RRPIM = 0.0;
          MyIndividualRecord[index].OIR = 0.0; MyIndividualRecord[index].OCI = 0.0;
          MyIndividualRecord[index].OCPF = 0.0; MyIndividualRecord[index].NAEIO = 0.0;
          MyIndividualRecord[index].NEA = 0.0; MyIndividualRecord[index].NEB = 0.0;
          MyIndividualRecord[index].ZooIndividualWeight = ZooIndividualWeight[k];
          MyIndividualRecord[index].ShellLength = ShellLength(ZooShellMass[k]);
          MyIndividualRecord[index].ZooShellMass = ZooShellMass[k];
          MyIndividualRecord[index].ZooIndividualFlux = ZooIndividualFlux[k];
          MyIndividualRecord[index].ZooIndividualFluxForTheShell = ZooIndividualFluxForTheShell[k];
          MyIndividualRecord[index].SoftTissueEnergeticTotal = SoftTissueEnergeticTotal[k];
          MyIndividualRecord[index].ShellEnergeticTotal = ShellEnergeticTotal[k];
          MyIndividualRecord[index].ZooCR = 0.0;
          MyIndividualRecord[index].GV = 0.0;
          MyIndividualRecord[index].SomC = SomC; MyIndividualRecord[index].SomN = SomN;
          MyIndividualRecord[index].StorC = StorC; MyIndividualRecord[index].OSW = OSW;
          MyIndividualRecord[index].DSW = MyIndividualRecord[index].ZooShellMass;
          MyIndividualRecord[index].GPT = 0.0; MyIndividualRecord[index].AE = 0.0;
          MyIndividualRecord[index].ARC = 0.0; MyIndividualRecord[index].ARN = 0.0;
          MyIndividualRecord[index].NetProdC = 0.0; MyIndividualRecord[index].NetProdN = 0.0;
          MyIndividualRecord[index].RespC = 0.0; MyIndividualRecord[index].RespR = 0.0; MyIndividualRecord[index].ExcR = 0.0;
          MyIndividualRecord[index].Age = Age; MyIndividualRecord[index].ConditionIndex = 0.0;
          MyIndividualRecord[index].SomFrac = 0.0; MyIndividualRecord[index].StorFrac = 0.0;
          for (int j = 0; j < NumberOfParameterRecords; j++)
          {
              MyIndividualRecord[index].IndividualParameters[j] = MyParsRecord[j];

              if (MyIndividualRecord[index].IndividualParameters[j].HigherValue -
                  MyIndividualRecord[index].IndividualParameters[j].LowerValue > 0.0)
              {
                  if (!MyIndividualRecord[index].IndividualParameters[j].RandomizeFlag)  // Sistematic increment of parameter value from its minimum till its maximum
                  {
                      //Debugger(index); Debugger(j);
                      if (index == 0) //First individual gets the lower value
                      {
                          MyIndividualRecord[index].IndividualParameters[j].Value = MyIndividualRecord[index].IndividualParameters[j].LowerValue;
                          //Debugger(index);
                          //Debugger(MyIndividualRecord[index].IndividualParameters[j].Value);
                      }
                      else
                      {
                          MyIndividualRecord[index].IndividualParameters[j].Value =
                              (MyIndividualRecord[index].IndividualParameters[j].HigherValue - MyIndividualRecord[index].IndividualParameters[j].LowerValue) /
                              (NumberOfIndividuals - 1) + MyIndividualRecord[index - 1].IndividualParameters[j].Value;
                          //Debugger(index);
                          //Debugger(MyIndividualRecord[index].IndividualParameters[j].Value);
                      }
                  }
                  else //Random generation of parameter values between lower and higher limits
                  {
                      double ARandomValue; double MaximumRandomValue = 10000;
                      ARandomValue = random(MaximumRandomValue);
                      //Debugger(index); Debugger(j);
                      //ARandomValue = rand() % MaximumRandomValue;
                      MyIndividualRecord[index].IndividualParameters[j].Value =
                          MyIndividualRecord[index].IndividualParameters[j].LowerValue +
                          (MyIndividualRecord[index].IndividualParameters[j].HigherValue - MyIndividualRecord[index].IndividualParameters[j].LowerValue) *
                          ARandomValue / MaximumRandomValue;
                      //Debugger(MyIndividualRecord[index].IndividualParameters[j].Value);
                  }
              }
          }
      }
    }
}

TMytilusGalloprovincialisIndividual::~TMytilusGalloprovincialisIndividual()
{
    freeMemory();
}

void TMytilusGalloprovincialisIndividual::freeMemory()
{
    //delete [] SomC; delete [] SomN; delete [] StorC; delete [] OSW; delete [] GPT;
    delete [] MytilusGalloprovincialisRecord;
    delete [] IRFlux; delete [] Egestion;
    //delete [] MyParsRecord; delete [] MyIndividualRecord;
    //delete [] VariableNameArray;
}



void TMytilusGalloprovincialisIndividual::Inquiry(char* srcName, double &Value,int BivalveNumber, int BoxNumber,
					char* ParameterName,int AnObjectCode)
{
    int i,j, index;
    char MyParameter[65], // 64 characters (max) for parameter name
    Myjstring[5],MyName[65];
    int MyBivalveNumber = BivalveNumber;
    int MyBoxNumber = BoxNumber;
    index = MyBivalveNumber * NumberOfBoxesWithMytilusGalloprovincialis + MyBoxNumber;
    strcpy(MyParameter, ParameterName);
    //Debugger(NumberOfParameterRecords); Debugger(NumberOfVariables);

    //Procura da string entre as vari?veis

    for (i = 0; i < NumberOfVariables; i++)
    {
      //MessageBox(0,"Caption",VariableNameArray[i],MB_OK | MB_ICONHAND);
      if (strcmp(MyParameter, VariableNameArray[i]) == 0)
      {
        for (j = 0; j < NumberOfClasses; j++)
        {
                strcpy( MyName, "M. galloprovincialis CR " );
                sprintf( Myjstring , "%i", j + 1 );
                strcat( MyName, Myjstring );
                if (strcmp(MyParameter, MyName) == 0){
                        Value = MyIndividualRecord[index].ZooCR;
                        LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
                        return;
                }
        }
        for (j = 0; j < NumberOfClasses; j++)
        {
                strcpy( MyName, "M. galloprovincialis meat weight " );
                sprintf( Myjstring , "%i", j + 1 );
                strcat( MyName, Myjstring );
                if (strcmp(MyParameter, MyName) == 0){
                        Value = MyIndividualRecord[index].ZooIndividualWeight;
                        LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
                        return;
                }
        }
        for (j = 0; j < NumberOfClasses; j++)
        {
                strcpy( MyName, "M. galloprovincialis shell mass " );
                sprintf( Myjstring , "%i", j + 1 );
                strcat( MyName, Myjstring );
                if (strcmp(MyParameter, MyName) == 0){
                        Value = MyIndividualRecord[index].DSW;
                        LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
                        return;
                }
        }
        for (j = 0; j < NumberOfClasses; j++)
        {
                strcpy( MyName, "Individual growth " );
                sprintf( Myjstring , "%i", j + 1 );
                strcat( MyName, Myjstring);
                if (strcmp(MyParameter, MyName) == 0){
                        Value = MyIndividualRecord[index].ZooIndividualFlux; // Values in g day-1 ind-1
                        LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
                        return;
                }
        }
        for (j = 0; j < NumberOfClasses; j++)
        {
                strcpy( MyName, "M. galloprovincialis shell length " );
                sprintf( Myjstring , "%i", j + 1 );
                strcat( MyName, Myjstring);
                if (strcmp(MyParameter, MyName) == 0) {
                        Value = MyIndividualRecord[index].ShellLength; // Values in mm
                        LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
                        return;
                }
        }
        if (strcmp(MyParameter, "IRPHY") == 0)
        {
           Value = MyIndividualRecord[index].IRPHY * DAYSTOSECONDS;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);return;
        }
        else if (strcmp(MyParameter, "IRDET") == 0)
        {
           Value = MyIndividualRecord[index].IRDET * DAYSTOSECONDS;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "IRPIM") == 0)
        {
           Value = MyIndividualRecord[index].IRPIM * DAYSTOSECONDS;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "IRTPM") == 0)
        {
           Value = MyIndividualRecord[index].IRTPM * DAYSTOSECONDS;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "FRTPM") == 0)
        {
           Value = MyIndividualRecord[index].FRTPM * DAYSTOSECONDS;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "FRPIM") == 0)
        {
           Value = MyIndividualRecord[index].FRPIM * DAYSTOSECONDS;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "RRTPM") == 0)
        {
           Value = MyIndividualRecord[index].RRTPM * DAYSTOSECONDS;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "RRPIM") == 0)
        {
           Value = MyIndividualRecord[index].RRPIM * DAYSTOSECONDS;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "OIR") == 0)
        {
           Value = MyIndividualRecord[index].OIR;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "OCI") == 0)
        {
           Value = MyIndividualRecord[index].OCI * DAYSTOSECONDS;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "OCPF") == 0)
        {
           Value = MyIndividualRecord[index].OCPF;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "NAEIO") == 0)
        {
           Value = MyIndividualRecord[index].NAEIO;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "NEA") == 0)
        {
           Value = MyIndividualRecord[index].NEA * DAYSTOSECONDS;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "NEB") == 0)
        {
           Value = MyIndividualRecord[index].NEB * DAYSTOSECONDS;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        /*else if (strcmp(MyParameter, "GutContents") == 0)
        {
           Value = MyIndividualRecord[index].GutContents;
        }  */
        else if (strcmp(MyParameter, "SomC") == 0)
        {
           Value = MyIndividualRecord[index].SomC;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "SomN") == 0)
        {
           Value = MyIndividualRecord[index].SomN;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "StorC") == 0)
        {
           Value = MyIndividualRecord[index].StorC;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "OSW") == 0)
        {
           Value = MyIndividualRecord[index].OSW;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "Gut passage time") == 0)
        {
           Value = MyIndividualRecord[index].GPT;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "ARC") == 0)
        {
           Value = MyIndividualRecord[index].ARC;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "ARN") == 0)
        {
           Value = MyIndividualRecord[index].ARN;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "NetProdC") == 0)
        {
           Value = MyIndividualRecord[index].NetProdC;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "NetProdN") == 0)
        {
           Value = MyIndividualRecord[index].NetProdN;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "RespC") == 0)
        {
           Value = MyIndividualRecord[index].RespC;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "RespR") == 0)
        {
           Value = MyIndividualRecord[index].RespR;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "ExcR") == 0)
        {
           Value = MyIndividualRecord[index].ExcR;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "Age") == 0)
        {
           Value = MyIndividualRecord[index].Age;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "DSW") == 0)
        {
           Value = MyIndividualRecord[index].DSW;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "ConditionIndex") == 0)
        {
           Value = MyIndividualRecord[index].ConditionIndex;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "SomFrac") == 0)
        {
           Value = MyIndividualRecord[index].SomFrac;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "StorFrac") == 0)
        {
           Value = MyIndividualRecord[index].StorFrac;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
      }
    }  //i
    //Fim de procura da string entre as vari?veis
    //Debugger(1);
    //Procura da string entre os par?metros
    if (strcmp(MyParameter, "Allometric coefficient for feeding") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Allometric coefficient for feeding") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Allometric coefficient for respiration") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Allometric coefficient for respiration") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Meat energetic contents") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Meat energetic contents") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Shell energetic contents") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Shell energetic contents") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Option_Respiration") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Option_Respiration") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    /*else if (strcmp(MyParameter, "Maintenance respiration") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Maintenance respiration") == 0)
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
        }
    }
    else if (strcmp(MyParameter, "Activity respiration coefficient") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Activity respiration coefficient") == 0)
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
        }
    }
    else if (strcmp(MyParameter, "Proportion of energy allocated to shell") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Proportion of energy allocated to shell") == 0)
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
        }
    }
    else if (strcmp(MyParameter, "Proportion of energy allocated to soft tissue") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Proportion of energy allocated to soft tissue") == 0)
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
        }
    }
    else if (strcmp(MyParameter, "umol NH4 to Joules") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "umol NH4 to Joules") == 0)
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
        }
    }
    else if (strcmp(MyParameter, "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
        }
    }  */
    else if (strcmp(MyParameter, "OSWFrac") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "OSWFrac") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "CDWFrac") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "CDWFrac") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "NDWFracInGametes") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "NDWFracInGametes") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "CDWFood") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "CDWFood") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "NDWFood") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "NDWFood") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Cnatmin") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Cnatmin") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Agc") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Agc") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Bgc") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Bgc") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "GPTMin") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "GPTMin") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "GPTMax") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "GPTMax") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "PHYORGDigestibility") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "PHYORGDigestibility") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;}
        }
    }
    else if (strcmp(MyParameter, "DETORGDigestibility") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "DETORGDigestibility") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Length_of_a_standard_mussel") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Length_of_a_standard_mussel") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "KStorExp") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "KStorExp") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "KStor") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "KStor") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "OSWperSOM") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "OSWperSOM") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "mL_O2_to_Joules") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "mL_O2_to_Joules") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Ares") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Ares") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Bres") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Bres") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;}
        }
    }
    else if (strcmp(MyParameter, "Q10Res") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Q10Res") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Reference temperature") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Reference temperature") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "ResAbsC") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "ResAbsC") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Aexcr") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Aexcr") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Bexcr") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Bexcr") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Dry weight of a standard animal") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Dry weight of a standard animal") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "ReproductionDate") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "ReproductionDate") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            //Debugger(j); Debugger(Value);
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "ReproductionExpenditure") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "ReproductionExpenditure") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "OSWtoDSW") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "OSWtoDSW") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    //Fim da procura da string entre os par?metros
#ifdef __BORLANDC__
    char Caption[129];
    strcpy(Caption, MyParameter);
    strcat(Caption, " does not exist in ");
    strcat(Caption, GetEcoDynClassName());
    MessageBox(0,Caption,"EcoDynamo Alert - Inquiry 1",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
		cerr << "TMytilusGalloprovincialisIndividual::Inquiry 1 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
}


void TMytilusGalloprovincialisIndividual::Inquiry(char* srcName, double &Value,
						int BoxNumber,char* ParameterName,int AnObjectCode)
{
    int i,j;
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
                      strcpy( MyName, "M. galloprovincialis density " );
                      sprintf( Myjstring , "%i", j + 1 );
                      strcat( MyName, Myjstring );
                      if (strcmp(MyParameter, MyName) == 0)
                              Value = ZooClassDensity[NumberOfBoxes * j + MyBoxNumber];
              }

              for (j = 0; j < NumberOfClasses; j++)
              {
                      strcpy( MyName, "M. galloprovincialis biomass " );
                      sprintf( Myjstring , "%i", j + 1 );
                      strcat( MyName, Myjstring );
                      if (strcmp(MyParameter, MyName) == 0)
                              Value = ZooClassBiomass[NumberOfBoxes * j + MyBoxNumber];
              }

             LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
             return;
    }
#ifdef __BORLANDC__
  char Caption[129];
  strcpy(Caption, MyParameter);
  strcat(Caption, " does not exist in ");
  strcat(Caption, GetEcoDynClassName());
  MessageBox(0,Caption,"EcoDynamo Alert - Inquiry 2",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
	cerr << "TMytilusGalloprovincialisIndividual::Inquiry 2 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
  Value = 0;
}

void TMytilusGalloprovincialisIndividual::Go()
{
   //Debugger(1);
   JulianDay = MyPEcoDynClass->GetJulianDay();
   int contador = 0, index;
   double MyDepth, MyChlorophyllCarbonRatio, MyCDWFrac, WSom, WStor, MyOSWtoDSW, MyEnergyContent;
   TEcoDynClass * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
                * MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer();
   if ((MySPMPointer != NULL) && (MyPhytoPointer != NULL))
   {
      for (int i = 0; i < NumberOfBoxes; i++)
      {
         MySPMPointer->Inquiry(GetEcoDynClassName(), TPM,i,"TPM",ObjectCode);
         MySPMPointer->Inquiry(GetEcoDynClassName(), POM,i,"POM",ObjectCode);
         PIM = TPM - POM;
         MyPhytoPointer->Inquiry(GetEcoDynClassName(), ChlorophyllToBiomass,i,"Chlorophyll to biomass", ObjectCode);
         MyPhytoPointer->Inquiry(GetEcoDynClassName(), phyto_mass,i,"Phytoplankton biomass",ObjectCode);
         PHYORG = phyto_mass / CUBIC * ChlorophyllToBiomass; //mg/l
         DETORG = POM - PHYORG;
         for (int  bivalve = 0; bivalve < NumberOfIndividuals; bivalve++)
         {
           index = bivalve * NumberOfBoxesWithMytilusGalloprovincialis + i;
           UpdateIndividualParameters(index);

           ZooIndividualWeight[i] = MyIndividualRecord[index].ZooIndividualWeight;
           for (int k = 0; k < NumberOfClasses; k++)
           {
              if (
                      ( ZooClassBiomass[k * NumberOfBoxes + i] > 0.0) &&
                      ( ZooClassDensity[k * NumberOfBoxes + i] > 0.0) &&
                      ( ZooIndividualWeight[k * NumberOfBoxes + i] > 0.0)
                  )
              {
                 Feed(i, bivalve);
                 Absorption(i, bivalve);
                 Respire(i, bivalve);
                 Excrete(i, bivalve);
                 Assimilation(i, bivalve);
                 Allocation(i, bivalve);
                 Reproduce(i, bivalve);
                 Aging(i, bivalve);
                 Inquiry(GetEcoDynClassName(), MyCDWFrac, bivalve, i,"CDWFrac",ObjectCode);
                 WSom = MyIndividualRecord[index].SomC / MyCDWFrac;
                 WStor = MyIndividualRecord[index].StorC / MyCDWFrac;
                 MyIndividualRecord[index].ZooIndividualWeight = WSom + WStor;
                 Inquiry(GetEcoDynClassName(), MyOSWtoDSW, bivalve, i,"OSWtoDSW",ObjectCode);
                 MyIndividualRecord[index].DSW = MyOSWtoDSW * MyIndividualRecord[index].OSW;
                 //Debugger(MyIndividualRecord[index].ShellLength);
                 MyIndividualRecord[index].ShellLength = 32.448 * pow(MyIndividualRecord[index].DSW,0.3596); //mm
                 /*if (bivalve == 0) {
                   Debugger(MyOSWtoDSW);
                   Debugger(MyIndividualRecord[index].ZooIndividualWeight);
                   Debugger(MyIndividualRecord[index].DSW);
                   Debugger(MyIndividualRecord[index].OSW);
                   Debugger(MyIndividualRecord[index].ShellLength );
                 } */
                 MyIndividualRecord[index].ConditionIndex = MyIndividualRecord[index].ZooIndividualWeight /
                                                            MyIndividualRecord[index].DSW;
                 MyIndividualRecord[index].SomFrac = WSom / MyIndividualRecord[index].ZooIndividualWeight;
                 MyIndividualRecord[index].StorFrac = WStor / MyIndividualRecord[index].ZooIndividualWeight;  
                 Inquiry(GetEcoDynClassName(), MyEnergyContent, bivalve, i,"Meat energetic contents",ObjectCode);
                 MyIndividualRecord[index].SoftTissueEnergeticTotal = MyIndividualRecord[index].ZooIndividualWeight * MyEnergyContent;
                 ZooClassBiomass[NumberOfBoxes * k + i] = ZooClassBiomass[NumberOfBoxes * k + i] +
                                                          MyIndividualRecord[index].ZooIndividualWeight +
                                                          MyIndividualRecord[index].DSW;
              }
           }
         }
      }
   }
   //Debugger(2);
}

void TMytilusGalloprovincialisIndividual::Integrate()
{

}

void TMytilusGalloprovincialisIndividual::Feed(int ABox, int ABivalve)
{
   double const SpFoodMass = 1.0; //{ mg/mm3}
   double StandardLength, AllometricCoefForFeeding, Q1, IRmax;
   int index;
   index = ABivalve * NumberOfBoxesWithMytilusGalloprovincialis + ABox;
   if (TPM > 0.0)
      Q1 = POM / TPM;
   else
      Q1 = 0.0;

   MyIndividualRecord[index].GPT = GutPassageTime(ABox, ABivalve);
   MyIndividualRecord[index].GV = GutVolume(ABox,ABivalve);
   IRmax = (MyIndividualRecord[index].GV * SpFoodMass)/MyIndividualRecord[index].GPT; //{ mg/h}

   Inquiry(GetEcoDynClassName(), StandardLength, ABivalve, ABox,"Length_of_a_standard_mussel",ObjectCode);
   Inquiry(GetEcoDynClassName(), AllometricCoefForFeeding, ABivalve, ABox,"Allometric coefficient for feeding",ObjectCode);
   //Debugger(Option_Clearance_Rate); Debugger(StandardLength); Debugger(AllometricCoefForFeeding);
   //Debugger(MyIndividualRecord[index].ShellLength);
   if (Option_Clearance_Rate == 1)
      ClearanceRate[ABox] = 3.9 * AllometricCorrection(MyIndividualRecord[index].ShellLength, StandardLength, AllometricCoefForFeeding);// 3.9 L/h}
   else if (Option_Clearance_Rate == 2)
   {
      if (phyto_mass < 1.18)
      {
         //Debugger(11);Debugger(TPM); Debugger(Q1);
         ClearanceRate[ABox] = MAX(
                                     ( 7.51 * TPM - 3.01 * pow(TPM,2.0) - 6.86 * Q1 + 2.26) *
                                     AllometricCorrection(MyIndividualRecord[index].ShellLength, StandardLength, AllometricCoefForFeeding),
                                     0.0
                                   );
      }
      else
      {
         ClearanceRate[ABox] = 3.9 * AllometricCorrection(MyIndividualRecord[index].ShellLength, StandardLength, AllometricCoefForFeeding);
         /*Debugger(11);
         Debugger(MyIndividualRecord[index].ShellLength); Debugger(StandardLength); Debugger(AllometricCoefForFeeding);
         Debugger(ClearanceRate[ABox]);  */
      }
   }
   else if (Option_Clearance_Rate == 3)
        ClearanceRate[ABox] = MIN(
                                    3.9 * AllometricCorrection(MyIndividualRecord[ABivalve].ShellLength, StandardLength, AllometricCoefForFeeding),
                                    MyIndividualRecord[index].GV / ((0.6794 * log(TPM) + 0.8648) * MyIndividualRecord[ABivalve].GPT)
                                  );
         //ClearanceRate[ABox] = 3.9 * AllometricCorrection(MyIndividualRecord[ABivalve].ShellLength, StandardLength, AllometricCoefForFeeding);
   MyIndividualRecord[index].FRTPM = ClearanceRate[ABox] * TPM;
   MyIndividualRecord[index].IRTPM = MIN(MyIndividualRecord[index].FRTPM,IRmax);//{ mg/h}
   //MyIndividualRecord[index].IRTPM = MyIndividualRecord[index].FRTPM;
   double MySpFoodMass = 0.6794 * log(TPM) + 0.8648;
   IRFlux[ABox] = MyIndividualRecord[index].IRTPM / SpFoodMass;
   //IRFlux[ABox] = MyIndividualRecord[index].IRTPM / MySpFoodMass;
   Egestion[ABox] = MyIndividualRecord[index].GV / MyIndividualRecord[index].GPT;

   if  (Option_Ingestion_Rate == 1) {
      MyIndividualRecord[index].IRPHY = ClearanceRate[ABox] * PHYORG;
      MyIndividualRecord[index].IRDET = ClearanceRate[ABox] * DETORG;
      MyIndividualRecord[index].IRTPM = MyIndividualRecord[index].IRPHY + MyIndividualRecord[index].IRDET + ClearanceRate[ABox] * (TPM - PHYORG - DETORG);
   }
   else {
      if (TPM > 0.0)
      {
         MyIndividualRecord[index].IRPHY = PHYORG / TPM * MyIndividualRecord[index].IRTPM;
         MyIndividualRecord[index].IRDET = DETORG / TPM * MyIndividualRecord[index].IRTPM;
      }
      else
      {
         MyIndividualRecord[index].IRPHY = 0.0;
         MyIndividualRecord[index].IRDET = 0.0;
      }
   }
   MyIndividualRecord[index].OIR = (MyIndividualRecord[index].IRPHY + MyIndividualRecord[index].IRDET) / CUBIC * DAYSTOHOURS; //{ g /d }
   //Debugger(Option_Clearance_Rate);

   /*Debugger(MyIndividualRecord[ABivalve].ShellLength);
   Debugger(StandardLength);
   Debugger(AllometricCoefForFeeding);  */
   //Debugger(Option_Ingestion_Rate);
   //Debugger(TPM);
   //Debugger(PHYORG);
   //Debugger(DETORG);
   /*Debugger(MyIndividualRecord[index].FRTPM);
   Debugger(MyIndividualRecord[index].IRTPM);
   Debugger(MyIndividualRecord[index].IRPHY);
   Debugger(MyIndividualRecord[index].IRDET);*/
   //Debugger(MyIndividualRecord[index].OIR);
   /*Debugger(IRmax);  */
   /*Debugger(MyIndividualRecord[index].GV);
   Debugger(MyIndividualRecord[index].GPT); */
   //Debugger(SpFoodMass);
}

double TMytilusGalloprovincialisIndividual::GutPassageTime(int ABox, int ABivalve)
//{Gut Passage Time in h, Scholten and Smaal, 1999, Aquatic Ecology 33, pag. 96}
{
   double MyPhyOrgDigest, MyDetorgDigest, MyGPTmin, MyGPTmax;
   Inquiry(GetEcoDynClassName(), MyPhyOrgDigest, ABivalve, ABox,"PHYORGDigestibility",ObjectCode);
   Inquiry(GetEcoDynClassName(), MyDetorgDigest, ABivalve, ABox,"DETORGDigestibility",ObjectCode);
   Inquiry(GetEcoDynClassName(), MyGPTmin, ABivalve, ABox,"GPTMin",ObjectCode);
   Inquiry(GetEcoDynClassName(), MyGPTmax, ABivalve, ABox,"GPTMax",ObjectCode);
   return (PHYORG * MyPhyOrgDigest + DETORG * MyDetorgDigest) / TPM * (MyGPTmin - MyGPTmax) + MyGPTmax;
}

double TMytilusGalloprovincialisIndividual::GutVolume(int ABox, int ABivalve)
//{Gut contents in mm3, Scholten and Smaal, 1999, Aquatic Ecology 33, pag. 94}
{
   double MyAgc, MyBgc;
   int index;
   index = ABivalve * NumberOfBoxesWithMytilusGalloprovincialis + ABox;
   Inquiry(GetEcoDynClassName(), MyAgc, ABivalve, ABox,"Agc",ObjectCode);
   Inquiry(GetEcoDynClassName(), MyBgc, ABivalve, ABox,"Bgc",ObjectCode);
   return MyAgc * pow(MyIndividualRecord[index].ZooIndividualWeight,MyBgc);
}

double TMytilusGalloprovincialisIndividual::AllometricCorrection(double AMeasure, double AStandardMeasure, double AnAllometricCoefficient)
{
   return pow(AMeasure / AStandardMeasure, AnAllometricCoefficient);
}

void TMytilusGalloprovincialisIndividual::Absorption(int ABox, int ABivalve)
{
   double Q1, CDWFood, NDWFood;
   double const TINNY = 0.000001;
   int index;
   index = ABivalve * NumberOfBoxesWithMytilusGalloprovincialis + ABox;
   if (TPM > 0.0)
      Q1 = POM / TPM;
   else
      Q1 = 0.0;  
   Inquiry(GetEcoDynClassName(), CDWFood, ABivalve, ABox,"CDWFood",ObjectCode);
   Inquiry(GetEcoDynClassName(), NDWFood, ABivalve, ABox,"NDWFood",ObjectCode);
   if (Q1 > TINNY)
      MyIndividualRecord[index].AE = MAX(0.95-0.18 / Q1,0.0); //{Equation derived by Uxio and Maria Jos?}
   else
      MyIndividualRecord[index].AE = 0.0;
   MyIndividualRecord[index].ARC = MyIndividualRecord[index].AE * MyIndividualRecord[index].OIR * CDWFood;//{ carbon absorption rate - gC/d}
   MyIndividualRecord[index].ARN = MyIndividualRecord[index].AE * MyIndividualRecord[index].OIR * NDWFood;//{ ARN - Absorption rate of Nitrogen - gN/d}
   /*Debugger(MyIndividualRecord[index].ARC);
   Debugger(MyIndividualRecord[index].AE);
   Debugger(MyIndividualRecord[index].OIR);
   Debugger(CDWFood); */
   //Debugger(MyIndividualRecord[index].ARN);
}

void TMytilusGalloprovincialisIndividual::Assimilation(int ABox, int ABivalve)
{
   int index;
   index = ABivalve * NumberOfBoxesWithMytilusGalloprovincialis + ABox;
   //Inquiry(GetEcoDynClassName(), AOption, ABivalve, ABox,"Option_Respiration",ObjectCode);
   if (Option_Respiration == 1)
     MyIndividualRecord[index].NetProdC = MyIndividualRecord[index].ARC - MyIndividualRecord[index].RespC; //{Respira??o calculada a partir da clorofila e do TPM}
   else
   {
     //Debugger(MyIndividualRecord[index].RespR);
     MyIndividualRecord[index].NetProdC = MyIndividualRecord[index].ARC - MyIndividualRecord[index].RespR;
   }
   MyIndividualRecord[index].NetProdN = MyIndividualRecord[index].ARN - MyIndividualRecord[index].ExcR; //{ Place right hand side of equation here... }

   /*Debugger(Option_Respiration);
   Debugger(MyIndividualRecord[index].ARC);
   Debugger(MyIndividualRecord[index].RespR);
   Debugger(MyIndividualRecord[index].ExcR);
   Debugger(MyIndividualRecord[index].NetProdC);
   Debugger(MyIndividualRecord[index].NetProdN); */
}


void TMytilusGalloprovincialisIndividual::Allocation(int ABox, int ABivalve)
{
   double ToStor, ToOSW, ToSOM, OSWCR, KStor, KStorExp, OSWperSOM, SomCFlux, StorCFlux, SomNFlux, MyCDWFrac;
   int index;
   index = ABivalve * NumberOfBoxesWithMytilusGalloprovincialis + ABox;
   SomCFlux = 0.0; StorCFlux = 0.0; SomNFlux = 0.0; MyCDWFrac = 0.0;
   Inquiry(GetEcoDynClassName(), KStor, ABivalve, ABox,"KStor",ObjectCode);
   Inquiry(GetEcoDynClassName(), KStorExp, ABivalve, ABox,"KStorExp",ObjectCode);
   ToStor = 1.0/(1.0 + KStor * exp(-KStorExp * MyIndividualRecord[ABivalve].Age));
   Inquiry(GetEcoDynClassName(), OSWperSOM, ABivalve, ABox,"OSWperSOM",ObjectCode);
   if (MyIndividualRecord[index].ShellLength <= AsymptoticLength)
   {
      ToOSW = OSWperSOM * (1.0 - ToStor);
      ToSOM = 1.0 - ToStor - ToOSW;
   }
   else
   {
      ToOSW = 0.0;
      ToSOM = 0.0;
   }
   //Debugger(MyIndividualRecord[index].NetProdC);
   if (MyIndividualRecord[index].NetProdC > 0.0)
     OSWCR = ToOSW * MyIndividualRecord[index].NetProdC;
   else
     OSWCR = 0.0;
   if (MyIndividualRecord[index].NetProdC > 0.0)
   {
     SomCFlux = ToSOM * MyIndividualRecord[index].NetProdC;
     StorCFlux = ToStor * MyIndividualRecord[index].NetProdC;
   }
   else
   {
      if (
           (MyIndividualRecord[index].NetProdC < 0.0) &&
           (-MyIndividualRecord[index].NetProdC * TimeStep / DAYSTOSECONDS < MyIndividualRecord[index].StorC)
         )
      {
         //Debugger(1);
         SomCFlux = 0.0;
         StorCFlux = MyIndividualRecord[index].NetProdC;
      }
      else if
      (
        (MyIndividualRecord[index].NetProdC < 0.0) &&
        (-MyIndividualRecord[index].NetProdC * TimeStep / DAYSTOSECONDS >= MyIndividualRecord[index].StorC) &&
        (-MyIndividualRecord[index].NetProdC * TimeStep / DAYSTOSECONDS < MyIndividualRecord[index].StorC + MyIndividualRecord[index].SomC)
      )
      {
        StorCFlux = -MyIndividualRecord[index].StorC;
        SomCFlux = -(fabs(MyIndividualRecord[index].NetProdC) - fabs(StorCFlux));
      }
      else
      {
        SomCFlux = 0.0;
        StorCFlux = 0.0;
      }
   }
   //Debugger(MyIndividualRecord[index].NetProdC );
   if (MyIndividualRecord[index].NetProdN > 0.0)
     SomNFlux = MyIndividualRecord[index].NetProdN * (ToSOM + ToStor);
   else
   if (
         (MyIndividualRecord[index].NetProdN < 0.0) &&
         (-MyIndividualRecord[index].NetProdN * TimeStep / DAYSTOSECONDS < MyIndividualRecord[index].SomN)
      )
      SomNFlux = MyIndividualRecord[index].NetProdN;
   else
      SomNFlux = 0.0;

   MyIndividualRecord[index].SomC = MyIndividualRecord[index].SomC + SomCFlux * TimeStep / DAYSTOSECONDS;
   MyIndividualRecord[index].StorC = MyIndividualRecord[index].StorC + StorCFlux * TimeStep / DAYSTOSECONDS;
   //Debugger(StorCFlux);
   MyIndividualRecord[index].SomN = MyIndividualRecord[index].SomN + SomNFlux * TimeStep / DAYSTOSECONDS;
   Inquiry(GetEcoDynClassName(), MyCDWFrac, ABivalve, ABox,"CDWFrac",ObjectCode);
   MyIndividualRecord[index].OSW = MyIndividualRecord[index].OSW + OSWCR / MyCDWFrac * TimeStep / DAYSTOSECONDS;
   //Debugger(MyCDWFrac); Debugger(OSWCR);
   //Debugger(MyIndividualRecord[index].OSW);
   /*Debugger(ToStor); Debugger(ToOSW); Debugger(ToSOM);
   Debugger(MyIndividualRecord[index].SomC); Debugger(MyIndividualRecord[index].StorC); Debugger(MyIndividualRecord[index].SomN); */
}

void TMytilusGalloprovincialisIndividual::Respire(int ABox, int ABivalve)
{
   int MyOption, index;
   index = ABivalve * NumberOfBoxesWithMytilusGalloprovincialis + ABox;
   //Inquiry(GetEcoDynClassName(), MyOption, ABivalve, ABox,"Option_Respiration",ObjectCode);
   if (Option_Respiration == 1) //Respiration calculated from chlorophyll and TPM
   {
     double MyCDWFrac, MyResp, MyStandardWeight, MyAllometricCoefficient, MyMeatEnergeticContents, My_mL_O2_to_Joules;
     Inquiry(GetEcoDynClassName(), MyCDWFrac, ABivalve, ABox,"CDWFrac",ObjectCode);

     if ((phyto_mass < 2.0) && (TPM < 2.5))
       MyResp = ((0.070886)+(-0.06634)*TPM+(0.275184)*phyto_mass+(0.001037)*TPM*TPM+(0.080626)*TPM*phyto_mass-
               (0.119201)*phyto_mass*phyto_mass);
     else
     if ((phyto_mass > 2.0) && (TPM < 2.5))
       MyResp = ((0.070886)+(-0.06634)*TPM+(0.275184)*2+(0.001037)*TPM*TPM+(0.080626)*TPM*2-(0.119201)*2*2);
     else
     if ((phyto_mass < 2.0) && (TPM > 2.5))
     {
       MyResp = ((0.070886)+(-0.06634)*2.5+(0.275184)*phyto_mass+(0.001037)*2.5*2.5+(0.080626)*2.5*phyto_mass-(0.119201)*phyto_mass*phyto_mass);
     }
     else
       MyResp = ((0.070886)+(-0.06634)*2.5+(0.275184)*2+(0.001037)*2.5*2.5+(0.080626)*2.5*2-(0.119201)*2*2);

     Inquiry(GetEcoDynClassName(), MyStandardWeight, ABivalve, ABox,"Dry weight of a standard animal",ObjectCode);
     Inquiry(GetEcoDynClassName(), MyAllometricCoefficient, ABivalve, ABox,"Allometric coefficient for respiration",ObjectCode);
     Inquiry(GetEcoDynClassName(), MyMeatEnergeticContents, ABivalve, ABox,"Meat energetic contents",ObjectCode);
     Inquiry(GetEcoDynClassName(), My_mL_O2_to_Joules, ABivalve, ABox,"mL_O2_to_Joules",ObjectCode);

     MyIndividualRecord[index].RespC = MyResp * AllometricCorrection(MyIndividualRecord[index].ZooIndividualWeight, MyStandardWeight, MyAllometricCoefficient) *
                                              DAYSTOHOURS * My_mL_O2_to_Joules / MyMeatEnergeticContents * MyCDWFrac;
     /*Debugger(AllometricCorrection(MyIndividualRecord[index].ZooIndividualWeight, MyStandardWeight, MyAllometricCoefficient));
     Debugger(My_mL_O2_to_Joules);
     Debugger(MyMeatEnergeticContents); */
   }
   else
   {
     double Q10res, MyWaterTemperature, Tres, Ares, ResBas, Bres, ResRout, ResAbsC, ReferenceTemperature;
     //double const ReferenceTemperature = 20.0;
     Inquiry(GetEcoDynClassName(), Q10res, ABivalve, ABox,"Q10Res",ObjectCode);
     Inquiry(GetEcoDynClassName(), ReferenceTemperature, ABivalve, ABox,"Reference temperature",ObjectCode);
     TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
     if (MyWaterTemperaturePointer != NULL)
        MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,ABox,"Water temperature",ObjectCode);
     else
        MyWaterTemperature = 20.0;
     //Inquiry(GetEcoDynClassName(), Q10res, ABivalve, ABox,"Q10Res",ObjectCode);
     Tres = pow(Q10res,((MyWaterTemperature-ReferenceTemperature)/ReferenceTemperature));
     Inquiry(GetEcoDynClassName(), Ares, ABivalve, ABox,"Ares",ObjectCode);
     Inquiry(GetEcoDynClassName(), Bres, ABivalve, ABox,"Bres",ObjectCode);
     ResBas = Tres * Ares * pow(MyIndividualRecord[index].ZooIndividualWeight,Bres);
     Inquiry(GetEcoDynClassName(), ResAbsC, ABivalve, ABox,"ResAbsC",ObjectCode);
     ResRout = ResAbsC * MyIndividualRecord[index].OIR;
     MyIndividualRecord[index].RespR = ResBas + ResRout; //Total respiration rate - gC/d
     //Debugger(Tres); Debugger(Ares); Debugger(pow(MyIndividualRecord[index].ZooIndividualWeight,Bres));
     //Debugger(MyIndividualRecord[index].RespR); Debugger(ResBas);Debugger(ResRout);
     //MyIndividualRecord[index].NetProdC = MyIndividualRecord[index].ARC - MyIndividualRecord[index].RespR;
   }
   /*Debugger(111);
   Debugger(MyIndividualRecord[index].RespC); */
}

void TMytilusGalloprovincialisIndividual::Excrete(int ABox, int ABivalve)
{
  double MyAexcr, MyBexcr, MyCnatmin, MyCDWFrac, MyNDWFrac, MytotCN, MyExcRBas, MyExcRcor;
  int index, MyOption;
  index = ABivalve * NumberOfBoxesWithMytilusGalloprovincialis + ABox;
  Inquiry(GetEcoDynClassName(), MyAexcr, ABivalve, ABox,"Aexcr",ObjectCode);
  Inquiry(GetEcoDynClassName(), MyBexcr, ABivalve, ABox,"Bexcr",ObjectCode);
  Inquiry(GetEcoDynClassName(), MyCnatmin, ABivalve, ABox,"Cnatmin",ObjectCode);
  Inquiry(GetEcoDynClassName(), MyCDWFrac, ABivalve, ABox,"CDWFrac",ObjectCode);
  //Inquiry(GetEcoDynClassName(), MyNDWFrac, ABivalve, ABox,"NDWFrac",ObjectCode);
  //Inquiry(GetEcoDynClassName(), MyOption, ABivalve, ABox,"Option_Respiration",ObjectCode);
  MyNDWFrac = MyIndividualRecord[index].SomN / (MyIndividualRecord[index].SomC + MyIndividualRecord[index].StorC);
  //MyNDWFrac = 0.1;
  MyExcRBas = MyAexcr*pow(MyIndividualRecord[index].ZooIndividualWeight, MyBexcr);
  if (MyIndividualRecord[index].SomN > 0.0)
     MytotCN = (MyIndividualRecord[index].SomC + MyIndividualRecord[index].StorC) / MyIndividualRecord[index].SomN;
  else
     MytotCN = (MyIndividualRecord[index].SomC + MyIndividualRecord[index].StorC)/0.00001;
  if (MytotCN > MyCnatmin)
     MyExcRcor = 0.0;
  else  {
    if (Option_Respiration == 1) //Respiration calculated from chlorophyll and TPM
       MyExcRcor = (MyNDWFrac / MyCDWFrac) * MyIndividualRecord[index].RespC;
    else
       MyExcRcor = (MyNDWFrac / MyCDWFrac) * MyIndividualRecord[index].RespR;
  }
  MyIndividualRecord[index].ExcR = MyExcRBas + MyExcRcor;
  /*Debugger(MyExcRBas);
  Debugger(MytotCN);
  Debugger(MyCnatmin);
  Debugger(MyExcRcor);
  Debugger(MyIndividualRecord[index].ExcR);  */
}

void TMytilusGalloprovincialisIndividual::Reproduce(int ABox, int ABivalve)
{
   int index;
   double MyReproductionDate;
   index = ABivalve * NumberOfBoxesWithMytilusGalloprovincialis + ABox;
   Inquiry(GetEcoDynClassName(), MyReproductionDate, ABivalve, ABox,"ReproductionDate",ObjectCode);
   /*Debugger(index);
   Debugger(MyReproductionDate); */
   if (MyPEcoDynClass->GetJulianDay() == MyReproductionDate)
   {
      double MyReproductionExpenditure, MyCDWFrac, MyNDWFrac;
      Inquiry(GetEcoDynClassName(), MyReproductionExpenditure, ABivalve, ABox,"ReproductionExpenditure",ObjectCode);
      MyIndividualRecord[index].StorC = MyIndividualRecord[index].StorC -
                                        MyReproductionExpenditure * MyIndividualRecord[index].StorC *
                                        TimeStep / DAYSTOSECONDS;
      Inquiry(GetEcoDynClassName(), MyCDWFrac, ABivalve, ABox,"CDWFrac",ObjectCode);
      Inquiry(GetEcoDynClassName(), MyNDWFrac, ABivalve, ABox,"NDWFracInGametes",ObjectCode);
      MyIndividualRecord[index].SomN = MyIndividualRecord[index].SomN -
                                       MyReproductionExpenditure * MyIndividualRecord[index].SomN *
                                       MyNDWFrac / MyCDWFrac *
                                       TimeStep / DAYSTOSECONDS;
      /*Debugger(MyReproductionDate);
      Debugger(MyReproductionExpenditure * MyIndividualRecord[index].StorC *
                                        TimeStep / DAYSTOSECONDS); */

   }
}

void TMytilusGalloprovincialisIndividual::Aging(int ABox, int ABivalve)
{
   int index;
   index = ABivalve * NumberOfBoxesWithMytilusGalloprovincialis + ABox;
   MyIndividualRecord[index].Age = MyIndividualRecord[index].Age + TimeStep / DAYSTOSECONDS;
}

double TMytilusGalloprovincialisIndividual::ShellLength(double AShellWeight)
{
   return 32.448 * pow(AShellWeight,0.3596);// mm
}

/* Penso que esta fun??o tem de ser adequada aos valores de cada bivalve

double TMytilusGalloprovincialisIndividual::GetParameterValue(char* MyParameter)
{
  double value = 0.0;
  int index;

    // parameters from TMytilusGalloprovincialis
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
    else if (strcmp(MyParameter, "Summer Intrinsic rate of mortality") == 0)
    {
        value = SummerIntrinsicRateOfMortality;
    }
    else if (strcmp(MyParameter, "Maintenance respiration") == 0)
    {
        value = MaintenanceRespiration;
    }
    else if (strcmp(MyParameter, "Activity respiration coefficient") == 0)
    {
        value = ActivityRespirationCoefficient;
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
    else if (strcmp(MyParameter, "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
    {
        value = SoftTissueEnergyToTotalEnergy;
    }
    else if (strcmp(MyParameter, "Number of classes") == 0)
    {
        value = NumberOfClasses;
    }
    else if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
    {
        value = TotalFWToMeatDW;
    }
    else if (strcmp(MyParameter, "Dry weight of a standard animal") == 0)
    {
        value = TotalFWToMeatDW;
    }
    // parameters from TMytilusGalloprovincialisIndividual

    else if (strcmp(MyParameter, "IndividualBasedVariables") == 0)
    {
        value = IndividualBasedVariables;
    }
    else
    {
        for (index = 0; index < NumberOfParameters; index++)
        {
            if (strcmp(MyParameter, MyParsRecord[index].ParameterName) == 0) {
                value = MyParsRecord[index].Value;
                break;
            }
        }
    }
    return value;
}  */

/* Penso que esta fun??o tem de ser adequada aos valores de cada bivalve

bool TMytilusGalloprovincialisIndividual::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int index;

    // parameters from TMytilusGalloprovincialis
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
    else if (strcmp(MyParameter, "Summer Intrinsic rate of mortality") == 0)
    {
        SummerIntrinsicRateOfMortality = value;
    }
    else if (strcmp(MyParameter, "Maintenance respiration") == 0)
    {
        MaintenanceRespiration = value;
    }
    else if (strcmp(MyParameter, "Activity respiration coefficient") == 0)
    {
        ActivityRespirationCoefficient = value;
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
    else if (strcmp(MyParameter, "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
    {
        SoftTissueEnergyToTotalEnergy = value;
    }
    else if (strcmp(MyParameter, "Number of classes") == 0)
    {
        NumberOfClasses = value;
    }
    else if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
    {
        TotalFWToMeatDW = value;
    }

    // parameters from TMytilusGalloprovincialisIndividual
    else if (strcmp(MyParameter, "IndividualBasedVariables") == 0)
    {
        IndividualBasedVariables = value;
    }
    else
    {
        for (index = 0; index < NumberOfParameters; index++)
        {
            if (strcmp(MyParameter, MyParsRecord[index].ParameterName) == 0) {
                MyParsRecord[index].Value = value;
                break;
            }
        }
        if (index == NumberOfParameters)
            rc = false;
    }

    return rc;
}*/

/*bool TMytilusGalloprovincialisIndividual::SaveParameters()
{
    TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("MytilusGalloprovincialis");
    if (PReadWrite == NULL)
    {
        return false;
    }


    PReadWrite->WriteCell("Mytilus galloprovincialis");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfParameters, 0);

    //save parameters' names and values

    for (int i = 0; i < NumberOfParameters; i++)
    {
        if (i > 0) {
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ParametersNameArray[i]);
        PReadWrite->WriteSeparator();

        // parameters from Mytilus galloprovincialis
        if (strcmp(ParametersNameArray[i], "Number of classes") == 0)
        {
            PReadWrite->WriteCell(NumberOfClasses, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Dry weight of a standard animal") == 0)
        {
            PReadWrite->WriteCell(DryWeightOfAStandardAnimal, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Allometric coefficient for feeding") == 0)
        {
            PReadWrite->WriteCell(AllometricCoefficientForFeeding, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Allometric coefficient for respiration") == 0)
        {
            PReadWrite->WriteCell(AllometricCoefficientForRespiration, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Meat energetic contents") == 0)
        {
            PReadWrite->WriteCell(MeatEnergeticContents, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Shell energetic contents") == 0)
        {
            PReadWrite->WriteCell(ShellEnergeticContents, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Intrinsic rate of mortality") == 0)
        {
            PReadWrite->WriteCell(IntrinsicRateOfMortality, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Summer Intrinsic rate of mortality") == 0)
        {
            PReadWrite->WriteCell(SummerIntrinsicRateOfMortality, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Maintenance respiration") == 0)
        {
            PReadWrite->WriteCell(MaintenanceRespiration, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Activity respiration coefficient") == 0)
        {
            PReadWrite->WriteCell(ActivityRespirationCoefficient, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Proportion of energy allocated to shell") == 0)
        {
            PReadWrite->WriteCell(ProportionOfEnergyAllocatedToShell, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Proportion of energy allocated to soft tissue") == 0)
        {
            PReadWrite->WriteCell(ProportionOfEnergyAllocatedToSoftTissue, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "umol NH4 to Joules") == 0)
        {
            PReadWrite->WriteCell(AmmoniaToJoules, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
        {
            PReadWrite->WriteCell(SoftTissueEnergyToTotalEnergy, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "TotalFW to meatDW") == 0)
        {
            PReadWrite->WriteCell(TotalFWToMeatDW, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "OSWFrac") == 0)
        {
            PReadWrite->WriteCell(OSWFrac, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "CDWFrac") == 0)
        {
            PReadWrite->WriteCell(CDWFrac, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "NDWFrac") == 0)
        {
            PReadWrite->WriteCell(NDWFrac, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "CDWFood") == 0)
        {
            PReadWrite->WriteCell(CDWFood, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "NDWFood") == 0)
        {
            PReadWrite->WriteCell(NDWFood, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Cnatmin") == 0)
        {
            PReadWrite->WriteCell(Cnatmin, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Agc") == 0)
        {
            PReadWrite->WriteCell(Agc, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Bgc") == 0)
        {
            PReadWrite->WriteCell(Bgc, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "GPTMin") == 0)
        {
            PReadWrite->WriteCell(GPTMin, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "GPTMax") == 0)
        {
            PReadWrite->WriteCell(GPTMax, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "PHYORGDigestibility") == 0)
        {
            PReadWrite->WriteCell(PHYORGDigestibility, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "DETORGDigestibilityDigestibility") == 0)
        {
            PReadWrite->WriteCell(DETORGDigestibility, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Length_of_a_standard_mussel") == 0)
        {
            PReadWrite->WriteCell(Length_of_a_standard_mussel, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Option_Ingestion_Rate") == 0)
        {
            PReadWrite->WriteCell(Option_Ingestion_Rate, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Option_Clearance_Rate") == 0)
        {
            PReadWrite->WriteCell(Option_Clearance_Rate, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "KStorExp") == 0)
        {
            PReadWrite->WriteCell(KStorExp, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "KStor") == 0)
        {
            PReadWrite->WriteCell(KStor, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "OSWperSOM") == 0)
        {
            PReadWrite->WriteCell(OSWperSOM, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "mL_O2_to_Joules") == 0)
        {
            PReadWrite->WriteCell(mL_O2_to_Joules, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Option_Respiration") == 0)
        {
            PReadWrite->WriteCell(Option_Respiration, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Ares") == 0)
        {
            PReadWrite->WriteCell(Ares, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Bres") == 0)
        {
            PReadWrite->WriteCell(Bres, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Q10Res") == 0)
        {
            PReadWrite->WriteCell(Q10Res, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "ResAbsC") == 0)
        {
            PReadWrite->WriteCell(ResAbsC, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Aexcr") == 0)
        {
            PReadWrite->WriteCell(Aexcr, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Bexcr") == 0)
        {
            PReadWrite->WriteCell(Bexcr, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Bexcr") == 0)
        {
            PReadWrite->WriteCell(Bexcr, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "IndividualBasedVariables") == 0)
        {
            PReadWrite->WriteCell(IndividualBasedVariables, 0);
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}  */


/*void TMytilusGalloprovincialisIndividual::UpdateIndividualVariables(int ABivalve, int ABox)
{
    MyIndividualRecord[ABivalve].IRPHY = IRPHY[ABox];
    MyIndividualRecord[ABivalve].IRDET = IRDET[ABox]; MyIndividualRecord[ABivalve].IRPIM = IRPIM[ABox];
    MyIndividualRecord[ABivalve].IRTPM = IRTPM[ABox]; MyIndividualRecord[ABivalve].FRTPM = FRTPM[ABox];
    MyIndividualRecord[ABivalve].FRPIM = FRPIM[ABox];
    MyIndividualRecord[ABivalve].RRTPM = RRTPM[ABox]; MyIndividualRecord[ABivalve].RRPIM = RRPIM[ABox];
    MyIndividualRecord[ABivalve].OIR = OIR[ABox]; MyIndividualRecord[ABivalve].OCI = OCI[ABox];
    MyIndividualRecord[ABivalve].OCPF = OCPF[ABox]; MyIndividualRecord[ABivalve].NAEIO = NAEIO[ABox];
    MyIndividualRecord[ABivalve].NEA = NEA[ABox]; MyIndividualRecord[ABivalve].NEB = NEB[ABox];
    MyIndividualRecord[ABivalve].ZooIndividualFlux = ZooIndividualFlux[ABox];
    MyIndividualRecord[ABivalve].ZooIndividualFluxForTheShell = ZooIndividualFluxForTheShell[ABox];
    MyIndividualRecord[ABivalve].ZooCR = ClearanceRate[ABox];
    //MyIndividualRecord[ABivalve].GutContents = GutContents[ABox];
    MyIndividualRecord[ABivalve].SomC = SomC[ABox]; MyIndividualRecord[ABivalve].SomN = SomN[ABox];
    MyIndividualRecord[ABivalve].StorC = StorC[ABox]; MyIndividualRecord[ABivalve].OSW = OSW[ABox];
    MyIndividualRecord[ABivalve].GPT = GPT[ABox];
} */

/*void TMytilusGalloprovincialisIndividual::UpdateIndividualParameters(int ABivalve)
{
    for (int j = 0; j < NumberOfParameterRecords; j++)
    {

        if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Allometric coefficient for feeding") == 0)
        {
            AllometricCoefficientForFeeding = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Allometric coefficient for respiration") == 0)
        {
            AllometricCoefficientForRespiration = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Meat energetic contents") == 0)
        {
            MeatEnergeticContents = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Shell energetic contents") == 0)
        {
            ShellEnergeticContents = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Intrinsic rate of mortality") == 0)
        {
            IntrinsicRateOfMortality = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Summer Intrinsic rate of mortality") == 0)
        {
            SummerIntrinsicRateOfMortality = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Maintenance respiration") == 0)
        {
            MaintenanceRespiration = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Activity respiration coefficient") == 0)
        {
            ActivityRespirationCoefficient = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Proportion of energy allocated to shell") == 0)
        {
            ProportionOfEnergyAllocatedToShell = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Proportion of energy allocated to soft tissue") == 0)
        {
            ProportionOfEnergyAllocatedToSoftTissue = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "umol NH4 to Joules") == 0)
        {
            AmmoniaToJoules = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
        {
            SoftTissueEnergyToTotalEnergy = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "TotalFW to meatDW") == 0)
        {
            TotalFWToMeatDW = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Dry weight of a standard animal") == 0)
        {
            DryWeightOfAStandardAnimal = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "OSWFrac") == 0)
        {
            OSWFrac = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "CDWFrac") == 0)
        {
            CDWFrac = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "NDWFrac") == 0)
        {
            NDWFrac = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "CDWFood") == 0)
        {
            CDWFood = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "NDWFood") == 0)
        {
            NDWFood = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Cnatmin") == 0)
        {
            Cnatmin = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Agc") == 0)
        {
            Agc = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Bgc") == 0)
        {
            Bgc = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "GPTMin") == 0)
        {
            GPTMin = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "GPTMax") == 0)
        {
            GPTMax = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "PHYORGDigestibility") == 0)
        {
            PHYORGDigestibility = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "DETORGDigestibility") == 0)
        {
            DETORGDigestibility = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Length_of_a_standard_mussel") == 0)
        {
            Length_of_a_standard_mussel = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Option_Ingestion_Rate") == 0)
        {
            Option_Ingestion_Rate = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Option_Clearance_Rate") == 0)
        {
            Option_Clearance_Rate = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "KStorExp") == 0)
        {
            KStorExp = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "KStor") == 0)
        {
            KStor = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "OSWperSOM") == 0)
        {
            OSWperSOM = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "mL_O2_to_Joules") == 0)
        {
            mL_O2_to_Joules = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Option_Respiration") == 0)
        {
            Option_Respiration = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Ares") == 0)
        {
            Ares = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Bres") == 0)
        {
            Bres = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "ResAbsC") == 0)
        {
            ResAbsC = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Aexcr") == 0)
        {
            Aexcr = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Bexcr") == 0)
        {
            Bexcr = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
    }
} */
