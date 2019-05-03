
      MODULE ecodynamoicecpp_mod

!
!svn $Id: ecodynamo_cpp.F 1372 2015-11-03 00:38:29 mitya $
!=======================================================================
!                                                                      !
!  ECODYNAMO CPP PACKAGE:                                              !
!                                                                      !
!  This package is used to provide interface to ecodynamo cpp functions!
!                                                                      !
!----------------------------------------------------------------------!

      implicit none
   
       INTERFACE
!      public :: icealgae

!      interface icealgae 
!         module procedure icealgae_new, &
!                          icealgae_go,  &
!                          icealgae_production
!      end interface

!      contains

         SUBROUTINE icealgae_new(ICEALGALOBJ,PICurveOption, &
         & LimitationType,Pmax,alfa,beta,TMinPhotosynthesis,&
         & TempCoefPhotosynthesis,CarbonToOxygenProd,       &
         & RDark,R0,Dlratio,TMinRespiration,                &
         & TempCoefRespiration,TMinMortality,               &
         & TempCoefMortality,MortalityAtZeroDegres,         &
         & MaxNPratio, MinNPRatio,MinNSiRatio,              &
         & KAmmonium,KNitratePlusNitrite,KP,KSi,VmaxN,VmaxP,&
         & VmaxSi,KNcell,KPcell,KSicell,KNO3External,       &
         & KNH4External,KPExternal,KSiExternal,Phi,ExudRate,&
         & Seeding,IntegrationSteps,                        &
         & NutLimitation,SaltLimitation,TempLimitation,     &
         & NmaxQuota,PmaxQuota,SimaxQuota,                  &
         & NminQuota,PminQuota,SiminQuota,                  &
         & LightThreshold,WinterResp)                       &                                                           
         BIND(C, NAME='icealgae_new__')
         USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_INT,C_DOUBLE,C_LONG,C_CHAR
         integer(C_LONG) :: ICEALGALOBJ
         integer(C_INT), intent(in) :: PICurveOption               ! nondimensional: 1 - PLATT; 2 - STEELE 
         integer(C_CHAR),intent(in) :: LimitationType              ! 'Internal' or 'External': the former is based on brine concentrations, the latter is based on cell quotas
         real(C_DOUBLE), intent(in) :: Pmax                        ! Maximum photosynthetic rate (1/h)
         real(C_DOUBLE), intent(in) :: alfa                        ! Initial slope or photosynthetic efficiency of the PI curve [mg C mg C-1 h-1 microM photons m2 s]
         real(C_DOUBLE), intent(in) :: beta                        ! Degree of photoinhibition - Negative slope of the PI curve, after the onset of photoinhibition [mg C mg C-1 h-1 microM photons m2 s]
         real(C_DOUBLE), intent(in) :: TMinPhotosynthesis          ! Reference temperature for photosynthesis [degrees]
         real(C_DOUBLE), intent(in) :: TempCoefPhotosynthesis      ! 1/degrees 
         real(C_DOUBLE), intent(in) :: CarbonToOxygenProd          ! mgC/mgO2
         real(C_DOUBLE), intent(in) :: RDark                       ! Linear coefficient of increase in biomass-specific respiration with gross photosynthesis (proportion)
         real(C_DOUBLE), intent(in) :: R0                          ! Maintenance respiration [mmol O2 mg Chl-1 h-1]
         real(C_DOUBLE), intent(in) :: Dlratio                     ! Ratio between respiration in the light and respiration in the dark
         real(C_DOUBLE), intent(in) :: TMinRespiration             ! Reference temperature for respiration [degrees]
         real(C_DOUBLE), intent(in) :: TempCoefRespiration         ! 1/degrees 
         real(C_DOUBLE), intent(in) :: TMinMortality               ! Reference temperature for mortality [degrees]
         real(C_DOUBLE), intent(in) :: TempCoefMortality           ! 1/degrees 
         real(C_DOUBLE), intent(in) :: MortalityAtZeroDegres       ! h-1
         real(C_DOUBLE), intent(in) :: MaxNPratio                  ! Maximal cellular nitrogen: phosphorus ratio
         real(C_DOUBLE), intent(in) :: MinNPratio                  ! Minimal cellular nitrogen: phosphorus ratio
         real(C_DOUBLE), intent(in) :: MinNSiratio                 ! Minimal cellular nitrogen: silica ratio
         real(C_DOUBLE), intent(in) :: KAmmonium                   ! Half saturation constant for ammonium uptake [microM]
         real(C_DOUBLE), intent(in) :: KNitratePlusNitrite         ! Half saturation constant for nitrate+nitrite uptake [microM]
         real(C_DOUBLE), intent(in) :: KP                          ! Half saturation constant for phosphate uptake [microM]
         real(C_DOUBLE), intent(in) :: KSi                         ! Half saturation constant for silica uptake [microM]
         real(C_DOUBLE), intent(in) :: VmaxN                       ! Maximal uptake rate of nitrogen [d-1]
         real(C_DOUBLE), intent(in) :: VmaxP                       ! Maximal uptake rate of phosphate [d-1]  
         real(C_DOUBLE), intent(in) :: VmaxSi                      ! Maximal uptake rate of silica [d-1]  
         real(C_DOUBLE), intent(in) :: KNcell                      ! Half saturation constant for growth limited by nitrogen cell quota [mgN mgC-1]
         real(C_DOUBLE), intent(in) :: KPcell                      ! Half saturation constant for growth limited by phosphorus cell quota [mgP mgC-1]
         real(C_DOUBLE), intent(in) :: KSicell                     ! Half saturation constant for growth limited by silica cell quota [mgSi mgC-1]
         real(C_DOUBLE), intent(in) :: KNO3External                ! Half saturation constant for growth limited by brine nitrate+nitrite [microM] 
         real(C_DOUBLE), intent(in) :: KNH4External                ! Half saturation constant for growth limited by brine ammonium [microM]
         real(C_DOUBLE), intent(in) :: KPExternal                  ! Half saturation constant for growth limited by brine phosphate [microM]
         real(C_DOUBLE), intent(in) :: KSiExternal                 ! Half saturation constant for growth limited by brine silica [microM]
         real(C_DOUBLE), intent(in) :: Phi                         ! Inhbition coefficient for nitrate+nitrite uptake in the presence of ammonium
         real(C_DOUBLE), intent(in) :: ExudRate                    ! Fraction of GPP exuded
         real(C_DOUBLE), intent(in) :: Seeding                     ! Value assumed as a minimum to restart ice algal growth [mg C m-2]
         real(C_DOUBLE), intent(in) :: IntegrationSteps            ! Number of vertical steps for numerical integration of the PI Platt curve
         real(C_INT),    intent(in) :: NutLimitation               ! 1 - Nutrient limitation on; 0 - Nutrient limitation off 
         real(C_INT),    intent(in) :: SaltLimitation              ! 1 - Salt limitation on; 0 - Salt limitation off 
         real(C_INT),    intent(in) :: TempLimitation              ! 1 - Temperature limitation on; 0 - Temperature limitation off 
         real(C_DOUBLE), intent(in) :: NmaxQuota                   ! Maximal nitrogen cell quota [mgN mgC-1]
         real(C_DOUBLE), intent(in) :: PmaxQuota                   ! Maximal phosphorus cell quota [mgP mgC-1]
         real(C_DOUBLE), intent(in) :: SimaxQuota                  ! Maximal silica cell quota [mgSi mgC-1]
         real(C_DOUBLE), intent(in) :: NminQuota                   ! Minimal nitrogen cell quota [mgN mgC-1]
         real(C_DOUBLE), intent(in) :: PminQuota                   ! Minimal phosphorus cell quota [mgP mgC-1]
         real(C_DOUBLE), intent(in) :: SiminQuota                  ! Minimal silica cell quota [mgSi mgC-1]
         real(C_DOUBLE), intent(in) :: LightThreshold              ! Minimum actinic PAR liht [microM photons m-2 s-1]
         real(C_DOUBLE), intent(in) :: WinterResp                  ! Metabolism during the polar night [day-1]
       
         END SUBROUTINE icealgae_new

         !void icealgae_go__(long* PIceAlgae, double* layerThickness, double* timeStep);  

         SUBROUTINE icealgae_go(ICEALGALOBJ,Hz,dt)                      & 
         BIND(C, NAME='icealgae_go__')
         USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_INT, C_DOUBLE, C_LONG
         integer(C_LONG), intent(in) :: ICEALGALOBJ
         real(C_DOUBLE), intent(in) ::  Hz, dt
         END SUBROUTINE icealgae_go

