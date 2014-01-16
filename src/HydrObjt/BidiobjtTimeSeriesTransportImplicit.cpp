// Bidimensional hydrodynamic model object CPP code file
// 	@ Pedro Duarte
// Rev. A 2005.12.22
/**
 * NPI work
 * 	15 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
    #include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "hydrobjt.h"
#include "iodll.h"

#include <algorithm>
#define SWAP(a,b) {dum=(a);(a)=(b);(b)=dum;}
#define TINY 1.0e-20
//#include "MLArrays.h"

TBiDimensionalTimeSeriesTransportImplicit::TBiDimensionalTimeSeriesTransportImplicit(TEcoDynClass* PEcoDynClass,
        		char* className, bool readValues, bool cyclic,
                int nrOfPeriods, SimulationPeriod* sPeriods)
        :TBiDimensionalTimeSeries(PEcoDynClass, className, readValues, cyclic,
                nrOfPeriods, sPeriods)
{
   int index;
   ATimeStep = MyPEcoDynClass->GetTimeStep() / 2.0;
   //Para compensar a multiplicação por 2 nos objectos ascendentes, feita de modo a seguir as equações da tese do Ramiro
   //em que o passo de cálculo é dividido em 2
   DeltaX = new double[NumberOfBoxes];
   DeltaY = new double[NumberOfBoxes];
   d = new double[NumberOfBoxes + 1];
   e = new double[NumberOfBoxes + 1];
   MyIndex = new int[NumberOfBoxes];
   //M = new double[NumberOfBoxes * NumberOfBoxes];
   //M = new double[5];
   MyCellsRecord = new CellsRecord[NumberOfBoxes];

   index = 0;
   for (int i = 0; i < NumberOfBoxes + 1; i++)
   {
       d[i] = 0.0; e[i] = 0.0;
       if (i < NumberOfBoxes)
       {
          DeltaX[i] = MyPEcoDynClass->GetBoxLength(1); DeltaY[i] = MyPEcoDynClass->GetBoxLength(1);
          MyCellsRecord[i].LineCoordinate = MyPEcoDynClass->GetLineNumber(i);
          MyCellsRecord[i].ColumnCoordinate = MyPEcoDynClass->GetColumnNumber(i);
          MyCellsRecord[i].CellNumber = i;
          MyCellsRecord[i].AnIndex = i;
          MyIndex[i] = i;
          /*for (int j = 0; j < NumberOfBoxes; j++)
          {
             M[index] = 0.0;
             index++;
          } */
       }
   }
   /*MyMLArrays = new MLArrays();
   if (!MyMLArrays->setupMLArrays(ep)) {
            MyMLArrays = NULL;
            return;
        }*/
   /*double Array1[25],Array2[5], Result[5];
   Array1[0] = 1;  Array1[1] = 2; Array1[2] = 0; Array1[3] = 0; Array1[4] = 0;
   Array1[5] = 6; Array1[6] = 7; Array1[7] = 8; Array1[8] = 10; Array1[9] = 9;
   Array1[10] = 3; Array1[11] = 7; Array1[12] = 8; Array1[13] = 9; Array1[14] = 9;
   Array1[15] = 6; Array1[16] = 7; Array1[17] = 8; Array1[18] = 3; Array1[19] = 1;
   Array1[20] = 0; Array1[21] = 0; Array1[22] = 8; Array1[23] = 9; Array1[24] = 9;

   Array2[0] = 1;  Array2[1] = 2; Array2[2] = 3; Array2[3] = 4; Array2[4] = 5;
   MyMLArrays->SolveSystem(5, Array1, Array2, M);
   for (int i = 0; i < 5; i++)
           Debugger(M[i]);                                                    */
}

TBiDimensionalTimeSeriesTransportImplicit::~TBiDimensionalTimeSeriesTransportImplicit()
{
    freeMemory();
}

void TBiDimensionalTimeSeriesTransportImplicit::freeMemory()
{
    try {
        delete [] DeltaX;
        delete [] DeltaY;
        delete [] d;
        delete [] e;
    /*    delete [] M1;
        delete [] M2;
        delete [] M3;*/
        delete [] MyCellsRecord;
        delete [] MyIndex;
        /*delete [] A;
        delete [] B;
        delete [] Indx;*/
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
        cerr << "TBiDimensionalTimeSeriesTransportImplicit::freeMemory - " << exc.what() << endl;
    }
#endif  // __BORLANDC__
}

