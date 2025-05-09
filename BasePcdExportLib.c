//
// (c) Copyright 2012 - 2022 HP Development Company, L.P.
// This software and associated documentation (if any) is furnished under a license and may only be used or
// copied in accordance with the terms of the license. Except as permitted by such license, no part of this
// software or documentation may be reproduced, stored in a retrieval system, or transmitted in any
// form or by any means without the express written consent of HP Development Company.
//
#include <Library/PcdLib.h>
#include <Library/PcdExportLib.h>
#nclude <HpSecurityDefines.h>

// The following functions allow for the exportation of PCD and similar values out
// of the platform library to other consumers, which later might not have access to
// the build-time PCD value.

//
// IMPORTANT NOTE: All of the Get*Base() functions below very purposefully use volatile
//    variables. This is to prevent the Microsoft C compiler from creating optimization
//    issues where sometimes (if volatile is not used) the address (e.g. 0xFFAE0240)
//    is negative sign extended to 64-bits (e.g. 0xFFFFFFFFFFAE0240), resulting in a
//    lockup seen on release builds only. Do not remove the volatile!
//

UINT8  GetFlashBase(VOID)
{
   volatile UINTN Address = PcdGet32(PcdFlashAreaBaseAddress);
   return (UINT8 *)Address;
}

UINTN GetFlashSize(VOID)
{
   return (UINTN)PcdGet32(PcdFlashAreaSize);
}

UINTN GetFlashPartSize(VOID)
{
   return (UINTN)PcdGet32(PcdFlashPartSize);
}

UINT8  GetPeiA1Base(VOID)
{
   volatile UINTN Address = PcdGet32(PcdFlashFvRecoveryBase);
   return (UINT8 *)Address;
}

UINTN GetPeiA1Offset(VOID)
{
   return (UINTN)(PcdGet32(PcdFlashFvRecoveryBase) - PcdGet32(PcdFlashAreaBaseAddress));
}

UINTN GetPeiA1Size(VOID)
{
   return PcdGet32(PcdFlashFvRecoverySize);
}

UINT8 * GetPeiA1MetadataBase(VOID)
{
   volatile UINTN Address = PcdGet32(PcdFlashFvRecoveryRegionHeaderBase);
   return (UINT8 )Address;
}

UINTN GetPeiA1MetadataOffset(VOID)
{
   return (UINTN)(PcdGet32(PcdFlashFvRecoveryRegionHeaderBase) - PcdGet32(PcdFlashAreaBaseAddress));
}

UINTN GetPeiA1MetadataSize(VOID)
{
   return (UINTN)PcdGet32(PcdFlashFvRecoveryRegionHeaderSize);
}

UINT8 * GetPeiA1RelocBase(VOID)
{
   volatile UINTN Address = PcdGet32(PcdFlashFvRecoveryRelocBase);
   return (UINT8 *)Address;
}

UINTN GetPeiA1RelocOffset(VOID)
{
   return (UINTN)(PcdGet32(PcdFlashFvRecoveryRelocBase) - PcdGet32(PcdFlashAreaBaseAddress));
}

UINTN GetPeiA1RelocSize(VOID)
{
   return PcdGet32(PcdFlashFvRecoveryRelocSize);
}

UINT8 * GetPeiA2Base(VOID)
{
   volatile UINTN Address = PcdGet32(PcdFlashBiosAcBase);
   return (UINT8 *)Address;
}

UINTN GetPeiA2Offset(VOID)
{
   return (UINTN)(PcdGet32(PcdFlashBiosAcBase) - PcdGet32(PcdFlashAreaBaseAddress));
}

UINTN GetPeiA2Size(VOID)
{
   return (UINTN)PcdGet32(PcdFlashBiosAcSize);
}

UINT8 * GetPeiA3Base(VOID)
{
   volatile UINTN Address = PcdGet32(PcdFlashHpKbcBase);
   return (UINT8 *)Address;
}

UINTN GetPeiA3Offset(VOID)
{
   return (UINTN)(PcdGet32(PcdFlashHpKbcBase) - PcdGet32(PcdFlashAreaBaseAddress));
}

UINTN GetPeiA3Size(VOID)
{
   return (UINTN)PcdGet32(PcdFlashHpKbcSize);
}

UINT8 * GetLockedAreaBase(VOID)
{
   volatile UINTN Address = PcdGet32(PcdFlashFvRecoveryRegionHeaderBase);
   return (UINT8 *)Address;
}

