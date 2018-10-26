#include <machine/asm.h>

#include "macro.h"

.intel_syntax
.global _C_LABEL(kbc_asm_ih)
.global _C_LABEL(scanByte)

.text

_C_LABEL(kbc_asm_ih):

  xor eax, eax
  inb STAT_REG
  test OBF, eax
  jz  ERR
  test PAR_ERR, eax
  jnz ERR
  test TO_ERR, eax
  jnz ERR
  inb OUT_BUFF
  jmp END
ERR:
  mov eax, 1
END:
  mov [scanByte], eax
  ret
