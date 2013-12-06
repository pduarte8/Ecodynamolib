// Dissolved object CPP code file
// @ Pedro Duarte
// Rev. A 01.04.27
/**
 * NPI work
 * 	03 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <vcl.h>
#else  // __BORLANDC__
	#include <iostream>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "dissobjt.h"
#include "iodll.h"
#include "params.h"
#include "EcoDynClass.h"


// TCarlNutrients Class
//


TCarlNutrients::TCarlNutrients(TEcoDynClass* APEcoDynClass, char* className)
            :TRiaF2DNutrients(APEcoDynClass, className)
{
}

// 97.06.19 - Apart from the oceansilica being the last box value, and not zeroing
// the data in the end of the routine, I can't understand why this function was sub-classed

void TCarlNutrients::Integrate()
{
    int i, j, index;
    double MyBoxDepth;
    Integration(Ammonia, NH4Flux,
                Nitrate, NO3Flux,
                Nitrite, NO2Flux,
                Phosphate, PO4Flux,
                Oxygen, OxygenFlux);

    // Transport the nutrients
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();

    if (MyTransportPointer != NULL)
    {

        /*MyTransportPointer->Go(Ammonia, NH4Load,
                           Nitrate, NO3Load,
                           Nitrite, NO2Load,
                           Phosphate, PO4Load,
                           Oxygen, OxygenLoad,
                           0.0, 0.0); */
        MyTransportPointer->Go(Ammonia, NH4Load,0.0, 0.0);
        MyTransportPointer->Go(Nitrate, NO3Load,0.0, 0.0);
        MyTransportPointer->Go(Nitrite, NO2Load,0.0, 0.0);


        //Southern boundary
        for (j = pSubDomain->FirstColumn;
        j <= pSubDomain->LastColumn; j++)
        {
           i = pSubDomain->ILine[j];
           if (i <= pSubDomain->FLine[j])
           {
              index = i * GridColumns + j;
              //Debugger(i); Debugger(j); Debugger(GridColumns); Debugger(index);
              MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth,
                                         index,
                                      "Box depth",
                                   ObjectCode);
              if (
                    (MyBoxDepth > CriticalDepthForLandBoundary) &&
                    (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2)  //Southern ocean boundary
                 )
              {
                 Ammonia[index] = BoundaryNH4Concentration[JulianDay-1];
                 Nitrate[index] = BoundaryNO3Concentration[JulianDay-1];
                 Nitrite[index] = BoundaryNO2Concentration[JulianDay-1];
                 Phosphate[index] = BoundaryPhosphateConcentration[JulianDay-1];
                 Oxygen[index] = BoundaryOxygenConcentration[JulianDay-1];
              }
           }
        }

        //Northern boundary
        for (j = pSubDomain->FirstColumn;
        j <= pSubDomain->LastColumn; j++)
        {
                //i = GridLines - 1;
           i = pSubDomain->FLine[j] /*- 1*/;
           if (i >= pSubDomain->ILine[j])
           {
              index = i * GridColumns + j;
              MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth,
                                         index,
                                      "Box depth",
                                   ObjectCode);
              if (
                    (MyBoxDepth > CriticalDepthForLandBoundary) &&
                    (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 2) //Northern ocean boundary
                 )
              {
                 Ammonia[index] = BoundaryNH4Concentration[JulianDay-1];
                 Nitrate[index] = BoundaryNO3Concentration[JulianDay-1];
                 Nitrite[index] = BoundaryNO2Concentration[JulianDay-1];
                 Phosphate[index] = BoundaryPhosphateConcentration[JulianDay-1];
                 Oxygen[index] = BoundaryOxygenConcentration[JulianDay-1];
             }
           }
        }

        //Eastern boundary
        for (i = pSubDomain->FirstLine;
        i <= pSubDomain->LastLine; i++)
        {
                //j = GridColumns - 1;
           j = pSubDomain->FColumn[i] /*- 1*/;
           if (j >= pSubDomain->IColumn[i])
           {
              index = i * GridColumns + j;
              MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth,
                                         index,
                                      "Box depth",
                                   ObjectCode);
              if (
                    (MyBoxDepth > CriticalDepthForLandBoundary) &&
                    (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2) //Eastern ocean boundary
                 )
              {
                 Ammonia[index] = BoundaryNH4Concentration[JulianDay-1];
                 Nitrate[index] = BoundaryNO3Concentration[JulianDay-1];
                 Nitrite[index] = BoundaryNO2Concentration[JulianDay-1];
                 Phosphate[index] = BoundaryPhosphateConcentration[JulianDay-1];
                 Oxygen[index] = BoundaryOxygenConcentration[JulianDay-1];
              }
           }
        }
        //Western boundary
        for (i = pSubDomain->FirstLine;
        i <= pSubDomain->LastLine; i++)
        {
                //j = 0;
           j = pSubDomain->IColumn[i];
           if (j <= pSubDomain->FColumn[i])
           {
              index = i * GridColumns + j;
               MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth,
                                         index,
                                      "Box depth",
                                   ObjectCode);
              if (
                    (MyBoxDepth > CriticalDepthForLandBoundary) &&
                    (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 2) //Western ocean boundary
                 )
              {
                 Ammonia[index] = BoundaryNH4Concentration[JulianDay-1];
                 Nitrate[index] = BoundaryNO3Concentration[JulianDay-1];
                 Nitrite[index] = BoundaryNO2Concentration[JulianDay-1];
                 Phosphate[index] = BoundaryPhosphateConcentration[JulianDay-1];
                 Oxygen[index] = BoundaryOxygenConcentration[JulianDay-1];
              }
           }
        }

    }
    TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
    double MySalinity, MyTemperature, MyParameter;
    bool MyBoolean;
    for (int k = 0; k < MyNumberOfBoxesInSubDomain; k++)
    {
       index = MyPEcoDynClass->GetSubDomain()->BoxNumber[k];
       if (Ammonia[index]   < 0.0)   Ammonia[index] = 0.0;
       if (Nitrite[index]   < 0.0)   Nitrite[index] = 0.0;
       if (Nitrate[index]   < 0.0)   Nitrate[index] = 0.0;
       if (Phosphate[index] < 0.0)   Phosphate[index] = 0.0;
       if (Oxygen[index]    < 0.0)   Oxygen[index] = 0.0;
       OxygenSaturation[index] = TemporaryOxygenSaturation[index];
       if (OxygenSaturation[index] < 0.0) OxygenSaturation[index] = 0.0;
       RaerationCoef[index] = TemporaryRaerationCoef[index];
    }
    Loads();
}


void TCarlNutrients::Go()
{
   // Active methods
   JulianDay = MyPEcoDynClass->GetJulianDay();
   Nitrification();
}

void TCarlNutrients::Nitrification()
{
    // Fixed terms for nitrification
    const double NH4_TO_NO2 = 0.1;
    const double NO2_TO_NO3 = 0.1;
    double TempNH4, TempNO2, TempNO3;
    int index;
    //for (int index = 0; index < NumberOfBoxes; index++)
    for (int k = 0; k < MyNumberOfBoxesInSubDomain; k++)
    {
        index = MyPEcoDynClass->GetSubDomain()->BoxNumber[k];
        TempNH4 = Ammonia[index] * -NH4_TO_NO2;
        TempNO2 = Ammonia[index] *  NH4_TO_NO2 + Nitrite[index] *  -NO2_TO_NO3;
        TempNO3 = Nitrite[index] *  NO2_TO_NO3;
        NH4Flux[index] = NH4Flux[index]+ TempNH4;
        NO2Flux[index] = NO2Flux[index]+ TempNO2;
        NO3Flux[index] = NO3Flux[index]+ TempNO3;
    }
}


