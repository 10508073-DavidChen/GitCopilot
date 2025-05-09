//
// (c) Copyright 2024 HP Development Company, L.P.
// This software and associated documentation (if any) is furnished under a license and may only be used or
// copied in accordance with the terms of the license. Except as permitted by such license, no part of this
// software or documentation may be reproduced, stored in a retrieval system, or transmitted in any form or
// by any means without the express written consent of HP Development Company.
//
// File Name: HpPlatformRcPolicyOverrideHookLibPei_DM800.c
//
// Abstract: N/A
//
#include "HpPlatformRcPolicyOverrideHookLibPei.h"
#include <Library/PcdLib.h>

#if HP_SERIES == DM800

//
// Routine Description:
//   It's used to update pre-memory SI RC policy for HP Platform
// Arguments:
//   PeiServices - the pointer of PEI sercices
//   SiPreMemPolicyPpi - the pointer of SI Pre-memory policy PPI
// Returns:
//   EFI_SUCCESS - policy need be updated
//   otherwise - do nothing
//
EFI_STATUS
EFIAPI
HpPlatformUpdateRcSiPreMemPolicy (
  IN EFI_PEI_SERVICES      **PeiServices,
  IN SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi
)
{

//
// Memory Config
//
   EFI_STATUS                              Status;
   SA_MISC_PEI_PREMEM_CONFIG               *MiscPeiPreMemConfig = NULL;
   MEMORY_CONFIGURATION                    *MemConfig;
   //PEI_DMI_CONFIG                          *PeiDmiConfig = NULL;

   Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gSaMiscPeiPreMemConfigGuid, (VOID *) &MiscPeiPreMemConfig);
   ASSERT_EFI_ERROR(Status);

   Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMemoryConfigGuid, (VOID *) &MemConfig);
   ASSERT_EFI_ERROR (Status);

   //Status = GetConfigBlock ((VOID *)SiPreMemPolicyPpi, &gPeiDmiConfigGuid, (VOID *)&PeiDmiConfig);
   //ASSERT_EFI_ERROR (Status);
   // Limit DMI speed to Gen1
   //PeiDmiConfig->DmiMaxLinkSpeed = 1;

   MemConfig->ExternalInputs.SaGv = 0;  //Temp for bring up.

   MiscPeiPreMemConfig->UserBd = 1;  //1=Desktop/DT Halo
   MiscPeiPreMemConfig->SkipExtGfxScan = 1;

   MiscPeiPreMemConfig->SpdAddressTable[0]   = 0xA8;  //Controller 0 Channel A DIMM 0
   MiscPeiPreMemConfig->SpdAddressTable[1]   = 0x00;  //Controller 0 Channel A DIMM 1
   MiscPeiPreMemConfig->SpdAddressTable[2]   = 0x00;  //Controller 0 Channel B DIMM 0
   MiscPeiPreMemConfig->SpdAddressTable[3]   = 0x00;  //Controller 0 Channel B DIMM 1
   MiscPeiPreMemConfig->SpdAddressTable[4]   = 0x00;
   MiscPeiPreMemConfig->SpdAddressTable[5]   = 0x00;
   MiscPeiPreMemConfig->SpdAddressTable[6]   = 0x00;
   MiscPeiPreMemConfig->SpdAddressTable[7]   = 0x00;

   MiscPeiPreMemConfig->SpdAddressTable[8]   = 0xA0;  //Controller 1 Channel A DIMM 0
   MiscPeiPreMemConfig->SpdAddressTable[9]   = 0x00;  //Controller 1 Channel A DIMM 1
   MiscPeiPreMemConfig->SpdAddressTable[10]  = 0x00;  //Controller 1 Channel B DIMM 0
   MiscPeiPreMemConfig->SpdAddressTable[11]  = 0x00;  //Controller 1 Channel B DIMM 1
   MiscPeiPreMemConfig->SpdAddressTable[12]  = 0x00;
   MiscPeiPreMemConfig->SpdAddressTable[13]  = 0x00;
   MiscPeiPreMemConfig->SpdAddressTable[14]  = 0x00;
   MiscPeiPreMemConfig->SpdAddressTable[15]  = 0x00;

