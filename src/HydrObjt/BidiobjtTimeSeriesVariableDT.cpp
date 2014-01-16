// Bidimensional hydrodynamic model object CPP code file
// 	@ Antonio Pereira
// Rev. A 2004.03.03
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
#include "iodll.h"





TBiDimensionalTimeSeriesVariableDT::TBiDimensionalTimeSeriesVariableDT(TEcoDynClass* PEcoDynClass,
        		char* className, bool readValues, bool cyclic,
                int nrOfPeriods, SimulationPeriod* sPeriods)
        :TBiDimensionalTimeSeries(PEcoDynClass, className, readValues, cyclic,
                nrOfPeriods, sPeriods)
{
   ATimeStep = MyPEcoDynClass->GetTimeStep();
   CriticalTimeStep = ATimeStep;
   CriticalNumberOfSteps = 1;
   Cells = new int[NumberOfBoxes];
   NumberOfCriticalCells = 0;
   ACellRecord = new CellRecord[NumberOfBoxes];
   CriticalCellDepth = new double[NumberOfBoxes];
   CriticalCellHeight = new double[NumberOfBoxes];
   for (int i = 0; i < NumberOfBoxes; i++)
   {
      Cells[i] = 0;
      ACellRecord[i].Critical = false;
      ACellRecord[i].ABoxNumber = i;
      ACellRecord[i].CellTimeStep = ATimeStep;
      ACellRecord[i].NorthFlow = 0.0; ACellRecord[i].SouthFlow = 0.0;
      ACellRecord[i].WestFlow = 0.0; ACellRecord[i].EastFlow = 0.0;
      CriticalCellDepth[i] = 0.0; CriticalCellHeight[i] = 0.0;
   }
}

TBiDimensionalTimeSeriesVariableDT::~TBiDimensionalTimeSeriesVariableDT()
{
    freeMemory();
}

void TBiDimensionalTimeSeriesVariableDT::freeMemory()
{
    try {
       if (NumberOfBoxes > 0) {
           delete [] Cells;
           delete [] ACellRecord;
           delete [] CriticalCellDepth;
           delete [] CriticalCellHeight;
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
        cerr << "TBiDimensionalTimeSeriesVariableDT::freeMemory - " << exc.what() << endl;
    }
#endif  // __BORLANDC__
}

/*void TBiDimensionalTimeSeriesVariableDT::Integrate()
{
   AdvectDiffuse(Salt, SaltLoad);
} */

void TBiDimensionalTimeSeriesVariableDT::Continuity()
{
    double MyWestUFlow, MyEastUFlow, MyNorthVFlow, MySouthVFlow;
    int i,j,index, k, ADay, m;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();

    EnableContinuity = false;       // AP, 2005.09.16 - moved to here

//    Generic = Salt;     // AP, 2005.08.30
    NumberOfCriticalCells = 0;
    for (m = 0; m < MyNumberOfBoxes; m++)
    {
        i = MyPEcoDynClass->GetLineIndex(MyBoxNumber[m]);
        j = MyPEcoDynClass->GetColumnIndex(MyBoxNumber[m]);
        if (
                  (i >= pSubDomain->ILine[j])
                  && (i <= pSubDomain->FLine[j] /*- 1*/)
                  && (j >= pSubDomain->IColumn[i])
                  && (j <= pSubDomain->FColumn[i] /*- 1*/)
            )
        {
            index = MyBoxNumber[m];
            ACellRecord[index].NumberOfSteps = 1;
            ACellRecord[index].Critical = false;

      	    if (
                //(BoxArray[index].Dummybool2) &&
                (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 2) &&
                (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 2) &&
                (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 2) &&
                (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 2) &&
                (BoxArray[index].Dummybool1) /*&&
                (BoxDepth[index] > 0.0) */
                )
            {
                MyWestUFlow = WesternComponentOfContinuity(i,j);
                MyEastUFlow = EasternComponentOfContinuity(i,j);
                MyNorthVFlow = NorthernComponentOfContinuity(i,j);
                MySouthVFlow = SouthernComponentOfContinuity(i,j);
                if (BoxDepth[index] > 0.0)
                   CheckTemporalResolution(index, BoxDepth[index], MyWestUFlow, MyEastUFlow, MyNorthVFlow, MySouthVFlow);
                if (ACellRecord[index].Critical)
                {
                   Cells[NumberOfCriticalCells] = index;
                   NumberOfCriticalCells++;
                   ACellRecord[index].WestFlow = MyWestUFlow;
                   ACellRecord[index].EastFlow = MyEastUFlow;
                   ACellRecord[index].NorthFlow = MyNorthVFlow;
                   ACellRecord[index].SouthFlow = MySouthVFlow;
                }
                DynamicHeight[index] = PastHeight[index] +
                                   (
                                        MyWestUFlow -
                                        MyEastUFlow -
                                        MyNorthVFlow +
                                        MySouthVFlow
                                    ) / (ASpatialStep * ASpatialStep);
                if (MyLoadRecord != NULL)
                {
                        for (k = 0; k < MyPEcoDynClass->GetNumberOfLoads(); k++)
                        {
                                if (
                                        (i == MyLoadRecord[k].LineCoordinate) &&
                                        (j == MyLoadRecord[k].ColumnCoordinate)
                                    )
                                {
                                        ADay = MIN(MyPEcoDynClass->GetJulianDay(),
                                        MyPEcoDynClass->GetNumberOfDaysForLoads());
                                        DynamicHeight[index] = DynamicHeight[index] + ATimeStep /*/ 2.0*/ *
                                                         MyLoadRecord[k].LoadFlow[ADay - 1] /
                                                         (
                                                            (ASpatialStep * ASpatialStep)
                                                         );
                                        break;
                                }
                        }
                }
            }
      	}
    }
    //AdvectDiffuse(Salt, SaltLoad);
}

void TBiDimensionalTimeSeriesVariableDT::AdvectDiffuseCriticalCells(
        double* Generic, double* PastGeneric, FlowCriticalCells* flowCC, double* GenericLoad)
{
    double MyWestUFlow, MyEastUFlow, MyNorthVFlow, MySouthVFlow,
           MyWestDiffusion, MyEastDiffusion, MyNorthDiffusion, MySouthDiffusion,
           MyPastWaterLevel, MyPresentWaterLevel,MyBoxDepth;
    MyWestDiffusion = 0.0; MyEastDiffusion = 0.0; MyNorthDiffusion = 0.0; MySouthDiffusion = 0.0;
    //As difusões são inicializadas a zero e, por enquanto, mantidas a zero (donde o código comentado abaixo),
    //pois decidi não as calcular nas células críticas,
    //para evitar problemas numéricos que surgiam nalgumas células
    int i,j,index, k, ADay, m;
    double *TemporaryBoxDepth, *TemporaryPastHeight;
    TemporaryBoxDepth = new double[NumberOfBoxes]; TemporaryPastHeight = new double[NumberOfBoxes];

    for (int MyStep = 0; MyStep < CriticalNumberOfSteps; MyStep++)
    {
        for (m = 0; m < NumberOfCriticalCells; m++)
        {
            index = Cells[m];
            i = MyPEcoDynClass->GetLineIndex(index);
            j = MyPEcoDynClass->GetColumnIndex(index);

            if (MyStep == 0)
            {
               TemporaryBoxDepth[index] = BoxDepth[index];
               TemporaryPastHeight[index] = PastHeight[index];
            }
            MyBoxDepth = TemporaryBoxDepth[index];
            MyPastWaterLevel = TemporaryPastHeight[index];

            ACellRecord[index].CellTimeStep = CriticalTimeStep;
            //Advective fluxes
            MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGeneric) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGeneric) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGeneric) * ACellRecord[index].CellTimeStep / ATimeStep;
            MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGeneric) * ACellRecord[index].CellTimeStep / ATimeStep;

            //Diffusive fluxes
            flowCC[index].NorthAdvecFlow += MyNorthVFlow;
            flowCC[index].SouthAdvecFlow += MySouthVFlow;
            flowCC[index].WestAdvecFlow += MyWestUFlow;
            flowCC[index].EastAdvecFlow += MyEastUFlow;

            flowCC[index].NorthDiffFlow += MyNorthDiffusion;
            flowCC[index].SouthDiffFlow += MySouthDiffusion;
            flowCC[index].WestDiffFlow += MyWestDiffusion;
            flowCC[index].EastDiffFlow += MyEastDiffusion;

            MyPresentWaterLevel = MyPastWaterLevel + (
                                                        ACellRecord[index].WestFlow -
                                                        ACellRecord[index].EastFlow -
                                                        ACellRecord[index].NorthFlow +
                                                        ACellRecord[index].SouthFlow
                                                      ) / (ASpatialStep * ASpatialStep) *
                                                      ACellRecord[index].CellTimeStep / ATimeStep;



            if (MyLoadRecord != NULL)
            {
               for (k = 0; k < MyPEcoDynClass->GetNumberOfLoads(); k++)
               {
                  if (
                      (i == MyLoadRecord[k].LineCoordinate) &&
                      (j == MyLoadRecord[k].ColumnCoordinate)
                     )
                  {
                     ADay = MIN(MyPEcoDynClass->GetJulianDay(),MyPEcoDynClass->GetNumberOfDaysForLoads());
                     MyPresentWaterLevel = MyPresentWaterLevel + ACellRecord[index].CellTimeStep *
                                                     MyLoadRecord[k].LoadFlow[ADay - 1] /
                                                     (
                                                        (ASpatialStep * ASpatialStep)
                                                     );
                      break;
                  }
               }
            }

            if (MyBoxDepth > CriticalDepthForLandBoundary)
            {
                Generic[index ] = PastGeneric[index]  * MyBoxDepth /
                                  (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel)
                                  + (
                                        MyWestUFlow -
                                        MyEastUFlow -
                                        MyNorthVFlow +
                                        MySouthVFlow
                                    ) / (ASpatialStep * ASpatialStep *
                                    (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel))
                                    +(
                                        MyWestDiffusion +
                                        MyEastDiffusion +
                                        MyNorthDiffusion +
                                        MySouthDiffusion
                                     ) / (2.0 * ASpatialStep * ASpatialStep *
                                     (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel));

                MyBoxDepth = MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel;

                MyPastWaterLevel = MyPresentWaterLevel;
                if (MyLoadRecord != NULL)
                {
                        for (k = 0; k < MyPEcoDynClass->GetNumberOfLoads(); k++)
                        {
                                if (
                                        (i == MyLoadRecord[k].LineCoordinate) &&
                                        (j == MyLoadRecord[k].ColumnCoordinate)
                                   )
                                {
                                                ADay = MIN(MyPEcoDynClass->GetJulianDay(),
                                                MyPEcoDynClass->GetNumberOfDaysForLoads());

                                                Generic[index ] = Generic[index ] + ACellRecord[index].CellTimeStep * GenericLoad[index] *
                                                        MyLoadRecord[k].LoadFlow[ADay - 1] /
                                                (
                                                        (MyBoxDepth/*BoxDepth[index] + MyPresentWaterLevel - MyPastWaterLevel*/) *
                                                        (ASpatialStep * ASpatialStep)
                                                );
                                                break;
                                }
                        }
                }
            }
            TemporaryPastHeight[index] = MyPresentWaterLevel;
            TemporaryBoxDepth[index] = MyBoxDepth;
            PastGeneric[index] = Generic[index];
            CriticalCellDepth[index] = TemporaryBoxDepth[index];
            CriticalCellHeight[index] = TemporaryPastHeight[index];
        }
    }
    delete [] TemporaryBoxDepth;
    delete [] TemporaryPastHeight;
}

