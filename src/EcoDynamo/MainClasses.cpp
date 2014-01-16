//---------------------------------------------------------------------------

#include "EcoDynamo.h"
#include "hydrobjt.h"
#include "tideobjt.h"
#include "twatobjt.h"
#include "flowobjt.h"
#include "windobjt.h"
#include "tairobjt.h"
#include "liteobjt.h"
#include "dissobjt.h"
#include "spmobjt.h"
#include "phytobjt.h"
#include "zoopobjt.h"
#include "zoobobjt.h"
#include "manobjt.h"
#include "macrobjt.h"
#include "sedimentobjt.h"
#include "preyobjt.h"
#include "predatorobjt.h"
#include "fishobjt.h"
#include "bacteriaobjt.h"
#include "iceobjt.h"
#include "IceAlgaeObjt.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

/*
 * Get available variables from object class ...
 * ...always changed when a new object class is appended
 *
 * 2004.03.12 (AP) - included TBiDimensionalTimeSeries class
 * 2004.08.25 (AP) - upgraded from PD. Classes included:
 *                   TOneDimensionalDITransport
 *                   TBiDimensionalVertical
 *                   TCrestumaLeverFlow
 *                   TWaterTemperatureCrestumaTwoDimensional
 *                   TMiraAirTemperature
 *                   TCrestumaLeverVert2DDissobjt
 *                   TSPMCrestuma
 *                   TCrestumaLeverPhytoplankton2DVIntLim
 * 2004.09.15 (AP) - upgraded from PD. Classes included:
 *                   TRiaFormosaAirTemperature
 *                   TRiaF2DNutrients
 *                   TRiaF2DPhytoplankton
 * 2007.05.08 (AP) - upgraded from PD. Classes included:
 *                   TPredator
 *                   TPrey
 * 2008.01.16 (AP) - upgraded from PD. Classes included:
 *                   TWaterTemperatureThreeDimensional
 *                   TWindTimeSeries
 *                   TAirTemperatureTimeSeries
 */
