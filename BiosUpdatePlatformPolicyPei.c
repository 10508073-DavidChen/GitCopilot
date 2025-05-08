//
// Copyright (c) 2014 Hewlett-Packard Company. All rights reserved
// This software and associated documentation (if any) is furnished under a license and may only be used or
// copied in accordance with the terms of the license. Except as permitted by such license, no part of this
// software or documentation may be reproduced, stored in a retrieval system, or transmitted in any
// form or by any means without the express written consent of Hewlett-Packard Company.
//
// File Name: BiosUpdatePlatformPolicyPei.c
//
// Abstract: Driver to publish the blocked bios versions info.
//
#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <BiosUpdatePlatformPolicyCommon.h>
#include <banned.h>

static EFI_PEI_PPI_DESCRIPTOR mPpiList = {
   (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
   &gHpBiosRollBackListTableGuid,
   &BiosRollBackListTable
};


//*********************************************************************************************************************
//Function: EntryPoint
//
//Summary: Entry point for this module.
//
//Global Variables:  None
//
//Parameters:
//    -----------------------------------------------------------------------------------------------------------------
//    Name:                   IN FileHandle
//    Description:            Standard parameter for PEIM
//    Valid values:           non-NULL
//    -----------------------------------------------------------------------------------------------------------------
//    Name:                   IN PeiServices
//    Description:            Standard parameter for driver entry point
//    Valid values:           non-NULL
//    -----------------------------------------------------------------------------------------------------------------
//
//Function Returns:  EFI_STATUS
//    -----------------------------------------------------------------------------------------------------------------
//    EFI_SUCCESS             -  This function successfully installed the ppi.
//    Other values
//    -----------------------------------------------------------------------------------------------------------------
//
//*********************************************************************************************************************
EFI_STATUS
EFIAPI
EntryPoint (
   IN EFI_PEI_FILE_HANDLE       FileHandle,
   IN CONST EFI_PEI_SERVICES    **PeiServices
   )
{
   EFI_STATUS              Status = EFI_NOT_STARTED;

   DBG_FUNCTION_STARTS;
   Status = PeiServicesInstallPpi (&mPpiList);
   DBG_FUNCTION_ENDS_STATUS(Status);
   return Status;
}