void TBiDimensionalTimeSeriesVariableDT::AdvectDiffuse(double* Generic, double* GenericLoad)
{
    double MyWestUFlow, MyEastUFlow, MyNorthVFlow, MySouthVFlow,
           MyWestDiffusion, MyEastDiffusion, MyNorthDiffusion, MySouthDiffusion, ResultantDiffusion;
    int i,j,index, k, ADay, m;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    double *PastGeneric = new double[NumberOfBoxes];
    FlowCriticalCells *flowCriticalCells = new FlowCriticalCells[NumberOfBoxes];

        memset(flowCriticalCells, 0, sizeof(FlowCriticalCells) * NumberOfBoxes);

        for (i = 0; i < NumberOfBoxes; i++)
   	        PastGeneric[i] = Generic[i];
        if (NumberOfCriticalCells > 0)
            AdvectDiffuseCriticalCells(Generic, PastGeneric, flowCriticalCells, GenericLoad);
        for (m = 0; m < MyNumberOfBoxes; m++)
        {
            i = MyPEcoDynClass->GetLineIndex(MyBoxNumber[m]);
            j = MyPEcoDynClass->GetColumnIndex(MyBoxNumber[m]);
            if (
                  (i >= pSubDomain->ILine[j])
                  && (i <= pSubDomain->FLine[j] /*- 1*/)
                  && (j >= pSubDomain->IColumn[i])
                  && (j <= pSubDomain->FColumn[i] /*- 1*/)
                )
            {
            index = MyBoxNumber[m];

            if (
                    //(BoxArray[index].Dummybool2) &&
                    (BoxDepth[index] > CriticalDepthForLandBoundary) &&
                    (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 2) &&
                    (BoxArray[index].Dummybool1) &&
                    (!ACellRecord[index].Critical)
                )
            {

                        //Advective fluxes
                   if ((j > 0) && (ACellRecord[index - 1].Critical))
                      MyWestUFlow = flowCriticalCells[index - 1].EastAdvecFlow;
                   else
                      MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGeneric);

                   if ((j < GridColumns - 1) && (ACellRecord[index + 1].Critical))
                      MyEastUFlow = flowCriticalCells[index + 1].WestAdvecFlow;
                   else
                      MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGeneric);

                   if ((i < GridLines) && (ACellRecord[(i + 1) * GridColumns + j].Critical))
                      MyNorthVFlow = flowCriticalCells[(i + 1) * GridColumns + j].SouthAdvecFlow;
                   else
                      MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGeneric);

                   if ((i > 0) && (ACellRecord[(i - 1) * GridColumns + j].Critical))
                      MySouthVFlow = flowCriticalCells[(i - 1) * GridColumns + j].NorthAdvecFlow;
                   else
                      MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGeneric);

                        //Diffusive fluxes
                   if ((j > 0) && (ACellRecord[index - 1].Critical))
                      MyWestDiffusion = - flowCriticalCells[index - 1].EastDiffFlow;
                   else
                      MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGeneric);

                   if ((j < GridColumns - 1) && (ACellRecord[index + 1].Critical))
                      MyEastDiffusion = - flowCriticalCells[index + 1].WestDiffFlow;
                   else
                      MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGeneric);

                   if ((i < GridLines) && (ACellRecord[(i + 1) * GridColumns + j].Critical))
                      MyNorthDiffusion = - flowCriticalCells[(i + 1) * GridColumns + j].SouthDiffFlow;
                   else
                      MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGeneric);

                   if ((i > 0) && (ACellRecord[(i - 1) * GridColumns + j].Critical))
                      MySouthDiffusion = - flowCriticalCells[(i - 1) * GridColumns + j].NorthDiffFlow;
                   else
                      MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGeneric);


                   if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                               MyNorthDiffusion, MySouthDiffusion, PastGeneric[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion +
                                             MyNorthDiffusion + MySouthDiffusion;
                   else
                        ResultantDiffusion = 0.0;

                    Generic[index ] = PastGeneric[index]  *
                            BoxDepth[index] /
                            (BoxDepth[index] + DynamicHeight[index] - PastHeight[index])
                            +
                            (
                                    MyWestUFlow -
                                    MyEastUFlow -
                                    MyNorthVFlow +
                                    MySouthVFlow
                            ) /
                            (ASpatialStep * ASpatialStep *
                            (BoxDepth[index] + DynamicHeight[index] - PastHeight[index]))
                            +
                            (
                                    ResultantDiffusion
                            ) /
                            (2.0 * ASpatialStep * ASpatialStep *
                            (BoxDepth[index] + DynamicHeight[index] - PastHeight[index]));

                    if (MyLoadRecord != NULL)
                    {
                            for (k = 0; k < MyPEcoDynClass->GetNumberOfLoads(); k++)
                            {
                                    if (
                                            (i == MyLoadRecord[k].LineCoordinate) &&
                                            (j == MyLoadRecord[k].ColumnCoordinate)
                                    )
                                    {
                                            ADay = MIN(MyPEcoDynClass->GetJulianDay(),
                                            MyPEcoDynClass->GetNumberOfDaysForLoads());

                                            Generic[index ] = Generic[index ] + ATimeStep * GenericLoad[index] *
                                                    MyLoadRecord[k].LoadFlow[ADay - 1] /
                                            (
                                                    (BoxDepth[index] + DynamicHeight[index] - PastHeight[index]) *
                                                    (ASpatialStep * ASpatialStep)
                                            );
                                            break;
                                    }
                            }
                   }
      	    }
        }
    }
    delete [] flowCriticalCells;
    delete [] PastGeneric;
}

void TBiDimensionalTimeSeriesVariableDT::CheckTemporalResolution(int ABoxNumber, double ABoxDepth,
                                                               double MyWestUFlow, double MyEastUFlow,
                                                               double MyNorthVFlow, double MySouthVFlow)
{
        double Input, Output, MyBoxVolume;
        Input = 0; Output = 0;
        if (MyWestUFlow >= 0.0)
                Input = Input + MyWestUFlow;
        else
                Output = Output + fabs(MyWestUFlow);
        if (MySouthVFlow >= 0.0)
                Input = Input + MySouthVFlow;
        else
                Output = Output + fabs(MySouthVFlow);
        if (MyEastUFlow < 0.0)
                Input = Input + fabs(MyEastUFlow);
        else
                Output = Output + MyEastUFlow;
        if (MyNorthVFlow < 0.0)
                Input = Input + fabs(MyNorthVFlow);
        else
                Output = Output + MyNorthVFlow;
        MyBoxVolume = ABoxDepth * ASpatialStep * ASpatialStep;

        if ((Input * ATimeStep >= 0.8 * MyBoxVolume) || (Output * ATimeStep >= 0.8 * MyBoxVolume))
        {
          ACellRecord[ABoxNumber].Critical = true;
          do
          {
           //Debugger(Input); Debugger(Output); Debugger(MyBoxVolume); Debugger(ACellRecord[ABoxNumber].CellTimeStep);
           ACellRecord[ABoxNumber].CellTimeStep = ACellRecord[ABoxNumber].CellTimeStep / 2.0;
           ACellRecord[ABoxNumber].NumberOfSteps = ACellRecord[ABoxNumber].NumberOfSteps * 2;
          } while ((ACellRecord[ABoxNumber].CellTimeStep >= 0.8 * MyBoxVolume / MAX(Input, Output)) && (ACellRecord[ABoxNumber].CellTimeStep > 1));
        }
        CriticalTimeStep = MIN(CriticalTimeStep, ACellRecord[ABoxNumber].CellTimeStep);
        CriticalNumberOfSteps = MAX(CriticalNumberOfSteps, ACellRecord[ABoxNumber].NumberOfSteps);
}


