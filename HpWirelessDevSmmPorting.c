
//
// Copyright (c) 2015 Hewlett-Packard Company. All rights reserved
// This software and associated documentation (if any) is furnished
// under a license and may only be used or copied in accordance
// with the terms of the license. Except as permitted by such
// license, no part of this software or documentation may be
// reproduced, stored in a retrieval system, or transmitted in any
// form or by any means without the express written consent of
// Hewlett-Packard Company.
//
// Module Name:
//   HpWirelessDevSmmPorting.c
//
if defined(CONDEV_ENABLE_ALL_DBG_MSG)
#pragma optimize ("",off)
#define WIRE_PRINT   DEBUG
#else
#define WIRE_PRINT(Expression)
#endif

#include <HpWirelessDevSmmPorting.h>

//---------------------------------------------------------------------------------------------------------------------
// Local Function Prototypes
//---------------------------------------------------------------------------------------------------------------------
VOID
WireBtnLEDContrl (
  BOOLEAN         TurnLedOn
  );

VOID
GenerateSW_SCI(
  VOID
);

EFI_STATUS
DeviceChangeOnOff (
  IN UINT32                    DevsToUpdate,
  IN UINT8                     Group,
  IN UINT32                    PadNumber,
  IN UINT8                     GpioEnable,
  IN UINT8                     ExpectDeviceState
  );

EFI_STATUS
LanLinkHandle (
  WIRELESS_INFO                  *WLInfo
  );

EFI_STATUS
LanLinkCallbackRegister (
  VOID
  );

EFI_STATUS HpWirelessDevSmmPortingEntry(
   IN EFI_HANDLE,
   IN EFI_SYSTEM_TABLE *
   );

static
HpSmmCoreVariablePolicyServices_t *
LocateVariableAccessPolicyProtocol(
   void
   );

static
EFI_HP_CONNECTION_DEVICE_PROTOCOL *
LocateConnectionManagerProtocol(
   void
   );

static
VOID
SioTimerLanLinkHandle (
   IN EFI_HANDLE                 DispatchHandle,
   IN SioSmiRegisterContext_t    *MiscRegisterContext
);

EFI_STATUS
LanWlanSwitchingWA (
  VOID
);

EFI_STATUS
LanWlanSwitchingWAResumeCallback (
   IN       EFI_HANDLE              DispatchHandle,
   IN       EFI_HP_SWSMI_CONTEXT    *RegisteredContext,
   IN OUT   EFI_SMM_SW_CONTEXT      *DispatchContext
);
//---------------------------------------------------------------------------------------------------------------------
// Local Structure Definitions
//---------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------
// Local Symbolic Constants
//---------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------
// Global Variables
//---------------------------------------------------------------------------------------------------------------------
static   HpSmmCoreVariablePolicyServices_t         *mHpSmmVariable = NULL;
static   EFI_HP_CONNECTION_DEVICE_PROTOCOL         *mHpConnectionDeviceProtocol = NULL;
static   HpVariablePolicyContext_t                 Context = { HP_VARIABLE_POLICY_SMM_TRUSTED, NULL, 0u, CredentialNone };
HP_SHARED_ASL_DATA                     *mHpSharedAslData = NULL;
HP_COMMON_ACPI_AREA_PROTOCOL           *mHpCommonAcpiArea = NULL;
HP_ACPI_CONDEV_AREA_PROTOCOL           *mHpAcpiConDevAreaProtocol = NULL;
WIRELESS_INFO                          *mWLInfo = NULL;
EFI_TIME                               mRtcTimeStamp = { 0 };
static BOOLEAN                         mWaitingForLanLinkDebounceSMI = FALSE;
static BOOLEAN                         mSavedLanLinkState;
static BOOLEAN                         mSMIHadBeTrigger = FALSE;

#define EFI_MIN(a, b)       (((a) < (b)) ? (a) : (b))
#define EFI_MAX(a, b)       (((a) > (b)) ? (a) : (b))

