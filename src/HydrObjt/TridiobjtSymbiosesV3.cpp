#ifdef __BORLANDC__
#include <math.h>
#else  // __BORLANDC__
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;
#endif  // __BORLANDC__

#define RAW_GRIDS
#define ENABLE_ADVECT
//#define POINT_GRID
#include "ecodyn.rh"
#include "hydrobjt.h"
#include "iodll.h"
#include "SymbiosesFramework.h"
#include "HydrodynamicModel.h"
#include "AtmosphericModel.h"


TTriDimensionalSymbiosesV3::TTriDimensionalSymbiosesV3(TEcoDynClass* PEcoDynClass,
        char* className) : TTriDimensionalSymbioses(PEcoDynClass, className)
{
    MyDimensions = (GridColumns + 2) * (GridLines + 2) * GridLayers;
    field = new float[MyDimensions];
    MySalt = new float[MyDimensions];
    for (int i = 0; i < MyDimensions; i++){
        field[i] = 0.0;MySalt[i] = 0.0;
    }
    int X,Y, XV, YV;
    double MyValue;
    char MyVariable[65];
    TReadWrite* PReadWrite  = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Transport");
    if (PReadWrite != NULL)
    {
        if (PReadWrite->FindString("Transport", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Array for state variable names created in TOneDimensionalDITransport
            // but not filled because the direction is wrong
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
                    for (int j = 0; j < MyDimensions; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, MyValue);
                        MySalt[j] = MyValue;
                    }

                }

            }
        }
        int MyColumn, MyLine;
        for (int k = 0; k < GridLayers; k++)
        {
            for (int i = 0; i < GridLines + 2; i++)
            {
                MyLine = i - 1;
                for (int j = 0; j < GridColumns + 2; j++)
                {
                    if ((j < GridColumns + 1) && (j > 0) && (i < GridLines + 1) && (i > 0))
                    {
                        MyColumn = j - 1;
                        Salt[Get3DIndex(MyLine,MyColumn,k)] =  MySalt[Get3DIndexWithBoundaries(i,j,k)];
                    }
                }
            }
        }
        //cout<<"Teste terminado"<<endl;
    }
    else
    {
#ifdef __BORLANDC__
        MessageBox(0, "Variables: Undefined object Transport", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TTriDimensional::BuildTriDimensionalIntertidal - variables: undefined object" << endl;
#endif  // __BORLANDC__
    }
    //End reading variables file
    CloseDataFile((void*)PReadWrite);
}

TTriDimensionalSymbiosesV3::~TTriDimensionalSymbiosesV3()
{
    freeMemory();
}

void TTriDimensionalSymbiosesV3::freeMemory()
{
    delete[] field;
    delete[] MySalt;
}

void TTriDimensionalSymbiosesV3::Go()   //This overwrites previous Go to prevent EcoDynamo from calulating hydrodynamics
{
    cout<<"Start Go TTriDimensionalSymbiosesV3"<<endl;
    GenericLoad = SaltLoad;
    cout<<"AdvectDiffuse"<<endl;
    AdvectDiffuse(Salt,MySalt); //Salinity being transported - once we get this from SYMBIOSES this line should be removed
    cout<<"End Go TTriDimensionalSymbiosesV3"<<endl;
}

void TTriDimensionalSymbiosesV3::AdvectDiffuse(double* Generic,float *GenericWithBoundaries)
{

    float MyLat, MyLong, layers[GridLayers];
    int index3D;
    index3D = 0;
    for (int k = GridLayers - 1; k >= 0; k--)  //From surface to bottom in EcoDynamo
    {
        for (int j = GridColumns + 1; j >= 0; j--)  // i and j are read in inverse order because origin in SINMOD seems to be located in the South-Eastern corner
        {
            for (int i = GridLines + 1; i >= 0; i--)  //Column major order for Fortra
            {
                field[index3D] = GenericWithBoundaries[Get3DIndexWithBoundaries(i,j,k)];
                index3D++;
            }
        }
    }
    /* saveField("c2f.dat", GenericWithBoundaries, MyDimensions); */

    ocean->advectUVW(field);

    int MyLine, MyColumn;
    MyLine = 0; MyColumn = 0; index3D = 0;
    for (int k = GridLayers - 1; k >= 0; k--)  //From surface to bottom in EcoDynamo
    {
        for (int j = GridColumns + 1; j >= 0; j--)
        {
            MyColumn = j - 1;
            for (int i = GridLines + 1; i >= 0; i--)//Column major order for Fortran
            {
                if ((j < GridColumns + 1)
                        && (j > 0) && (i < GridLines + 1) && (i > 0)) {
                    MyLine = i - 1;
                }
                Generic[Get3DIndex(MyLine,MyColumn,k)] =  field[index3D];
                GenericWithBoundaries[Get3DIndexWithBoundaries(i,j,k)] =  field[index3D];
                index3D++;
            }
            //index3D++;
        }
    }
    /* saveField("f2c.dat", GenericWithBoundaries, MyDimensions); */
}

void TTriDimensionalSymbiosesV3::saveField(const char *fname, float *fld, int dim) {
    ofstream f;
    f.open(fname);
    for (int i=0; i < dim; i++) {
        f << fld[i] << endl;
    }
    f.close();
}