//---------------------------------------------------------------------------
void EcoDynamo::getVariables(Queue* pEDCQueue, char* className,
                         TEcoDynClass::VarsRecord* pVars)
{
  bool cyclic;

    if (strcmp(className, "THydrodynamic") == 0) {
        PTransport = new TTransport(MyPEcoDynClass, className);
       	MyPEcoDynClass->SetTransportPointer(PTransport);    // hydrodynamics
        pEDCQueue->InsertObject((TObject*)PTransport);
        addAvailableVariables(PTransport, pVars);
    }
    else if (strcmp(className, "TBiDimensionalSango3") == 0) {
        PTransport = new TBiDimensionalSango3(MyPEcoDynClass, className);
       	MyPEcoDynClass->SetTransportPointer(PTransport);    // hydrodynamics
        pEDCQueue->InsertObject((TObject*)PTransport);
        addAvailableVariables(PTransport, pVars);
    }
    else if (strcmp(className, "TCarl2D") == 0) {
        PTransport = new TCarl2D(MyPEcoDynClass, className);
       	MyPEcoDynClass->SetTransportPointer(PTransport);    // hydrodynamics
        pEDCQueue->InsertObject((TObject*)PTransport);
        addAvailableVariables(PTransport, pVars);
    }
    else if (strcmp(className, "TBiDimensionalRiaFormosa") == 0) {
        PTransport = new TBiDimensionalRiaFormosa(MyPEcoDynClass, className);
       	MyPEcoDynClass->SetTransportPointer(PTransport);    // hydrodynamics
        pEDCQueue->InsertObject((TObject*)PTransport);
        addAvailableVariables(PTransport, pVars);
    }
    else if (strcmp(className, "TBiDimensionalTidalTestIntertidal") == 0) {
        PTransport = new TBiDimensionalTidalTestIntertidal(MyPEcoDynClass, className);
       	MyPEcoDynClass->SetTransportPointer(PTransport);    // hydrodynamics
        pEDCQueue->InsertObject((TObject*)PTransport);
        addAvailableVariables(PTransport, pVars);
    }
    else if (strcmp(className, "TBiDimensionalTimeSeries") == 0) {
        cyclic = (strcmp(defaultsRecord.SimulationRunType, "Cycle") == 0);
        PTransport = new TBiDimensionalTimeSeries(MyPEcoDynClass,
                className, true, cyclic,
                defaultsRecord.NumberOfPeriods, defaultsRecord.period);
       	MyPEcoDynClass->SetTransportPointer(PTransport);    // hydrodynamics
        pEDCQueue->InsertObject((TObject*)PTransport, true);
        addAvailableVariables(PTransport, pVars);
    }
    else if (strcmp(className, "TBiDimensionalTimeSeriesVariableDT") == 0) {
        cyclic = (strcmp(defaultsRecord.SimulationRunType, "Cycle") == 0);
        PTransport = new TBiDimensionalTimeSeriesVariableDT(MyPEcoDynClass,
                className, true, cyclic,
                defaultsRecord.NumberOfPeriods, defaultsRecord.period);
       	MyPEcoDynClass->SetTransportPointer(PTransport);    // hydrodynamics
        pEDCQueue->InsertObject((TObject*)PTransport, true);
        addAvailableVariables(PTransport, pVars);
    }
    else if (strcmp(className, "TBiDimensionalTimeSeriesTransportImplicit") == 0) {
        cyclic = (strcmp(defaultsRecord.SimulationRunType, "Cycle") == 0);
        PTransport = new TBiDimensionalTimeSeriesTransportImplicit(MyPEcoDynClass,
                className, true, cyclic,
                defaultsRecord.NumberOfPeriods, defaultsRecord.period);
       	MyPEcoDynClass->SetTransportPointer(PTransport);    // hydrodynamics
        pEDCQueue->InsertObject((TObject*)PTransport, true);
        addAvailableVariables(PTransport, pVars);
    }
    else if (strcmp(className, "TOneDimensionalDITransport") == 0) {
        PTransport = new TOneDimensionalDITransport(MyPEcoDynClass, className);
       	MyPEcoDynClass->SetTransportPointer(PTransport);    // hydrodynamics
        pEDCQueue->InsertObject((TObject*)PTransport);
        addAvailableVariables(PTransport, pVars);
    }
    else if (strcmp(className, "TBiDimensionalVertical") == 0) {
        PTransport = new TBiDimensionalVertical(MyPEcoDynClass, className);
       	MyPEcoDynClass->SetTransportPointer(PTransport);    // hydrodynamics
        pEDCQueue->InsertObject((TObject*)PTransport);
        addAvailableVariables(PTransport, pVars);
    }
    else if (strcmp(className, "TBiDimensionalDouro") == 0) {
        PTransport = new TBiDimensionalDouro(MyPEcoDynClass, className);
       	MyPEcoDynClass->SetTransportPointer(PTransport);    // hydrodynamics
        pEDCQueue->InsertObject((TObject*)PTransport);
        addAvailableVariables(PTransport, pVars);
    }
    else if (strcmp(className, "TTriDimensional") == 0) {
        PTransport = new TTriDimensional(MyPEcoDynClass, className);
       	MyPEcoDynClass->SetTransportPointer(PTransport);    // hydrodynamics
        pEDCQueue->InsertObject((TObject*)PTransport);
        addAvailableVariables(PTransport, pVars);
    }
    else if (strcmp(className, "TTriDimensionalDynamicSurfaceLayer") == 0) {
        PTransport = new TTriDimensionalDynamicSurfaceLayer(MyPEcoDynClass, className);
       	MyPEcoDynClass->SetTransportPointer(PTransport);    // hydrodynamics
        pEDCQueue->InsertObject((TObject*)PTransport);
        addAvailableVariables(PTransport, pVars);
    }
    else if (strcmp(className, "TTriDimensionalWithBodyDrag") == 0) {
        PTransport = new TTriDimensionalWithBodyDrag(MyPEcoDynClass, className);
       	MyPEcoDynClass->SetTransportPointer(PTransport);    // hydrodynamics
        pEDCQueue->InsertObject((TObject*)PTransport);
        addAvailableVariables(PTransport, pVars);
    }
    else if (strcmp(className, "TTriDimensionalWithBodyDrag2") == 0) {
        PTransport = new TTriDimensionalWithBodyDrag2(MyPEcoDynClass, className);
       	MyPEcoDynClass->SetTransportPointer(PTransport);    // hydrodynamics
        pEDCQueue->InsertObject((TObject*)PTransport);
        addAvailableVariables(PTransport, pVars);
    }
    else if (strcmp(className, "TMixedReactor") == 0) {
        PTransport = new TMixedReactor(MyPEcoDynClass, className);
       	MyPEcoDynClass->SetTransportPointer(PTransport);    // hydrodynamics
        pEDCQueue->InsertObject((TObject*)PTransport);
        addAvailableVariables(PTransport, pVars);
    }
    else if (strcmp(className, "TFlow") == 0) {
        PFlow = new TFlow(MyPEcoDynClass, className);
        MyPEcoDynClass->SetFlowPointer(PFlow);    // flow
        pEDCQueue->InsertObject((TObject*)PFlow);
        addAvailableVariables(PFlow, pVars);
    }
    else if (strcmp(className, "TCarlFlow") == 0) {
        PFlow = new TCarlFlow(MyPEcoDynClass, className);
        MyPEcoDynClass->SetFlowPointer(PFlow);    // flow
        pEDCQueue->InsertObject((TObject*)PFlow);
        addAvailableVariables(PFlow, pVars);
    }
    else if (strcmp(className, "TCrestumaLeverFlow") == 0) {
        PFlow = new TCrestumaLeverFlow(MyPEcoDynClass, className);
        MyPEcoDynClass->SetFlowPointer(PFlow);    // flow
        pEDCQueue->InsertObject((TObject*)PFlow);
        addAvailableVariables(PFlow, pVars);
    }
    else if (strcmp(className, "TRiaFormosaFlow") == 0) {
        PFlow = new TRiaFormosaFlow(MyPEcoDynClass, className);
        MyPEcoDynClass->SetFlowPointer(PFlow);    // flow
        pEDCQueue->InsertObject((TObject*)PFlow);
        addAvailableVariables(PFlow, pVars);
    }
    else if (strcmp(className, "TLight") == 0) {
        PLight = new TLight(MyPEcoDynClass, className);
        MyPEcoDynClass->SetLightPointer(PLight);    // light
        pEDCQueue->InsertObject((TObject*)PLight);
        addAvailableVariables(PLight, pVars);
    }
    else if (strcmp(className, "TLightWithIce") == 0) {
        PLight = new TLightWithIce(MyPEcoDynClass, className);
        MyPEcoDynClass->SetLightPointer(PLight);    // light
        pEDCQueue->InsertObject((TObject*)PLight);
        addAvailableVariables(PLight, pVars);
    }
    else if (strcmp(className, "TTide") == 0) {
        PTide = new TTide(MyPEcoDynClass, className);
        MyPEcoDynClass->SetTidePointer(PTide);    // tide
        pEDCQueue->InsertObject((TObject*)PTide, true);
        addAvailableVariables(PTide, pVars);
    }
    else if (strcmp(className, "TTideWithWanContinuousHarmonics") == 0) {
        PTide = new TTideWithWanContinuousHarmonics(MyPEcoDynClass, className);
        MyPEcoDynClass->SetTidePointer(PTide);    // tide
        pEDCQueue->InsertObject((TObject*)PTide, true);
        addAvailableVariables(PTide, pVars);
    }
    else if (strcmp(className, "TCarlWaterTemperature") == 0) {
        PWaterTemperature = new TCarlWaterTemperature(MyPEcoDynClass, className);
        MyPEcoDynClass->SetWaterTemperaturePointer(PWaterTemperature); // water temperature
        pEDCQueue->InsertObject((TObject*)PWaterTemperature);
        addAvailableVariables(PWaterTemperature, pVars);
    }
    else if (strcmp(className, "TWaterTemperatureTwoDimensionalForSango") == 0) {
        PWaterTemperature = new TWaterTemperatureTwoDimensionalForSango(
                MyPEcoDynClass, className);
        MyPEcoDynClass->SetWaterTemperaturePointer(PWaterTemperature); // water temperature
        pEDCQueue->InsertObject((TObject*)PWaterTemperature);
        addAvailableVariables(PWaterTemperature, pVars);
    }
    else if (strcmp(className, "TWaterTemperatureTimeSeries") == 0) {
        PWaterTemperature = new TWaterTemperatureTimeSeries(
                MyPEcoDynClass, className);
        MyPEcoDynClass->SetWaterTemperaturePointer(PWaterTemperature); // water temperature
        pEDCQueue->InsertObject((TObject*)PWaterTemperature);
        addAvailableVariables(PWaterTemperature, pVars);
    }
    else if (strcmp(className, "TWaterTemperatureTimeSeries2") == 0) {
        PWaterTemperature = new TWaterTemperatureTimeSeries2(
                MyPEcoDynClass, className);
        MyPEcoDynClass->SetWaterTemperaturePointer(PWaterTemperature); // water temperature
        pEDCQueue->InsertObject((TObject*)PWaterTemperature);
        addAvailableVariables(PWaterTemperature, pVars);
    }
    else if (strcmp(className, "TWaterTemperatureCrestumaTwoDimensional") == 0) {
        PWaterTemperature = new TWaterTemperatureCrestumaTwoDimensional(
                MyPEcoDynClass, className);
        MyPEcoDynClass->SetWaterTemperaturePointer(PWaterTemperature); // water temperature
        pEDCQueue->InsertObject((TObject*)PWaterTemperature);
        addAvailableVariables(PWaterTemperature, pVars);
    }
    else if (strcmp(className, "TWaterTemperatureTwoDimensionalForRiaFormosa") == 0) {
        PWaterTemperature = new TWaterTemperatureTwoDimensionalForRiaFormosa(
                MyPEcoDynClass, className);
        MyPEcoDynClass->SetWaterTemperaturePointer(PWaterTemperature); // water temperature
        pEDCQueue->InsertObject((TObject*)PWaterTemperature);
        addAvailableVariables(PWaterTemperature, pVars);
    }
    else if (strcmp(className, "TWaterTemperatureThreeDimensional") == 0) {
        PWaterTemperature = new TWaterTemperatureThreeDimensional(
                MyPEcoDynClass, className);
        MyPEcoDynClass->SetWaterTemperaturePointer(PWaterTemperature); // water temperature
        pEDCQueue->InsertObject((TObject*)PWaterTemperature);
        addAvailableVariables(PWaterTemperature, pVars);
    }
    else if (strcmp(className, "TWind") == 0) {
        PWind = new TWind(MyPEcoDynClass, className);
        MyPEcoDynClass->SetWindPointer(PWind);    // wind
        pEDCQueue->InsertObject((TObject*)PWind);
        addAvailableVariables(PWind, pVars);
    }
    else if (strcmp(className, "TWindDailyTimeSeries") == 0) {
        PWind = new TWindDailyTimeSeries(MyPEcoDynClass, className);
        MyPEcoDynClass->SetWindPointer(PWind);    // wind
        pEDCQueue->InsertObject((TObject*)PWind);
        addAvailableVariables(PWind, pVars);
    }
    else if (strcmp(className, "TWindTimeSeries") == 0) {
        PWind = new TWindTimeSeries(MyPEcoDynClass, className);
        MyPEcoDynClass->SetWindPointer(PWind);    // wind
        pEDCQueue->InsertObject((TObject*)PWind);
        addAvailableVariables(PWind, pVars);
    }
    else if (strcmp(className, "TWindTimeSeriesV1") == 0) {
        PWind = new TWindTimeSeriesV1(MyPEcoDynClass, className);
        MyPEcoDynClass->SetWindPointer(PWind);    // wind
        pEDCQueue->InsertObject((TObject*)PWind);
        addAvailableVariables(PWind, pVars);
    }
    else if (strcmp(className, "TAirTemperature") == 0) {
        PAirTemperature = new TAirTemperature(MyPEcoDynClass, className);
        MyPEcoDynClass->SetAirTemperaturePointer(PAirTemperature);     // air temperature
        pEDCQueue->InsertObject((TObject*)PAirTemperature);
        addAvailableVariables(PAirTemperature, pVars);
    }
    else if (strcmp(className, "TCarlAirTemperature") == 0) {
        PAirTemperature = new TAirTemperature(MyPEcoDynClass, className);
        MyPEcoDynClass->SetAirTemperaturePointer(PAirTemperature);     // air temperature
        pEDCQueue->InsertObject((TObject*)PAirTemperature);
        addAvailableVariables(PAirTemperature, pVars);
    }
    else if (strcmp(className, "TAdriaticAirTemperature") == 0) {
        PAirTemperature = new TAdriaticAirTemperature(MyPEcoDynClass, className);
        MyPEcoDynClass->SetAirTemperaturePointer(PAirTemperature);     // air temperature
        pEDCQueue->InsertObject((TObject*)PAirTemperature);
        addAvailableVariables(PAirTemperature, pVars);
    }
    else if (strcmp(className, "TMiraAirTemperature") == 0) {
        PAirTemperature = new TMiraAirTemperature(MyPEcoDynClass, className);
        MyPEcoDynClass->SetAirTemperaturePointer(PAirTemperature);     // air temperature
        pEDCQueue->InsertObject((TObject*)PAirTemperature);
        addAvailableVariables(PAirTemperature, pVars);
    }
    else if (strcmp(className, "TRiaFormosaAirTemperature") == 0) {
        PAirTemperature = new TRiaFormosaAirTemperature(MyPEcoDynClass, className);
        MyPEcoDynClass->SetAirTemperaturePointer(PAirTemperature);     // air temperature
        pEDCQueue->InsertObject((TObject*)PAirTemperature);
        addAvailableVariables(PAirTemperature, pVars);
    }
    else if (strcmp(className, "TAirTemperatureTimeSeries") == 0) {
        PAirTemperature = new TAirTemperatureTimeSeries(MyPEcoDynClass, className);
        MyPEcoDynClass->SetAirTemperaturePointer(PAirTemperature);     // air temperature
        pEDCQueue->InsertObject((TObject*)PAirTemperature);
        addAvailableVariables(PAirTemperature, pVars);
    }
    else if (strcmp(className, "TAirTemperatureTimeSeries2") == 0) {
        PAirTemperature = new TAirTemperatureTimeSeries2(MyPEcoDynClass, className);
        MyPEcoDynClass->SetAirTemperaturePointer(PAirTemperature);     // air temperature
        pEDCQueue->InsertObject((TObject*)PAirTemperature);
        addAvailableVariables(PAirTemperature, pVars);
    }
    /*else if (strcmp(className, "TAirTemperatureTimeSeriesSubDomains") == 0) {
        PAirTemperature = new TAirTemperatureTimeSeriesSubDomains(MyPEcoDynClass, className);
        MyPEcoDynClass->SetAirTemperaturePointer(PAirTemperature);     // air temperature
        pEDCQueue->InsertObject((TObject*)PAirTemperature);
        addAvailableVariables(PAirTemperature, pVars);
    }*/
    else if (strcmp(className, "TSangoNutrients") == 0) {
        PNutrients = new TSangoNutrients(MyPEcoDynClass, className);
        MyPEcoDynClass->SetNutrientPointer(PNutrients);    // nutrients
        pEDCQueue->InsertObject((TObject*)PNutrients);
        addAvailableVariables(PNutrients, pVars);
    }
    else if (strcmp(className, "TCarlNutrients") == 0) {
        PNutrients = new TCarlNutrients(MyPEcoDynClass, className);
        MyPEcoDynClass->SetNutrientPointer(PNutrients);    // nutrients
        pEDCQueue->InsertObject((TObject*)PNutrients);
        addAvailableVariables(PNutrients, pVars);
    }
    else if (strcmp(className, "TCrestumaLeverDissobjt") == 0) {
        PNutrients = new TCrestumaLeverDissobjt(MyPEcoDynClass, className);
        MyPEcoDynClass->SetNutrientPointer(PNutrients);    // nutrients
        pEDCQueue->InsertObject((TObject*)PNutrients);
        addAvailableVariables(PNutrients, pVars);
    }
    else if (strcmp(className, "TCrestumaLeverVert2DDissobjt") == 0) {
        PNutrients = new TCrestumaLeverVert2DDissobjt(MyPEcoDynClass, className);
        MyPEcoDynClass->SetNutrientPointer(PNutrients);    // nutrients
        pEDCQueue->InsertObject((TObject*)PNutrients);
        addAvailableVariables(PNutrients, pVars);
    }
    else if (strcmp(className, "TRiaF2DNutrients") == 0) {
        PNutrients = new TRiaF2DNutrients(MyPEcoDynClass, className);
        MyPEcoDynClass->SetNutrientPointer(PNutrients);    // nutrients
        pEDCQueue->InsertObject((TObject*)PNutrients);
        addAvailableVariables(PNutrients, pVars);
    }
    else if (strcmp(className, "TNutrientsForcing") == 0) {
        PNutrients = new TNutrientsForcing(MyPEcoDynClass, className);
        MyPEcoDynClass->SetNutrientPointer(PNutrients);    // nutrients
        pEDCQueue->InsertObject((TObject*)PNutrients);
        addAvailableVariables(PNutrients, pVars);
    }
    else if (strcmp(className, "TNutrients3D") == 0) {
        PNutrients = new TNutrients3D(MyPEcoDynClass, className);
        MyPEcoDynClass->SetNutrientPointer(PNutrients);    // nutrients
        pEDCQueue->InsertObject((TObject*)PNutrients);
        addAvailableVariables(PNutrients, pVars);
    }
    else if (strcmp(className, "TSangoResuspendDeposit") == 0) {
        PSuspendedMatter = new TSangoResuspendDeposit(MyPEcoDynClass, className);
        MyPEcoDynClass->SetSPMPointer(PSuspendedMatter);    // suspended matter
        pEDCQueue->InsertObject((TObject*)PSuspendedMatter);
        addAvailableVariables(PSuspendedMatter, pVars);
    }
    else if (strcmp(className, "TCarlingfordSPM") == 0) {
        PSuspendedMatter = new TCarlingfordSPM(MyPEcoDynClass, className);
        MyPEcoDynClass->SetSPMPointer(PSuspendedMatter);    // suspended matter
        pEDCQueue->InsertObject((TObject*)PSuspendedMatter);
        addAvailableVariables(PSuspendedMatter, pVars);
    }
    else if (strcmp(className, "TSPMTimeSeries") == 0) {
        PSuspendedMatter = new TSPMTimeSeries(MyPEcoDynClass, className);
        MyPEcoDynClass->SetSPMPointer(PSuspendedMatter);    // suspended matter
        pEDCQueue->InsertObject((TObject*)PSuspendedMatter);
        addAvailableVariables(PSuspendedMatter, pVars);
    }
    else if (strcmp(className, "TSPMCrestuma") == 0) {
        PSuspendedMatter = new TSPMCrestuma(MyPEcoDynClass, className);
        MyPEcoDynClass->SetSPMPointer(PSuspendedMatter);    // suspended matter
        pEDCQueue->InsertObject((TObject*)PSuspendedMatter);
        addAvailableVariables(PSuspendedMatter, pVars);
    }
    else if (strcmp(className, "TSPMCrestumaVariableKValue") == 0) {
        PSuspendedMatter = new TSPMCrestumaVariableKValue(
                MyPEcoDynClass, className);
        MyPEcoDynClass->SetSPMPointer(PSuspendedMatter);    // suspended matter
        pEDCQueue->InsertObject((TObject*)PSuspendedMatter);
        addAvailableVariables(PSuspendedMatter, pVars);
    }
    else if (strcmp(className, "TRiaFormosaResuspendDeposit") == 0) {
        PSuspendedMatter = new TRiaFormosaResuspendDeposit(
                MyPEcoDynClass, className);
        MyPEcoDynClass->SetSPMPointer(PSuspendedMatter);    // suspended matter
        pEDCQueue->InsertObject((TObject*)PSuspendedMatter);
        addAvailableVariables(PSuspendedMatter, pVars);
    }
    else if (strcmp(className, "TPisciculturaResuspendDeposit") == 0) {
        PSuspendedMatter = new TPisciculturaResuspendDeposit(MyPEcoDynClass, className);
        MyPEcoDynClass->SetSPMPointer(PSuspendedMatter);    // suspended matter
        pEDCQueue->InsertObject((TObject*)PSuspendedMatter);
        addAvailableVariables(PSuspendedMatter, pVars);
    }
    else if (strcmp(className, "TSPM3D") == 0) {
        PSuspendedMatter = new TSPM3D(
                MyPEcoDynClass, className);
        MyPEcoDynClass->SetSPMPointer(PSuspendedMatter);    // suspended matter
        pEDCQueue->InsertObject((TObject*)PSuspendedMatter);
        addAvailableVariables(PSuspendedMatter, pVars);
    }
    else if (strcmp(className, "TSeeds") == 0) {
        PSuspendedMatter = new TSeeds(MyPEcoDynClass, className);
        MyPEcoDynClass->SetSPMPointer(PSuspendedMatter);    // seeds object
        pEDCQueue->InsertObject((TObject*)PSuspendedMatter);
        addAvailableVariables(PSuspendedMatter, pVars);
    }
    else if (strcmp(className, "TCarlPhytoplankton") == 0) {
        PPhytoplankton = new TCarlPhytoplankton(MyPEcoDynClass, className);
        MyPEcoDynClass->SetPhytoplanktonPointer(PPhytoplankton);    // phytoplankton
        pEDCQueue->InsertObject((TObject*)PPhytoplankton);
        addAvailableVariables(PPhytoplankton, pVars);
    }
    else if (strcmp(className, "TSangoPhytoplankton") == 0) {
        PPhytoplankton = new TSangoPhytoplankton(MyPEcoDynClass, className);
        MyPEcoDynClass->SetPhytoplanktonPointer(PPhytoplankton);    // phytoplankton
        pEDCQueue->InsertObject((TObject*)PPhytoplankton);
        addAvailableVariables(PPhytoplankton, pVars);
    }
    else if (strcmp(className, "TSangoPhytoplanktonIntLim") == 0) {
        PPhytoplankton = new TSangoPhytoplanktonIntLim(MyPEcoDynClass, className);
        MyPEcoDynClass->SetPhytoplanktonPointer(PPhytoplankton);    // phytoplankton
        pEDCQueue->InsertObject((TObject*)PPhytoplankton);
        addAvailableVariables(PPhytoplankton, pVars);
    }
    else if (strcmp(className, "TCrestumaLeverPhytoplankton2DV") == 0) {
        PPhytoplankton = new TCrestumaLeverPhytoplankton2DV(
                MyPEcoDynClass, className);
        MyPEcoDynClass->SetPhytoplanktonPointer(PPhytoplankton);    // phytoplankton
        pEDCQueue->InsertObject((TObject*)PPhytoplankton);
        addAvailableVariables(PPhytoplankton, pVars);
    }
    else if (strcmp(className, "TPhytoplanktonTimeSeries") == 0) {
        PPhytoplankton = new TPhytoplanktonTimeSeries(
                MyPEcoDynClass, className);
        MyPEcoDynClass->SetPhytoplanktonPointer(PPhytoplankton);    // phytoplankton
        pEDCQueue->InsertObject((TObject*)PPhytoplankton);
        addAvailableVariables(PPhytoplankton, pVars);
    }
    else if (strcmp(className, "TCrestumaLeverPhytoplankton2DVIntLim") == 0) {
        PPhytoplankton = new TCrestumaLeverPhytoplankton2DVIntLim(
                MyPEcoDynClass, className);
        MyPEcoDynClass->SetPhytoplanktonPointer(PPhytoplankton);    // phytoplankton
        pEDCQueue->InsertObject((TObject*)PPhytoplankton);
        addAvailableVariables(PPhytoplankton, pVars);
    }
    else if (strcmp(className, "TRiaF2DPhytoplankton") == 0) {
        PPhytoplankton = new TRiaF2DPhytoplankton(
                MyPEcoDynClass, className);
        MyPEcoDynClass->SetPhytoplanktonPointer(PPhytoplankton);    // phytoplankton
        pEDCQueue->InsertObject((TObject*)PPhytoplankton);
        addAvailableVariables(PPhytoplankton, pVars);
    }
    else if (strcmp(className, "TPhytoplankton3D") == 0) {
        PPhytoplankton = new TPhytoplankton3D(
                MyPEcoDynClass, className);
        MyPEcoDynClass->SetPhytoplanktonPointer(PPhytoplankton);    // phytoplankton
        pEDCQueue->InsertObject((TObject*)PPhytoplankton);
        addAvailableVariables(PPhytoplankton, pVars);
    }
    else if (strcmp(className, "TIceAlgae") == 0) {
        PIceAlgae = new TIceAlgae(
                MyPEcoDynClass, className);
        MyPEcoDynClass->SetIceAlgaePointer(PIceAlgae);    // phytoplankton
        pEDCQueue->InsertObject((TObject*)PIceAlgae);
        addAvailableVariables(PIceAlgae, pVars);
    }
    else if (strcmp(className, "TSangoZooplankton") == 0) {
        PZooplankton = new TSangoZooplankton(MyPEcoDynClass, className);
        MyPEcoDynClass->SetZooplanktonPointer(PZooplankton);    // zooplankton
        pEDCQueue->InsertObject((TObject*)PZooplankton);
        addAvailableVariables(PZooplankton, pVars);
    }
    else if (strcmp(className, "TCarlZooplankton") == 0) {
        PZooplankton = new TCarlZooplankton(MyPEcoDynClass, className);
        MyPEcoDynClass->SetZooplanktonPointer(PZooplankton);    // zooplankton
        pEDCQueue->InsertObject((TObject*)PZooplankton);
        addAvailableVariables(PZooplankton, pVars);
    }
    else if (strcmp(className, "TChlamysFarreriV8") == 0) {
        PZoobenthos = new TChlamysFarreriV8(MyPEcoDynClass, className);
        MyPEcoDynClass->SetZoobenthosPointer(PZoobenthos);    // Zoobenthos
        pEDCQueue->InsertObject((TObject*)PZoobenthos);
        addAvailableVariables(PZoobenthos, pVars);
    }
    else if (strcmp(className, "TCrassostreaGigas7") == 0) {
        PZoobenthos2 = new TCrassostreaGigas7(MyPEcoDynClass, className);
        MyPEcoDynClass->SetZoobenthosPointer2(PZoobenthos2);
        pEDCQueue->InsertObject((TObject*)PZoobenthos2);
        addAvailableVariables(PZoobenthos2, pVars);
    }
    else if (strcmp(className, "TBivalvesForSango") == 0) {
        PZoobenthos = new TChlamysFarreriV8(MyPEcoDynClass, "TChlamysFarreriV8");
        MyPEcoDynClass->SetZoobenthosPointer(PZoobenthos);    // Zoobenthos
        pEDCQueue->InsertObject((TObject*)PZoobenthos);
        addAvailableVariables(PZoobenthos, pVars);

        PZoobenthos2 = new TCrassostreaGigas7(MyPEcoDynClass, "TCrassostreaGigas7");
        MyPEcoDynClass->SetZoobenthosPointer2(PZoobenthos2);
        pEDCQueue->InsertObject((TObject*)PZoobenthos2);
        addAvailableVariables(PZoobenthos2, pVars);
    }
    else if (strcmp(className, "TRDecussatusIndivGrowthPhytoAndTPM") == 0) {
        PZoobenthos = new TRDecussatusIndivGrowthPhytoAndTPM(MyPEcoDynClass, className);
        MyPEcoDynClass->SetZoobenthosPointer(PZoobenthos);
        pEDCQueue->InsertObject((TObject*)PZoobenthos);
        addAvailableVariables(PZoobenthos, pVars);
    }
    else if (strcmp(className, "TChlamysFarreriIndividual") == 0) {
        PZoobenthos = new TChlamysFarreriIndividual(MyPEcoDynClass, className);
        MyPEcoDynClass->SetZoobenthosPointer(PZoobenthos);    // Zoobenthos
        pEDCQueue->InsertObject((TObject*)PZoobenthos);
        addAvailableVariables(PZoobenthos, pVars);
    }
    else if (strcmp(className, "TMytilusGalloprovincialisIndividual") == 0) {
        PZoobenthos = new TMytilusGalloprovincialisIndividual(MyPEcoDynClass, className);
        MyPEcoDynClass->SetZoobenthosPointer(PZoobenthos);    // Zoobenthos
        pEDCQueue->InsertObject((TObject*)PZoobenthos);
        addAvailableVariables(PZoobenthos, pVars);
    }
    else if (strcmp(className, "TMytilusGalloprovincialisIndividualDEB") == 0) {
        PZoobenthos = new TMytilusGalloprovincialisIndividualDEB(MyPEcoDynClass, className);
        MyPEcoDynClass->SetZoobenthosPointer(PZoobenthos);    // Zoobenthos
        pEDCQueue->InsertObject((TObject*)PZoobenthos);
        addAvailableVariables(PZoobenthos, pVars);
    }
    else if (strcmp(className, "TIrishMan") == 0) {
        PMan = new TIrishMan(MyPEcoDynClass, className);
        MyPEcoDynClass->SetManPointer(PMan);    // Man
        pEDCQueue->InsertObject((TObject*)PMan);
        addAvailableVariables(PMan, pVars);
    }
    else if (strcmp(className, "TSangoMan") == 0) {
        PMan = new TSangoMan(MyPEcoDynClass, className);
        MyPEcoDynClass->SetManPointer(PMan);    // Man
        pEDCQueue->InsertObject((TObject*)PMan);
        addAvailableVariables(PMan, pVars);
    }
    else if (strcmp(className, "TFisherMan") == 0) {
        PFisherMan = new TFisherMan(MyPEcoDynClass, className);
        MyPEcoDynClass->SetFisherPointer(PFisherMan);    // FisherMan
        pEDCQueue->InsertObject((TObject*)PFisherMan);
        addAvailableVariables(PFisherMan, pVars);
    }
    else if (strcmp(className, "TLaminariaJaponicaExponentialGrowth") == 0) {
        PMacrophytes = new TLaminariaJaponicaExponentialGrowth(
                MyPEcoDynClass, className);
        MyPEcoDynClass->SetMacrophytePointer(PMacrophytes);
        pEDCQueue->InsertObject((TObject*)PMacrophytes);
        addAvailableVariables(PMacrophytes, pVars);
    }
    else if (strcmp(className, "TEnteromorphaSp") == 0) {
        PMacrophytes = new TEnteromorphaSp(
                MyPEcoDynClass, className);
        MyPEcoDynClass->SetMacrophytePointer(PMacrophytes);
        pEDCQueue->InsertObject((TObject*)PMacrophytes);
        addAvailableVariables(PMacrophytes, pVars);
    }
    else if (strcmp(className, "TUlvaSp") == 0) {
        PMacrophytes = new TUlvaSp(
                MyPEcoDynClass, className);
        MyPEcoDynClass->SetMacrophytePointer(PMacrophytes);
        pEDCQueue->InsertObject((TObject*)PMacrophytes);
        addAvailableVariables(PMacrophytes, pVars);
    }
    else if (strcmp(className, "TSaltMarshForcing") == 0) {
        PMacrophytes = new TSaltMarshForcing(
                MyPEcoDynClass, className);
        MyPEcoDynClass->SetMacrophytePointer(PMacrophytes);
        pEDCQueue->InsertObject((TObject*)PMacrophytes);
        addAvailableVariables(PMacrophytes, pVars);
    }
    else if (strcmp(className, "TZosteraNoltii") == 0) {
        PMacrophytes = new TZosteraNoltii(
                MyPEcoDynClass, className);
        MyPEcoDynClass->SetMacrophytePointer(PMacrophytes);
        pEDCQueue->InsertObject((TObject*)PMacrophytes);
        addAvailableVariables(PMacrophytes, pVars);
    }
    else if (strcmp(className, "TSediments") == 0) {
        PSediments = new TSediments(MyPEcoDynClass, className);
        MyPEcoDynClass->SetSedimentPointer(PSediments);    // Sediments biogeochemistry
        pEDCQueue->InsertObject((TObject*)PSediments);
        addAvailableVariables(PSediments, pVars);
    }
    else if (strcmp(className, "TSedimentsTimeSeries") == 0) {
        PSediments = new TSedimentsTimeSeries(MyPEcoDynClass, className);
        MyPEcoDynClass->SetSedimentPointer(PSediments);    // Sediments biogeochemistry
        pEDCQueue->InsertObject((TObject*)PSediments);
        addAvailableVariables(PSediments, pVars);
    }
    else if (strcmp(className, "TSedimentsPlus") == 0) {
        PSediments = new TSedimentsPlus(MyPEcoDynClass, className);
        MyPEcoDynClass->SetSedimentPointer(PSediments);    // Sediments biogeochemistry
        pEDCQueue->InsertObject((TObject*)PSediments);
        addAvailableVariables(PSediments, pVars);
    }
    else if (strcmp(className, "TPrey") == 0) {
        PPrey = new TPrey(MyPEcoDynClass, className);
        MyPEcoDynClass->SetPreyPointer(PPrey);    // Generic prey
        pEDCQueue->InsertObject((TObject*)PPrey);
        addAvailableVariables(PPrey, pVars);
    }
    else if (strcmp(className, "TPredator") == 0) {
        PPredator = new TPredator(MyPEcoDynClass, className);
        MyPEcoDynClass->SetPredatorPointer(PPredator);    // Generic predator
        pEDCQueue->InsertObject((TObject*)PPredator);
        addAvailableVariables(PPredator, pVars);
    }
    else if (strcmp(className, "TDiplodusSargusDEB") == 0) {
        PFish = new TDiplodusSargusDEB(MyPEcoDynClass, className);
        MyPEcoDynClass->SetFishPointer(PFish);
        pEDCQueue->InsertObject((TObject*)PFish);
        addAvailableVariables(PFish, pVars);
    }
    else if (strcmp(className, "TSparusAurataDEB") == 0) {
        PFish = new TSparusAurataDEB(MyPEcoDynClass, className);
        MyPEcoDynClass->SetFishPointer(PFish);
        pEDCQueue->InsertObject((TObject*)PFish);
        addAvailableVariables(PFish, pVars);
    }
    else if (strcmp(className, "TDiplodusSargusDEBWithWeakHomeostasis") == 0) {
        PFish = new TDiplodusSargusDEBWithWeakHomeostasis(MyPEcoDynClass, className);
        MyPEcoDynClass->SetFishPointer(PFish);
        pEDCQueue->InsertObject((TObject*)PFish);
        addAvailableVariables(PFish, pVars);
    }
    else if (strcmp(className, "TSparusAurataDEBWithWeakHomeostasis") == 0) {
        PFish = new TSparusAurataDEBWithWeakHomeostasis(MyPEcoDynClass, className);
        MyPEcoDynClass->SetFishPointer(PFish);
        pEDCQueue->InsertObject((TObject*)PFish);
        addAvailableVariables(PFish, pVars);
    }
    else if (strcmp(className, "TEscherichiaColi") == 0) {
        PBacteria = new TEscherichiaColi(MyPEcoDynClass, className);
        MyPEcoDynClass->SetBacteriaPointer(PBacteria);
        pEDCQueue->InsertObject((TObject*)PBacteria);
        addAvailableVariables(PBacteria, pVars);
    }
    else if (strcmp(className, "TIce") == 0) {
        PSeaIce = new TIce(MyPEcoDynClass, className);
        MyPEcoDynClass->SetSeaIcePointer(PSeaIce);
        pEDCQueue->InsertObject((TObject*)PSeaIce);
        addAvailableVariables(PSeaIce, pVars);
    }
    else if (strcmp(className, "TIceTimeSeries") == 0) {
        PSeaIce = new TIceTimeSeries(MyPEcoDynClass,className);
        MyPEcoDynClass->SetSeaIcePointer(PSeaIce);
        pEDCQueue->InsertObject((TObject*)PSeaIce);
        addAvailableVariables(PSeaIce, pVars);
    }
    else if (strcmp(className, "TIceAlgae") == 0) {
        PIceAlgae = new TIceAlgae(MyPEcoDynClass, className);
        MyPEcoDynClass->SetIceAlgaePointer(PIceAlgae);
        pEDCQueue->InsertObject((TObject*)PIceAlgae);
        addAvailableVariables(PIceAlgae, pVars);
    }
}

