//
// (c) Copyright 2014 - 2024 HP Development Company, L.P.
// This software and associated documentation (if any) is furnished under a license and may only be used or
// copied in accordance with the terms of the license. Except as permitted by such license, no part of this
// software or documentation may be reproduced, stored in a retrieval system, or transmitted in any form or
// by any means without the express written consent of HP Development Company.
//
// File Name: Dsdt.asl
//
// Abstract: N/A
//
/** @file
  ACPI DSDT table

@copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2023 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worldwide copyright and trade secret laws and treaty
  provisions. No part of the Material may be used, copied, reproduced, modified,
  published, uploaded, posted, transmitted, distributed, or disclosed in any way
  without Intel's prior express written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  of the Materials, either expressly, by implication, inducement, estoppel or
  otherwise. Any license under such intellectual property rights must be
  express and approved by Intel in writing.

  Unless otherwise agreed by Intel in writing, you may not remove or alter
  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.

  This file contains a 'Sample Driver' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may be modified
  by the user, subject to the additional terms of the license agreement.

@par Specification Reference:
**/

include "PlatformBoardId.h"
#include <Include/AcpiDebug.h>
#include <Register/PmcRegs.h>


//[HP_ADD]+ Define for HP ASL code
#include <AcpiAsl.h>
#include <CoreLongNameMapAsl.h>
//[HP_FEATURE]+
#include <HpAcpiFeatureLongNameMapAsl.h>
#include <HpAcpiFeatureAslDef.h>
//[HP_FEATURE]-
#include <DsdtDef.h>
#include <PlatformAcpiAsl.h>
#include <PlatLongNameMapAsl.h>
#include <HpIntelChipsetLongNameMapAsl.h>
#include <ConDevAcpiDef.h>
#include <ConDevLongNameMapAsl.h>
#include <HpSioLongNameMapAsl.h>
#include <HpDtLongNameMapAsl.h>
#include <HpTbtAcpiDef.h>
#include <HpPrivateWmiSureStartSymbols.h>
//[HP_ADD]-

// Definitions for INTB (Interrupt descriptor buffer method)
#define INT_LEVEL_TRIG  0
#define INT_EDGE_TRIG   1
#define INT_ACTIVE_LOW  0
#define INT_ACTIVE_HIGH 1

DefinitionBlock (
  "DSDT.aml",
  "DSDT",
  0x02, // DSDT revision.
        // A Revision field value greater than or equal to 2 signifies that integers
        // declared within the Definition Block are to be evaluated as 64-bit values
  "INTEL",   // OEM ID (6 byte string)
  "ARL-S   ",// OEM table ID  (8 byte string)
  0x0 // OEM version of DSDT table (4 byte Integer)
)