//
// DDI Config
//
   GRAPHICS_PEI_PREMEM_CONFIG            *GtPreMemConfig;

   Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gGraphicsPeiPreMemConfigGuid, (VOID *) &GtPreMemConfig);
   ASSERT_EFI_ERROR (Status);

//   GtPreMemConfig->DdiConfiguration.DdiPortAConfig = DdiPortDisabled;
   GtPreMemConfig->DdiConfiguration.DdiPortBConfig = DdiPortDisabled;
/*
   GtPreMemConfig->DdiConfiguration.DdiPortAHpd    = DdiHpdDisable;     // Need hot plug detect
   GtPreMemConfig->DdiConfiguration.DdiPortBHpd    = DdiHpdDisable;
   GtPreMemConfig->DdiConfiguration.DdiPortCHpd    = DdiHpdDisable;

   GtPreMemConfig->DdiConfiguration.DdiPort1Hpd    = DdiHpdDisable;    // No hotplug detect
   GtPreMemConfig->DdiConfiguration.DdiPort2Hpd    = DdiHpdDisable;    // No hotplug detect
   GtPreMemConfig->DdiConfiguration.DdiPort3Hpd    = DdiHpdDisable;    // No hotplug detect
   GtPreMemConfig->DdiConfiguration.DdiPort4Hpd    = DdiHpdDisable;    // No hotplug detect

   GtPreMemConfig->DdiConfiguration.DdiPortADdc    = DdiDisable;
   GtPreMemConfig->DdiConfiguration.DdiPortBDdc    = DdiDisable;
   GtPreMemConfig->DdiConfiguration.DdiPortCDdc    = DdiDisable;

   GtPreMemConfig->DdiConfiguration.DdiPort1Ddc    = DdiDisable;       // No HDMI
   GtPreMemConfig->DdiConfiguration.DdiPort2Ddc    = DdiDisable;       // No HDMI
   GtPreMemConfig->DdiConfiguration.DdiPort3Ddc    = DdiDisable;       // No HDMI
   GtPreMemConfig->DdiConfiguration.DdiPort4Ddc    = DdiDisable;       // No HDMI
*/
   /*
UINT8 DdiPortAConfig; /// The Configuration of DDI port A, this settings must match VBT's settings. DdiPortDisabled - No LFP is connected on DdiPortA, <b>DdiPortEdp - Set DdiPortA to eDP</b>
UINT8 DdiPortBConfig; /// The Configuration of DDI port B, this settings must match VBT's settings. DdiPortDisabled - No LFP is connected on DdiPortB, <b>DdiPortEdp - Set DdiPortB to eDP</b>
UINT8 DdiPortAHpd;		/// The HPD setting of DDI Port A, this settings must match VBT's settings. <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
UINT8 DdiPortBHpd;		/// The HPD setting of DDI Port B, this settings must match VBT's settings. DdiHpdDisable - Disable HPD, <b>DdiHpdEnable - Enable HPD</b>
UINT8 DdiPortCHpd;		/// The HPD setting of DDI Port C, this settings must match VBT's settings. <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
UINT8 DdiPort1Hpd;		/// The HPD setting of DDI Port 1, this settings must match VBT's settings. <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
UINT8 DdiPort2Hpd;		/// The HPD setting of DDI Port 2, this settings must match VBT's settings. <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
UINT8 DdiPort3Hpd;		/// The HPD setting of DDI Port 3, this settings must match VBT's settings. <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
UINT8 DdiPort4Hpd;		/// The HPD setting of DDI Port 4, this settings must match VBT's settings. <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
UINT8 DdiPortADdc;		/// The DDC setting of DDI Port A, this settings must match VBT's settings. <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
UINT8 DdiPortBDdc;		/// The DDC setting of DDI Port B, this settings must match VBT's settings. DdiDisable - Disable DDC, <b>DdiDdcEnable - Enable DDC </b>
UINT8 DdiPortCDdc;		/// The DDC setting of DDI Port C, this settings must match VBT's settings. <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
UINT8 DdiPort1Ddc;		/// The DDC setting of DDI Port 1, this settings must match VBT's settings. <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
UINT8 DdiPort2Ddc;		/// The DDC setting of DDI Port 2, this settings must match VBT's settings. <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
UINT8 DdiPort3Ddc;		/// The DDC setting of DDI Port 3, this settings must match VBT's settings. <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
UINT8 DdiPort4Ddc;		/// The DDC setting of DDI Port 4, this settings must match VBT's settings. <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
*/


