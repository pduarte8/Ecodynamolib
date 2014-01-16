/*
 * FinalizeOutput.cpp
 * 
 * - implement functions finalizing output files
 * 
 *   Public:
 *     void FinalizeOutput()
 * 
 * Copyright 2013 - A.Pereira
 */


#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

#include "outputResults.h"

#ifdef __BORLANDC__
    #include "MLHDF.h"
#endif  // __BORLANDC__

//---------------------------------------------------------------------------
void OutputResults::FinalizeOutput()
{
    if (outputToFile())
        FinalizeFile();
    clearMeanSamples();
}
//---------------------------------------------------------------------------
void OutputResults::FinalizeFile()
{
#ifdef __BORLANDC__
    if(MyMLHDF != NULL)
        MyMLHDF->cleanMLHDF();
    if (iceFile && MyMLHDFIce != NULL)
        MyMLHDFIce->cleanMLHDF();
#endif  // __BORLANDC__

}
//---------------------------------------------------------------------------