void TBiDimensionalTimeSeriesVariableDT::AdvectDiffuseCriticalCells(
        double* GenericA, double* PastGenericA, double *ALoad, FlowCriticalCells* flowCCA,
        double* GenericB, double* PastGenericB, double *BLoad, FlowCriticalCells* flowCCB,
        double* GenericC, double* PastGenericC, double *CLoad, FlowCriticalCells* flowCCC,
        double* GenericD, double* PastGenericD, double *DLoad, FlowCriticalCells* flowCCD,
        double* GenericE, double* PastGenericE, double *ELoad, FlowCriticalCells* flowCCE)
{
    double MyWestUFlowA, MyEastUFlowA, MyNorthVFlowA, MySouthVFlowA,
           MyWestDiffusionA, MyEastDiffusionA, MyNorthDiffusionA, MySouthDiffusionA,
           MyWestUFlowB, MyEastUFlowB, MyNorthVFlowB, MySouthVFlowB,
           MyWestDiffusionB, MyEastDiffusionB, MyNorthDiffusionB, MySouthDiffusionB,
           MyWestUFlowC, MyEastUFlowC, MyNorthVFlowC, MySouthVFlowC,
           MyWestDiffusionC, MyEastDiffusionC, MyNorthDiffusionC, MySouthDiffusionC,
           MyWestUFlowD, MyEastUFlowD, MyNorthVFlowD, MySouthVFlowD,
           MyWestDiffusionD, MyEastDiffusionD, MyNorthDiffusionD, MySouthDiffusionD,
           MyWestUFlowE, MyEastUFlowE, MyNorthVFlowE, MySouthVFlowE,
           MyWestDiffusionE, MyEastDiffusionE, MyNorthDiffusionE, MySouthDiffusionE,
           MyPastWaterLevel, MyPresentWaterLevel,MyBoxDepth;
    MyWestDiffusionA = 0.0; MyEastDiffusionA = 0.0; MyNorthDiffusionA = 0.0; MySouthDiffusionA = 0.0;
    MyWestDiffusionB = 0.0; MyEastDiffusionB = 0.0; MyNorthDiffusionB = 0.0; MySouthDiffusionB = 0.0;
    MyWestDiffusionC = 0.0; MyEastDiffusionC = 0.0; MyNorthDiffusionC = 0.0; MySouthDiffusionC = 0.0;
    MyWestDiffusionD = 0.0; MyEastDiffusionD = 0.0; MyNorthDiffusionD = 0.0; MySouthDiffusionD = 0.0;
    MyWestDiffusionE = 0.0; MyEastDiffusionE = 0.0; MyNorthDiffusionE = 0.0; MySouthDiffusionE = 0.0;
    //As difusões são inicializadas a zero e, por enquanto, mantidas a zero (donde o código comentado abaixo),
    //pois decidi não as calcular nas células críticas,
    //para evitar problemas numéricos que surgiam nalgumas células
    int i,j,index, k, ADay, m;
    double *TemporaryBoxDepth, *TemporaryPastHeight;
    TemporaryBoxDepth = new double[NumberOfBoxes]; TemporaryPastHeight = new double[NumberOfBoxes];
    //for (m = 0; m < NumberOfCriticalCells; m++)
    for (int MyStep = 0; MyStep < CriticalNumberOfSteps; MyStep++)
    {
        //for (int MyStep = 0; MyStep < ACellRecord[index].NumberOfSteps; MyStep++)
        for (m = 0; m < NumberOfCriticalCells; m++)
        {
            index = Cells[m];
            i = MyPEcoDynClass->GetLineIndex(index);
            j = MyPEcoDynClass->GetColumnIndex(index);

            if (MyStep == 0)
            {
               TemporaryBoxDepth[index] = BoxDepth[index];
               TemporaryPastHeight[index] = PastHeight[index];
            }
            MyBoxDepth = TemporaryBoxDepth[index];
            MyPastWaterLevel = TemporaryPastHeight[index];
            //MyPastWaterLevel = PastHeight[index];
            //MyBoxDepth = BoxDepth[index];

            ACellRecord[index].CellTimeStep = CriticalTimeStep;
            //Advective fluxes
            MyWestUFlowA = WesternComponentOfAdvectionDiffusion(i,j,PastGenericA) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyWestUFlowB = WesternComponentOfAdvectionDiffusion(i,j,PastGenericB) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyWestUFlowC = WesternComponentOfAdvectionDiffusion(i,j,PastGenericC) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyWestUFlowD = WesternComponentOfAdvectionDiffusion(i,j,PastGenericD) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyWestUFlowE = WesternComponentOfAdvectionDiffusion(i,j,PastGenericE) * ACellRecord[index].CellTimeStep / ATimeStep;

            MyEastUFlowA = EasternComponentOfAdvectionDiffusion(i,j,PastGenericA) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyEastUFlowB = EasternComponentOfAdvectionDiffusion(i,j,PastGenericB) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyEastUFlowC = EasternComponentOfAdvectionDiffusion(i,j,PastGenericC) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyEastUFlowD = EasternComponentOfAdvectionDiffusion(i,j,PastGenericD) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyEastUFlowE = EasternComponentOfAdvectionDiffusion(i,j,PastGenericE) * ACellRecord[index].CellTimeStep / ATimeStep;

            MyNorthVFlowA = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericA) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyNorthVFlowB = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericB) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyNorthVFlowC = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericC) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyNorthVFlowD = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericD) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyNorthVFlowE = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericE) * ACellRecord[index].CellTimeStep / ATimeStep;

            MySouthVFlowA = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericA) * ACellRecord[index].CellTimeStep / ATimeStep;
            MySouthVFlowB = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericB) * ACellRecord[index].CellTimeStep / ATimeStep;
            MySouthVFlowC = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericC) * ACellRecord[index].CellTimeStep / ATimeStep;
            MySouthVFlowD = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericD) * ACellRecord[index].CellTimeStep / ATimeStep;
            MySouthVFlowE = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericE) * ACellRecord[index].CellTimeStep / ATimeStep;

            //Diffusive fluxes

            flowCCA[index].NorthAdvecFlow += MyNorthVFlowA;
            flowCCA[index].SouthAdvecFlow += MySouthVFlowA;
            flowCCA[index].WestAdvecFlow += MyWestUFlowA;
            flowCCA[index].EastAdvecFlow += MyEastUFlowA;

            flowCCA[index].NorthDiffFlow += MyNorthDiffusionA;
            flowCCA[index].SouthDiffFlow += MySouthDiffusionA;
            flowCCA[index].WestDiffFlow += MyWestDiffusionA;
            flowCCA[index].EastDiffFlow += MyEastDiffusionA;

            flowCCB[index].NorthAdvecFlow += MyNorthVFlowB;
            flowCCB[index].SouthAdvecFlow += MySouthVFlowB;
            flowCCB[index].WestAdvecFlow += MyWestUFlowB;
            flowCCB[index].EastAdvecFlow += MyEastUFlowB;

            flowCCB[index].NorthDiffFlow += MyNorthDiffusionB;
            flowCCB[index].SouthDiffFlow += MySouthDiffusionB;
            flowCCB[index].WestDiffFlow += MyWestDiffusionB;
            flowCCB[index].EastDiffFlow += MyEastDiffusionB;

            flowCCC[index].NorthAdvecFlow += MyNorthVFlowC;
            flowCCC[index].SouthAdvecFlow += MySouthVFlowC;
            flowCCC[index].WestAdvecFlow += MyWestUFlowC;
            flowCCC[index].EastAdvecFlow += MyEastUFlowC;

            flowCCC[index].NorthDiffFlow += MyNorthDiffusionC;
            flowCCC[index].SouthDiffFlow += MySouthDiffusionC;
            flowCCC[index].WestDiffFlow += MyWestDiffusionC;
            flowCCC[index].EastDiffFlow += MyEastDiffusionC;

            flowCCD[index].NorthAdvecFlow += MyNorthVFlowD;
            flowCCD[index].SouthAdvecFlow += MySouthVFlowD;
            flowCCD[index].WestAdvecFlow += MyWestUFlowD;
            flowCCD[index].EastAdvecFlow += MyEastUFlowD;

            flowCCD[index].NorthDiffFlow += MyNorthDiffusionD;
            flowCCD[index].SouthDiffFlow += MySouthDiffusionD;
            flowCCD[index].WestDiffFlow += MyWestDiffusionD;
            flowCCD[index].EastDiffFlow += MyEastDiffusionD;

            flowCCE[index].NorthAdvecFlow += MyNorthVFlowE;
            flowCCE[index].SouthAdvecFlow += MySouthVFlowE;
            flowCCE[index].WestAdvecFlow += MyWestUFlowE;
            flowCCE[index].EastAdvecFlow += MyEastUFlowE;

            flowCCE[index].NorthDiffFlow += MyNorthDiffusionE;
            flowCCE[index].SouthDiffFlow += MySouthDiffusionE;
            flowCCE[index].WestDiffFlow += MyWestDiffusionE;
            flowCCE[index].EastDiffFlow += MyEastDiffusionE;

            MyPresentWaterLevel = MyPastWaterLevel + (
                                                        ACellRecord[index].WestFlow -
                                                        ACellRecord[index].EastFlow -
                                                        ACellRecord[index].NorthFlow +
                                                        ACellRecord[index].SouthFlow
                                                      ) / (ASpatialStep * ASpatialStep) *
                                                      ACellRecord[index].CellTimeStep / ATimeStep;



            if (MyLoadRecord != NULL)
            {
               for (k = 0; k < MyPEcoDynClass->GetNumberOfLoads(); k++)
               {
                  if (
                      (i == MyLoadRecord[k].LineCoordinate) &&
                      (j == MyLoadRecord[k].ColumnCoordinate)
                     )
                  {
                     ADay = MIN(MyPEcoDynClass->GetJulianDay(),MyPEcoDynClass->GetNumberOfDaysForLoads());
                     MyPresentWaterLevel = MyPresentWaterLevel + ACellRecord[index].CellTimeStep *
                                                     MyLoadRecord[k].LoadFlow[ADay - 1] /
                                                     (
                                                        (ASpatialStep * ASpatialStep)
                                                     );
                      break;
                  }
               }
            }

            if (MyBoxDepth > CriticalDepthForLandBoundary)
            {
                GenericA[index ] = PastGenericA[index]  * MyBoxDepth /
                                  (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel)
                                  + (
                                        MyWestUFlowA -
                                        MyEastUFlowA -
                                        MyNorthVFlowA +
                                        MySouthVFlowA
                                    ) / (ASpatialStep * ASpatialStep *
                                    (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel))
                                    +(
                                        MyWestDiffusionA +
                                        MyEastDiffusionA +
                                        MyNorthDiffusionA +
                                        MySouthDiffusionA
                                     ) / (2.0 * ASpatialStep * ASpatialStep *
                                     (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel));

                GenericB[index ] = PastGenericB[index]  * MyBoxDepth /
                                  (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel)
                                  + (
                                        MyWestUFlowB -
                                        MyEastUFlowB -
                                        MyNorthVFlowB +
                                        MySouthVFlowB
                                    ) / (ASpatialStep * ASpatialStep *
                                    (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel))
                                    +(
                                        MyWestDiffusionB +
                                        MyEastDiffusionB +
                                        MyNorthDiffusionB +
                                        MySouthDiffusionB
                                     ) / (2.0 * ASpatialStep * ASpatialStep *
                                     (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel));

                GenericC[index ] = PastGenericC[index]  * MyBoxDepth /
                                  (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel)
                                  + (
                                        MyWestUFlowC -
                                        MyEastUFlowC -
                                        MyNorthVFlowC +
                                        MySouthVFlowC
                                    ) / (ASpatialStep * ASpatialStep *
                                    (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel))
                                    +(
                                        MyWestDiffusionC +
                                        MyEastDiffusionC +
                                        MyNorthDiffusionC +
                                        MySouthDiffusionC
                                     ) / (2.0 * ASpatialStep * ASpatialStep *
                                     (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel));

                 GenericD[index ] = PastGenericD[index]  * MyBoxDepth /
                                  (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel)
                                  + (
                                        MyWestUFlowD -
                                        MyEastUFlowD -
                                        MyNorthVFlowD +
                                        MySouthVFlowD
                                    ) / (ASpatialStep * ASpatialStep *
                                    (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel))
                                    +(
                                        MyWestDiffusionD +
                                        MyEastDiffusionD +
                                        MyNorthDiffusionD +
                                        MySouthDiffusionD
                                     ) / (2.0 * ASpatialStep * ASpatialStep *
                                     (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel));

                GenericE[index ] = PastGenericE[index]  * MyBoxDepth /
                                  (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel)
                                  + (
                                        MyWestUFlowE -
                                        MyEastUFlowE -
                                        MyNorthVFlowE +
                                        MySouthVFlowE
                                    ) / (ASpatialStep * ASpatialStep *
                                    (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel))
                                    +(
                                        MyWestDiffusionE +
                                        MyEastDiffusionE +
                                        MyNorthDiffusionE +
                                        MySouthDiffusionE
                                     ) / (2.0 * ASpatialStep * ASpatialStep *
                                     (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel));

                MyBoxDepth = MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel;

                MyPastWaterLevel = MyPresentWaterLevel;
                if (MyLoadRecord != NULL)
                {
                    for (k = 0; k < MyPEcoDynClass->GetNumberOfLoads(); k++)
                    {
                        if (
                                (i == MyLoadRecord[k].LineCoordinate) &&
                                (j == MyLoadRecord[k].ColumnCoordinate)
                           )
                        {
                            ADay = MIN(MyPEcoDynClass->GetJulianDay(),
                            MyPEcoDynClass->GetNumberOfDaysForLoads());

                            GenericA[index ] = GenericA[index ] + ACellRecord[index].CellTimeStep * ALoad[index] *
                                    MyLoadRecord[k].LoadFlow[ADay - 1] /
                            (
                                    MyBoxDepth *
                                    ASpatialStep * ASpatialStep
                            );

                            GenericB[index ] = GenericB[index ] + ACellRecord[index].CellTimeStep * BLoad[index] *
                                    MyLoadRecord[k].LoadFlow[ADay - 1] /
                            (
                                    MyBoxDepth *
                                    ASpatialStep * ASpatialStep
                            );

                            GenericC[index ] = GenericC[index ] + ACellRecord[index].CellTimeStep * CLoad[index] *
                                    MyLoadRecord[k].LoadFlow[ADay - 1] /
                            (
                                    MyBoxDepth *
                                    ASpatialStep * ASpatialStep
                            );

                            GenericD[index ] = GenericD[index ] + ACellRecord[index].CellTimeStep * DLoad[index] *
                                    MyLoadRecord[k].LoadFlow[ADay - 1] /
                            (
                                    MyBoxDepth *
                                    ASpatialStep * ASpatialStep
                            );

                            GenericE[index ] = GenericE[index ] + ACellRecord[index].CellTimeStep * ELoad[index] *
                                    MyLoadRecord[k].LoadFlow[ADay - 1] /
                            (
                                    MyBoxDepth *
                                    ASpatialStep * ASpatialStep
                            );
                            break;
                        }
                    }
                }
            }
            TemporaryPastHeight[index] = MyPresentWaterLevel;
            TemporaryBoxDepth[index] = MyBoxDepth;
            //PastHeight[index] = MyPresentWaterLevel;
            //BoxDepth[index] = MyBoxDepth;
            PastGenericA[index] = GenericA[index];
            PastGenericB[index] = GenericB[index];
            PastGenericC[index] = GenericC[index];
            PastGenericD[index] = GenericD[index];
            PastGenericE[index] = GenericE[index];
            CriticalCellDepth[index] = TemporaryBoxDepth[index];
            CriticalCellHeight[index] = TemporaryPastHeight[index];
      	}
    }
    delete [] TemporaryBoxDepth;
    delete [] TemporaryPastHeight;
}