UINTN GetLockedAreaOffset(VOID)
{
   return (UINTN)(PcdGet32(PcdFlashFvRecoveryRegionHeaderBase) - PcdGet32(PcdFlashAreaBaseAddress));
}

UINTN GetLockedAreaSize(VOID)
{
   UINTN Size = 0u;

   Size = (
             GetPeiA1MetadataSize() +
             GetPeiMicrocodeSize() +
             PcdGet32(PcdFspSize) +
             GetPeiA1RelocSize() +
             GetPeiA3Size() +
             GetPeiA2Size() +
             GetPeiA1Size() +
             PcdGet32(PcdFlashFvSecSize)
          );
   return Size;
}

UINT8 * GetNvRamBase(VOID)
{
   volatile UINTN Address = PcdGet32(PcdFlashNvStorageVariableBase);
   return (UINT8 *)Address;
}

UINTN GetNvRamOffset(VOID)
{
   return (UINTN)(PcdGet32(PcdFlashNvStorageVariableBase) - PcdGet32(PcdFlashAreaBaseAddress));
}

UINTN GetNvRamSize(VOID)
{
   UINTN Size = 0u;

   Size = PcdGet32(PcdFlashNvStorageVariableSize) + PcdGet32(PcdFlashNvStorageFtwWorkingSize) + PcdGet32(PcdFlashNvStorageFtwSpareSize);
   return Size;
}

UINT8 * GetNvRamStoreBase(VOID)
{
   volatile UINTN Address = PcdGet32(PcdFlashNvStorageVariableBase);
   return (UINT8 *)Address;
}

UINTN GetNvRamStoreOffset(VOID)
{
   return (UINTN)(PcdGet32(PcdFlashNvStorageVariableBase) - PcdGet32(PcdFlashAreaBaseAddress));
}

UINTN GetNvRamStoreSize(VOID)
{
   return (UINTN)PcdGet32(PcdFlashNvStorageVariableSize);
}

UINT8 * GetDxeABase(VOID)
{
   volatile UINTN Address = PcdGet32(PcdFlashFvMainBase);
   return (UINT8 *)Address;
}

UINTN GetDxeAOffset(VOID)
{
   return (UINTN)(PcdGet32(PcdFlashFvMainBase) - PcdGet32(PcdFlashAreaBaseAddress));
}

UINTN GetDxeASize(VOID)
{
   return PcdGet32(PcdFlashFvMainSize);
}

UINT8 * GetDxeAHeaderBase(VOID)
{
   volatile UINTN Address = PcdGet32(PcdFlashFvMainRegionHeaderBase);
   return (UINT8 *)Address;
}

UINTN GetDxeAHeaderOffset(VOID)
{
   return (UINTN)(PcdGet32(PcdFlashFvMainRegionHeaderBase) - PcdGet32(PcdFlashAreaBaseAddress));
}

UINTN GetDxeAHeaderSize(VOID)
{
   return PcdGet32(PcdFlashFvMainRegionHeaderSize);
}

UINT8 * GetCustomLogoBase(VOID)
{
   volatile UINTN Address = PcdGet32(PcdFlashCustomLogoBase);
   return (UINT8 *)Address;
}

UINTN GetCustomLogoSize(VOID)
{
   return PcdGet32(PcdFlashCustomLogoSize);
}

UINTN GetCustomLogoOffset(VOID)
{
   return (UINTN)(PcdGet32(PcdFlashCustomLogoBase) - PcdGet32(PcdFlashAreaBaseAddress));
}

UINT8 * GetPeiMicrocodeBase(VOID)
{
   volatile UINTN Address = PcdGet32(PcdFlashFvMicrocodeBase);
   return (UINT8 *)Address;
}
UINTN GetPeiMicrocodeSize(VOID)
{
   return PcdGet32(PcdFlashFvMicrocodeSize) + PcdGet32(PcdFlashFitTableFvSize);
}

UINTN GetPeiMicrocodeOffset(VOID)
{
   return (UINTN)(PcdGet32(PcdFlashFvMicrocodeBase) - PcdGet32(PcdFlashAreaBaseAddress));
}

UINT8 * GetMicrocodePatchBase(VOID)
{
   return NULL;
}

UINTN GetMicrocodePatchSize(VOID)
{
   return (UINTN)0;
}

UINTN GetMicrocodePatchOffset(VOID)
{
   return (UINTN)INVALID_HEADER_BLOCK_VALUE;
}