static
WirelessDevSmmPortingProtocol_t mWirelessDevPortingSmmProtocol = \
{
  LanLinkCallbackRegister,
  LanLinkHandle,
  DeviceChangeOnOff,
  GenerateSW_SCI
};

/*++

Routine Description:

  Dispatch function for a GPI SMI handler.

Arguments:

  DispatchHandle  - Handle of this dispatch function
  DispatchContext - Pointer to the dispatch function's context.
                    The DispatchContext fields are filled in by the dispatching driver
                    prior to invoke this dispatch function

Returns:

  None

-*/
UINT32
GetUnixTime (
  IN EFI_TIME              Time
  )
/*++

Routine Description:
  Return Unix Time (seconds from 1/1/1970)

Arguments:
  None

Returns:
  Unix Time

-*/
{
  UINT32    thisyear;
  UINT32    years;
  UINT32    days;
  UINT32    unix_seconds;
  UINT32 days_till_this_month[] = { 0,
  31,                             // Feb
  31+28,
  31+28+31,                       // April
  31+28+31+30,
  31+28+31+30+31,                 // June
  31+28+31+30+31+30,
  31+28+31+30+31+30+31,           // Aug
  31+28+31+30+31+30+31+31,
  31+28+31+30+31+30+31+31+30,     // Oct
  31+28+31+30+31+30+31+31+30+31,
  31+28+31+30+31+30+31+31+30+31+30 // Dec
  };

  thisyear = Time.Year;

  years = (UINT32)thisyear - 1970 ;
  days  = 365 * years;
  // Add leap year days
  days += (thisyear - 1969)/4;
  if ( (thisyear % 4 == 0  && thisyear != 0) || thisyear %400 ==0) {
    if (Time.Month >= 3)
      days ++;
  }
  days += days_till_this_month[Time.Month - 1];
  days +=  Time.Day;

  unix_seconds = (3600*24*days) + (Time.Hour*60  + Time.Minute)*60 + Time.Second;
  return unix_seconds;
}


UINT32
CompareTimeStamp (
  IN  EFI_TIME             *TimeStamp1,
  IN  EFI_TIME             *TimeStamp2
  )
{
  UINT32 UnixTime1;
  UINT32 UnixTime2;


  //if (EFI_ERROR (RtcTimeFieldsValid (TimeStamp1)) || EFI_ERROR (RtcTimeFieldsValid (TimeStamp2)))
  //  return 0;

  UnixTime1 = GetUnixTime (*TimeStamp1);
  UnixTime2 = GetUnixTime (*TimeStamp2);

  return (EFI_MAX (UnixTime1, UnixTime2) - EFI_MIN (UnixTime1, UnixTime2));
}// end CompareTimeStamp