!   void icealgae_production__(long* PIceAlgae, double* lightAtTop, double* lightAtBottom, double* kValue,double* iceTemperature,
!                               int* piCurveOption, double* julianDay, double* GrossProduction, double* nPhyto, double* pPhyto, double* biomass, 
!                               double *TIC, double *Slope, double* Chl2Carbon, double *OxygenProduction);

         SUBROUTINE icealgae_production(     ICEALGALOBJ,               &
     &                                            TopLight,             & 
     &                                            BottomLight,          &
     &                                            KValue,               &
     &                                            temp,casep, yday,     &
     &                                            GrossProduction,      &
     &                                            PhyN,PhyP,Phyt,       &
     &                                            TIC,                  &
     &                                            PhyIS,                &
     &                                            Chl2Carbon,           &
     &                                            OxygenProduction)     &
         BIND(C,NAME='icealgae_production__')
         USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_INT, C_DOUBLE, C_LONG
         integer(C_LONG), intent(in) :: ICEALGALOBJ
         real(C_DOUBLE),  intent(in) :: TopLight
         real(C_DOUBLE),  intent(in) :: BottomLight
         real(C_DOUBLE),  intent(in) :: KValue
         real(C_DOUBLE),  intent(in) :: temp
         integer(C_INT),  intent(in) :: casep
         real(C_DOUBLE),  intent(in) :: yday
         real(C_DOUBLE),  intent(inout) :: GrossProduction
         real(C_DOUBLE),  intent(in) :: PhyN                ! Phytoplankton concentration in nitrogen units (mmol N / m3)
         real(C_DOUBLE),  intent(in) :: PhyP                ! Phytoplankton concentration in phosphorus units (mmol P / m3)
         real(C_DOUBLE),  intent(in) :: Phyt                ! Phytoplankton concentration in carbon units (mmol C / m3)
         real(C_DOUBLE),  intent(in) :: TIC                   
         real(C_DOUBLE),  intent(in) :: PhyIS               ! m2/W
         real(C_DOUBLE),  intent(in) :: Chl2Carbon
         real(C_DOUBLE),  intent(inout) :: OxygenProduction
         END SUBROUTINE icealgae_production

      