//
// CPU Config
//
   PCH_PCIE_RP_PREMEM_CONFIG *CpuPcieRpPreMemConfig;
//   UINTN                                   Index = 0;

   Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPcieRpPreMemConfigGuid, (VOID *) &CpuPcieRpPreMemConfig);
   ASSERT_EFI_ERROR (Status);
 

// clock free run
//   for (Index = 0; Index < 7; Index++)
//   {
//      CpuPcieRpPreMemConfig->PcieClock[Index].Usage = 0x80;
//      CpuPcieRpPreMemConfig->PcieClock[Index].ClkReq = 0xFF;
//   }
//   CpuPcieRpPreMemConfig->RpEnabledMask = 0xFFFFFFFF;

   // DMI (U4)
   // CpuPcieRpPreMemConfig->PcieClock[0].Usage                = PchClockUsageUnspecified;
   // CpuPcieRpPreMemConfig->PcieClock[0].ClkReq               = 0;   
   // dGPU (P80)
   CpuPcieRpPreMemConfig->PcieClock[1].Usage                = PCIE_PCH;
   CpuPcieRpPreMemConfig->PcieClock[1].ClkReq               = 1;    
   // PCIE Gen5 SSD#1 (J39)
   CpuPcieRpPreMemConfig->PcieClock[4].Usage                = PCIE_PCH + 4;
   CpuPcieRpPreMemConfig->PcieClock[4].ClkReq               = 4;
   // PCIe Gen5 SSD#2 (J42)
   CpuPcieRpPreMemConfig->PcieClock[3].Usage                = PCIE_PCH + 3;
   CpuPcieRpPreMemConfig->PcieClock[3].ClkReq               = 3;

//
// USB Config
//

  DEBUG ((DEBUG_ERROR, "[%a] End\n",__FUNCTION__));

  return EFI_SUCCESS;
}

