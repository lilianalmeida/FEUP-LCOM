#include <machine/asm.h>

#include "i8042.h"

.intel_syntax
.global _C_LABEL(kbc_asm_ih)
.global _C_LABEL(scanByte)
.global _C_LABEL(kbc_ih_error)

.data
_C_LABEL(scanByte):
  .long 0
_C_LABEL(kbc_ih_error):
  .short 0

.text

_C_LABEL(kbc_asm_ih):

  mov ecx, 6    #counter
L1: 
  dec ecx;      #decreases the counter in 1 unit
  cmp ecx, 0
  je ERRORS         #if the counter is 0, there was nothing to read and jumps to ERRORS
  xor eax, eax      #resets eax to 0
  in eax, STAT_REG  #reads value in statuts register
  test eax, OBF     #checks ouput buffer
  jz L1             #repeats the cicle if there is nothing to read
  
  ##if there is something to read in the output buffer 
  mov ebx, eax 
  xor eax, eax       #resets eax to 0
  in eax, OUT_BUFF   #reads value in output buffer
  
  test ebx, (PAR_ERR | TO_ERR)  #checks if there are errors
  jnz ERRORS                    #and if there are jumps to ERRORS
  jmp END                       #if there isn't jumps to END

ERRORS:
  mov BYTE PTR kbc_ih_error, 1  #returns kbc_ih_error true, indicating that wasn't read any byte
  mov BYTE PTR scanByte, 0      #moves to scanByte 0 if there are erros
  ret

END:
  mov BYTE PTR kbc_ih_error, 0  #returns kbc_ih_error false, indicating that was read one byte
  mov scanByte, al              #moves to scanByte the byte read
  ret