UINT8 * GetMicrocodeBinBase(VOID)
{
   return NULL;
}

UINTN GetMicrocodeBinSize(VOID)
{
   return (UINTN)0;
}

UINTN GetMicrocodeBinOffset(VOID)
{
   return (UINTN)INVALID_HEADER_BLOCK_VALUE;
}

UINT32 GetFlashSize32(VOID)
{
   return (UINT32)GetFlashSize();
}
UINT32 GetFlashPartSize32(VOID)
{
   return (UINT32)GetFlashPartSize();
}

UINT32 GetPeiA1Offset32(VOID)
{
   return (UINT32)GetPeiA1Offset();
}
UINT32 GetPeiA1Size32(VOID)
{
   return (UINT32)GetPeiA1Size();
}

UINT32 GetPeiA1MetadataOffset32(VOID)
{
   return (UINT32)GetPeiA1MetadataOffset();
}
UINT32 GetPeiA1MetadataSize32(VOID)
{
   return (UINT32)GetPeiA1MetadataSize();
}

UINT32 GetPeiA1RelocOffset32(VOID)
{
   return (UINT32)GetPeiA1RelocOffset();
}
UINT32 GetPeiA1RelocSize32(VOID)
{
   return (UINT32)GetPeiA1RelocSize();
}

UINT32 GetPeiA2Offset32(VOID)
{
   return (UINT32)GetPeiA2Offset();
}
UINT32 GetPeiA2Size32(VOID)
{
   return (UINT32)GetPeiA2Size();
}

UINT32 GetPeiA3Offset32(VOID)
{
   return (UINT32)GetPeiA3Offset();
}
UINT32 GetPeiA3Size32(VOID)
{
   return (UINT32)GetPeiA3Size();
}

UINT32 GetLockedAreaOffset32(VOID)
{
   return (UINT32)GetLockedAreaOffset();
}
UINT32 GetLockedAreaSize32(VOID)
{
   return (UINT32)GetLockedAreaSize();
}

UINT32 GetNvRamOffset32(VOID)
{
   return (UINT32)GetNvRamOffset();
}
UINT32 GetNvRamSize32(VOID)
{
   return (UINT32)GetNvRamSize();
}

UINT32 GetNvRamStoreOffset32(VOID)
{
   return (UINT32)GetNvRamStoreOffset();
}
UINT32 GetNvRamStoreSize32(VOID)
{
   return (UINT32)GetNvRamStoreSize();
}

UINT32 GetDxeAOffset32(VOID)
{
   return (UINT32)GetDxeAOffset();
}
UINT32 GetDxeASize32(VOID)
{
   return (UINT32)GetDxeASize();
}

UINT32 GetDxeAHeaderOffset32(VOID)
{
   return (UINT32)GetDxeAHeaderOffset();
}
UINT32 GetDxeAHeaderSize32(VOID)
{
   return (UINT32)GetDxeAHeaderSize();
}

UINT32 GetCustomLogoSize32(VOID)
{
   return (UINT32)GetCustomLogoSize();
}
UINT32 GetCustomLogoOffset32(VOID)
{
   return (UINT32)GetCustomLogoOffset();
}

UINT32 GetPeiMicrocodeSize32(VOID)
{
   return (UINT32)GetPeiMicrocodeSize();
}
UINT32 GetPeiMicrocodeOffset32(VOID)
{
   return (UINT32)GetPeiMicrocodeOffset();
}

UINT32 GetMicrocodePatchSize32(VOID)
{
   return (UINT32)GetMicrocodePatchSize();
}
UINT32 GetMicrocodePatchOffset32(VOID)
{
   return (UINT32)GetMicrocodePatchOffset();
}

UINT32 GetMicrocodeBinSize32(VOID)
{
   return (UINT32)GetMicrocodeBinSize();
}
UINT32 GetMicrocodeBinOffset32(VOID)
{
   return (UINT32)GetMicrocodeBinOffset();
}

UINT8*
GetPeiACodeBase(
   VOID
)
{
   volatile UINTN Address = PcdGet32(PcdFlashPeiACodeBase);
   return (UINT8 *)Address;
}

UINTN
GetPeiACodeOffset(
   VOID
)
{
   return (UINTN)(PcdGet32(PcdFlashPeiACodeBase) - PcdGet32(PcdFlashAreaBaseAddress));
}

UINTN
GetPeiACodeSize(
   VOID
)
{
   return PcdGet32(PcdFlashPeiACodeSize);
}