//
// Routine Description:
//   It's used to update pre-memory PCH RC policy for HP Platform
// Arguments:
//   PeiServices - the pointer of PEI sercices
//   SiPreMemPolicyPpi - the pointer of PCH Pre-memory policy PPI
// Returns:
//   EFI_SUCCESS - policy need be updated
//   otherwise - do nothing
//
EFI_STATUS
EFIAPI
HpPlatformUpdateRcPchPreMemPolicy (
  IN EFI_PEI_SERVICES      **PeiServices,
  IN PCH_PREMEM_POLICY_PPI  *PchPreMemPolicyPpi
)
{

//
// PCH Config
//
   PCH_PCIE_RP_PREMEM_CONFIG                  *PchPcieRpPreMemConfig;
//   UINTN                                      Index;
   EFI_STATUS                                 Status;
   //PCH_PREMEM_POLICY_PPI                      *PchPreMemPolicyPpi = NULL;

   //Status = PeiServicesLocatePpi (
   //                      &gPchPreMemPolicyPpiGuid,
   //                      0,
   //                      NULL,
   //                      (VOID **) &PchPreMemPolicyPpi
   //                      );
   //ASSERT_EFI_ERROR (Status);
   //if (EFI_ERROR (Status) || PchPreMemPolicyPpi == NULL) {
   //   DEBUG ((DEBUG_ERROR, "[HpPlatformUpdatePreMemRcPolicy] Fail to get PchPreMemPolicyPpi\n"));
   //}

   Status = GetConfigBlock ((VOID *) PchPreMemPolicyPpi, &gPcieRpPreMemConfigGuid, (VOID *) &PchPcieRpPreMemConfig);

//   clock free run
//   for (Index = 0; Index < 14; Index++)
//   {
//      PchPcieRpPreMemConfig->PcieClock[Index].Usage = 0x80;
//      PchPcieRpPreMemConfig->PcieClock[Index].ClkReq = 0xFF;
//   }
//   PchPcieRpPreMemConfig->RpEnabledMask = 0xFFFFFFFF;

   // PCIE X2 (P72) Flex IO1
   PchPcieRpPreMemConfig->PcieClock[5].Usage           = PchClockUsagePchPcie5;
   PchPcieRpPreMemConfig->PcieClock[5].ClkReq          = 5;
   // PCIE X1 (U10) Intel Lan
   PchPcieRpPreMemConfig->PcieClock[6].Usage           = PchClockUsagePchPcie3;
   PchPcieRpPreMemConfig->PcieClock[6].ClkReq          = 6;
   // PCIE X1 (J38) M.2 WLAN
   PchPcieRpPreMemConfig->PcieClock[7].Usage           = PchClockUsagePchPcie4;
   PchPcieRpPreMemConfig->PcieClock[7].ClkReq          = 7;


//
// Audio Config
//
  HDAUDIO_PREMEM_CONFIG           *HdAudioPreMemConfig = NULL;

  Status = GetConfigBlock ((VOID *) PchPreMemPolicyPpi, &gHdAudioPreMemConfigGuid, (VOID *) &HdAudioPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  HdAudioPreMemConfig->DspEnable = 0;
  HdAudioPreMemConfig->AudioLinkHda.Enable     = 1;

  HdAudioPreMemConfig->SubSystemIds = (((UINT32)PcdGet16(PcdPlatformSsid))<<16)|PcdGet16(PcdPlatformSvid);


  DEBUG ((DEBUG_ERROR, "[%a] End\n",__FUNCTION__));

  return EFI_SUCCESS;
}


//
// Routine Description:
//   It's used to update post-memory RC policy for HP Platform
// Arguments:
//   PeiServices - the pointer of PEI sercices
//   SiPolicyPpi - the pointer of Si post-memory policy PPI
// Returns:
//   EFI_SUCCESS - policy need be updated
//   otherwise - do nothing
//
EFI_STATUS
EFIAPI
HpPlatformUpdateRcSiPostMemPolicy (
  IN EFI_PEI_SERVICES   **PeiServices,
  IN SI_POLICY_PPI      *SiPolicyPpi
  )
{

//
// CPU Config
//
   EFI_STATUS                              Status;
   CPU_POWER_DELIVERY_CONFIG           *CpuPowerDeliveryConfig;
   // UINTN                                   PciePort;
   PCH_PCIE_CONFIG               *CpuPcieRpConfig = NULL;

   Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerDeliveryConfigGuid, (VOID *)&CpuPowerDeliveryConfig);
   ASSERT_EFI_ERROR (Status);

   Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPchPcieConfigGuid, (VOID *)&CpuPcieRpConfig);
   ASSERT_EFI_ERROR (Status);
   CpuPowerDeliveryConfig->DisableProcHotOut = 0x0;

   // for (PciePort = 0; PciePort < 6; PciePort++)  //HP-CorrectRcPolicy
   // {
   //   CpuPcieRpConfig->RootPort[PciePort].PcieRpCommonConfig.Aspm = PchPcieAspmDisabled;
   // }

   // dGPU (P80)
   CpuPcieRpConfig->RootPort[0].PcieRpCommonConfig.ClkReqDetect  = TRUE;
   CpuPcieRpConfig->RootPort[0].PcieRpCommonConfig.L1Substates   = PchPcieL1SubstatesL1_1_2;
   // CpuPcieRpConfig->RootPort[0].PcieRpCommonConfig.Aspm          = PchPcieAspmDisabled;
   // PCIE Gen5 SSD#1 (J39)
   CpuPcieRpConfig->RootPort[20].PcieRpCommonConfig.ClkReqDetect  = TRUE;
   CpuPcieRpConfig->RootPort[20].PcieRpCommonConfig.L1Substates   = PchPcieL1SubstatesL1_1_2;
   // CpuPcieRpConfig->RootPort[20].PcieRpCommonConfig.Aspm          = PchPcieAspmDisabled;
   // PCIe Gen5 SSD#2 (J42)
   CpuPcieRpConfig->RootPort[12].PcieRpCommonConfig.ClkReqDetect  = TRUE;
   CpuPcieRpConfig->RootPort[12].PcieRpCommonConfig.L1Substates   = PchPcieL1SubstatesL1_1_2;
   // CpuPcieRpConfig->RootPort[12].PcieRpCommonConfig.Aspm          = PchPcieAspmDisabled;

