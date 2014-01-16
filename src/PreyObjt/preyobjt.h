/**
 * NPI work
 * 	18 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */


#ifndef PREYOBJTH
#define PREYOBJTH
#include "EcoDynClass.h"


// DLL header file

#ifndef __BORLANDC__
#define _export 
#endif  // __BORLANDC__

class _export TPrey : public TEcoDynClass
{
    public :
      TPrey(TEcoDynClass* APEcoDynClass, char* className);
      ~TPrey();
      virtual void freeMemory();
      virtual void Go ();
      virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
      virtual void Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode);
      virtual void Integrate();

      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
      virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);

      // Methods called from EcoDynamo when closing model:
      virtual bool SaveVariables();
      virtual bool SaveParameters();

    protected :
      void BuildPrey(char* className);
      void Grow();
      double *PreyAbundance, *PreyFlux;
      double IntrinsicRateOfIncrease;
};

static TEcoDynClass *PPrey;

#endif //PREYOBJTH