void TBiDimensionalTimeSeriesTransportImplicit::FillMatrix(int n)
{
   int index, i, j, AIndex, DIndex, EIndex, ALine, kk;
   double Distance;
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
   m1 = 0; m2 = 0;

   for (int m = 0; m < n; m++)  //n corresponde ao número de células "activas", ou seja, as células do sub-domínio cujo dummybool2 = true
   {
      B[m + 1] = r[m]; //Matriz vector "one-based - a matriz é utilizada a partir do segundo elemento de índice "1"
      index = MyIndex[m]; //MyIndex é um pointer que contem o índice de cada célula activa (ver AdvectDiffuse)
      i = MyPEcoDynClass->GetLineIndex(index);
      j = MyPEcoDynClass->GetColumnIndex(index);
      for (int k = 0; k < n; k++)
      {
         AIndex = m * n + k + 1;   //AIndex é o índice corrente da matriz A "one-based" que de seguida é preenchida
         if (k == m - 1) A[AIndex] = a[m];  //Preenche diagonal inferior à diagonal principal
         else if (k == m) A[AIndex] = b[m];  //Preenche diagonal principal
         else if (k == m + 1) A[AIndex] = c[m];  //Preenche diagonal superior à diagonal principal
         else A[AIndex] = 0.0; //Preenche restantes valores com zeros
      }
      //Determinação das coordenadas dos valores de "d" e de "e" e actualização de A

      if (i > 0)
      {
         DIndex = (i - 1) * GridColumns + j;    //Índice da célula situada a Sul
         ALine = i - 1;
                                //Número da linha situada a Sul
         if (
               (ALine > pSubDomain->ILine[j]) &&
               (BoxArray[DIndex].Dummybool2) &&
               (BoxArray[DIndex].BoxToBoundary[1].TypeOfBoundary != 2) &&
               (BoxArray[DIndex].BoxToBoundary[2].TypeOfBoundary != 2) &&
               (BoxArray[DIndex].BoxToBoundary[3].TypeOfBoundary != 2) &&
               (BoxArray[DIndex].BoxToBoundary[4].TypeOfBoundary != 2) &&
               (BoxArray[DIndex].Dummybool1)
            )
         {
            kk = MyCellsRecord[DIndex].AnIndex;  //Número de ordem (de entre todas as células activas) da célula situada a Sul
            A[m * n + kk + 1] = d[m];            //O número de ordem anterior é usado para determinar a posição do valor de "d" na linha "m" da matriz A
            Distance = (m * n + m + 1) - (m * n + kk + 1);
            if (Distance > m1) m1 = Distance;

         }
      }
      if (i < GridLines - 1)
      {
         EIndex = (i + 1) * GridColumns + j;     //Índice da célula situada a Norte
         ALine = i + 1;                          //Número da linha situada a Norte
         if (
               (ALine < pSubDomain->FLine[j]) &&
               (BoxArray[EIndex].Dummybool2) &&
               (BoxArray[EIndex].BoxToBoundary[1].TypeOfBoundary != 2) &&
               (BoxArray[EIndex].BoxToBoundary[2].TypeOfBoundary != 2) &&
               (BoxArray[EIndex].BoxToBoundary[3].TypeOfBoundary != 2) &&
               (BoxArray[EIndex].BoxToBoundary[4].TypeOfBoundary != 2) &&
               (BoxArray[EIndex].Dummybool1)
            )
         {
            kk = MyCellsRecord[EIndex].AnIndex;  //Número de ordem (de entre todas as células activas) da célula situada a Norte
            A[m * n + kk + 1] = e[m];            //O número de ordem anterior é usado para determinar a posição do valor de "e" na linha "m" da matriz A
            Distance = (m * n + kk + 1) - (m * n + m + 1);
            if (Distance > m2) m2 = Distance;
         }
      }
   }
}

