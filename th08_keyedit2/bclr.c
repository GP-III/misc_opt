#include "molios.h"

void __molios_bclr (HANDLE fHandle) {

  char clrmem[MOLIOS_PROGRAM_SIZE+MOLIOS_PGRAM_SIZE+MOLIOS_RDATA_SIZE];
  DWORD temp__;
  ZeroMemory (clrmem, sizeof (clrmem));
  SetFilePointer (fHandle, MOLIOS_PEPROGRAM_ADDRESS, NULL, FILE_BEGIN);
  WriteFile (fHandle, clrmem, sizeof (clrmem), & temp__, NULL);
}