#include "molios.h"

__declspec (naked)
__cdecl 
__board (void)
{
  __asm 
  {
    mov ecx, MOLIOS_PGRAM_ADDRESS+MOLIOS_PGRAMOFFSET_DATASEG + AS_Global_Init    // !!! global_init 
    cmp dword ptr[ecx],0
    jne dosub 
    push MOLIOS_TEMP_GLOBALUSE
    push PAGE_EXECUTE_READ
    push MOLIOS_PROGRAM_SIZE
    push MOLIOS_PROGRAM_ADDRESS
    mov eax, IAT_VirtualProtect
    call dword ptr[eax]
    push MOLIOS_TEMP_GLOBALUSE
    push PAGE_READWRITE
    push MOLIOS_PGRAM_SIZE
    push MOLIOS_PGRAM_ADDRESS
    mov eax, IAT_VirtualProtect
    call dword ptr[eax]
dosub:
    nop 
    nop 
    nop 
    nop 
    nop 
    nop 
    push MOLIOS_PGRAM_ADDRESS+MOLIOS_PGRAMOFFSET_RDATAP // !!! rdataSegp
    push MOLIOS_PGRAM_ADDRESS+MOLIOS_PGRAMOFFSET_DATASEG     // !!! dataSeg
    lea eax, [ebp+0xFFFFFF3B] 
    push eax 
    push ecx 
    mov eax, MOLIOS_PROGRAM_ADDRESS // !!! sub_proc 
    call eax 
    mov eax, MOLIOS_RET 
    jmp eax 
  }
}