void TBiDimensionalTimeSeriesVariableDT::AdvectDiffuse(double *A, double *ALoad,
                        double *B, double *BLoad,
                        double *C, double *CLoad,
                        double *D, double *DLoad,
                        double *E, double *ELoad)
{
	double MyWestUFlow, MyEastUFlow, MyNorthVFlow, MySouthVFlow,
        MyWestDiffusion, MyEastDiffusion, MyNorthDiffusion, MySouthDiffusion, ResultantDiffusion;
    double H1, Q1;
    int i,j,index, k, ADay, m;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();

    double *PastGenericA, *PastGenericB, *PastGenericC, *PastGenericD, *PastGenericE;
    FlowCriticalCells *flowCriticalCellsA, *flowCriticalCellsB, *flowCriticalCellsC,
            *flowCriticalCellsD, *flowCriticalCellsE;

    flowCriticalCellsA = new FlowCriticalCells[NumberOfBoxes];
    flowCriticalCellsB = new FlowCriticalCells[NumberOfBoxes];
    flowCriticalCellsC = new FlowCriticalCells[NumberOfBoxes];
    flowCriticalCellsD = new FlowCriticalCells[NumberOfBoxes];
    flowCriticalCellsE = new FlowCriticalCells[NumberOfBoxes];
    memset(flowCriticalCellsA, 0, sizeof(FlowCriticalCells) * NumberOfBoxes);
    memset(flowCriticalCellsB, 0, sizeof(FlowCriticalCells) * NumberOfBoxes);
    memset(flowCriticalCellsC, 0, sizeof(FlowCriticalCells) * NumberOfBoxes);
    memset(flowCriticalCellsD, 0, sizeof(FlowCriticalCells) * NumberOfBoxes);
    memset(flowCriticalCellsE, 0, sizeof(FlowCriticalCells) * NumberOfBoxes);

    PastGenericA = new double[NumberOfBoxes];
    PastGenericB = new double[NumberOfBoxes];
    PastGenericC = new double[NumberOfBoxes];
    PastGenericD = new double[NumberOfBoxes];
    PastGenericE = new double[NumberOfBoxes];


    for (i = 0; i < NumberOfBoxes; i++)
    {
        PastGenericA[i] = A[i];
        PastGenericB[i] = B[i];
        PastGenericC[i] = C[i];
        PastGenericD[i] = D[i];
        PastGenericE[i] = E[i];
    }

    if (NumberOfCriticalCells > 0)
        AdvectDiffuseCriticalCells(A, PastGenericA, ALoad, flowCriticalCellsA,
                                   B, PastGenericB, BLoad, flowCriticalCellsB,
                                   C, PastGenericC, CLoad, flowCriticalCellsC,
                                   D, PastGenericD, DLoad, flowCriticalCellsD,
                                   E, PastGenericE, ELoad, flowCriticalCellsE);

         for (m = 0; m < MyNumberOfBoxes; m++)
         {
            i = MyPEcoDynClass->GetLineIndex(MyBoxNumber[m]);
            j = MyPEcoDynClass->GetColumnIndex(MyBoxNumber[m]);
            if (
                  (i >= pSubDomain->ILine[j])
                  && (i <= pSubDomain->FLine[j] /*- 1*/)
                  && (j >= pSubDomain->IColumn[i])
                  && (j <= pSubDomain->FColumn[i] /*- 1*/)
                )
            {
            index = i * GridColumns + j;

            if (
                    //(BoxArray[index].Dummybool2) &&
                    (BoxDepth[index] > CriticalDepthForLandBoundary) &&
                    (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 2) &&
                    (BoxArray[index].Dummybool1) &&
                    (!ACellRecord[index].Critical)
                )
            {

                // AP, 2005.05.19 ...
                H1 = BoxDepth[index] + DynamicHeight[index] - PastHeight[index];
                Q1 = ASpatialStep * ASpatialStep * H1;

         	    //Advective fluxes and Diffusive fluxes

                // AP, 2005.05.19 ...
//                PastGeneric = PastGenericA;
               if ((j > 0) && (ACellRecord[index - 1].Critical)) {
                  MyWestUFlow = flowCriticalCellsA[index - 1].EastAdvecFlow;
                  MyWestDiffusion = - flowCriticalCellsA[index - 1].EastDiffFlow;
               }
               else {
                  MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericA);
                  MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericA);
               }

               if ((j < GridColumns - 1) && (ACellRecord[index + 1].Critical)) {
                  MyEastUFlow = flowCriticalCellsA[index + 1].WestAdvecFlow;
                  MyEastDiffusion = - flowCriticalCellsA[index + 1].WestDiffFlow;
               }
               else {
                  MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericA);
                  MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericA);
               }

               if ((i < GridLines) && (ACellRecord[(i + 1) * GridColumns + j].Critical)) {
                  MyNorthVFlow = flowCriticalCellsA[(i + 1) * GridColumns + j].SouthAdvecFlow;
                  MyNorthDiffusion = - flowCriticalCellsA[(i + 1) * GridColumns + j].SouthDiffFlow;
               }
               else {
                  MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericA);
                  MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericA);
               }

               if ((i > 0) && (ACellRecord[(i - 1) * GridColumns + j].Critical)) {
                  MySouthVFlow = flowCriticalCellsA[(i - 1) * GridColumns + j].NorthAdvecFlow;
                  MyNorthDiffusion = - flowCriticalCellsA[(i + 1) * GridColumns + j].SouthDiffFlow;
               }
               else {
                  MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericA);
                  MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericA);
               }

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                             MyNorthDiffusion, MySouthDiffusion, PastGenericA[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion + MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;

                A[index] = PastGenericA[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + (ResultantDiffusion) / (2.0 * Q1);
                if ((A[index] < 0.0) && (A[index] > -0.000001))
                    A[index] = 0.0;

//                PastGeneric = PastGenericB;
               if ((j > 0) && (ACellRecord[index - 1].Critical)) {
                  MyWestUFlow = flowCriticalCellsB[index - 1].EastAdvecFlow;
                  MyWestDiffusion = - flowCriticalCellsB[index - 1].EastDiffFlow;
               }
               else {
                  MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericB);
                  MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericB);
               }

               if ((j < GridColumns - 1) && (ACellRecord[index + 1].Critical)) {
                  MyEastUFlow = flowCriticalCellsB[index + 1].WestAdvecFlow;
                  MyEastDiffusion = - flowCriticalCellsB[index + 1].WestDiffFlow;
               }
               else {
                  MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericB);
                  MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericB);
               }

               if ((i < GridLines) && (ACellRecord[(i + 1) * GridColumns + j].Critical)) {
                  MyNorthVFlow = flowCriticalCellsB[(i + 1) * GridColumns + j].SouthAdvecFlow;
                  MyNorthDiffusion = - flowCriticalCellsB[(i + 1) * GridColumns + j].SouthDiffFlow;
               }
               else {
                  MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericB);
                  MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericB);
               }

               if ((i > 0) && (ACellRecord[(i - 1) * GridColumns + j].Critical)) {
                  MySouthVFlow = flowCriticalCellsB[(i - 1) * GridColumns + j].NorthAdvecFlow;
                  MyNorthDiffusion = - flowCriticalCellsB[(i + 1) * GridColumns + j].SouthDiffFlow;
               }
               else {
                  MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericB);
                  MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericB);
               }

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                             MyNorthDiffusion, MySouthDiffusion, PastGenericB[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion + MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;

                B[index] = PastGenericB[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + (ResultantDiffusion) / (2.0 * Q1);
                if ((B[index] < 0.0) && (B[index] > -0.000001))
                    B[index] = 0.0;

//                PastGeneric = PastGenericC;
               if ((j > 0) && (ACellRecord[index - 1].Critical)) {
                  MyWestUFlow = flowCriticalCellsC[index - 1].EastAdvecFlow;
                  MyWestDiffusion = - flowCriticalCellsC[index - 1].EastDiffFlow;
               }
               else {
                  MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericC);
                  MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericC);
               }

               if ((j < GridColumns - 1) && (ACellRecord[index + 1].Critical)) {
                  MyEastUFlow = flowCriticalCellsC[index + 1].WestAdvecFlow;
                  MyEastDiffusion = - flowCriticalCellsC[index + 1].WestDiffFlow;
               }
               else {
                  MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericC);
                  MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericC);
               }

               if ((i < GridLines) && (ACellRecord[(i + 1) * GridColumns + j].Critical)) {
                  MyNorthVFlow = flowCriticalCellsC[(i + 1) * GridColumns + j].SouthAdvecFlow;
                  MyNorthDiffusion = - flowCriticalCellsC[(i + 1) * GridColumns + j].SouthDiffFlow;
               }
               else {
                  MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericC);
                  MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericC);
               }

               if ((i > 0) && (ACellRecord[(i - 1) * GridColumns + j].Critical)) {
                  MySouthVFlow = flowCriticalCellsC[(i - 1) * GridColumns + j].NorthAdvecFlow;
                  MyNorthDiffusion = - flowCriticalCellsC[(i + 1) * GridColumns + j].SouthDiffFlow;
               }
               else {
                  MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericC);
                  MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericC);
               }

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                             MyNorthDiffusion, MySouthDiffusion, PastGenericC[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion + MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;
                        
                C[index] = PastGenericC[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + (ResultantDiffusion) / (2.0 * Q1);
                if ((C[index] < 0.0) && (C[index] > -0.000001))
                    C[index] = 0.0;

//                PastGeneric = PastGenericD;
               if ((j > 0) && (ACellRecord[index - 1].Critical)) {
                  MyWestUFlow = flowCriticalCellsD[index - 1].EastAdvecFlow;
                  MyWestDiffusion = - flowCriticalCellsD[index - 1].EastDiffFlow;
               }
               else {
                  MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericD);
                  MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericD);
               }

               if ((j < GridColumns - 1) && (ACellRecord[index + 1].Critical)) {
                  MyEastUFlow = flowCriticalCellsD[index + 1].WestAdvecFlow;
                  MyEastDiffusion = - flowCriticalCellsD[index + 1].WestDiffFlow;
               }
               else {
                  MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericD);
                  MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericD);
               }

               if ((i < GridLines) && (ACellRecord[(i + 1) * GridColumns + j].Critical)) {
                  MyNorthVFlow = flowCriticalCellsD[(i + 1) * GridColumns + j].SouthAdvecFlow;
                  MyNorthDiffusion = - flowCriticalCellsD[(i + 1) * GridColumns + j].SouthDiffFlow;
               }
               else {
                  MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericD);
                  MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericD);
               }

               if ((i > 0) && (ACellRecord[(i - 1) * GridColumns + j].Critical)) {
                  MySouthVFlow = flowCriticalCellsD[(i - 1) * GridColumns + j].NorthAdvecFlow;
                  MyNorthDiffusion = - flowCriticalCellsD[(i + 1) * GridColumns + j].SouthDiffFlow;
               }
               else {
                  MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericD);
                  MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericD);
               }

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                             MyNorthDiffusion, MySouthDiffusion, PastGenericD[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion + MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;

                D[index] = PastGenericD[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + (ResultantDiffusion) / (2.0 * Q1);
                if ((D[index] < 0.0) && (D[index] > -0.000001))
                    D[index] = 0.0;

//                PastGeneric = PastGenericE;
               if ((j > 0) && (ACellRecord[index - 1].Critical)) {
                  MyWestUFlow = flowCriticalCellsE[index - 1].EastAdvecFlow;
                  MyWestDiffusion = - flowCriticalCellsE[index - 1].EastDiffFlow;
               }
               else {
                  MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericE);
                  MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericE);
               }

               if ((j < GridColumns - 1) && (ACellRecord[index + 1].Critical)) {
                  MyEastUFlow = flowCriticalCellsE[index + 1].WestAdvecFlow;
                  MyEastDiffusion = - flowCriticalCellsE[index + 1].WestDiffFlow;
               }
               else {
                  MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericE);
                  MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericE);
               }

               if ((i < GridLines) && (ACellRecord[(i + 1) * GridColumns + j].Critical)) {
                  MyNorthVFlow = flowCriticalCellsE[(i + 1) * GridColumns + j].SouthAdvecFlow;
                  MyNorthDiffusion = - flowCriticalCellsE[(i + 1) * GridColumns + j].SouthDiffFlow;
               }
               else {
                  MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericE);
                  MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericE);
               }

               if ((i > 0) && (ACellRecord[(i - 1) * GridColumns + j].Critical)) {
                  MySouthVFlow = flowCriticalCellsE[(i - 1) * GridColumns + j].NorthAdvecFlow;
                  MyNorthDiffusion = - flowCriticalCellsE[(i + 1) * GridColumns + j].SouthDiffFlow;
               }
               else {
                  MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericE);
                  MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericE);
               }

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                             MyNorthDiffusion, MySouthDiffusion, PastGenericE[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion + MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;

                E[index] = PastGenericE[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + (ResultantDiffusion) / (2.0 * Q1);
                if ((E[index] < 0.0) && (E[index] > -0.000001))
                    E[index] = 0.0;


                if (MyLoadRecord != NULL)
                {
                    for (k = 0; k < MyPEcoDynClass->GetNumberOfLoads(); k++)
                    {
                        if (
                          (i == MyLoadRecord[k].LineCoordinate) &&
                          (j == MyLoadRecord[k].ColumnCoordinate)
                        )
                        {
                            ADay = MIN(MyPEcoDynClass->GetJulianDay(),
                                       MyPEcoDynClass->GetNumberOfDaysForLoads());

                            // AP, 2005.05.19 ...
                            A[index] = A[index] + ACellRecord[index].CellTimeStep * ALoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            B[index] = B[index] + ACellRecord[index].CellTimeStep * BLoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            C[index] = C[index] + ACellRecord[index].CellTimeStep * CLoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            D[index] = D[index] + ACellRecord[index].CellTimeStep * DLoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            E[index] = E[index] + ACellRecord[index].CellTimeStep * ELoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
/* AP, 2005.05.19 ...
                            Generic[index ] = Generic[index ] + ATimeStep * GenericLoad[index] *
                                              MyLoadRecord[k].LoadFlow[ADay - 1] /
                                              (
                                               (BoxDepth[index] + DynamicHeight[index] - PastHeight[index]) *
                                               (ASpatialStep * ASpatialStep)
                                              );
 */
                            break;
                        }
                    }
                }
      	    }
            }
         }