/*void TBiDimensionalTimeSeriesTransportImplicit::CompactMatrix(int n)
{
   int n1, n3, I1, M, index1, index2, NumberOfColumnsInCompactedMatrix;
   double const Code = -9.999999;
   NumberOfColumnsInCompactedMatrix = m1 + m2 + 1;
   //Debugger(n * NumberOfColumnsInCompactedMatrix);
   CompactedMatrix = new double[n * NumberOfColumnsInCompactedMatrix];
   n1 = n - m1; //Number of elements of first subdiagonal to start with and then update for each diagonal
   I1 = n - n1; //First line with non wasted elements in first subdiagonal to update in subsquente diagonals
   n3 = n - 1; //Number of elements of first supradiagonal to update in subsquente diagonals
   M = m1;
   for (int i = 0; i < NumberOfColumnsInCompactedMatrix; i++)
   {
      //Introdução de códigos nos valores que não serão utizados na matriz compactada
      if (M > 0)
      {
         for (int j = 0; j < I1; j++)
            CompactedMatrix[j * NumberOfColumnsInCompactedMatrix + i] = Code;
         I1--;
      }
      if (M < 0)
      {
         for (int j = n3; j < n; j++)
            CompactedMatrix[j * NumberOfColumnsInCompactedMatrix + i] = Code;
         n3--;
      }
      for (int j = 0; j < n1; j++)
      {
         if (M >= 0)
         {
            index1 = (j + M) * n + j;
            index2 = (j + M) * NumberOfColumnsInCompactedMatrix + i;
         }
         else
         {
            index1 = j * n + j - M;
            index2 = j * NumberOfColumnsInCompactedMatrix + i;
         }
         CompactedMatrix[index2] = A[index1];
      }
      if (M > 0) n1++;
      else n1--;
      M--;
   }
}*/


void TBiDimensionalTimeSeriesTransportImplicit::CompactMatrix(int n)
{
   int n1, n3, I1, M, index1, index2, NumberOfColumnsInCompactedMatrix;
   double const Code = -9999999;
   NumberOfColumnsInCompactedMatrix = m1 + m2 + 1;
   //Debugger(n * NumberOfColumnsInCompactedMatrix);
   CompactedMatrix = new double[n * NumberOfColumnsInCompactedMatrix + 1];
   n1 = n - m1; //Number of elements of first subdiagonal to start with and then update for each diagonal
   I1 = n - n1 + 1; //First line with non wasted elements in first subdiagonal to update in subsquente diagonals
   n3 = n - 1; //Number of elements of first supradiagonal to update in subsquente diagonals
   M = m1;
   for (int i = 1; i <= NumberOfColumnsInCompactedMatrix; i++)
   {
      //Introdução de códigos nos valores que não serão utizados na matriz compactada
      if (M > 0)
      {
         for (int j = 1; j <= I1; j++)
            CompactedMatrix[(j - 1) * NumberOfColumnsInCompactedMatrix + i] = Code;
         I1--;
      }
      if (M < 0)
      {
         for (int j = n3 + 1; j <= n; j++)
            CompactedMatrix[(j - 1) * NumberOfColumnsInCompactedMatrix + i] = Code;
         n3--;
      }
      for (int j = 1; j <= n1; j++)
      {
         if (M >= 0)
         {
            index1 = (j - 1 + M) * n + j;
            index2 = (j - 1 + M) * NumberOfColumnsInCompactedMatrix + i;
         }
         else
         {
            index1 = (j - 1) * n + j - M;
            index2 = (j - 1) * NumberOfColumnsInCompactedMatrix + i;
         }
         CompactedMatrix[index2] = A[index1];
      }
      if (M > 0) n1++;
      else n1--;
      M--;
   }
}