//*********************************************************************************************************************
// Function:  GenerateSW_SCI
//
// Summary:
//
// Global Variables:  none
//
// Parameters:    VOID
//
// Function Returns:  void
//
// Details:
//
//*********************************************************************************************************************
VOID
GenerateSW_SCI(
  VOID
)
{
   UINT8	temp0;

//   temp0 = IoRead8(PcdGet16 (PcdPchAcpiIoBaseAddress) + PcdGet8 (PcdGpeCntlOffset)) | PcdGet8 (PcdSwGpeCtrlBit);
//   IoWrite8(PcdGet16 (PcdPchAcpiIoBaseAddress) + PcdGet8 (PcdGpeCntlOffset), temp0);
  temp0 = IoRead8(PCH_ACPI_BASE_ADDRESS + GPE_CNTL) | SWGPE_CTRL;
  IoWrite8(PCH_ACPI_BASE_ADDRESS + GPE_CNTL, temp0);
}
//*********************************************************************************************************************
// Function:  DeviceChangeOnOff
//
// Summary:   Chipset specific setting hook, turn on/off POWER or other.
//
// Global Variables:  none
//
// Parameters:     VOID
//
// Function Returns:  void
//
// Details:
//
//*********************************************************************************************************************
EFI_STATUS
DeviceChangeOnOff (
  IN UINT32                    DevsToUpdate,
  IN UINT8                     Group,
  IN UINT32                    PadNumber,
  IN UINT8                     GpioEnable,
  IN UINT8                     ExpectDeviceState
  )
{
   EFI_STATUS   Status = EFI_UNSUPPORTED;

   if (PadNumber != 0xFF)
     Status = HpGpioEnable (Group, PadNumber, GpioEnable, ExpectDeviceState); // set device state to on/off

   return Status;

}
//*********************************************************************************************************************
// Function:  LanLinkHandle
//
// Summary:   When Lan Link, disable Wire-Lan, fill out the wireless table, and wait for turn on/off GPIO or other.
//
// Global Variables:  none
//
// Parameters:
//           WIRELESS_INFO                  *WLInfo  - Update the wireless device table. When Lan link, disable Wlan.
//
// Function Returns:  void
//
// Details:
//
//*********************************************************************************************************************
EFI_STATUS
LanLinkHandle (
  WIRELESS_INFO                  *WLInfo
  )
{
   return EFI_SUCCESS;
}
// *********************************************************************************************************************
// Function:         SioTimerLanLinkHandle
//
// Summary:          Main hardware monitor SMI handler
//
// Global Variables: none
//
// Parameters:
//    -----------------------------------------------------------------------------------------------------------------
//    Name:          IN DispatchHandle
//    Description:   The unique handle assigned to this handler by register function.
//    Valid values:  non-NULL
//    -----------------------------------------------------------------------------------------------------------------
//    Name:          IN MiscRegisterContext
//    Description:   Context information used by the dispatcher to prioritize dispatch order
//    Valid values:  non-NULL
//    -----------------------------------------------------------------------------------------------------------------
//
// Function Returns: none
//
// Details:
//
// *********************************************************************************************************************
static
VOID
SioTimerLanLinkHandle (
   IN EFI_HANDLE                 DispatchHandle,
   IN SioSmiRegisterContext_t    *MiscRegisterContext
)
{
   UINT8          DevStat = 0;
   BOOLEAN        NewLanLinkState;

//   ReportStatusCode (EFI_PROGRESS_CODE, HP_WIRELESS_SIO_TIMER_ENTRY); // [PostCode:0x8C10]

   if (mWaitingForLanLinkDebounceSMI)
   {
      mWaitingForLanLinkDebounceSMI = FALSE;

      if ((mHpSharedAslData == NULL) || (mHpSharedAslData->OSInfo != WINDOWS_7))
        return;

   #ifdef GROUP_GPIO
      NewLanLinkState = HpGpioRead(PcdGet8 (PcdLanLinkGpioPinGroup),PcdGet8 (PcdLanLinkGpioPin));    //Read the GPIO LVL value
   #else
      NewLanLinkState = HpGpioRead(PcdGet8 (PcdLanLinkGpioPin));    //Read the GPIO LVL value
   #endif
     WIRE_PRINT((EFI_D_ERROR, "[SioTimerLanLinkHandle]Lan/Wlan switching Enable, NewLanLinkState = %x!!!\n",NewLanLinkState));

      if (NewLanLinkState == mSavedLanLinkState){
   #ifdef GROUP_GPIO
         if (HpGpioRead(PcdGet8 (PcdLanLinkGpioPinGroup),PcdGet8 (PcdLanLinkGpioPin)))           // Is a cable inserted/active link? (active low)
   #else
         if (HpGpioRead(PcdGet8 (PcdLanLinkGpioPin)))           // Is a cable inserted/active link? (active low)
   #endif
         {                                         // No: Clear out current link state bit
            mWLInfo->Devices[mWLInfo->WLANIndex & ~DEVICE_DETECTED].DeviceSpecific &= ~WL_WLAN_LINK;
         }
         else                                      // Yes: set bit to indicate link active
         {
            mWLInfo->Devices[mWLInfo->WLANIndex & ~DEVICE_DETECTED].DeviceSpecific |= WL_WLAN_LINK;
            DevStat = WL_DEVSTAT_DIS_OTHER;
         }
         mWLInfo->Devices[mWLInfo->WLANIndex & ~DEVICE_DETECTED].DeviceStatus &= ~WL_DEVSTAT_DIS_OTHER;
         mWLInfo->Devices[mWLInfo->WLANIndex & ~DEVICE_DETECTED].DeviceStatus |= DevStat;
      }
      if (LocateConnectionManagerProtocol() != NULL)
      {
         mHpConnectionDeviceProtocol->UpdateWirelessState(WLAN_LOC_M);
      }
   }

//   ReportStatusCode (EFI_PROGRESS_CODE, HP_WIRELESS_SIO_TIMER_EXIT); // [PostCode:0x9C10]

   return;
}