//      }  //j
//   }  //i
    delete [] flowCriticalCellsA;
    delete [] flowCriticalCellsB;
    delete [] flowCriticalCellsC;
    delete [] flowCriticalCellsD;
    delete [] flowCriticalCellsE;

    delete [] PastGenericA;
    delete [] PastGenericB;
    delete [] PastGenericC;
    delete [] PastGenericD;
    delete [] PastGenericE;
}


void TBiDimensionalTimeSeriesVariableDT::AdvectDiffuseCriticalCells(
        double* GenericA, double* PastGenericA, double *ALoad, FlowCriticalCells *flowCCA,
        double* GenericB, double* PastGenericB, double *BLoad, FlowCriticalCells *flowCCB,
        double* GenericC, double* PastGenericC, double *CLoad, FlowCriticalCells *flowCCC,
        double* GenericD, double* PastGenericD, double *DLoad, FlowCriticalCells *flowCCD,
        double* GenericE, double* PastGenericE, double *ELoad, FlowCriticalCells *flowCCE,
        double* GenericF, double* PastGenericF, double *FLoad, FlowCriticalCells *flowCCF,
        double* GenericG, double* PastGenericG, double *GLoad, FlowCriticalCells *flowCCG,
        double* GenericH, double* PastGenericH, double *HLoad, FlowCriticalCells *flowCCH)
{
    double MyWestUFlowA, MyEastUFlowA, MyNorthVFlowA, MySouthVFlowA,
           MyWestDiffusionA, MyEastDiffusionA, MyNorthDiffusionA, MySouthDiffusionA,
           MyWestUFlowB, MyEastUFlowB, MyNorthVFlowB, MySouthVFlowB,
           MyWestDiffusionB, MyEastDiffusionB, MyNorthDiffusionB, MySouthDiffusionB,
           MyWestUFlowC, MyEastUFlowC, MyNorthVFlowC, MySouthVFlowC,
           MyWestDiffusionC, MyEastDiffusionC, MyNorthDiffusionC, MySouthDiffusionC,
           MyWestUFlowD, MyEastUFlowD, MyNorthVFlowD, MySouthVFlowD,
           MyWestDiffusionD, MyEastDiffusionD, MyNorthDiffusionD, MySouthDiffusionD,
           MyWestUFlowE, MyEastUFlowE, MyNorthVFlowE, MySouthVFlowE,
           MyWestDiffusionE, MyEastDiffusionE, MyNorthDiffusionE, MySouthDiffusionE,
           MyWestUFlowF, MyEastUFlowF, MyNorthVFlowF, MySouthVFlowF,
           MyWestDiffusionF, MyEastDiffusionF, MyNorthDiffusionF, MySouthDiffusionF,
           MyWestUFlowG, MyEastUFlowG, MyNorthVFlowG, MySouthVFlowG,
           MyWestDiffusionG, MyEastDiffusionG, MyNorthDiffusionG, MySouthDiffusionG,
           MyWestUFlowH, MyEastUFlowH, MyNorthVFlowH, MySouthVFlowH,
           MyWestDiffusionH, MyEastDiffusionH, MyNorthDiffusionH, MySouthDiffusionH,
           MyPastWaterLevel, MyPresentWaterLevel,MyBoxDepth;
    MyWestDiffusionA = 0.0; MyEastDiffusionA = 0.0; MyNorthDiffusionA = 0.0; MySouthDiffusionA = 0.0;
    MyWestDiffusionB = 0.0; MyEastDiffusionB = 0.0; MyNorthDiffusionB = 0.0; MySouthDiffusionB = 0.0;
    MyWestDiffusionC = 0.0; MyEastDiffusionC = 0.0; MyNorthDiffusionC = 0.0; MySouthDiffusionC = 0.0;
    MyWestDiffusionD = 0.0; MyEastDiffusionD = 0.0; MyNorthDiffusionD = 0.0; MySouthDiffusionD = 0.0;
    MyWestDiffusionE = 0.0; MyEastDiffusionE = 0.0; MyNorthDiffusionE = 0.0; MySouthDiffusionE = 0.0;
    MyWestDiffusionF = 0.0; MyEastDiffusionF = 0.0; MyNorthDiffusionF = 0.0; MySouthDiffusionF = 0.0;
    MyWestDiffusionG = 0.0; MyEastDiffusionG = 0.0; MyNorthDiffusionG = 0.0; MySouthDiffusionG = 0.0;
    MyWestDiffusionH = 0.0; MyEastDiffusionH = 0.0; MyNorthDiffusionH = 0.0; MySouthDiffusionH = 0.0;
    //As difusões são inicializadas a zero e, por enquanto, mantidas a zero (donde o código comentado abaixo),
    //pois decidi não as calcular nas células críticas,
    //para evitar problemas numéricos que surgiam nalgumas células
    int i,j,index, k, ADay, m;
    double *TemporaryBoxDepth, *TemporaryPastHeight;
    TemporaryBoxDepth = new double[NumberOfBoxes]; TemporaryPastHeight = new double[NumberOfBoxes];
    //for (m = 0; m < NumberOfCriticalCells; m++)
    for (int MyStep = 0; MyStep < CriticalNumberOfSteps; MyStep++)
    {
        //for (int MyStep = 0; MyStep < ACellRecord[index].NumberOfSteps; MyStep++)
        for (m = 0; m < NumberOfCriticalCells; m++)
        {
            index = Cells[m];
            i = MyPEcoDynClass->GetLineIndex(index);
            j = MyPEcoDynClass->GetColumnIndex(index);
            if (MyStep == 0)
            {
               TemporaryBoxDepth[index] = BoxDepth[index];
               TemporaryPastHeight[index] = PastHeight[index];
            }
            MyBoxDepth = TemporaryBoxDepth[index];
            MyPastWaterLevel = TemporaryPastHeight[index];

            //MyPastWaterLevel = PastHeight[index];
            //MyBoxDepth = BoxDepth[index];

            //Advective fluxes

            MyWestUFlowA = WesternComponentOfAdvectionDiffusion(i,j,PastGenericA) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyEastUFlowA = EasternComponentOfAdvectionDiffusion(i,j,PastGenericA) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyNorthVFlowA = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericA) * ACellRecord[index].CellTimeStep / ATimeStep;
            MySouthVFlowA = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericA) * ACellRecord[index].CellTimeStep / ATimeStep;

            MyWestUFlowB = WesternComponentOfAdvectionDiffusion(i,j,PastGenericB) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyEastUFlowB = EasternComponentOfAdvectionDiffusion(i,j,PastGenericB) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyNorthVFlowB = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericB) * ACellRecord[index].CellTimeStep / ATimeStep;
            MySouthVFlowB = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericB) * ACellRecord[index].CellTimeStep / ATimeStep;

            MyWestUFlowC = WesternComponentOfAdvectionDiffusion(i,j,PastGenericC) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyEastUFlowC = EasternComponentOfAdvectionDiffusion(i,j,PastGenericC) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyNorthVFlowC = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericC) * ACellRecord[index].CellTimeStep / ATimeStep;
            MySouthVFlowC = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericC) * ACellRecord[index].CellTimeStep / ATimeStep;

            MyWestUFlowD = WesternComponentOfAdvectionDiffusion(i,j,PastGenericD) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyEastUFlowD = EasternComponentOfAdvectionDiffusion(i,j,PastGenericD) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyNorthVFlowD = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericD) * ACellRecord[index].CellTimeStep / ATimeStep;
            MySouthVFlowD = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericD) * ACellRecord[index].CellTimeStep / ATimeStep;

            MyWestUFlowE = WesternComponentOfAdvectionDiffusion(i,j,PastGenericE) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyEastUFlowE = EasternComponentOfAdvectionDiffusion(i,j,PastGenericE) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyNorthVFlowE = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericE) * ACellRecord[index].CellTimeStep / ATimeStep;
            MySouthVFlowE = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericE) * ACellRecord[index].CellTimeStep / ATimeStep;

            MyWestUFlowF = WesternComponentOfAdvectionDiffusion(i,j,PastGenericF) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyEastUFlowF = EasternComponentOfAdvectionDiffusion(i,j,PastGenericF) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyNorthVFlowF = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericF) * ACellRecord[index].CellTimeStep / ATimeStep;
            MySouthVFlowF = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericF) * ACellRecord[index].CellTimeStep / ATimeStep;

            MyWestUFlowG = WesternComponentOfAdvectionDiffusion(i,j,PastGenericG) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyEastUFlowG = EasternComponentOfAdvectionDiffusion(i,j,PastGenericG) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyNorthVFlowG = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericG) * ACellRecord[index].CellTimeStep / ATimeStep;
            MySouthVFlowG = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericG) * ACellRecord[index].CellTimeStep / ATimeStep;

            MyWestUFlowH = WesternComponentOfAdvectionDiffusion(i,j,PastGenericH) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyEastUFlowH = EasternComponentOfAdvectionDiffusion(i,j,PastGenericH) * ACellRecord[index].CellTimeStep / ATimeStep;
            MyNorthVFlowH = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericH) * ACellRecord[index].CellTimeStep / ATimeStep;
            MySouthVFlowH = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericH) * ACellRecord[index].CellTimeStep / ATimeStep;

            //Diffusive fluxes

            flowCCA[index].NorthAdvecFlow += MyNorthVFlowA;
            flowCCA[index].SouthAdvecFlow += MySouthVFlowA;
            flowCCA[index].WestAdvecFlow += MyWestUFlowA;
            flowCCA[index].EastAdvecFlow += MyEastUFlowA;

            flowCCA[index].NorthDiffFlow += MyNorthDiffusionA;
            flowCCA[index].SouthDiffFlow += MySouthDiffusionA;
            flowCCA[index].WestDiffFlow += MyWestDiffusionA;
            flowCCA[index].EastDiffFlow += MyEastDiffusionA;

            flowCCB[index].NorthAdvecFlow += MyNorthVFlowB;
            flowCCB[index].SouthAdvecFlow += MySouthVFlowB;
            flowCCB[index].WestAdvecFlow += MyWestUFlowB;
            flowCCB[index].EastAdvecFlow += MyEastUFlowB;

            flowCCB[index].NorthDiffFlow += MyNorthDiffusionB;
            flowCCB[index].SouthDiffFlow += MySouthDiffusionB;
            flowCCB[index].WestDiffFlow += MyWestDiffusionB;
            flowCCB[index].EastDiffFlow += MyEastDiffusionB;

            flowCCC[index].NorthAdvecFlow += MyNorthVFlowC;
            flowCCC[index].SouthAdvecFlow += MySouthVFlowC;
            flowCCC[index].WestAdvecFlow += MyWestUFlowC;
            flowCCC[index].EastAdvecFlow += MyEastUFlowC;

            flowCCC[index].NorthDiffFlow += MyNorthDiffusionC;
            flowCCC[index].SouthDiffFlow += MySouthDiffusionC;
            flowCCC[index].WestDiffFlow += MyWestDiffusionC;
            flowCCC[index].EastDiffFlow += MyEastDiffusionC;

            flowCCD[index].NorthAdvecFlow += MyNorthVFlowD;
            flowCCD[index].SouthAdvecFlow += MySouthVFlowD;
            flowCCD[index].WestAdvecFlow += MyWestUFlowD;
            flowCCD[index].EastAdvecFlow += MyEastUFlowD;

            flowCCD[index].NorthDiffFlow += MyNorthDiffusionD;
            flowCCD[index].SouthDiffFlow += MySouthDiffusionD;
            flowCCD[index].WestDiffFlow += MyWestDiffusionD;
            flowCCD[index].EastDiffFlow += MyEastDiffusionD;

            flowCCE[index].NorthAdvecFlow += MyNorthVFlowE;
            flowCCE[index].SouthAdvecFlow += MySouthVFlowE;
            flowCCE[index].WestAdvecFlow += MyWestUFlowE;
            flowCCE[index].EastAdvecFlow += MyEastUFlowE;

            flowCCE[index].NorthDiffFlow += MyNorthDiffusionE;
            flowCCE[index].SouthDiffFlow += MySouthDiffusionE;
            flowCCE[index].WestDiffFlow += MyWestDiffusionE;
            flowCCE[index].EastDiffFlow += MyEastDiffusionE;

            flowCCF[index].NorthAdvecFlow += MyNorthVFlowF;
            flowCCF[index].SouthAdvecFlow += MySouthVFlowF;
            flowCCF[index].WestAdvecFlow += MyWestUFlowF;
            flowCCF[index].EastAdvecFlow += MyEastUFlowF;

            flowCCF[index].NorthDiffFlow += MyNorthDiffusionF;
            flowCCF[index].SouthDiffFlow += MySouthDiffusionF;
            flowCCF[index].WestDiffFlow += MyWestDiffusionF;
            flowCCF[index].EastDiffFlow += MyEastDiffusionF;

            flowCCG[index].NorthAdvecFlow += MyNorthVFlowG;
            flowCCG[index].SouthAdvecFlow += MySouthVFlowG;
            flowCCG[index].WestAdvecFlow += MyWestUFlowG;
            flowCCG[index].EastAdvecFlow += MyEastUFlowG;

            flowCCG[index].NorthDiffFlow += MyNorthDiffusionG;
            flowCCG[index].SouthDiffFlow += MySouthDiffusionG;
            flowCCG[index].WestDiffFlow += MyWestDiffusionG;
            flowCCG[index].EastDiffFlow += MyEastDiffusionG;

            flowCCH[index].NorthAdvecFlow += MyNorthVFlowH;
            flowCCH[index].SouthAdvecFlow += MySouthVFlowH;
            flowCCH[index].WestAdvecFlow += MyWestUFlowH;
            flowCCH[index].EastAdvecFlow += MyEastUFlowH;

            flowCCH[index].NorthDiffFlow += MyNorthDiffusionH;
            flowCCH[index].SouthDiffFlow += MySouthDiffusionH;
            flowCCH[index].WestDiffFlow += MyWestDiffusionH;
            flowCCH[index].EastDiffFlow += MyEastDiffusionH;

            MyPresentWaterLevel = MyPastWaterLevel + (
                                                        ACellRecord[index].WestFlow -
                                                        ACellRecord[index].EastFlow -
                                                        ACellRecord[index].NorthFlow +
                                                        ACellRecord[index].SouthFlow
                                                      ) / (ASpatialStep * ASpatialStep) *
                                                      ACellRecord[index].CellTimeStep / ATimeStep;

            if (MyLoadRecord != NULL)
            {
               for (k = 0; k < MyPEcoDynClass->GetNumberOfLoads(); k++)
               {
                  if (
                      (i == MyLoadRecord[k].LineCoordinate) &&
                      (j == MyLoadRecord[k].ColumnCoordinate)
                     )
                  {
                     ADay = MIN(MyPEcoDynClass->GetJulianDay(),MyPEcoDynClass->GetNumberOfDaysForLoads());
                     MyPresentWaterLevel = MyPresentWaterLevel + ACellRecord[index].CellTimeStep *
                                                     MyLoadRecord[k].LoadFlow[ADay - 1] /
                                                     (
                                                        (ASpatialStep * ASpatialStep)
                                                     );
                      break;
                  }
               }
            }

            if (MyBoxDepth > CriticalDepthForLandBoundary)
            {
                GenericA[index ] = PastGenericA[index]  * MyBoxDepth /
                                  (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel)
                                  + (
                                        MyWestUFlowA -
                                        MyEastUFlowA -
                                        MyNorthVFlowA +
                                        MySouthVFlowA
                                    ) / (ASpatialStep * ASpatialStep *
                                    (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel))
                                    +(
                                        MyWestDiffusionA +
                                        MyEastDiffusionA +
                                        MyNorthDiffusionA +
                                        MySouthDiffusionA
                                     ) / (2.0 * ASpatialStep * ASpatialStep *
                                     (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel));

                GenericB[index ] = PastGenericB[index]  * MyBoxDepth /
                                  (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel)
                                  + (
                                        MyWestUFlowB -
                                        MyEastUFlowB -
                                        MyNorthVFlowB +
                                        MySouthVFlowB
                                    ) / (ASpatialStep * ASpatialStep *
                                    (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel))
                                    +(
                                        MyWestDiffusionB +
                                        MyEastDiffusionB +
                                        MyNorthDiffusionB +
                                        MySouthDiffusionB
                                     ) / (2.0 * ASpatialStep * ASpatialStep *
                                     (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel));

                GenericC[index ] = PastGenericC[index]  * MyBoxDepth /
                                  (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel)
                                  + (
                                        MyWestUFlowC -
                                        MyEastUFlowC -
                                        MyNorthVFlowC +
                                        MySouthVFlowC
                                    ) / (ASpatialStep * ASpatialStep *
                                    (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel))
                                    +(
                                        MyWestDiffusionC +
                                        MyEastDiffusionC +
                                        MyNorthDiffusionC +
                                        MySouthDiffusionC
                                     ) / (2.0 * ASpatialStep * ASpatialStep *
                                     (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel));

                 GenericD[index ] = PastGenericD[index]  * MyBoxDepth /
                                  (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel)
                                  + (
                                        MyWestUFlowD -
                                        MyEastUFlowD -
                                        MyNorthVFlowD +
                                        MySouthVFlowD
                                    ) / (ASpatialStep * ASpatialStep *
                                    (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel))
                                    +(
                                        MyWestDiffusionD +
                                        MyEastDiffusionD +
                                        MyNorthDiffusionD +
                                        MySouthDiffusionD
                                     ) / (2.0 * ASpatialStep * ASpatialStep *
                                     (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel));

                GenericE[index ] = PastGenericE[index]  * MyBoxDepth /
                                  (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel)
                                  + (
                                        MyWestUFlowE -
                                        MyEastUFlowE -
                                        MyNorthVFlowE +
                                        MySouthVFlowE
                                    ) / (ASpatialStep * ASpatialStep *
                                    (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel))
                                    +(
                                        MyWestDiffusionE +
                                        MyEastDiffusionE +
                                        MyNorthDiffusionE +
                                        MySouthDiffusionE
                                     ) / (2.0 * ASpatialStep * ASpatialStep *
                                     (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel));

                GenericF[index ] = PastGenericF[index]  * MyBoxDepth /
                                  (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel)
                                  + (
                                        MyWestUFlowF -
                                        MyEastUFlowF -
                                        MyNorthVFlowF +
                                        MySouthVFlowF
                                    ) / (ASpatialStep * ASpatialStep *
                                    (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel))
                                    +(
                                        MyWestDiffusionF +
                                        MyEastDiffusionF +
                                        MyNorthDiffusionF +
                                        MySouthDiffusionF
                                     ) / (2.0 * ASpatialStep * ASpatialStep *
                                     (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel));

                GenericG[index ] = PastGenericG[index]  * MyBoxDepth /
                                  (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel)
                                  + (
                                        MyWestUFlowG -
                                        MyEastUFlowG -
                                        MyNorthVFlowG +
                                        MySouthVFlowG
                                    ) / (ASpatialStep * ASpatialStep *
                                    (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel))
                                    +(
                                        MyWestDiffusionG +
                                        MyEastDiffusionG +
                                        MyNorthDiffusionG +
                                        MySouthDiffusionG
                                     ) / (2.0 * ASpatialStep * ASpatialStep *
                                     (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel));

                GenericH[index ] = PastGenericH[index]  * MyBoxDepth /
                                  (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel)
                                  + (
                                        MyWestUFlowH -
                                        MyEastUFlowH -
                                        MyNorthVFlowH +
                                        MySouthVFlowH
                                    ) / (ASpatialStep * ASpatialStep *
                                    (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel))
                                    +(
                                        MyWestDiffusionH +
                                        MyEastDiffusionH +
                                        MyNorthDiffusionH +
                                        MySouthDiffusionH
                                     ) / (2.0 * ASpatialStep * ASpatialStep *
                                     (MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel));

                MyBoxDepth = MyBoxDepth + MyPresentWaterLevel - MyPastWaterLevel;

                MyPastWaterLevel = MyPresentWaterLevel;
                if (MyLoadRecord != NULL)
                {
                    for (k = 0; k < MyPEcoDynClass->GetNumberOfLoads(); k++)
                    {
                        if (
                                (i == MyLoadRecord[k].LineCoordinate) &&
                                (j == MyLoadRecord[k].ColumnCoordinate)
                           )
                        {
                            ADay = MIN(MyPEcoDynClass->GetJulianDay(),
                            MyPEcoDynClass->GetNumberOfDaysForLoads());

                            GenericA[index ] = GenericA[index ] + ACellRecord[index].CellTimeStep * ALoad[index] *
                                    MyLoadRecord[k].LoadFlow[ADay - 1] /
                            (
                                    MyBoxDepth *
                                    ASpatialStep * ASpatialStep
                            );

                            GenericB[index ] = GenericB[index ] + ACellRecord[index].CellTimeStep * BLoad[index] *
                                    MyLoadRecord[k].LoadFlow[ADay - 1] /
                            (
                                    MyBoxDepth *
                                    ASpatialStep * ASpatialStep
                            );

                            GenericC[index ] = GenericC[index ] + ACellRecord[index].CellTimeStep * CLoad[index] *
                                    MyLoadRecord[k].LoadFlow[ADay - 1] /
                            (
                                    MyBoxDepth *
                                    ASpatialStep * ASpatialStep
                            );

                            GenericD[index ] = GenericD[index ] + ACellRecord[index].CellTimeStep * DLoad[index] *
                                    MyLoadRecord[k].LoadFlow[ADay - 1] /
                            (
                                    MyBoxDepth *
                                    ASpatialStep * ASpatialStep
                            );

                            GenericE[index ] = GenericE[index ] + ACellRecord[index].CellTimeStep * ELoad[index] *
                                    MyLoadRecord[k].LoadFlow[ADay - 1] /
                            (
                                    MyBoxDepth *
                                    ASpatialStep * ASpatialStep
                            );

                            GenericF[index ] = GenericF[index ] + ACellRecord[index].CellTimeStep * FLoad[index] *
                                    MyLoadRecord[k].LoadFlow[ADay - 1] /
                            (
                                    MyBoxDepth *
                                    ASpatialStep * ASpatialStep
                            );

                            GenericG[index ] = GenericG[index ] + ACellRecord[index].CellTimeStep * GLoad[index] *
                                    MyLoadRecord[k].LoadFlow[ADay - 1] /
                            (
                                    MyBoxDepth *
                                    ASpatialStep * ASpatialStep
                            );

                            GenericH[index ] = GenericH[index ] + ACellRecord[index].CellTimeStep * HLoad[index] *
                                    MyLoadRecord[k].LoadFlow[ADay - 1] /
                            (
                                    MyBoxDepth *
                                    ASpatialStep * ASpatialStep
                            );
                            break;
                        }
                    }
                }
            }
            TemporaryPastHeight[index] = MyPresentWaterLevel;
            TemporaryBoxDepth[index] = MyBoxDepth;
            //PastHeight[index] = MyPresentWaterLevel;
            //BoxDepth[index] = MyBoxDepth;
            PastGenericA[index] = GenericA[index];
            PastGenericB[index] = GenericB[index];
            PastGenericC[index] = GenericC[index];
            PastGenericD[index] = GenericD[index];
            PastGenericE[index] = GenericE[index];
            PastGenericF[index] = GenericF[index];
            PastGenericG[index] = GenericG[index];
            PastGenericH[index] = GenericH[index];
            CriticalCellDepth[index] = TemporaryBoxDepth[index];
            CriticalCellHeight[index] = TemporaryPastHeight[index];
      	}
    }
    delete [] TemporaryBoxDepth;
    delete [] TemporaryPastHeight;
}

