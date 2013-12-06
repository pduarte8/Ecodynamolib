#include "ecodyn.rh"
#include "liteobjt.h"

/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
	#include <vcl.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <cstring>
	
	using namespace std;
#endif  // __BORLANDC__


TLightWithIce::TLightWithIce(TEcoDynClass* APEcoDynClass, char* className): TLight(APEcoDynClass, className)
{
    IceVisibleLightAtBottom = new double[NumberOfBoxes];
    IceVisibleLightAboveChlLayer = new double[NumberOfBoxes];
    IceNearInfraRedLightAtBottom = new double[NumberOfBoxes];
    IceNearInfraRedLightAboveChlLayer = new double[NumberOfBoxes];
    for (int i = 0; i < NumberOfBoxes; i++)
    {
       IceVisibleLightAtBottom[i] = 0.0;
       IceVisibleLightAboveChlLayer[i] = 0.0;
       IceNearInfraRedLightAtBottom[i] = 0.0;
       IceNearInfraRedLightAboveChlLayer[i] = 0.0;
    }
    MyIceDepth = 0.0; MySnowDepth = 0.0;
    //MyAlbedo = 0.0; MyK_Ice = 0.0;
    //MyI0_Ice = 0.0; MyIceAlgaeK = 0.0; MyIceAlgaeLayerThickness = 0.0;
    MyKforVisibleLightInIce = 0.0; MyKforNearInfraRedLightInIce = 0.0; MyI0forVisibleLightInIce = 0.0;
    MyI0forNearInfraRedLightInIce = 0.0; MyAlbedoForVisibleLightInIce = 0.0; MyAlbedoForNearInfraredLightInIce = 0.0;
    MyKforVisibleLightInSnow = 0.0; MyAlbedoForVisibleLightInSnow = 0.0;
    TINNY = 0.01;
}

TLightWithIce::~TLightWithIce()
{
   freeMemory();
}

void TLightWithIce::freeMemory()
{
   if (NumberOfBoxes > 0)
   {
      delete [] IceVisibleLightAtBottom;
      delete [] IceVisibleLightAboveChlLayer;
      delete [] IceNearInfraRedLightAtBottom;
      delete [] IceNearInfraRedLightAboveChlLayer;
   }
}

void TLightWithIce::Integrate()
{
   GetDaylightHours();
   GetLightAtSurface (12.0);
   GetLightAtSurface();
   GetPhoticDepth();
   GetHorizontalMeanWaterColumnRadiation();
   GetIceBottomLight();
}