!    void phytoplankton_nitrogen_uptake__(int* PPhytoplankton, double* Ammonia, double* Nitrate, double* Nitrite,double* cffNH4, double *cffNO3NO2, double* nPhyto, double* biomass);  
      
!         SUBROUTINE phytoplankton_nitrogen_uptake(    PHYOBJ,           &
!     &                                             NH4,NO3,dumNitrite,  & 
!     &                                             cffNH4, cffNO3NO2,   &
!     &                                             PhyN,Phyt)           &
!         BIND(C,NAME='phytoplankton_nitrogen_uptake__')
!         USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_DOUBLE, C_LONG
!         integer(C_LONG), intent(in) :: PHYOBJ
!         real(C_DOUBLE), intent(in) :: NH4                  ! [millimole/m3]
!         real(C_DOUBLE), intent(in) :: NO3                  ! [millimole/m3]
!         real(C_DOUBLE), intent(in) :: dumNitrite
!         real(C_DOUBLE), intent(inout) :: cffNH4, cffNO3NO2
!         real(C_DOUBLE), intent(in) :: PhyN                 ! Phytoplankton concentration in nitrogen units (mmol N / m3)
!         real(C_DOUBLE), intent(in) :: Phyt                 ! Phytoplankton concentration in carbon units (mmol C / m3)         
!         END SUBROUTINE phytoplankton_nitrogen_uptake