void TBiDimensionalTimeSeriesVariableDT::AdvectDiffuse(double *A, double *ALoad,
                                             double *B, double *BLoad,
                                             double *C, double *CLoad,
                                             double *D, double *DLoad,
                                             double *E, double *ELoad,
                                             double *F, double *FLoad,
                                             double *G, double *GLoad,
                                             double *H, double *HLoad)
{
    double MyWestUFlow, MyEastUFlow, MyNorthVFlow, MySouthVFlow,
    MyWestDiffusion, MyEastDiffusion, MyNorthDiffusion, MySouthDiffusion, ResultantDiffusion;
    double H1, Q1;
    int i,j,index, k, ADay, m;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    double *PastGenericA, *PastGenericB, *PastGenericC, *PastGenericD,
           *PastGenericE, *PastGenericF, *PastGenericG, *PastGenericH;

    FlowCriticalCells *flowCriticalCellsA, *flowCriticalCellsB,
            *flowCriticalCellsC, *flowCriticalCellsD, *flowCriticalCellsE,
            *flowCriticalCellsF, *flowCriticalCellsG, *flowCriticalCellsH;

    flowCriticalCellsA = new FlowCriticalCells[NumberOfBoxes];
    flowCriticalCellsB = new FlowCriticalCells[NumberOfBoxes];
    flowCriticalCellsC = new FlowCriticalCells[NumberOfBoxes];
    flowCriticalCellsD = new FlowCriticalCells[NumberOfBoxes];
    flowCriticalCellsE = new FlowCriticalCells[NumberOfBoxes];
    flowCriticalCellsF = new FlowCriticalCells[NumberOfBoxes];
    flowCriticalCellsG = new FlowCriticalCells[NumberOfBoxes];
    flowCriticalCellsH = new FlowCriticalCells[NumberOfBoxes];
    memset(flowCriticalCellsA, 0, sizeof(FlowCriticalCells) * NumberOfBoxes);
    memset(flowCriticalCellsB, 0, sizeof(FlowCriticalCells) * NumberOfBoxes);
    memset(flowCriticalCellsC, 0, sizeof(FlowCriticalCells) * NumberOfBoxes);
    memset(flowCriticalCellsD, 0, sizeof(FlowCriticalCells) * NumberOfBoxes);
    memset(flowCriticalCellsE, 0, sizeof(FlowCriticalCells) * NumberOfBoxes);
    memset(flowCriticalCellsF, 0, sizeof(FlowCriticalCells) * NumberOfBoxes);
    memset(flowCriticalCellsG, 0, sizeof(FlowCriticalCells) * NumberOfBoxes);
    memset(flowCriticalCellsH, 0, sizeof(FlowCriticalCells) * NumberOfBoxes);

    PastGenericA = new double[NumberOfBoxes];
    PastGenericB = new double[NumberOfBoxes];
    PastGenericC = new double[NumberOfBoxes];
    PastGenericD = new double[NumberOfBoxes];
    PastGenericE = new double[NumberOfBoxes];
    PastGenericF = new double[NumberOfBoxes];
    PastGenericG = new double[NumberOfBoxes];
    PastGenericH = new double[NumberOfBoxes];

    for (i = 0; i < NumberOfBoxes; i++)
    {
        PastGenericA[i] = A[i];
        PastGenericB[i] = B[i];
        PastGenericC[i] = C[i];
        PastGenericD[i] = D[i];
        PastGenericE[i] = E[i];
        PastGenericF[i] = F[i];
        PastGenericG[i] = G[i];
        PastGenericH[i] = H[i];
    }

    if (NumberOfCriticalCells > 0)
        AdvectDiffuseCriticalCells(A, PastGenericA, ALoad, flowCriticalCellsA,
               B, PastGenericB, BLoad, flowCriticalCellsB,
               C, PastGenericC, CLoad, flowCriticalCellsC,
               D, PastGenericD, DLoad, flowCriticalCellsD,
               E, PastGenericE, ELoad, flowCriticalCellsE,
               F, PastGenericF, FLoad, flowCriticalCellsF,
               G, PastGenericG, GLoad, flowCriticalCellsG,
               H, PastGenericH, HLoad, flowCriticalCellsH);

     for (m = 0; m < MyNumberOfBoxes; m++)
     {
        i = MyPEcoDynClass->GetLineIndex(MyBoxNumber[m]);
        j = MyPEcoDynClass->GetColumnIndex(MyBoxNumber[m]);
        if (
              (i >= pSubDomain->ILine[j])
              && (i <= pSubDomain->FLine[j] /*- 1*/)
              && (j >= pSubDomain->IColumn[i])
              && (j <= pSubDomain->FColumn[i] /*- 1*/)
            )
        {
            index = i * GridColumns + j;

            if (
                    //(BoxArray[index].Dummybool2) &&
                    (BoxDepth[index] > CriticalDepthForLandBoundary) &&
                    (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 2) &&
                    (BoxArray[index].Dummybool1)
                )
            {

                // AP, 2005.05.19 ...
                H1 = BoxDepth[index] + DynamicHeight[index] - PastHeight[index];
                Q1 = ASpatialStep * ASpatialStep * H1;

         	    //Advective fluxes and Diffusive fluxes

                // AP, 2005.05.19 ...
//                PastGeneric = PastGenericA;
                MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericA);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericA);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericA);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericA);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericA);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericA);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericA);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericA);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                             MyNorthDiffusion, MySouthDiffusion, PastGenericA[index]))
                    ResultantDiffusion = MyWestDiffusion + MyEastDiffusion + MyNorthDiffusion + MySouthDiffusion;
                else
                    ResultantDiffusion = 0.0;

                A[index] = PastGenericA[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + (ResultantDiffusion) / (2.0 * Q1);
                if ((A[index] < 0.0) && (A[index] > -0.000001))
                    A[index] = 0.0;

//                PastGeneric = PastGenericB;
                MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericB);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericB);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericB);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericB);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericB);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericB);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericB);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericB);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                             MyNorthDiffusion, MySouthDiffusion, PastGenericB[index]))
                    ResultantDiffusion = MyWestDiffusion + MyEastDiffusion + MyNorthDiffusion + MySouthDiffusion;
                else
                    ResultantDiffusion = 0.0;

                B[index] = PastGenericB[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + (ResultantDiffusion) / (2.0 * Q1);
                if ((B[index] < 0.0) && (B[index] > -0.000001))
                    B[index] = 0.0;

//                PastGeneric = PastGenericC;
                MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericC);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericC);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericC);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericC);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericC);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericC);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericC);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericC);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                             MyNorthDiffusion, MySouthDiffusion, PastGenericC[index]))
                    ResultantDiffusion = MyWestDiffusion + MyEastDiffusion + MyNorthDiffusion + MySouthDiffusion;
                else
                    ResultantDiffusion = 0.0;

                C[index] = PastGenericC[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + (ResultantDiffusion) / (2.0 * Q1);
                if ((C[index] < 0.0) && (C[index] > -0.000001))
                    C[index] = 0.0;

//                PastGeneric = PastGenericD;
                MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericD);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericD);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericD);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericD);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericD);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericD);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericD);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericD);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                             MyNorthDiffusion, MySouthDiffusion, PastGenericD[index]))
                    ResultantDiffusion = MyWestDiffusion + MyEastDiffusion + MyNorthDiffusion + MySouthDiffusion;
                else
                    ResultantDiffusion = 0.0;

                D[index] = PastGenericD[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + (ResultantDiffusion) / (2.0 * Q1);
                if ((D[index] < 0.0) && (D[index] > -0.000001))
                    D[index] = 0.0;

//                PastGeneric = PastGenericE;
                MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericE);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericE);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericE);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericE);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericE);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericE);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericE);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericE);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                             MyNorthDiffusion, MySouthDiffusion, PastGenericE[index]))
                    ResultantDiffusion = MyWestDiffusion + MyEastDiffusion + MyNorthDiffusion + MySouthDiffusion;
                else
                    ResultantDiffusion = 0.0;

                E[index] = PastGenericE[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + (ResultantDiffusion) / (2.0 * Q1);
                if ((E[index] < 0.0) && (E[index] > -0.000001))
                    E[index] = 0.0;

//                PastGeneric = PastGenericF;
                MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericF);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericF);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericF);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericF);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericF);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericF);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericF);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericF);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                             MyNorthDiffusion, MySouthDiffusion, PastGenericF[index]))
                    ResultantDiffusion = MyWestDiffusion + MyEastDiffusion + MyNorthDiffusion + MySouthDiffusion;
                else
                    ResultantDiffusion = 0.0;

                F[index] = PastGenericF[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + (ResultantDiffusion) / (2.0 * Q1);
                if ((F[index] < 0.0) && (F[index] > -0.000001))
                    F[index] = 0.0;

//                PastGeneric = PastGenericG;
                MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericG);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericG);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericG);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericG);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericG);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericG);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericG);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericG);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                             MyNorthDiffusion, MySouthDiffusion, PastGenericG[index]))
                    ResultantDiffusion = MyWestDiffusion + MyEastDiffusion + MyNorthDiffusion + MySouthDiffusion;
                else
                    ResultantDiffusion = 0.0;

                G[index] = PastGenericG[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + (ResultantDiffusion) / (2.0 * Q1);
                if ((G[index] < 0.0) && (G[index] > -0.000001))
                    G[index] = 0.0;

//                PastGeneric = PastGenericH;
                MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericH);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericH);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericH);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericH);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericH);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericH);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericH);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericH);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                             MyNorthDiffusion, MySouthDiffusion, PastGenericH[index]))
                    ResultantDiffusion = MyWestDiffusion + MyEastDiffusion + MyNorthDiffusion + MySouthDiffusion;
                else
                    ResultantDiffusion = 0.0;

                H[index] = PastGenericH[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + (ResultantDiffusion) / (2.0 * Q1);
                if ((H[index] < 0.0) && (H[index] > -0.000001))
                    H[index] = 0.0;

                if (MyLoadRecord != NULL)
                {
                    for (k = 0; k < MyPEcoDynClass->GetNumberOfLoads(); k++)
                    {
                        if (
                          (i == MyLoadRecord[k].LineCoordinate) &&
                          (j == MyLoadRecord[k].ColumnCoordinate)
                        )
                        {
                            ADay = MIN(MyPEcoDynClass->GetJulianDay(),
                                       MyPEcoDynClass->GetNumberOfDaysForLoads());

                            // AP, 2005.05.19 ...
                            A[index] = A[index] + ACellRecord[index].CellTimeStep * ALoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            B[index] = B[index] + ACellRecord[index].CellTimeStep * BLoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            C[index] = C[index] + ACellRecord[index].CellTimeStep * CLoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            D[index] = D[index] + ACellRecord[index].CellTimeStep * DLoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            E[index] = E[index] + ACellRecord[index].CellTimeStep * ELoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            F[index] = F[index] + ACellRecord[index].CellTimeStep * FLoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            G[index] = G[index] + ACellRecord[index].CellTimeStep * GLoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            H[index] = H[index] + ACellRecord[index].CellTimeStep * HLoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            break;
                        }
                    }
                }
      	    }
        }
     }
    delete [] flowCriticalCellsA;
    delete [] flowCriticalCellsB;
    delete [] flowCriticalCellsC;
    delete [] flowCriticalCellsD;
    delete [] flowCriticalCellsE;
    delete [] flowCriticalCellsF;
    delete [] flowCriticalCellsG;
    delete [] flowCriticalCellsH;

    delete [] PastGenericA;
    delete [] PastGenericB;
    delete [] PastGenericC;
    delete [] PastGenericD;
    delete [] PastGenericE;
    delete [] PastGenericF;
    delete [] PastGenericG;
    delete [] PastGenericH;
}