//
// USB Config
//

   TCSS_PEI_CONFIG              *TcssPeiConfig;


   Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gTcssPeiConfigGuid, (VOID *) &TcssPeiConfig);
   ASSERT_EFI_ERROR (Status);


   // OC Map for TCSS USB3.0 Ports
   TcssPeiConfig->UsbConfig.PortUsb30[0].OverCurrentPin = 3;   // P72  FLEX IO B2B              Flex IO 1
   TcssPeiConfig->UsbConfig.PortUsb30[1].OverCurrentPin = 4;   // P73  FLEX IO FLY              Flex IO 2
   TcssPeiConfig->UsbConfig.OverCurrentEnable           = TRUE;

   DEBUG ((DEBUG_ERROR, "[%a] End\n",__FUNCTION__));

   return EFI_SUCCESS;
}

//
// Routine Description:
//   It's used to update post-memory PCH policy for HP Platform
// Arguments:
//   PeiServices - the pointer of PEI sercices
//   SiPolicyPpi - the pointer of PCH post-memory policy PPI
// Returns:
//   EFI_SUCCESS - policy need be updated
//   otherwise - do nothing
//
EFI_STATUS
EFIAPI
HpPlatformUpdateRcPchPostMemPolicy (
  IN EFI_PEI_SERVICES   **PeiServices,
  IN PCH_POLICY_PPI      *PchPolicyPpi
  )
{
//
// PCH Config
//
   EFI_STATUS                              Status;
//   UINTN                                   PciePort;
   PCH_PCIE_CONFIG            *PchPcieConfig;

   Status = GetConfigBlock ((VOID *) PchPolicyPpi, &gPchPcieConfigGuid, (VOID *) &PchPcieConfig);
   ASSERT_EFI_ERROR (Status);

//   for (PciePort = 0; PciePort < 24; PciePort++)
//   {
//     PchPcieConfig->RootPort[PciePort].PcieRpCommonConfig.Aspm = PchPcieAspmDisabled;
//   }

   // PCIE X2 (P72) Flex IO1
   PchPcieConfig->RootPort[4].PcieRpCommonConfig.ClkReqDetect  = TRUE;
   PchPcieConfig->RootPort[4].PcieRpCommonConfig.L1Substates   = PchPcieL1SubstatesL1_1_2;
//   PchPcieConfig->RootPort[1].PcieRpCommonConfig.Aspm          = PchPcieAspmDisabled;
//
   // PCIE X1 (U10) Intel Lan
   PchPcieConfig->RootPort[2].PcieRpCommonConfig.ClkReqDetect  = TRUE;
   PchPcieConfig->RootPort[2].PcieRpCommonConfig.L1Substates   = PchPcieL1SubstatesDisabled;
//   PchPcieConfig->RootPort[2].PcieRpCommonConfig.Aspm          = PchPcieAspmDisabled;
//
   // PCIE X2 (J38) M.2 Wlan
   PchPcieConfig->RootPort[3].PcieRpCommonConfig.ClkReqDetect  = TRUE;
   PchPcieConfig->RootPort[3].PcieRpCommonConfig.L1Substates   = PchPcieL1SubstatesDisabled;
//   PchPcieConfig->RootPort[3].PcieRpCommonConfig.Aspm          = PchPcieAspmDisabled;


//
// USB Config
//
   USB_CONFIG                   *UsbConfig;

   Status = GetConfigBlock ((VOID *) PchPolicyPpi, &gUsbConfigGuid, (VOID *) &UsbConfig);
   ASSERT_EFI_ERROR (Status);

/*
Rear
    ____________                    ______________
   | J11 TOP(2) |  ______________  | P73 FLEX2 (5)|
  _| J11 BOT(1) |_| P72 FLEX1(3) |_| J102 TBT  (4)|_

Front
    ________   ________   ________
  _| J90(1) |_| J80(2) |_| J70(3) |_
*/

   // OC Map for USB2.0 Ports
   UsbConfig->PortUsb20[ 0].OverCurrentPin = 0xFF;       //      Not used
   UsbConfig->PortUsb20[ 1].OverCurrentPin = 0x00;       //      J11 BOT   REAR USB-A               Rear  1
   UsbConfig->PortUsb20[ 2].OverCurrentPin = 0x00;       //      J11 TOP   REAR USB-A               Rear  2
   UsbConfig->PortUsb20[ 3].OverCurrentPin = 0xFF;       //      Not used
   UsbConfig->PortUsb20[ 4].OverCurrentPin = 0xFF;       //      J102      REAR TBT                 Rear  4
   UsbConfig->PortUsb20[ 5].OverCurrentPin = 0x03;       //      P72       REAR FLEX-IO1            Rear  3
   UsbConfig->PortUsb20[ 6].OverCurrentPin = 0x05;       //      J90       FRONT TYPE-C             Front 1
   UsbConfig->PortUsb20[ 7].OverCurrentPin = 0x06;       //      P73       REAR FLEX-IO2            Rear  5
   UsbConfig->PortUsb20[ 8].OverCurrentPin = 0xFF;       //      Not used
   UsbConfig->PortUsb20[ 9].OverCurrentPin = 0x04;       //      J80       FRONT USB-A              Front 2
   UsbConfig->PortUsb20[10].OverCurrentPin = 0xFF;       //      Not used
   UsbConfig->PortUsb20[11].OverCurrentPin = 0x01;       //      J70       FRONT USB-A              Front 3
   UsbConfig->PortUsb20[12].OverCurrentPin = 0xFF;       //      Not used
   UsbConfig->PortUsb20[13].OverCurrentPin = 0xFF;       //      BlueTooth

   // OC Map for USB3.0 Ports
   UsbConfig->PortUsb30[ 0].OverCurrentPin = 0x05;       //      J90       FRONT TYPE-C             Front 1
   UsbConfig->PortUsb30[ 1].OverCurrentPin = 0x05;       //      J90       FRONT TYPE-C             Front 1
   UsbConfig->PortUsb30[ 2].OverCurrentPin = 0x01;       //      J70       FRONT USB-A              Front 3
   UsbConfig->PortUsb30[ 3].OverCurrentPin = 0x04;       //      J80       FRONT USB-A              Front 2
   UsbConfig->PortUsb30[ 4].OverCurrentPin = 0xFF;       //      Not used
   UsbConfig->PortUsb30[ 5].OverCurrentPin = 0xFF;       //      Not used
   UsbConfig->PortUsb30[ 6].OverCurrentPin = 0x00;       //      J11 BOT   REAR USB-A               Front 1
   UsbConfig->PortUsb30[ 7].OverCurrentPin = 0x00;       //      J11 TOP   REAR USB-A               Front 2
   UsbConfig->PortUsb30[ 8].OverCurrentPin = 0x06;       //      P73       REAR FLEX-IO2            Rear  5
   UsbConfig->PortUsb30[ 9].OverCurrentPin = 0xFF;       //      Not used
   UsbConfig->OverCurrentEnable            = TRUE;    //TRUE/FALSE Enable USB OC mapping control
   UsbConfig->UaolEnable                   = 0;       //Disable USB Audio Offload
   UsbConfig->PdoProgramming               = FALSE;

   DEBUG ((DEBUG_ERROR, "[%a] End\n",__FUNCTION__));

   return EFI_SUCCESS;
}


#endif