!    void phytoplankton_phosphorus_uptake__(int* PPhytoplankton, double* Phosphate,double* cffPO4, double* pPhyto, double* biomass); 

!         SUBROUTINE phytoplankton_phosphorus_uptake(PHYOBJ,             &
!     &                                             PO4,cffPO4,          &
!     &                                             PhyP,Phyt)           &
!         BIND(C,NAME='phytoplankton_phosphorus_uptake__')
!         USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_DOUBLE, C_LONG
!         integer(C_LONG), intent(in) :: PHYOBJ
!         real(C_DOUBLE), intent(in) :: PO4                  ! [millimole/m3]
!         real(C_DOUBLE), intent(inout) :: cffPO4
!         real(C_DOUBLE), intent(in) :: PhyP                 ! Phytoplankton concentration in phosphorus units (mmol N / m3)
!         real(C_DOUBLE), intent(in) :: Phyt                 ! Phytoplankton concentration in carbon units (mmol C / m3)         
!         END SUBROUTINE phytoplankton_phosphorus_uptake
         
!    void phytoplankton_respiration__(int* PPhytoplankton, double* waterTemperature, double* cffCRespiration, double *GrossProduction, double* biomass,  double* Chl2Carbon);

!         SUBROUTINE phytoplankton_respiration(PHYOBJ,                   &
!     &                                        temp,                     &
!     &                                        cffCResp,                 &
!     &                                        GrossProduction,          &
!     &                                        Phyt,                     &
!     &                                        Oxygen,                   &
!     &                                        Chl2Carbon,               &
!     &                                        OxygenConsumption)        &
!   
!         BIND(C,NAME='phytoplankton_respiration__')
!         USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_DOUBLE, C_LONG
!         integer(C_LONG), intent(in) :: PHYOBJ
!         real(C_DOUBLE), intent(in) :: temp
!         real(C_DOUBLE), intent(inout) :: cffCResp
!         real(C_DOUBLE), intent(in) :: GrossProduction
!         real(C_DOUBLE), intent(in) :: Phyt                   ! Phytoplankton concentration in carbon units (mmol C / m3)  
!         real(C_DOUBLE), intent(in) :: Oxygen       
!         real(C_DOUBLE), intent(in) :: Chl2Carbon
!         real(C_DOUBLE), intent(inout) :: OxygenConsumption
!         END SUBROUTINE phytoplankton_respiration         

!    void phytoplankton_exudation__(int* PPhytoplankton, double* cffCExudation, double *GrossProduction, double* biomass);

!         SUBROUTINE phytoplankton_exudation(  PHYOBJ,                   &
!     &                                        cffCExud,                 &
!     &                                        GrossProduction,          &
!     &                                        Phyt,                     &
!     &                                        NCellQuota,               &
!     &                                        PCellQuota)               &
!         BIND(C,NAME='phytoplankton_exudation__')
!         USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_DOUBLE, C_LONG
!         integer(C_LONG), intent(in) :: PHYOBJ
!         real(C_DOUBLE), intent(inout) :: cffCExud
!         real(C_DOUBLE), intent(in) :: GrossProduction
!         real(C_DOUBLE), intent(in) :: Phyt                   ! Phytoplankton concentration in carbon units (mmol C / m3) 
!         real(C_DOUBLE), intent(in) :: NCellQuota    
!         real(C_DOUBLE), intent(in) :: PCellQuota             
!         END SUBROUTINE phytoplankton_exudation      
   
      END INTERFACE

      END MODULE ecodynamoicecpp_mod

     