void TBiDimensionalTimeSeriesVariableDT::Reinitialize()
{
    AdvectDiffuse(Salt, SaltLoad);
    double MyWaterTemperature;
    double const MinCritDepth = 0.01;  //0.00001 m
    int i, j, index;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();

    EnableReinitializeFromGo = false;   // AP, 2005.09.16 - moved to here
    EnableReinitializeFromIntegrate = false;

    TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();

    for (i = pSubDomain->FirstLine;
         i <= pSubDomain->LastLine; i++)
    {
       for (j = pSubDomain->IColumn[i];
         j <= pSubDomain->FColumn[i]; j++)
       {
          index = i * GridColumns + j;
      	  atemporary[index] = 0.0; btemporary[index] = 0.0;
          ctemporary[index] = 0.0; rtemporary[index] = 0.0;
          if (ACellRecord[index].Critical)
             BoxDepth[index] = CriticalCellDepth[index] +
                              (DynamicHeight[index] - CriticalCellHeight[index]);
          else
             BoxDepth[index] = BoxDepth[index] +
                              (DynamicHeight[index] - PastHeight[index]);
          if (BoxDepth[index] <= CriticalDepthForLandBoundary)
          {
             BoxArray[index].Dummybool2 = false;
          }   
      }
   }
   for (i = pSubDomain->FirstLine;
        i <= pSubDomain->LastLine + 1; i++)
   {
      for (j = pSubDomain->IColumn[i];
         j <= pSubDomain->FColumn[i] + 1; j++)
      {
         index = i * GridColumns + j;
         if (index < NumberOfBoxes)
         {
             PastHeight[index] = DynamicHeight[index];
             Cells[index] = 0;
             ACellRecord[index].Critical = false;
             ACellRecord[index].CellTimeStep = ATimeStep;
             ACellRecord[index].NorthFlow = 0.0; ACellRecord[index].SouthFlow = 0.0;
             ACellRecord[index].WestFlow = 0.0; ACellRecord[index].EastFlow = 0.0;
             //Estes fluxos são reinicializados a zero no final do AdvectDiffuse
      	     if (MyWaterTemperaturePointer != NULL)
             {
                MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature, index,"Water temperature",ObjectCode);
      	        WaterDensity[index] = MyPEcoDynClass->GetDensity(Salt[index],MyWaterTemperature);
      	     }
      	     else
             WaterDensity[index] = 1026.75; // seawater at 35psu and 10oC;

             if   (
                  (! BoxArray[index].Dummybool2) && (BoxDepth[index] >= MinCritDepth) &&  //Tive que mudar este valor de zero para 0.00001 pois havia casos
                  (                                                                       // em que a BoxDepth estava tão próxima de zero que ocorria um floating point error
                  	(                                                                     //no AdvectDiffuse no cálculo da concentração, quando divide pela soma
                     	(j < GridColumns - 1) &&                                          // da BoxDepth com a diferença entre a elevação presente e a elevação passada
                        //(DynamicHeight[index + 1] > BoxDepth[index]) &&
                        (DynamicHeight[index + 1] > DynamicHeight[index]) &&
                        (BoxDepth[index + 1] > CriticalDepthForLandBoundary)
                     ) ||
                     (
                     	(j > 0) &&
                        //(DynamicHeight[index - 1] > BoxDepth[index]) &&
                        (DynamicHeight[index - 1] > DynamicHeight[index]) &&
                        (BoxDepth[index - 1] > CriticalDepthForLandBoundary)
                     ) ||
                  	(
                  		(i < GridLines - 1)  &&
                        //(DynamicHeight[(i + 1) * GridColumns + j] > BoxDepth[index]) &&
                        (DynamicHeight[(i + 1) * GridColumns + j] > DynamicHeight[index]) &&
                        (BoxDepth[(i + 1) * GridColumns + j] > CriticalDepthForLandBoundary)
                  	)    ||
                  	(
                  		(i > 0) &&
                        //(DynamicHeight[(i - 1) * GridColumns + j] > BoxDepth[index]) &&
                        (DynamicHeight[(i - 1) * GridColumns + j] > DynamicHeight[index]) &&
                        (BoxDepth[(i - 1) * GridColumns + j] > CriticalDepthForLandBoundary)
                  	)
               	   )
            	)
               BoxArray[index].Dummybool2 = true;

             if (
               (!BoxArray[index].Dummybool2) ||
               (
                  (BoxArray[index].Dummybool2) &&
                  (BoxDepth[index] <= CriticalDepthForLandBoundary) &&
                  (DynamicHeight[index] < PastHeight[index ])
               )
             )
             {
                UVelocity[i * (GridColumns + 1) + j] = 0.0;
                VVelocity[index] = 0.0;
                UVelocity[i * (GridColumns + 1) + j + 1] = 0.0;
                VVelocity[(i + 1) * GridColumns + j] = 0.0;
                //DynamicHeight[index] = BoxDepth[index ];
             }
          }
          if (index < NumberOfBoxes + GridLines)
             UVelocityPast[i * (GridColumns + 1) + j] = UVelocity[i * (GridColumns + 1) + j];

          if (index < NumberOfBoxes + GridColumns)
             VVelocityPast[index] = VVelocity[index];
       }
    }
   RiverForcing();
   if (SemiTimeStep == 1) SemiTimeStep = 2;
   else if (SemiTimeStep == 2) SemiTimeStep = 1;

   ReadVelocitiesAndFlows();
}