void TLightWithIce::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
    char MyParameter[65]; // 64 characters (max) for parameter name
    int MyBoxNumber = BoxNumber;
    strcpy(MyParameter, ParameterName);

    if (strcmp(MyParameter, "Total surface irradiance") == 0)
            Value = TotalSurfaceLight[MyBoxNumber];
    else if (strcmp(MyParameter, "TotalSurfaceLightTimeSeries") == 0)
            Value = TotalSurfaceLight[MyBoxNumber];
    else if (strcmp(MyParameter, "PAR surface irradiance") == 0)
            Value = ParSurfaceLight[MyBoxNumber];
    else if (strcmp(MyParameter, "Daylight hours") == 0)
            Value = HoursOfSun[MyBoxNumber];
    else if (strcmp(MyParameter, "Mean horizontal water irradiance") == 0)
            Value = AverageHorizontalLight[MyBoxNumber];
    else if (strcmp(MyParameter, "Mean vertical water irradiance") == 0)
            Value = AverageVerticalLight[MyBoxNumber];
    else if (strcmp(MyParameter, "Mean horizontal water PAR irradiance") == 0)
            Value = AverageHorizontalLight[MyBoxNumber] * LightToPAR;
    else if (strcmp(MyParameter, "Mean vertical water PAR irradiance") == 0)
            Value = AverageVerticalLight[MyBoxNumber] * LightToPAR;
    else if (strcmp(MyParameter, "Noon surface PAR") == 0)
            Value = AnyPar[MyBoxNumber] * LightToPAR;
    else if (strcmp(MyParameter, "Photic depth") == 0)
            Value = PhoticDepth[MyBoxNumber];
    else if (strcmp(MyParameter, "Sub-surface irradiance") == 0)
            Value = SubSurfaceLight[MyBoxNumber];
    else if (strcmp(MyParameter, "Sub-surface PAR irradiance") == 0)
            Value = SubSurfaceLight[MyBoxNumber] * LightToPAR;
    else if (strcmp(MyParameter, "Atmospheric IR") == 0)
            //Value = AtmosphericIR[MyBoxNumber];
            Value = GetAtmosphericInfraRed(MyBoxNumber);
    else if (strcmp(MyParameter, "Latitude") == 0)
            Value = Latitude[MyBoxNumber];
    else if (strcmp(MyParameter, "Julian day") == 0)
            Value = MyPEcoDynClass->GetJulianDay();
    else if (strcmp(MyParameter, "Current time") == 0)
            Value = MyPEcoDynClass->GetCurrTime();
    else if (strcmp(MyParameter, "KValue") == 0)
		Value = KValue;
    else if (strcmp(MyParameter, "Daily average light") == 0)
   	    Value = DailyAverageLight[MyBoxNumber];
    else if (strcmp(MyParameter, "Ice visible light at bottom") == 0)
   	    Value = IceVisibleLightAtBottom[MyBoxNumber];
    else if (strcmp(MyParameter, "Ice visible light above chl layer") == 0)
   	    Value = IceVisibleLightAboveChlLayer[MyBoxNumber];
    else if (strcmp(MyParameter, "Ice near infrared at bottom") == 0)
   	    Value = IceNearInfraRedLightAtBottom[MyBoxNumber];
    else if (strcmp(MyParameter, "Ice near infrared above chl layer") == 0)
   	    Value = IceNearInfraRedLightAboveChlLayer[MyBoxNumber];
    #ifndef _PORT_FORTRAN_
    else
    {
        Value = 0;
        int j;
        for (j = 0; j < NumberOfVariables; j++)
        {
            if (strcmp(MyParameter, VariableNameArray[j]) == 0)
                break;
        }
        if (j == NumberOfVariables)
        {
#ifdef __BORLANDC__
            char Caption[129];
            strcpy(Caption, MyParameter);
            strcat(Caption, " does not exist in ");
            strcat(Caption, GetEcoDynClassName());
            MessageBox(0,Caption,"EcoDynamo Alert - Inquiry 1", MB_OK);
#else  // __BORLANDC__
						cerr << "TLight::Inquiry 1 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
    #endif
}


void TLightWithIce::GetIceBottomLight()
{
   TEcoDynClass* MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer(),
               * MyIceAlgaePointer = MyPEcoDynClass->GetIceAlgaePointer();
   double MyPAR, MinimalSnowLayer;
   MinimalSnowLayer = 0.005; //Stephen Hudson suggested that snow albedo should be taken into account if snow depth is larger than 0.5 cm
   for (int i = 0; i < NumberOfBoxes; i++)
   {
       if (MySeaIcePointer != NULL)
       {
          MySeaIcePointer->Inquiry(GetEcoDynClassName(), MyKforVisibleLightInIce, i,"KforVisibleLightInIce", ObjectCode);
          MySeaIcePointer->Inquiry(GetEcoDynClassName(), MyKforNearInfraRedLightInIce, i,"KforNearInfraRedLightInIce", ObjectCode);
          MySeaIcePointer->Inquiry(GetEcoDynClassName(), MyI0forVisibleLightInIce, i,"I0forVisibleLightInIce", ObjectCode);
          MySeaIcePointer->Inquiry(GetEcoDynClassName(), MyI0forNearInfraRedLightInIce, i,"I0forNearInfraRedLightInIce", ObjectCode);
          MySeaIcePointer->Inquiry(GetEcoDynClassName(), MyAlbedoForVisibleLightInIce, i,"AlbedoForVisibleLightInIce", ObjectCode);
          MySeaIcePointer->Inquiry(GetEcoDynClassName(), MyAlbedoForNearInfraredLightInIce, i,"AlbedoForNearInfraredLightInIce", ObjectCode);
          MySeaIcePointer->Inquiry(GetEcoDynClassName(), MyKforVisibleLightInSnow, i,"KforVisibleLightInSnow", ObjectCode);
          MySeaIcePointer->Inquiry(GetEcoDynClassName(), MyAlbedoForVisibleLightInSnow, i,"AlbedoForVisibleLightInSnow", ObjectCode);
          MySeaIcePointer->Inquiry(GetEcoDynClassName(), MyIceDepth, i,"SIheff", ObjectCode);
          MySeaIcePointer->Inquiry(GetEcoDynClassName(), MySnowDepth, i,"SIhsnow", ObjectCode);
       }
       if (MyIceAlgaePointer != NULL)
       {
          MyIceAlgaePointer->Inquiry(GetEcoDynClassName(), MyIceAlgaeK, i,"KForIceAlgae", ObjectCode);
          MyIceAlgaePointer->Inquiry(GetEcoDynClassName(), MyIceAlgaeLayerThickness, i,"IceAlgaeLayerThickness", ObjectCode);
       }
       if (MySnowDepth > MinimalSnowLayer)  //Light reduction from snow albedo and absorption as in Smith et al. (1988) - MEPS 48: 251-263, Equation 1
          MyPAR = ParSurfaceLight[i] * (1 - MyAlbedoForVisibleLightInSnow) * exp(-MyKforVisibleLightInSnow * MySnowDepth);
       else
          MyPAR = ParSurfaceLight[i];
       //Based on: JOURNAL OF GEOPHYSICAL RESEARCH, VOL. 113, C03023, doi:10.1029/2006JC003977, 2008  "WHAT ABOUT THE EFFECT OF SNOW DEPTH? CHECK IT!!!" - Stephen says he may come out with some relationship between sonow depth and light tranmission...
       if ((MyIceDepth > 0.0) && (MyIceDepth > MyIceAlgaeLayerThickness))
       {
          if (MySnowDepth < MinimalSnowLayer)
             MyPAR = MyPAR * (1.0 - MyAlbedoForVisibleLightInIce); //Ice albedo accounted for only when snow albedo is not calculated due to very low or no snow cover
          IceVisibleLightAboveChlLayer[i] = MyPAR *  MyI0forVisibleLightInIce *  exp(-MyKforVisibleLightInIce * (MyIceDepth - MyIceAlgaeLayerThickness));//Visibel light as a surrogate for PAR light

          IceVisibleLightAtBottom[i] = IceVisibleLightAboveChlLayer[i] * exp(-MyIceAlgaeK * MyIceAlgaeLayerThickness);

           //Regarding near infrared I am not taking into account the effect of snow so, I have to check this out!

          IceNearInfraRedLightAboveChlLayer[i] = (TotalSurfaceLight[i]-ParSurfaceLight[i]) * (1.0 - MyAlbedoForNearInfraredLightInIce) *  MyI0forNearInfraRedLightInIce *
                                                 exp(-MyKforNearInfraRedLightInIce * (MyIceDepth - MyIceAlgaeLayerThickness));
          //I am assuming that the difference between total sun irradiance and PAR is a good surrogate for near infrared light, since the UV fraction is relatively small (I discussed this with Stephen Hudson)
          IceNearInfraRedLightAtBottom[i] = IceNearInfraRedLightAboveChlLayer[i] * exp(-MyKforNearInfraRedLightInIce * MyIceAlgaeLayerThickness);
          //Assumed that algae will not absorb near infrared, so all absorption will be due to ice
       }
   }
}