//*********************************************************************************************************************
// Function:  LanLinkSmiCallBack
//
// Summary:  Dispatch function for a GPI SMI handler. Invert the GPI setting.
//
// Global Variables:  none
//
//  Parameters:       IN  EFI_HANDLE                              DispatchHandle,
//                    IN  AMD_SMM_MISC_DISPATCH_CONTEXT           *DispatchContext
//
// Function Returns:  void
//
// Details:
//
//*********************************************************************************************************************
VOID
LanLinkSmiCallBack (
    IN EFI_HANDLE                 DispatchHandle,
    IN CONST VOID                 *CallbackContext,
    IN OUT VOID                 *CommBuffer,
    IN OUT UINTN                  *CommBufferSize
  )
{

//  ReportStatusCode (EFI_PROGRESS_CODE, HP_WIRELESS_SMM_LANWLAN_ENTRY); // [PostCode:0x8C14]

  if (!mSMIHadBeTrigger)
    mSMIHadBeTrigger = TRUE;

  WIRE_PRINT ((EFI_D_ERROR, "\n[LanLinkSmiCallBack] LanLinkSmiCallBack start.\n"));

  if (mHpSharedAslData != NULL)
  {
    WIRE_PRINT ((EFI_D_ERROR, "[LanLinkSmiCallBack] OSInfo:%d.\n",mHpSharedAslData->OSInfo));

    if (mHpSharedAslData->OSInfo > WINDOWS_8)
      mWLInfo->WLFlags &= ~LANWAN_SWITCHING_EN;
  }

  // Clear GPI status.
  HpAltGpiSMIStsClear (PcdGet8 (PcdLanLinkGpioPinGroup), PcdGet8 (PcdLanLinkGpioPin));

  if (mWLInfo->WLFlags & LANWAN_SWITCHING_EN){
     EFI_TIME              RtcTime = { 0 };

     WIRE_PRINT((EFI_D_ERROR, "[LanLinkSmiCallBack]Lan/Wlan switching Enable, WaitingForLanLinkDebounceSMI = %x!!!\n",mWaitingForLanLinkDebounceSMI));

     if(mWaitingForLanLinkDebounceSMI){
        if (!EFI_ERROR (HpRtcGetTime(&RtcTime))) {
           if (CompareTimeStamp (&RtcTime, &mRtcTimeStamp) > 2) {
              mWaitingForLanLinkDebounceSMI = FALSE;
           }
        }
     }

     if(mWaitingForLanLinkDebounceSMI == FALSE){
        HpRtcGetTime (&mRtcTimeStamp);
#ifdef GROUP_GPIO
        mSavedLanLinkState = HpGpioRead(PcdGet8 (PcdLanLinkGpioPinGroup),PcdGet8 (PcdLanLinkGpioPin));    //Read the GPIO LVL value
#else
        mSavedLanLinkState = HpGpioRead(PcdGet8 (PcdLanLinkGpioPin));    //Read the GPIO LVL value
#endif
        mWaitingForLanLinkDebounceSMI = TRUE;

        WIRE_PRINT((EFI_D_ERROR, "[LanLinkSmiCallBack]Lan/Wlan switching Enable, mSavedLanLinkState = %x!!!\n",mSavedLanLinkState));
        EnableWatchdogTimerEvent(1, 2);
     }

  }else{
     HpAltGpiSMIEnWrite(PcdGet8 (PcdLanLinkGpioPinGroup), PcdGet8 (PcdLanLinkGpioPin), 0);
  }
  // Still need to toggle the invert bit so the next SMI will fire.
  HpGpioINVInvert (PcdGet8 (PcdLanLinkGpioPinGroup), PcdGet8 (PcdLanLinkGpioPin));

  WIRE_PRINT ((EFI_D_ERROR, "\n[LanWlanGpiSmi]  LanLinkSmiCallBack End.\n"));

//  ReportStatusCode (EFI_PROGRESS_CODE, HP_WIRELESS_SMM_LANWLAN_EXIT); // [PostCode:0x9C14]

}
//*********************************************************************************************************************
// Function:  LanLinkCallbackRegister
//
// Summary:   Register an GPI event callback when LANLINK. Enable GPIO GPI event.
//
// Global Variables:  none
//
// Parameters:     VOID
//
// Function Returns:  void
//
// Details:
//
//*********************************************************************************************************************
EFI_STATUS
LanLinkCallbackRegister (
  VOID
  )
{
  EFI_STATUS                       Status = EFI_UNSUPPORTED;
  EFI_HANDLE                       GpiHandle = NULL;
  EFI_SMM_GPI_DISPATCH2_PROTOCOL   *GpiDispatch = NULL;
  EFI_SMM_GPI_REGISTER_CONTEXT     GpiContext;
  UINTN                            GpiNum = 0;
  UINT32                           GpioPad = 0;
  EFI_HANDLE                       DispatchHandle = NULL;
  SioSmiRegisterContext_t          MiscRegisterContext;
  static SioSmiDispatchProtocol_t  *SioSmiDispatchProtocol = NULL;
  EFI_HP_SMM_DISPATCH_PROTOCOL     *HpSmmDispatch;
  EFI_HP_SWSMI_CONTEXT             HpSwSmiContext;
  EFI_HANDLE                       HpSmmDispatchHandle;

  if (PchSeries () == PCH_LP) {
     GpioPad = GPIO_VER2_LP_GPP_B0;
  } else {
     GpioPad = GPIO_VER4_S_GPP_A0;
  }

  //
  // Register Port Replicator SMI handler
  //
  // register LanLink SMI on two-chip platform only until this wokrs on one-chip.
  if (PcdGet8 (PcdLanLinkGpioPin) != 0xFF)
  {
     GpioPad |= ((PcdGet8 (PcdLanLinkGpioPinGroup) << 16) | PcdGet8 (PcdLanLinkGpioPin));
     WIRE_PRINT ((EFI_D_ERROR, "\n[LanWlanGpiSmi] GpioPad %x\n", GpioPad));

     Status = GpioGetGpiSmiNum(GpioPad, &GpiNum);
     GpiContext.GpiNum = GpiNum;
     WIRE_PRINT ((EFI_D_ERROR, "\n[LanWlanGpiSmi] GpiNum: %x\n", GpiNum));

     HpAltGpiSMIEnWrite(PcdGet8 (PcdLanLinkGpioPinGroup), PcdGet8 (PcdLanLinkGpioPin), 1);
     //
     // Have to use "gBS->LocateProtocol" since "gEfiSmmGpiDispatchProtocolGuid" is installed via gBS->InstallMultipleProtocolInterfaces
     //
     Status = gSmst->SmmLocateProtocol (&gEfiSmmGpiDispatch2ProtocolGuid, NULL, (VOID **) &GpiDispatch);

     Status = GpiDispatch->Register (
               GpiDispatch,
               (EFI_SMM_HANDLER_ENTRY_POINT2) LanLinkSmiCallBack,
               &GpiContext,
               &GpiHandle
               );

     ASSERT_EFI_ERROR (Status);
     //
     // Locate the SIO SMI dispatcher protocol
     //
     Status = gSmst->SmmLocateProtocol(
                 &gSioSmiDispatchProtocolGuid,
                 NULL,
                 &SioSmiDispatchProtocol
              );
      //
      // Register the HW monitoring SMI handler through the SIO SMI dispatcher interface
      //
      MiscRegisterContext.Order = SIO_SMM_DEFAULT_PRIORITY;

      Status = SioSmiDispatchProtocol->Register(
                  SioTimerLanLinkHandle,
                  &MiscRegisterContext,
                  &DispatchHandle
               );
      // Still need to toggle the invert bit so the next SMI will fire. // WA for Lan status error
      HpGpioINVInvert (PcdGet8 (PcdLanLinkGpioPinGroup), PcdGet8 (PcdLanLinkGpioPin));

      //
      // Get the Sw dispatch protocol and register SMI callback functions.
      //
      Status = gSmst->SmmLocateProtocol (&gEfiHpSmmDispatchProtocolGuid, NULL, &HpSmmDispatch);
      ASSERT_EFI_ERROR (Status);

      //
      // Register SWSMI for Lan/Wlan WA.
      //
      HpSwSmiContext.KeySwSmiValue = 0xEA9E;
      Status = HpSmmDispatch->RegisterSwSmi(
                                 HpSmmDispatch,
                                 LanWlanSwitchingWAResumeCallback,
                                 &HpSwSmiContext,
                                 &HpSmmDispatchHandle
                                );
      ASSERT_EFI_ERROR (Status);

   }

   return Status;
}
EFI_STATUS
LanWlanSwitchingWAResumeCallback (
   IN       EFI_HANDLE              DispatchHandle,
   IN       EFI_HP_SWSMI_CONTEXT    *RegisteredContext,
   IN OUT   EFI_SMM_SW_CONTEXT      *DispatchContext
)
{
//   ReportStatusCode (EFI_PROGRESS_CODE, HP_WIRELESS_SMM_LANWLAN_WA_ENTRY); // [PostCode:0x8C0F]

   if ((mWLInfo->WLFlags & LANWAN_SWITCHING_EN) && (mHpSharedAslData->OSInfo == WINDOWS_7))
   {
      if ((!mSMIHadBeTrigger) || (mHpCommonAcpiArea->REBX == 3))
      {
         mWaitingForLanLinkDebounceSMI = TRUE;
         HpRtcGetTime (&mRtcTimeStamp);
         mSavedLanLinkState = HpGpioRead(PcdGet8 (PcdLanLinkGpioPinGroup),PcdGet8 (PcdLanLinkGpioPin));    //Read the GPIO LVL value
         EnableWatchdogTimerEvent(1, 5);
         if (mHpCommonAcpiArea->REBX == 3)
         {
            // Still need to toggle the invert bit so the next SMI will fire. // WA for Lan status error
            HpGpioINVWrite (PcdGet8 (PcdLanLinkGpioPinGroup), PcdGet8 (PcdLanLinkGpioPin), 0);
         }
      }
   }

//   ReportStatusCode (EFI_PROGRESS_CODE, HP_WIRELESS_SMM_LANWLAN_WA_EXIT); // [PostCode:0x9C0F]

   return EFI_SUCCESS;
}