double TBiDimensionalTimeSeriesVariableDT::WesternComponentOfContinuity(int i, int j)
{
    int index = i * (GridColumns + 1) + j;
    //Western flow
    return (ATimeStep * UFlowBetweenBoxes[index]);
}

double TBiDimensionalTimeSeriesVariableDT::EasternComponentOfContinuity(int i, int j)
{
     int index = i * (GridColumns + 1) + j + 1;
     //Eastern flow
     return (ATimeStep  * UFlowBetweenBoxes[index]);
}

double TBiDimensionalTimeSeriesVariableDT::NorthernComponentOfContinuity(int i, int j)
{
      int index = (i + 1) * GridColumns + j;
     //Northern flow
     return (ATimeStep * VFlowBetweenBoxes[index]);
}

double TBiDimensionalTimeSeriesVariableDT::SouthernComponentOfContinuity(int i, int j)
{
        int index = i * GridColumns + j;
        //Southern flow
        return (ATimeStep * VFlowBetweenBoxes[index]);
}



double TBiDimensionalTimeSeriesVariableDT::WesternComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric)
{
        int index = i * GridColumns + j;
	//Western flow
        if (j == 0)
   		return (ATimeStep * UFlowBetweenBoxes[i * (GridColumns + 1) + j] * PastGeneric[index]);
   	else
   	{
      	   if (UFlowBetweenBoxes[i * (GridColumns + 1) + j] > 0.0)
              return  (ATimeStep * UFlowBetweenBoxes[i * (GridColumns + 1) + j] *PastGeneric[index - 1]);
           else
              return (ATimeStep * UFlowBetweenBoxes[i * (GridColumns + 1) + j] * PastGeneric[index]);
   	}
}

double TBiDimensionalTimeSeriesVariableDT::EasternComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric)
{
        int index = i * GridColumns + j;
	//Eastern flow
   	if (j == GridColumns - 1)
   	{
   		return (ATimeStep * UFlowBetweenBoxes[i * (GridColumns + 1) + j + 1] * PastGeneric[index]);
   	}
   	else
   	{
           if (UFlowBetweenBoxes[i * (GridColumns + 1) + j + 1] > 0.0)
                return (ATimeStep * UFlowBetweenBoxes[i * (GridColumns + 1) + j + 1] * PastGeneric[index]);
           else
         	return (ATimeStep * UFlowBetweenBoxes[i * (GridColumns + 1) + j + 1] * PastGeneric[index + 1]);
   	}
}

double TBiDimensionalTimeSeriesVariableDT::NorthernComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric)
{
  int index = i * GridColumns + j;

	//Northern flow
   	if (i == GridLines - 1)
   		return (ATimeStep * VFlowBetweenBoxes[(i + 1) * GridColumns + j] * PastGeneric[index]);
   	else
   	{
      	        if (VFlowBetweenBoxes[(i + 1) * GridColumns + j] > 0.0)
                   return (ATimeStep  * VFlowBetweenBoxes[(i + 1) * GridColumns + j] * PastGeneric[index]);
                else
         	   return (ATimeStep * VFlowBetweenBoxes[(i + 1) * GridColumns + j] * PastGeneric[(i + 1) * GridColumns + j]);
   	}
}

double TBiDimensionalTimeSeriesVariableDT::SouthernComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric)
{
        int index = i * GridColumns + j;
	//Southern flow
        if (i == 0)
   		return (ATimeStep * VFlowBetweenBoxes[index ] * PastGeneric[index ]);
   	else
   	{
      	        if (VFlowBetweenBoxes[index] > 0.0)
                   return (ATimeStep  * VFlowBetweenBoxes[index ] * PastGeneric[(i - 1) * GridColumns + j]);
                else
         	   return (ATimeStep * VFlowBetweenBoxes[index ] * PastGeneric[i * GridColumns + j]);
   	}
}


double TBiDimensionalTimeSeriesVariableDT::SaltWesternHorizontalDiffusion(int i, int j, double* PastGeneric)
{
        int index = i * GridColumns + j;
        if (
   		(j > 0) &&
                (BoxArray[index].Dummybool2)&&
                (BoxArray[index - 1].Dummybool2) &&
                (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 1)
   	)
   	   return
             -ATimeStep *
             (
             	HorizontalDiffCoef *
                CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[index - 1]), UPStreamUVelocity(i, j)) *
      		(BoxDepth[index] + BoxDepth[index - 1]) *  PastGeneric[index ] -
                HorizontalDiffCoef *
                CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[index - 1]), UPStreamUVelocity(i, j)) *
      		(BoxDepth[index] + BoxDepth[index - 1]) * PastGeneric[index - 1]
             );
        else
   	   return 0.0;

}

double TBiDimensionalTimeSeriesVariableDT::SaltEasternHorizontalDiffusion(int i, int j, double* PastGeneric)
{
        int index = i * GridColumns + j;

	if (
   		(j < GridColumns - 1) &&(BoxArray[index].Dummybool2) &&
                (BoxArray[index + 1].Dummybool2) &&
                (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 1)
   	   )
        {
   	  return
              ATimeStep *
              (
             	HorizontalDiffCoef *
                CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[index + 1]), DOWNStreamUVelocity(i, j)) *
                (BoxDepth[index + 1] + BoxDepth[index]) *
                PastGeneric[index + 1] - HorizontalDiffCoef *
                CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[index + 1]), DOWNStreamUVelocity(i, j)) *
                (BoxDepth[index + 1] + BoxDepth[index]) *
                PastGeneric[index ]
             );
        }
   else
   	return 0.0;
}

double TBiDimensionalTimeSeriesVariableDT::SaltNorthernVerticalDiffusion(int i, int j, double* PastGeneric)
{
        int index = i * GridColumns + j;
	if (
   		(i < GridLines - 1) &&
                (BoxArray[index].Dummybool2) &&
                (BoxArray[(i + 1) * GridColumns + j].Dummybool2) &&
                (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 1)
   	   )
   	   return
              ATimeStep *
              (
             	HorizontalDiffCoef *
                CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[(i + 1) * GridColumns + j]), DOWNStreamVVelocity(i, j)) *
                (BoxDepth[(i + 1) * GridColumns + j] + BoxDepth[index]) *
                PastGeneric[(i + 1) * GridColumns + j] - HorizontalDiffCoef *
                CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[(i + 1) * GridColumns + j]), DOWNStreamVVelocity(i, j)) *
                (BoxDepth[(i + 1) * GridColumns + j] + BoxDepth[index]) * PastGeneric[index ]
             );

        else
   	  return 0.0;
}

double TBiDimensionalTimeSeriesVariableDT::SaltSouthernVerticalDiffusion(int i, int j, double* PastGeneric)
{
        int index = i * GridColumns + j;
	if (
   		(i > 0) &&
                (BoxArray[index].Dummybool2) &&
                (BoxArray[(i - 1) * GridColumns + j].Dummybool2) &&
                (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 1)
   	   )
   	   return
             -ATimeStep *
             (
             	HorizontalDiffCoef *
                CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[(i - 1) * GridColumns + j]), UPStreamVVelocity(i, j)) *
      		(BoxDepth[index] + BoxDepth[(i - 1) * GridColumns + j]
             ) *
             PastGeneric[index] - HorizontalDiffCoef *
             CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[(i - 1) * GridColumns + j]), UPStreamVVelocity(i, j)) *
             (BoxDepth[index] + BoxDepth[(i - 1) * GridColumns + j]) * PastGeneric[(i - 1) * GridColumns + j]);
        else
   	  return 0.0;
}