void TBiDimensionalTimeSeriesTransportImplicit::AdvectDiffuse(double *Generic)
{
	 double MyWestUFlow, /*MyEastUFlow, MyNorthVFlow, MySouthVFlow,*/MyWestUDischarge, MyEastUDischarge, MyNorthVDischarge, MySouthVDischarge,
                MyWestDiffusion, MyEastDiffusion, MyNorthDiffusion, MySouthDiffusion, ResultantDiffusion;
         int i,j,index, k, ADay, m, kk;
         SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    double* PastGeneric = new double[NumberOfBoxes];

         for (i = 0; i < NumberOfBoxes; i++)
   	        PastGeneric[i] = Generic[i];

         kk = 0; FinalLine = 0;
         for (m = 0; m < MyNumberOfBoxes; m++)
         {
            i = MyPEcoDynClass->GetLineIndex(MyBoxNumber[m]);
            j = MyPEcoDynClass->GetColumnIndex(MyBoxNumber[m]);
            if (
                  (i >= pSubDomain->ILine[j])
                  && (i <= pSubDomain->FLine[j])
                  && (j >= pSubDomain->IColumn[i])
                  && (j <= pSubDomain->FColumn[i])
                )
            {
            index = i * GridColumns + j;

            if (
                    (BoxArray[index].Dummybool2) &&
                    (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 2) &&
                    (BoxArray[index].Dummybool1)
                )
            {
                if (kk == 0) InitialLine = MyPEcoDynClass->GetLineIndex(index);
                r[kk] = PastGeneric[index] * BoxDepth[index] / (BoxDepth[index]+ DynamicHeight[index] - PastHeight[index]);
                a[kk] = ACalculation(i,j); b[kk] = BCalculation(i,j); c[kk] = CCalculation(i,j);
                d[kk] = DCalculation(i,j); e[kk] = ECalculation(i,j);
                if ((index > 0) &&
                    ((j == pSubDomain->IColumn[i]) ||
                     (BoxArray[index - 1].BoxToBoundary[4].TypeOfBoundary == 2) ||
                     (!BoxArray[index - 1].Dummybool1))
                   )
                {
                   r[kk] = r[kk] - a[kk] * PastGeneric[index - 1];
                   a[kk] = 0.0;
                }
                if ((index < NumberOfBoxes - 1) &&
                    ((j == pSubDomain->FColumn[i]) ||
                     (BoxArray[index + 1].BoxToBoundary[2].TypeOfBoundary == 2) ||
                     (!BoxArray[index + 1].Dummybool1))
                   )
                {
                   r[kk] = r[kk] - c[kk] * PastGeneric[index + 1];
                   c[kk] = 0.0;
                }
                if ((i > 0) &&
                    ((i == pSubDomain->ILine[j]) ||
                     (BoxArray[(i - 1) * GridColumns + j].BoxToBoundary[3].TypeOfBoundary == 2) ||
                     (!BoxArray[(i - 1) * GridColumns + j].Dummybool1))
                   )
                {
                   r[kk] = r[kk] - d[kk] * PastGeneric[(i - 1) * GridColumns + j];
                   d[kk] = 0.0;
                }
                if ((i < GridLines - 1) &&
                    ((i == pSubDomain->FLine[j]) ||
                     (BoxArray[(i + 1) * GridColumns + j].BoxToBoundary[1].TypeOfBoundary == 2) ||
                     (!BoxArray[(i + 1) * GridColumns + j].Dummybool1))
                   )
                {
                   r[kk] = r[kk] - e[kk] * PastGeneric[(i + 1) * GridColumns + j];
                   e[kk] = 0.0;
                }
                MyCellsRecord[index].AnIndex = kk;
                MyIndex[kk] = index;
                kk++;
                FinalLine = MAX(FinalLine,MyPEcoDynClass->GetLineIndex(index));
      	    }
            }
         }
         //InitialLine = MyPEcoDynClass->GetLineIndex(MyIndex[0]);
         //FinalLine = MyPEcoDynClass->GetLineIndex(MyIndex[kk]);
         //Debugger(kk); Debugger(MyIndex[kk]);
         //Debugger(InitialLine); Debugger(FinalLine);
         //Debugger(1);
         //FillMatrix(kk);

         /*A = new double[49];
         A[0] = 3;  A[1] = 1; A[2] = 0; A[3] = 0; A[4] = 0;A[5] = 0; A[6] = 0;
         A[7] = 4; A[8] = 1; A[9] = 5; A[10] = 0; A[11] = 0; A[12] = 0; A[13] = 0;
         A[14] = 9; A[15] = 2; A[16] = 6; A[17] = 5; A[18] = 0; A[19] = 0; A[20] = 0;
         A[21] = 0; A[22] = 3; A[23] = 5; A[24] = 8; A[25] = 9; A[26] = 0;A[27] = 0;
         A[28] = 0; A[29] = 0; A[30] = 7; A[31] = 9; A[32] = 3; A[33] = 2;A[34] = 0;
         A[35] = 0; A[36] = 0; A[37] = 0; A[38] = 3; A[39] = 8; A[40] = 4;A[41] = 6;
         A[42] = 0; A[43] = 0; A[44] = 0; A[45] = 0; A[46] = 2; A[47] = 4;A[48] = 4;  */

          /*B = new double[7];
         B[0] = 1;  B[1] = 2; B[2] = 3; B[3] = 4; B[4] = 5; B[5] = 6; B[6] = 7; */


         /*A = new double[50];
         A[0] = 0;
         A[1] = 3;  A[2] = 1; A[3] = 0; A[4] = 0; A[5] = 0;A[6] = 0; A[7] = 0;
         A[8] = 4; A[9] = 1; A[10] = 5; A[11] = 0; A[12] = 0; A[13] =  1; A[14] = 0;
         A[15] = 9; A[16] = 2; A[17] = 6; A[18] = 5; A[19] = 0; A[20] = 0; A[21] = 0;
         A[22] = 0; A[23] = 3; A[24] = 5; A[25] = 8; A[26] = 9; A[27] = 0;A[28] = 0;
         A[29] = 0; A[30] = 0; A[31] = 7; A[32] = 9; A[33] = 3; A[34] = 2;A[35] = 0;
         A[36] = 0; A[37] = 0; A[38] = 0; A[39] = 3; A[40] = 8; A[41] = 4;A[42] = 6;
         A[43] = 0; A[44] = 0; A[45] = 0; A[46] = 0; A[47] = 2; A[48] = 4;A[49] = 4;

         B = new double[8];
         B[0] = 0;
         B[1] = 1;  B[2] = 2; B[3] = 3; B[4] = 4; B[5] = 5; B[6] = 6; B[7] = 7;
         m1 = 2; m2 = 4;
         LowerTriangularMatrix= new double [8 * m1]; Indx = new int[8];*/


         //FillMatrix(7);
         //CompactMatrix(7);
         //Debugger(3);
         //bandec(7, m1, m2);
         //banbks(7, m1, m2);
         //for (int aa = 1; aa <= 7; aa++)
         //   Debugger(B[aa]);
         //for (int aa = 1; aa <= 49; aa++)
         //   Debugger(CompactedMatrix[aa]);
         //Debugger(4);
         //Indx = new int[8]; //a1 = new double [kk * m1];
         //UpperTriangularMatrix = new double[8 * m1];
         A = new double [kk * kk + 1]; B = new double[kk + 1];
         Indx = new int[kk + 1];
         dd = 0.0;

         //bandec(7, m1, m2);
         //bandec(kk, m1, m2);
         //Debugger(66);
         //banbks(/*float a[][max], */7, m1, m2/*, float al[][max], int indx[], float b[]*/);
         //Debugger(1);
         FillMatrix(kk);
         //Debugger(m1); Debugger(m2);
         //Debugger(2);
         CompactMatrix(kk);
         LowerTriangularMatrix = new double [kk * m1 + 1];
         //Debugger(kk * m1 + 1);
         //Debugger(3);
         bandec(kk, m1, m2);
         //Debugger(4);
         banbks(kk, m1, m2);
         //Debugger(5);

         //Debugger(222);
         /*MyMLArrays = new MLArrays();
         if (!MyMLArrays->setupMLArrays(ep)) {
            MyMLArrays = NULL;
            return;
          }
         MyMLArrays->SolveSystem(kk, M1, M2, M3);*/
         /*A = new double[25]; B = new double[5]; Indx = new int[5];
         A[0] = 1;  A[1] = 2; A[2] = 0; A[3] = 0; A[4] = 0;
         A[5] = 6; A[6] = 7; A[7] = 8; A[8] = 10; A[9] = 9;
         A[10] = 3; A[11] = 7; A[12] = 8; A[13] = 9; A[14] = 9;
         A[15] = 6; A[16] = 7; A[17] = 8; A[18] = 3; A[19] = 1;
         A[20] = 0; A[21] = 0; A[22] = 8; A[23] = 9; A[24] = 9;

         B[0] = 1;  B[1] = 2; B[2] = 3; B[3] = 4; B[4] = 5;

         Ludcmp(5, A, Indx);
         Debugger(3);
         Lubksb(5, A, Indx, B);
         Debugger(4);
         for (int aa = 0; aa < 5; aa++)
            Debugger(B[aa]); */



         /*Ludcmp(kk, A, Indx);
         Debugger(3);
         Lubksb(kk, A, Indx, B);
         Debugger(4); */
         for (int m = 0; m < kk; m++)  //n corresponde ao número de células "activas", ou seja, as células do sub-domínio cujo dummybool2 = true
         {
            index = MyIndex[m]; //MyIndex é um pointer que contem o índice de cada célula activa (ver AdvectDiffuse
            Generic[index] = B[m + 1];
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

                            Generic[index ] = Generic[index ] + ATimeStep / 2.0 * GenericLoad[index] *
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
         if (A != NULL) delete [] A;
         if (B != NULL) delete [] B;
         if (Indx != NULL) delete [] Indx;
         if (LowerTriangularMatrix != NULL) delete [] LowerTriangularMatrix;
    delete [] PastGeneric;
}

double TBiDimensionalTimeSeriesTransportImplicit::ACalculation(int i, int j)
{
   int index = i * GridColumns + j;
   double MyValue, UpdatedDepth;
   MyValue = 0.0;
   if ((j > 0) && (BoxArray[index - 1].Dummybool2))
   {
      UpdatedDepth = BoxDepth[index] + DynamicHeight[index] - PastHeight[index];
      if (UFlowBetweenBoxes[i * (GridColumns + 1) + j] > 0.0)
      {
         MyValue =  -(
                        ATimeStep *
         	        UFlowBetweenBoxes[i * (GridColumns + 1) + j]
                     ) / (DeltaX[index] * DeltaY[index] * UpdatedDepth);
      }
      MyValue = MyValue - ATimeStep * HorizontalDiffCoef * (BoxDepth[index - 1] + BoxDepth[index]) /
                          (2.0 * DeltaX[index] * DeltaX[index] * UpdatedDepth);
   }
   return MyValue;
}

double TBiDimensionalTimeSeriesTransportImplicit::BCalculation(int i, int j)
{
   int index = i * GridColumns + j;
   double MyValue, UpdatedDepth;
   MyValue = 0.0;
   MyValue = MyValue + 1;
   UpdatedDepth = BoxDepth[index] + DynamicHeight[index] - PastHeight[index];
   if ((j > 0) && (BoxArray[index - 1].Dummybool2))
   {
      if (UFlowBetweenBoxes[i * (GridColumns + 1) + j] < 0.0)
      {
         MyValue =  -(
                        ATimeStep *
         	        UFlowBetweenBoxes[i * (GridColumns + 1) + j]
                     ) / (DeltaX[index] * DeltaY[index] * UpdatedDepth);
      }
      MyValue = MyValue  + ATimeStep * HorizontalDiffCoef * (BoxDepth[index - 1] + BoxDepth[index]) /
                          (2.0 * DeltaX[index] * DeltaX[index] * UpdatedDepth);
   }
   if ((j < GridColumns - 1) && (BoxArray[index + 1].Dummybool2))
   {
      if (UFlowBetweenBoxes[i * (GridColumns + 1) + j + 1] > 0.0)
      {
         MyValue =  MyValue +
                    (
                        ATimeStep *
         	        UFlowBetweenBoxes[i * (GridColumns + 1) + j + 1]
                     ) / (DeltaX[index] * DeltaY[index] * UpdatedDepth);
      }
      MyValue = MyValue + ATimeStep * HorizontalDiffCoef * (BoxDepth[index] + BoxDepth[index + 1]) /
                          (2.0 * DeltaX[index] * DeltaX[index] * UpdatedDepth);
   }
   if ((i > 0) && (BoxArray[(i - 1) * GridColumns + j].Dummybool2))
   {
      if (VFlowBetweenBoxes[index] < 0.0)
      {
         MyValue = MyValue +
                    -(
                        ATimeStep *
         	        VFlowBetweenBoxes[index]
                     ) / (DeltaX[index] * DeltaY[index] * UpdatedDepth);
      }
      MyValue = MyValue + ATimeStep * HorizontalDiffCoef * (BoxDepth[(i - 1) * GridColumns + j] + BoxDepth[index]) /
                          (2.0 * DeltaX[index] * DeltaX[index] * UpdatedDepth);
   }
   if ((i < GridLines - 1) && (BoxArray[(i + 1) * GridColumns + j].Dummybool2))
   {
      if (VFlowBetweenBoxes[(i + 1) * GridColumns + j] > 0.0)
      {
         MyValue =  MyValue +
                     (
                        ATimeStep *
         	        VFlowBetweenBoxes[(i + 1) * GridColumns + j]
                     ) / (DeltaX[index] * DeltaY[index] * UpdatedDepth);
      }
      MyValue = MyValue + ATimeStep * HorizontalDiffCoef * (BoxDepth[index] + BoxDepth[(i + 1) * GridColumns + j]) /
                          (2.0 * DeltaX[index] * DeltaX[index] * UpdatedDepth);
   }
   return MyValue;
}

double TBiDimensionalTimeSeriesTransportImplicit::CCalculation(int i, int j)
{
   int index = i * GridColumns + j;
   double MyValue, UpdatedDepth;
   MyValue = 0.0;
   if ((j < GridColumns - 1) && (BoxArray[index + 1].Dummybool2))
   {
      UpdatedDepth = BoxDepth[index] + DynamicHeight[index] - PastHeight[index];
      if (UFlowBetweenBoxes[i * (GridColumns + 1) + j + 1] < 0.0)
      {
         MyValue =  (
                        ATimeStep *
         	        UFlowBetweenBoxes[i * (GridColumns + 1) + j + 1]
                     ) / (DeltaX[index] * DeltaY[index] * UpdatedDepth);
      }
      MyValue = MyValue - ATimeStep * HorizontalDiffCoef * (BoxDepth[index] + BoxDepth[index + 1]) /
                          (2.0 * DeltaX[index] * DeltaX[index] * UpdatedDepth);
   }
   return MyValue;
}

double TBiDimensionalTimeSeriesTransportImplicit::DCalculation(int i, int j)
{
   int index = i * GridColumns + j;
   double MyValue, UpdatedDepth;
   MyValue = 0.0;
   if ((i > 0) && (BoxArray[(i - 1) * GridColumns + j].Dummybool2))
   {
      UpdatedDepth = BoxDepth[index] + DynamicHeight[index] - PastHeight[index];
      if (VFlowBetweenBoxes[index] > 0.0)
      {
         MyValue = MyValue +
                    -(
                        ATimeStep *
         	        VFlowBetweenBoxes[index]
                     ) / (DeltaX[index] * DeltaY[index] * UpdatedDepth);
      }
      MyValue = MyValue - ATimeStep * HorizontalDiffCoef * (BoxDepth[(i - 1) * GridColumns + j] + BoxDepth[index]) /
                          (2.0 * DeltaX[index] * DeltaX[index] * UpdatedDepth);
   }
   return MyValue;
}

double TBiDimensionalTimeSeriesTransportImplicit::ECalculation(int i, int j)
{
   int index = i * GridColumns + j;
   double MyValue, UpdatedDepth;
   MyValue = 0.0;
   if ((i < GridLines - 1) && (BoxArray[(i + 1) * GridColumns + j].Dummybool2))
   {
      UpdatedDepth = BoxDepth[index] + DynamicHeight[index] - PastHeight[index];
      if (VFlowBetweenBoxes[(i + 1) * GridColumns + j] < 0.0)
      {
         MyValue =  MyValue +
                     (
                        ATimeStep *
         	        VFlowBetweenBoxes[(i + 1) * GridColumns + j]
                     ) / (DeltaX[index] * DeltaY[index] * UpdatedDepth);
      }
      MyValue = MyValue - ATimeStep * HorizontalDiffCoef * (BoxDepth[index] + BoxDepth[(i + 1) * GridColumns + j]) /
                          (2.0 * DeltaX[index] * DeltaX[index] * UpdatedDepth);
   }
   return MyValue;
}


//void TBiDimensionalTimeSeriesTransportImplicit::bandec(/*double a[][max],*/ int n, int m1, int m2/*, double *a1/*al[][max],
//	int indx[], float *dd*/)
/*{
    int i, j, k, l;
    int mm;
    double dum;
    mm = m1 + m2 + 1;
    l = m1;
    for (i = 0; i < m1; i++) {
	for (j = m1 - i; j < mm; j++) CompactedMatrix[i * mm + j - 1] = CompactedMatrix[i * mm + j];
	l--;
	for (j = mm - l - 1; j < mm; j++) CompactedMatrix[i * mm + j] = 0.0;
    }
    //*dd = 1.0;
    dd = 1.0;
    l = m1;
    for (k = 0; k < n; k++) {
	dum = CompactedMatrix[k * mm];
     	i = k;
	if (l < n) l++;
	for (j = k + 1; j < l; j++) {
	    if (fabs(CompactedMatrix[j * mm]) > fabs(dum)) {
		dum = CompactedMatrix[j * mm];
		i = j;
	    }
	}
    	Indx[k] = i;
    	if (dum == 0.0) CompactedMatrix[k * mm] = TINY;
    	if (i != k) {
	    //*dd = - (*dd);
            dd = -dd;
  	    for (j = 0; j < mm; j++) SWAP(CompactedMatrix[k * mm + j], CompactedMatrix[i * mm + j])
    	}
    	for (i = k + 1; i < l; i++) {
	    dum = CompactedMatrix[i * mm] / CompactedMatrix[k * mm];
	    LowerTriangularMatrix[k * m1 + i - k ] = dum;
	    for (j = 1; j < mm; j++)
               CompactedMatrix[i * mm + j - 1] = CompactedMatrix[i * mm + j] - dum * CompactedMatrix[k * mm + j];
	    //a[i][mm - 1] = 0.0;
            CompactedMatrix[i * mm + mm - 1] = 0.0;
    	}
    }
}      */

//void TBiDimensionalTimeSeriesTransportImplicit::banbks(/*float a[][max], */int n, int m1, int m2/*, float al[][max],
//    int indx[], float b[]*/)
/*{
    int i, k, l;
    int mm;
    double dum;

    mm = m1 + m2 + 1;
    l = m1;
    for (k = 0; k < n; k++) {
	i = Indx[k];
	if (i != k) SWAP(B[k], B[i])
	if (l < n) l++;
	for (i = k + 1; i < l; i++) B[i] -= LowerTriangularMatrix[k * m1 + i - k - 1] * B[k];
    }
    l = 1;
    for (i = n - 1; i >= 0; i--) {
	dum = B[i];
	for (k = 1; k < l; k++) dum -= CompactedMatrix[i * mm + k] * B[k + i];
	B[i] = dum / CompactedMatrix[i * mm];
	if (l < mm) l++;
    }
}  */


void TBiDimensionalTimeSeriesTransportImplicit::bandec(/*double a[][max],*/ int n, int m1, int m2/*, double *a1/*al[][max],
	int indx[], float *dd*/)
{
    int i, j, k, l;
    int mm;
    double dum;
    mm = m1 + m2 + 1;
    l = m1;
    for (i = 1; i <= m1; i++) {
	for (j = m1 + 2 - i; j <= mm; j++) CompactedMatrix[(i - 1) * mm + j - l/*!!!*/] = CompactedMatrix[(i - 1) * mm + j];
	l--;
	for (j = mm - l; j <= mm; j++)
        {
           CompactedMatrix[(i - 1) * mm + j] = 0.0;
        }
    }
    //*dd = 1.0;
    dd = 1.0;
    l = m1;
    for (k = 1; k <= n; k++) {
	dum = CompactedMatrix[(k - 1) * mm + 1];
     	i = k;
	if (l < n) l++;
	for (j = k + 1; j <= l; j++) {
	    if (fabs(CompactedMatrix[(j - 1) * mm + 1]) > fabs(dum)) {
		dum = CompactedMatrix[(j - 1) * mm + 1];
		i = j;
	    }
	}
    	Indx[k] = i;
    	if (dum == 0.0) CompactedMatrix[(k - 1) * mm + 1] = TINY;
    	if (i != k) {
	    //*dd = - (*dd);
            dd = -dd;
  	    for (j = 1; j <= mm; j++) SWAP(CompactedMatrix[(k - 1) * mm + j], CompactedMatrix[(i - 1) * mm + j])
    	}
        //if (k >= 26) Debugger(111);
    	for (i = k + 1; i <= l; i++) {
	    dum = CompactedMatrix[(i - 1) * mm + 1] / CompactedMatrix[(k - 1) * mm + 1];
	    LowerTriangularMatrix[(k - 1) * m1 + i - k ] = dum;
	    for (j = 2; j <= mm; j++)
            {
               CompactedMatrix[(i - 1) * mm + j - 1] = CompactedMatrix[(i - 1) * mm + j] - dum * CompactedMatrix[(k - 1) * mm + j];
            }
            CompactedMatrix[(i - 1) * mm + mm] = 0.0;
    	}
    }
}

void TBiDimensionalTimeSeriesTransportImplicit::banbks(/*float a[][max], */int n, int m1, int m2/*, float al[][max],
    int indx[], float b[]*/)
{
    int i, k, l;
    int mm;
    double dum;

    mm = m1 + m2 + 1;
    l = m1;
    for (k = 1; k <= n; k++) {
	i = Indx[k];
	if (i != k) SWAP(B[k], B[i])
	if (l < n) l++;
	for (i = k + 1; i <= l; i++) B[i] -= LowerTriangularMatrix[(k - 1) * m1 + i - k] * B[k];
    }
    l = 1;
    for (i = n; i >= 1; i--) {
	dum = B[i];
	for (k = 2; k <= l; k++) dum -= CompactedMatrix[(i - 1) * mm + k] * B[k + i - 1];
	B[i] = dum / CompactedMatrix[(i - 1) * mm + 1];
	if (l < mm) l++;
    }
}