// BEGIN OF ASL SCOPE
{
  External (\_SB.PC00.GFX0.CLID)
  External (\_SB.PC00.GFX0.CDCK)
  External (\_SB.PC00.GFX0.DD1F, DeviceObj)
  External (\_SB.PC00.GFX0.DD2F, DeviceObj)
  External(\_SB.PC00.SAT0.PRT6, DeviceObj)
  External(\_SB.PC00.SAT0.PRT7, DeviceObj)
  External (\_SB.PC00.GFX0.GDCK, MethodObj)
  External (\_SB.PC00.GFX0.GLID, MethodObj)
  External (\_SB.PC00.RP01, DeviceObj)
  External (\_SB.PC00.RP02, DeviceObj)
  External (\_SB.PC00.RP03, DeviceObj)
  External (\_SB.PC00.RP04, DeviceObj)
  External (\_SB.PC00.RP05, DeviceObj)
  External (\_SB.PC00.RP06, DeviceObj)
  External (\_SB.PC00.RP07, DeviceObj)
  External (\_SB.PC00.RP08, DeviceObj)
  External (\_SB.PC00.RP09, DeviceObj)
  External (\_SB.PC00.RP10, DeviceObj)
  External (\_SB.PC00.RP11, DeviceObj)
  External (\_SB.PC00.RP12, DeviceObj)
  External (\_SB.PC02.RP01, DeviceObj)
  External (\_SB.PC02.RP04, DeviceObj)
  External (\_SB.PC02.RP05, DeviceObj)
  External (\_SB.PC02.RP08, DeviceObj)
  External (\_SB.PC02.RP09, DeviceObj)
  External (\_SB.PC02.RP21, DeviceObj)
  External (\_SB.PR00._PSS, MethodObj)
  External (\_SB.PR00.LPSS, PkgObj)
  External (\_SB.PR00.TPSS, PkgObj)
  External (\_SB.PR00._PPC, MethodObj)
  External (\_SB.CPPC, IntObj)
  External (\_TZ.TZ00, DeviceObj)
  External (\_TZ.ETMD, IntObj)
  External (\_TZ.FN00._OFF, MethodObj)
  External (\_SB.PC00.TXHC, DeviceObj) // CPU XHCI object of TCSS, it is a must existed object for CPU TCSS
  External (\STD3, FieldUnitObj) // 0: D3 disable. 1: D3Hot support for Storages. 2: D3Cold support for Storages.
  // Miscellaneous services enabled in Project
  External(DGBA)
  External(\_SB.PC00.RP12.CLID)
  External(\_SB.PC00.RP12.CBLV)
  External(\_SB.PC00.RP12.DLID, MethodObj)
  External(\_SB.PGPI.GGPE, MethodObj)
//[HP_ADD]+  
  //
  // HP: PCCD.PENB is defined in Cppc.asl, which is built into its own SSDT, and referenced in Platform.asl
  //
  //External(\_SB.PCCD.PENB, IntObj)
//[HP_ADD]-
  Name (SANB, 0xFFFF0000)  // SA NVS Base address
  Name (SANL, 0xAA55)      // SA NVS Length
  Name (VMNB, 0xFFFF0000)
  Name (VMNL, 0xAA55)

  Name (BRMT, 0)
  Name (RDLY, 160)

//[HP_ADD]+
  Include ("CoreSupport.asl")       // HP: various core code including core shared ASL memory
#if HP_MEMORY_DEBUG_ENABLE == 1
//  Include ("MemDebug.asl")
#endif
  Include ("HpSioSharedAslDef.h")   // HP: various common code including SioAcpiAreaData ASL memory
  Include ("HpGfxSharedAslDef.h")   // HP: various common code including AcpiGfxArea ASL memory
//TBD
//  Include ("HpThermalSharedAslDef.h")   // HP: various common code including AcpiThermalArea ASL memory
//TBD
  Include ("HpConDevSharedAslDef.h")// HP: various common code including AcpiConDevArea ASL memory
//TBD
//  Include ("PublicWmi.asl")         // HP: Public WMI methods
//  Include ("PrivateWmi.asl")        // HP: Private WMI methods
//TBD
  Include ("HpCommonSharedAslDef.h")
  Include ("HpIntelChipsetSharedAslDef.h") // HP: various common code including AcpiIntelArea ASL memory
  Include ("HpDtSharedAslDef.h")
//  Include ("HpIntelChipsetEvent.asl")
  Include ("HpStorageSharedAslDef.h")      // HP: various common code including HpStorageAcpiArea ASL memory
  Include ("HpFeatureAcpiNvsAreaDef.h")
//[HP_ADD]-
  Include ("PcieRpObjCommonExternalInclude.asl")

  Include ("SaNvs.asl")
#if (FixedPcdGetBool (PcdVmdEnable) == 1)
  Include ("VmdNvs.asl")
#endif
  Include ("AmlUpd.asl")
  Include ("PlatformNvs.asl")
  Include ("PciTree.asl")
  If (LEqual (PCHS, PCH_S)) {
    #define WADT_WAKE          GPE_PME
    #define WADT_ACPI_DEV_PATH \_SB.PC02.PPMC
    Scope(\_SB.PC02) {
      Include ("TimeAndAlarmDevice.asl")
    }
    #undef WADT_ACPI_DEV_PATH
    #undef WADT_WAKE
  } Else {
    #define WADT_WAKE          GPE_WADT_EN
    #define WADT_ACPI_DEV_PATH \_SB.PC00
    Scope(\_SB.PC00) {
      Include ("TimeAndAlarmDevice.asl")
    }
    #undef WADT_ACPI_DEV_PATH
    #undef WADT_WAKE
  }
//[HP_RMV]+
//  Include ("AcpiAssert.asl")
//[HP_RMV]-
  if (LEqual (ECR1,1)) {
    Scope (\_SB.PC00) {
      //
      // PCI-specific method's GUID
      //
      Name (PCIG, ToUUID ("E5C937D0-3553-4d7a-9117-EA4D19C3434D"))
      //
      // PCI's _DSM - an attempt at modular _DSM implementation
      // When writing your own _DSM function that needs to include PCI-specific methods, do this:
      //
      // Method (_YOUR_DSM,4) {
      //   if (Lequal (Arg0,PCIG)) { return (PCID (Arg0,Arg1,Arg2,Arg3)) }
      //   ...continue your _DSM by checking different GUIDs...
      //   else { return (0) }
      // }
      //
      Method (PCID, 4, Serialized) {
        If (LEqual (Arg0, PCIG)) {         // PCIE capabilities UUID
          If (LGreaterEqual (Arg1,3)) {                                                // revision at least 3
            If (LEqual (Arg2,0)) { Return (Buffer (2) {0x01,0x03}) }                   // function 0: list of supported functions
            If (LEqual (Arg2,8)) { Return (1) }                                        // function 8: Avoiding Power-On Reset Delay Duplication on Sx Resume
            If (LEqual (Arg2,9)) { Return (Package (5) {50000,Ones,Ones,50000,Ones}) } // function 9: Specifying Device Readiness Durations
          }
        }
        return (Buffer (1) {0})
      }
    }//scope
    If (CondRefOf (\_SB.PC02)) {
      Scope (\_SB.PC02) {
        Method (PCID, 4, Serialized) {
          Return (\_SB.PC02.PCID (Arg0, Arg1, Arg2, Arg3))
        }
      }
    }
  }//if

  Scope (\_SB.PC00) {
    //PciCheck, Arg0=UUID, returns true if support for 'PCI delays optimization ECR' is enabled and the UUID is correct
    Method (PCIC,1,Serialized) {
      If (LEqual (ECR1,1)) {
        If (LEqual (Arg0, PCIG)) {
          return (1)
        }
      }
      return (0)
    }

    If (LEqual (PCHA, 0)) {
      //
      // Create I2C Bus Resource descriptor for _CRS usage
      // Arg0 - I2C bus address of the connection (Target Address)
      // Arg1 - I2C controller number (Resource Source)
      // Returns buffer with 'I2cSerialBusV2' resource descriptor
      //
      Method (IICB, 2, Serialized) {
        Switch (ToInteger (Arg1)) {
          Case (0) {Name (IIC0, ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC00.I2C0", 0x00, ResourceConsumer, DEV0, , ) })
            CreateWordField (IIC0, DEV0._ADR, DAD0)
            Store (Arg0, DAD0)
            Return (IIC0) }
          Case (1) {Name (IIC1, ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC00.I2C1", 0x00, ResourceConsumer, DEV1, , ) })
            CreateWordField (IIC1, DEV1._ADR, DAD1)
            Store (Arg0, DAD1)
            Return (IIC1) }
          Case (2) {Name (IIC2, ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC00.I2C2", 0x00, ResourceConsumer, DEV2, , ) })
            CreateWordField (IIC2, DEV2._ADR, DAD2)
            Store (Arg0, DAD2)
            Return (IIC2) }
          Case (3) {Name (IIC3, ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC00.I2C3", 0x00, ResourceConsumer, DEV3, , ) })
            CreateWordField (IIC3, DEV3._ADR, DAD3)
            Store (Arg0, DAD3)
            Return (IIC3) }
          Case (4) {Name (IIC4, ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC00.I2C4", 0x00, ResourceConsumer, DEV4, , ) })
            CreateWordField (IIC4, DEV4._ADR, DAD4)
            Store (Arg0, DAD4)
            Return (IIC4) }
          Case (5) {Name (IIC5, ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC00.I2C5", 0x00, ResourceConsumer, DEV5, , ) })
            CreateWordField (IIC5, DEV5._ADR, DAD5)
            Store (Arg0, DAD5)
            Return (IIC5) }
          Case(6) { Name (IIC6 , ResourceTemplate () { I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC00.I2C6", 0x00, ResourceConsumer, DEV6,) })
            CreateWordField (IIC6, DEV6._ADR, DAD6)
            Store (Arg0, DAD6)
            Return (IIC6) }
          Case(7) { Name (IIC7 , ResourceTemplate () { I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC00.I2C7", 0x00, ResourceConsumer, DEV7,) })
            CreateWordField (IIC7, DEV7._ADR, DAD7)
            Store (Arg0, DAD7)
            Return (IIC7) }
          Default {Return (0) }
        }
      }
    }

    //
    // Create I2C Bus Resource descriptor for _CRS usage
    // Arg0 - I2C bus address of the connection (Peripheral Address)
    // Arg1 - I2C Device number
    // Returns buffer with 'I2cSerialBusV2' resource descriptor
    //
    Method (VIIC, 2, Serialized) {
      Switch (ToInteger (Arg1))
      {
        Case (Zero) // When virtual I2C device VIC0 is selected
        {
          Name (VI00, ResourceTemplate () {
              I2cSerialBusV2 (0, ControllerInitiated, 400000,
                  AddressingMode7Bit, "\\_SB.PC00.XHCI.RHUB.VIC0",
                  0x00, ResourceConsumer, VI0P,)
          })

          CreateWordField (VI00, VI0P._ADR, DAR0)
          Store (Arg0, DAR0)
          Return (VI00)
        }
        Case (One) // When virtual I2C device VIC1 is selected
        {
          Name (VI11, ResourceTemplate () {
              I2cSerialBusV2 (0, ControllerInitiated, 400000,
                  AddressingMode7Bit, "\\_SB.PC00.XHCI.RHUB.VIC1",
                  0x00, ResourceConsumer, VI1M,)
          })

          CreateWordField (VI11, VI1M._ADR, DAQ1)
          Store (Arg0, DAQ1)
          Return (VI11)
        }
        Default
        {
          Return (ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000,AddressingMode7Bit, "\\_SB.PC00.XHCI.RHUB.VICX",0x00, ResourceConsumer, ,)}) // Dummy
        }
      }
    }

    //
    // Create Interrupt Resource descriptor for _CRS usage
    // Arg0 - GPIO Pad used as Interrupt source
    // Arg1 - Edge (1) or Level (0) interrupt triggering
    // Arg2 - Active Level: High (1) or Low (0)
    // Returns buffer with 'Interrupt' resource descriptor
    //
    Method (INTB, 3, Serialized) {
      Name (INTR, ResourceTemplate () { Interrupt (ResourceConsumer, Level, ActiveLow, ExclusiveAndWake,,,INTI) {0} })
      // Update APIC Interrupt descriptor
      CreateDWordField (INTR, INTI._INT, NUMI) // Interrupt Number
      Store (INUM (Arg0), NUMI)
      CreateBitField (INTR, INTI._HE, LEVI) // Level or Edge
      Store (Arg1, LEVI)
      CreateBitField (INTR, INTI._LL, ACTI) // Active High or Low
      Store (Arg2, ACTI)

      Return (INTR)
    }

  } // END Scope (\_SB.PC00)

  If (CondRefOf (\_SB.PC02)) {
    Scope (\_SB.PC02) {
      //PciCheck, Arg0=UUID, returns true if support for 'PCI delays optimization ECR' is enabled and the UUID is correct
      Method (PCIC,1,Serialized) {
        Return (\_SB.PC00.PCIC (Arg0))
      }
      Method (IICB, 2, Serialized) {
        Switch (ToInteger (Arg1)) {
          Case (0) {Name (IIC0, ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC02.I2C0", 0x00, ResourceConsumer, DEV0, , ) })
            CreateWordField (IIC0, DEV0._ADR, DAD0)
            Store (Arg0, DAD0)
            Return (IIC0) }
          Case (1) {Name (IIC1, ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC02.I2C1", 0x00, ResourceConsumer, DEV1, , ) })
            CreateWordField (IIC1, DEV1._ADR, DAD1)
            Store (Arg0, DAD1)
            Return (IIC1) }
          Case (2) {Name (IIC2, ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC02.I2C2", 0x00, ResourceConsumer, DEV2, , ) })
            CreateWordField (IIC2, DEV2._ADR, DAD2)
            Store (Arg0, DAD2)
            Return (IIC2) }
          Case (3) {Name (IIC3, ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC02.I2C3", 0x00, ResourceConsumer, DEV3, , ) })
            CreateWordField (IIC3, DEV3._ADR, DAD3)
            Store (Arg0, DAD3)
            Return (IIC3) }
          Case (4) {Name (IIC4, ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC02.I2C4", 0x00, ResourceConsumer, DEV4, , ) })
            CreateWordField (IIC4, DEV4._ADR, DAD4)
            Store (Arg0, DAD4)
            Return (IIC4) }
          Case (5) {Name (IIC5, ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC02.I2C5", 0x00, ResourceConsumer, DEV5, , ) })
            CreateWordField (IIC5, DEV5._ADR, DAD5)
            Store (Arg0, DAD5)
            Return (IIC5) }
          Case(6) { Name (IIC6 , ResourceTemplate () { I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC02.I2C6", 0x00, ResourceConsumer, DEV6,) })
            CreateWordField (IIC6, DEV6._ADR, DAD6)
            Store (Arg0, DAD6)
            Return (IIC6) }
          Case(7) { Name (IIC7 , ResourceTemplate () { I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC02.I2C7", 0x00, ResourceConsumer, DEV7,) })
            CreateWordField (IIC7, DEV7._ADR, DAD7)
            Store (Arg0, DAD7)
            Return (IIC7) }
          Default {Return (0) }
        }
      }

      //
      // Create Interrupt Resource descriptor for _CRS usage
      // Arg0 - GPIO Pad used as Interrupt source
      // Arg1 - Edge (1) or Level (0) interrupt triggering
      // Arg2 - Active Level: High (1) or Low (0)
      // Returns buffer with 'Interrupt' resource descriptor
      //
      Method (INTB, 3, Serialized) {
        Name (INTR, ResourceTemplate () { Interrupt (ResourceConsumer, Level, ActiveLow, ExclusiveAndWake,,,INTI) {0} })
        // Update APIC Interrupt descriptor
        CreateDWordField (INTR, INTI._INT, NUMI) // Interrupt Number
        Store (INUM (Arg0), NUMI)
        CreateBitField (INTR, INTI._HE, LEVI) // Level or Edge
        Store (Arg1, LEVI)
        CreateBitField (INTR, INTI._LL, ACTI) // Active High or Low
        Store (Arg2, ACTI)

        Return (INTR)
      }
    }
  }

  Include ("MtlSoc.asl")

  If (LNotEqual (PCHA, 0)) {
    Include ("MtlPch.asl")
  }

#if (FixedPcdGetBool (PcdMtlSSupport) == 1)
  Include ("PcieMtlS.asl")
  #define PCH_ROOT_BRIDGE   PC02
  #define CNVI_BT_PORT      HS14
  #define CNVI_UART_DEV_STR "\\_SB.PC02.UA00"
  #define CNVI_GNUM_STR     "\\_SB.PGPI"
  #define CNVI_GNUM_PATH    \_SB.PGPI
  #define PCH_USB2_NUM      \_SB.PC02.PU2C
  #define USB_BT_PORT       14
  #define GPIOV2_COMMON_PATH \_SB.PGPI
#else
  Include ("PcieMtl.asl")
  #define PCH_ROOT_BRIDGE   PC00
  #define CNVI_BT_PORT      HS10
  #define CNVI_UART_DEV_STR "\\_SB.PC00.UA00"
  #define CNVI_GNUM_STR     "\\_SB.GPI0"
  #define CNVI_GNUM_PATH    \_SB
  #define PCH_USB2_NUM      \PU2C
  #define USB_BT_PORT       10
  #define GPIOV2_COMMON_PATH \_SB
#endif
  #define GPIO_RESOURCE       CNVI_GNUM_STR
  #define GPIO_RESOURCE_PATH  CNVI_GNUM_PATH

  Include ("LpcB.asl")
  Include ("Platform.asl")
  Include ("Cpu.asl")
  Include ("PciDrc.asl")
//[HP_ADD]+
//  Include ("ThermalZone.asl")
//[HP_ADD]-

//[HP_ADD]+ This is Intel CRB solution to handle Brightness notification
#if CRB_BOOT_SUPPORT
//[HP_ADD]-
  Include ("Video.asl")
//[HP_ADD]+
#endif
//[HP_ADD]-
  Include ("PlatformGnvsTestMenu.asl")

//[HP_ADD]+ related HP GFX ACPI control methods
//  Include ("MiscGfx.asl")
//  Include ("VideoDev.asl")      // Intel IGD device (B0, D2, F0)
//[HP_ADD]-
  Include ("Gpe.asl")
//[HP_ADD]+
#if CRB_BOOT_SUPPORT
//[HP_ADD]-
  Include ("PcieDock.asl")
  Include ("PchRpPxsxWrapper.asl")
  Include ("WifiDynamicSar.asl")
  Include ("Pep.asl")
  Include ("Psm.asl")
  Include ("Connectivity.asl")
//[HP_ADD]+
#endif
//[HP_ADD]-
  Include ("HdaDspModules.asl")
  Include ("HdaCodecsDevices.asl")
//[HP_ADD]+ HP specific PCH related code in HpBdwPkg  (must be after PCH.ASL to build)
  Include ("PchSupport.asl")
//[HP_ADD]-
  If (LEqual (PCHA, 0)) {
    Include ("MipiCamSensors.asl")
    Include ("WirelessConfigDevice.asl")
    Include ("Pld.asl")
  }
#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  If (LNotEqual (PCHA, 0)) {
    If (LNotEqual (WITX, 0)) {
      Include ("SerialIoTestDevicesEmbeddedPch.asl")
    }
  } Else {
    Include ("SerialIoTestDevicesEmbedded.asl")
  }
#else
#endif

  Include ("SerialIoDevices.asl")
#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  Include ("SerialIoTimingParametersEmbedded.asl")
#else
#endif
//[HP_ADD]+
#if CRB_BOOT_SUPPORT
//[HP_ADD]-
  Include ("HidPlatformEventDev.asl")
  Include ("HidDynamicSarDsm.asl")
//[HP_ADD]+
#endif
//[HP_ADD]-
  Include ("PinDriverLib.asl")
//[HP_ADD]+
#if CRB_BOOT_SUPPORT
//[HP_ADD]-
  If (LEqual (CVFS, 1)) {
    If (ARLP) {
      Include ("Cvs.asl")
    } Else {
    Include ("Cvf.asl")
  }
//[HP_ADD]+
#endif
//[HP_ADD]-

#if (FixedPcdGetBool (PcdVmdEnable) == 1)
   If (LEqual(VMDE,1)) {
    Name(VDSD, 1) // VMD _DSD Method - 1: Expose 0: Hide

    If (LEqual (\STD3, 0)) {
      Store (0, VDSD)
    }
    ADBG (Concatenate ("VMD _DSD Method option:", ToHexString (VDSD)))
#if (FixedPcdGetBool (PcdMtlSSupport) == 1)
    Include ("VmdMtlS.asl")
#else
    Include ("VmdMtl.asl")
#endif
  }
#endif

  //
  // PCIe RootPort Power Resources for Wake
  //
#if (FixedPcdGetBool (PcdMtlSSupport) == 0)

  Scope (\_SB.PC00.RP01) {
    Method (_PRW, 0) {
      If (LAnd (LNotEqual (P1WG, 0), LEqual (P1RN, 0x01))) {
        Return (GPRW (GGPE (P1WG), 4))
      } Else {
        Return (GPRW (GPE1_PCI_EXP_SPA, 4))
      }
    }
  }

  Scope (\_SB.PC00.RP02) {
    Method (_PRW, 0) {
      Return (GPRW (GPE1_PCI_EXP_SPA, 4))
    }
  }
  Scope (\_SB.PC00.RP03) {
    Method (_PRW, 0) {
      If (LAnd (LNotEqual (WLWK, 0), LEqual (WLRP, 0x03))) {
        Return (GPRW (GGPE (WLWK), 4))
      } Else {
        Return (GPRW (GPE1_PCI_EXP_SPA, 4))
      }
    }
  }

  Scope (\_SB.PC00.RP04) {
    Method (_PRW, 0) {
      Return (GPRW (GPE1_PCI_EXP_SPA, 4))
    }
  }
  Scope (\_SB.PC00.RP05) {
    Method (_PRW, 0) {
      If (LAnd (LNotEqual (P2WG, 0), LEqual (P2RN, 0x05))) {
        Return (GPRW (GGPE (P2WG), 4))
      } Else {
        Return (GPRW (GPE1_PCI_EXP_SPB, 4))
      }
    }
  }
  Scope (\_SB.PC00.RP06) {
    Method (_PRW, 0) {
      If (LAnd (LNotEqual (P2WG, 0), LEqual (P2RN, 0x06))) {
        Return (GPRW (GGPE (P2WG), 4))
      } ElseIf (LAnd (LNotEqual (P3WG, 0), LEqual (P3RN, 0x06))) {
        Return (GPRW (GGPE (P3WG), 4))
      } Else {
        Return (GPRW (GPE1_PCI_EXP_SPB, 4))
      }
    }
  }
  Scope (\_SB.PC00.RP07) {
    Method (_PRW, 0) {
      If (LAnd (LNotEqual (WWKP, 0), LEqual (WWRP, 0x07))) {
        Return (GPRW (GGPE (WWKP), 4))
      } Else {
        Return (GPRW (GPE1_PCI_EXP_SPB, 4))
      }
    }
  }
  Scope (\_SB.PC00.RP08) {
    Method (_PRW, 0) {
      If (LAnd (LNotEqual (WLWK, 0), LEqual (WLRP, 0x08))) {
        Return (GPRW (GGPE (WLWK), 4))
      } Else {
        Return (GPRW (GPE1_PCI_EXP_SPB, 4))
      }
    }
  }

  Scope (\_SB.PC00.RP09) {
    Method (_PRW, 0) {
      If (LAnd (LNotEqual (WWKP, 0), LEqual (WWRP, 0x09))) {
        Return (GPRW (GGPE (WWKP), 4))
      } ElseIf (LAnd (LNotEqual (P3WG, 0), LEqual (P3RN, 0x09))) {
        Return (GPRW (GGPE (P3WG), 4))
      } Else {
        Return (GPRW (GPE1_PCI_EXP_SPC, 4))
      }
    }
  }

  Scope (\_SB.PC00.RP10) {
    Method (_PRW, 0) {
      If (LAnd (LNotEqual (P2WG, 0), LEqual (P2RN, 0x0A))) {
        Return (GPRW (GGPE (P2WG), 4))
      } ElseIf (LAnd (LNotEqual (P5WG, 0), LEqual (P5RN, 0xD))) {
        Return (GPRW (GGPE (P5WG), 4))
      } Else {
        Return (GPRW (GPE1_IOE_PCI_EXP, 4))
      }
    }
  }

  Scope (\_SB.PC00.RP11) {
    Method (_PRW, 0) {
      If (LAnd (LNotEqual (P3WG, 0), LEqual (P3RN, 0x0B))) {
        Return (GPRW (GGPE (P3WG), 4))
      } ElseIf (LAnd (LNotEqual (P6WG, 0), LEqual (P6RN, 0x11))) {
        Return (GPRW (GGPE (P6WG), 4))
      } Else {
        Return (GPRW (GPE1_IOE_PCI_EXP, 4))
      }
    }
  }
  Scope (\_SB.PC00.RP12) {
    Method (_PRW, 0) {
      If (LAnd (LNotEqual (P3WG, 0), LEqual (P3RN, 0x15))) {
        Return (GPRW (GGPE (P3WG), 4))
      } ElseIf (LAnd (LNotEqual (P4WG, 0), LEqual (P4RN, 0x15))) {
        Return (GPRW (GGPE (P4WG), 4))
      } ElseIf (LAnd (LNotEqual (P4WG, 0), LEqual (P4RN, 0x0C))) {
        Return (GPRW (GGPE (P4WG), 4))
      } ElseIf (LAnd (LNotEqual (P7WG, 0), LEqual (P7RN, 0x15))) {
        Return (GPRW (GGPE (P7WG), 4))
      } Else {
        Return (GPRW (GPE1_IOE_PCI_EXP, 4))
      }
    }
  }
#endif

#if (FixedPcdGetBool (PcdMtlSSupport) == 1)
//
// Any slots wake GPIOs are coming from PCH which is tied GPE_PME wake GPE including "PCH GPIO" , the respective declaration for _PRW and event check should follow below example -
//
// 1. Report mapped GPE bit to _PRW when the slot is using PCH GPIO.
//      Method (_PRW, 0) {
//       Return (GPRW (GPE_PME, 4))
//        }
//
//  2. Identify if the GPE_PME event was asserted for the slot or not in AL6B () by using \_SB.PGPI.IGPI () and other methods for GPIO operations, assume below slot as example -
//         Method(AL6B) {
//             .................
//                   // PCH RP09 - X4 PCIe Connector (SLOT3) wake event
//                   If (\_SB.PGPI.IGPI(P3WG))  // identify if the GPIO is the wake source
//                   {
//                        \_SB.PGPI.SHPO(P3WG, 1) // set gpio ownership to driver(0=ACPI mode, 1=GPIO mode)
//                        Notify(\_SB.PC02.RP09, 0x02)   // notify OS for device wake
//                        \_SB.PGPI.CGPI(P3WG)
//                  }
//
  If (LEqual (PCHA, 1)) {
    //
    // Wake Pin from PCH Dide GPIO pin that GPE number map to GPE_PME
    //
    Scope (\_SB.PC00.RP01) {
      Method (_PRW, 0) {
        Return (GPRW (GPE_PME, 4))
      }
    }

    Scope (\_SB.PC00.RP09) {
      Method (_PRW, 0) {
        Return (GPRW (GPE_PME, 4))
      }
    }

    Scope (\_SB.PC00.RP17) {
      Method (_PRW, 0) {
        Return (GPRW (GPE_PME, 4))
      }
    }

    Scope (\_SB.PC00.RP21) {
      Method (_PRW, 0) {
        Return (GPRW (GPE_PME, 4))
      }
    }

    Scope (\_SB.PC02.RP01) {
      Method (_PRW, 0) {
        Return (GPRW (GPE_PME, 4))
      }
    }

    Scope (\_SB.PC02.RP04) {
      Method (_PRW, 0) {
        Return (GPRW (GPE_PME, 4))
      }
    }

    Scope (\_SB.PC02.RP05) {
      Method (_PRW, 0) {
        Return (GPRW (GPE_PME, 4))
      }
    }

    Scope (\_SB.PC02.RP07) {
      Method (_PRW, 0) {
        Return (GPRW (GPE_PME, 4))
      }
    }

    Scope (\_SB.PC02.RP08) {
      Method (_PRW, 0) {
        Return (GPRW (GPE_PME, 4))
      }
    }

    Scope (\_SB.PC02.RP09) {
      Method (_PRW, 0) {
        Return (GPRW (GPE_PME, 4))
      }
    }

    Scope (\_SB.PC02.RP21) {
      Method (_PRW, 0) {
        Return (GPRW (GPE_PME, 4))
      }
    }
  }
#endif

#if (FixedPcdGetBool (PcdMtlSSupport) == 0)
  If (LEqual (PCHA, 0)) {
    Scope (\_SB.PC00.SAT0) {
      //
      // Check for platform defined _DSM method support
      //
      // Arg0 - _DSM GUID
      //
      Method(SATC, 1, Serialized) {
        If(LEqual(Arg0, ToUUID("E4DB149B-FCFE-425b-A6D8-92357D78FC7F"))) {
          ADBG("Win Sata DSM UUID : E4DB149B-FCFE-425b-A6D8-92357D78FC7F")
          Return (1)
        } Else {
          Return (0)
        }
      }

      //
      // Execute platform defined _DSM method
      //
      // Arg0 - _DSM GUID
      // Arg1 - _DSM revision
      // Arg2 - Function index
      // Arg3 - Package with parameters
      //
      Method(SATD, 4, Serialized) {
        Switch (ToInteger(Arg2)) {
          Case (0) {
            ///Case 0: Standard query - A bitmask of functions supported
            ADBG("Win Sata DSM fun 0")
            Return(Buffer() {0x3})
          }
          Default {
            Return (0)
          }
        }
        Return(Buffer() {0x0})
      }

      //
      // ACPI Method to support PUIS capability for each Sata port
      //
      If (LAnd (LEqual (\S0ID,1), LEqual (\PUIS,1))) {
        Scope (PRT0) {
          Include ("SataPuis.asl")
        }

        Scope (PRT1) {
          Include ("SataPuis.asl")
        }

        Scope (PRT2) {
          Include ("SataPuis.asl")
        }

        Scope (PRT3) {
          Include ("SataPuis.asl")
        }

        Scope (PRT4) {
          Include ("SataPuis.asl")
        }

        Scope (PRT5) {
          Include ("SataPuis.asl")
        }

        If(CondRefOf(PRT6)) {
          Scope(PRT6) {
            Include ("SataPuis.asl")
          }
        }

        If(CondRefOf(PRT7)) {
          Scope(PRT7) {
            Include ("SataPuis.asl")
          }
        }
      }
    }
  }
#endif

#if (FixedPcdGetBool (PcdMtlSSupport) == 1)
  If (LEqual (PCHA, 1)) {
    Scope (\_SB.PC02.SAT0) {
      External(ZPOV, MethodObj)
      //
      // Check for platform defined _DSM method support
      //
      // Arg0 - _DSM GUID
      //
      Method(SATC, 1, Serialized) {
        If(LEqual(Arg0, ToUUID("E4DB149B-FCFE-425b-A6D8-92357D78FC7F"))) {
          ADBG("Win Sata DSM UUID : E4DB149B-FCFE-425b-A6D8-92357D78FC7F")
          Return (1)
        } Else {
          Return (0)
        }
      }

      //
      // Execute platform defined _DSM method
      //
      // Arg0 - _DSM GUID
      // Arg1 - _DSM revision
      // Arg2 - Function index
      // Arg3 - Package with parameters
      //
      Method (SATD, 4, Serialized) {
        // Windows ZPODD support _DSM
        If (CondRefOf (ZPOV)) {
          Return (ZPOV (Arg1, Arg2, Arg3))
        } Else {
          Switch (ToInteger(Arg2)) {
            Case (0) {
              // Case 0: Standard query - A bitmask of functions supported
              ADBG("Win Sata DSM fun 0")
              Return(Buffer() {0x3})
            }
            Default {
              Return (0)
            }
          }
        }
        Return(Buffer() {0x0})
      }
      //
      // ACPI Method to support PUIS capability for each Sata port
      //
      If (LAnd (LEqual (\S0ID,1), LEqual (\PUIS,1))) {
        If(CondRefOf(PRT0)) {
          Scope (PRT0) {
            Include ("SataPuis.asl")
          }
        }

        If(CondRefOf(PRT1)) {
          Scope (PRT1) {
            Include ("SataPuis.asl")
          }
        }

        If(CondRefOf(PRT2)) {
          Scope (PRT2) {
            Include ("SataPuis.asl")
          }
        }

        If(CondRefOf(PRT3)) {
          Scope (PRT3) {
            Include ("SataPuis.asl")
          }
        }

        If(CondRefOf(PRT4)) {
          Scope (PRT4) {
            Include ("SataPuis.asl")
          }
        }

        If(CondRefOf(PRT5)) {
          Scope (PRT5) {
            Include ("SataPuis.asl")
          }
        }

        If(CondRefOf(PRT6)) {
          Scope(PRT6) {
            Include ("SataPuis.asl")
          }
        }

        If(CondRefOf(PRT7)) {
          Scope(PRT7) {
            Include ("SataPuis.asl")
          }
        }
      }
    }
  }
#endif

  If (LOr ((LEqual (PFLV, FlavorMobile)), (LEqual (PFLV, FlavorMobileWorkstation)))) {
    //
    // Add WWAN Firmware Flash Device Support
    //
    If (LEqual (WFFD, 1)) {
//[HP_RMV]+ Don't need WwanFlash.asl in DT projects. Required to comment out this file due to the build error caused by external path error in it.
//    Include ("WwanFlash.asl")
//[HP_RMV]-
    }
  }

  Name (\_S0, Package (4) {0x0,0x0,0,0}) // mandatory System state
  if (SS1) { Name (\_S1, Package (4) {0x1,0x0,0,0})}
  if (SS3) { Name (\_S3, Package (4) {0x5,0x0,0,0})}
  if (SS4) { Name (\_S4, Package (4) {0x6,0x0,0,0})}
  Name (\_S5, Package (4) {0x7,0x0,0,0}) // mandatory System state

  Method (PTS, 1) {        // METHOD CALLED FROM _PTS PRIOR TO ENTER ANY SLEEP STATE
    If (Arg0)              // entering any sleep state
      {
      }
    }
    Method (WAK, 1) {      // METHOD CALLED FROM _WAK RIGHT AFTER WAKE UP
  }

  //
  // PCH xDCI scope for platform specific functions
  //
//[HP_ADD]+
#if CRB_BOOT_SUPPORT
//[HP_ADD]-
  If (CondRefOf (\_SB.PC00.XDCI)) {
    Scope (\_SB.PC00.XDCI) {
      Method(XDAT, 0, Serialized) {
        If (CondRefOf (\_SB.PC00.LPCB.H_EC.XDAT)) {
          Return (\_SB.PC00.LPCB.H_EC.XDAT ())
        }
        Return (0)
      }
    }
  }
//[HP_ADD]+
#endif
//[HP_ADD]-
}// End of ASL File