//*********************************************************************************************************************
// Function:  HpWirelessDevSmmPortingEntry
//
// Summary:
//   Initial entry point for DXE SMM driver.  This function installs a protocol used to control wireless device enable/disable
//   , Lan/Wlan switch for different chipset setting.
//
// Global Variables:
//
// Parameters:
//     ----------------------------------------------------------------------------------------------------------------
//     Name:                   ImageHandle
//     Description:            Standard parameter for driver entry point
//     Valid values:           non-NULL
//     ----------------------------------------------------------------------------------------------------------------
//     Name:                   SystemTable
//     Description:            Standard parameter for driver entry point
//     Valid values:           non-NULL
//     ----------------------------------------------------------------------------------------------------------------
//
// Function Returns:  EFI_STATUS
//     ----------------------------------------------------------------------------------------------------------------
//     EFI_SUCCESS             -  This function successfully installed the gWirelessDevSmmPortingProtocolGuid protocol
//     Other values
//     ----------------------------------------------------------------------------------------------------------------
//
//*********************************************************************************************************************
EFI_STATUS HpWirelessDevSmmPortingEntry(
    IN EFI_HANDLE             ImageHandle,
    IN EFI_SYSTEM_TABLE       *SystemTable
)
{
   EFI_STATUS                 Status;
   EFI_HANDLE                 ProtocolHandle = NULL;
   HP_ACPI_PROTOCOL           *HpAcpiProtocol = NULL;
   UINT32                        GpioPadBase = 0;

   DEBUG (( EFI_D_ERROR, "WirelessPorting.HpWirelessDevSmmPortingEntry()\n" ));

   Status = gBS->LocateProtocol (&gHpCommonAcpiAreaProtocolGuid, NULL, &mHpCommonAcpiArea);
   ASSERT_EFI_ERROR (Status);

   Status = gBS->LocateProtocol (&gHpAcpiConDevAreaProtocolGuid, NULL, &mHpAcpiConDevAreaProtocol);
   ASSERT_EFI_ERROR (Status);

   mWLInfo = &(mHpAcpiConDevAreaProtocol->WirelessDevInfo);

   // Locate Hp ACPI Protocol
   Status = gBS->LocateProtocol (&gHpAcpiProtocolGuid, NULL, &HpAcpiProtocol);
   mHpSharedAslData = (HP_SHARED_ASL_DATA*) HpAcpiProtocol->HpCoreSharedAslData;
   
   if ( IsPchLp() ) {
      GpioPadBase = GPIO_VER2_LP_GPP_B0;
   } else {
      GpioPadBase = GPIO_VER4_S_GPP_A0;
   }
   if (PcdGet8 (PcdBluetoothGpioPin) != 0xFF)
      mHpAcpiConDevAreaProtocol->HpBluetoothRFKill = GpioPadBase|((PcdGet8 (PcdBluetoothGpioPinGroup) << 16) | PcdGet8 (PcdBluetoothGpioPin));

   //
   // Install our protocols
   //
   Status = gSmst->SmmInstallProtocolInterface ( &ProtocolHandle,
                                                 &gWirelessDevSmmPortingProtocolGuid,
                                                 EFI_NATIVE_INTERFACE,
                                                 &mWirelessDevPortingSmmProtocol);

   DEBUG (( EFI_D_ERROR, "WirelessPorting.HpWirelessDevSmmPortingEntry(): Protocol installation RC = %02x\n", Status ));

   return Status;
}

//Helper fucntion to locate variable access policy protocol
static
HpSmmCoreVariablePolicyServices_t *
LocateVariableAccessPolicyProtocol(
   void
   )
{
   if(mHpSmmVariable == NULL)
   {
      gSmst->SmmLocateProtocol (&gHpVariableAccessPolicyProtocolGuid, NULL, &mHpSmmVariable);
   }
   return mHpSmmVariable;
}

//Helper fucntion to locate variable access policy protocol
static
EFI_HP_CONNECTION_DEVICE_PROTOCOL *
LocateConnectionManagerProtocol(
   void
   )
{
   if(mHpConnectionDeviceProtocol == NULL)
   {
      gSmst->SmmLocateProtocol (&gEfiConnectionDeviceProtocolGuid, NULL, &mHpConnectionDeviceProtocol);
   }
   return mHpConnectionDeviceProtocol;
}

