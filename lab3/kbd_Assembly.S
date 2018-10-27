#include <machine/asm.h>

#include "macro.h"

.intel_syntax
.global _C_LABEL(kbc_asm_ih)
.global _C_LABEL(scanByte_asm)
.global _C_LABEL(kbc_ih_error_asm)

.data
_C_LABEL(scanByte_asm):
	.long 0
_C_LABEL(kbc_ih_error_asm):
	.short 0

.text

_C_LABEL(kbc_asm_ih):

	mov ecx, 6			#contador
L1:	dec ecx;			#decrementa o contador em 1 unidade
	cmp ecx, 0
	je ERROR			#se o contador chegar a 0, nao recebeu nada para ler e salta
	xor eax, eax		#poe registo eax a zero
  	in eax, STAT_REG	#le valor do status register
  	test eax, OBF 		#verifica estado do ouput buffer
  	jz L1				#repete ciclo se nao houver nada para ler
  	
  	#se houver alguma coisa para ler no output buffer 
  	mov ebx, eax		#guarda o valor do eax
  	xor eax, eax		#poe registo eax a zero
  	in eax, OUT_BUF		#le o que esta no output buffer
  	
  	test ebx, (PAR_ERR | TO_ERR)	#verifica se ha erros
  	jnz ERROR 			#e se der erro salta
  	jmp END				#nao havendo erros salta para o fim

ERROR:
	mov BYTE PTR kbc_ih_error_asm, 1	#retorna variavel a indicar que nao foi lido nenhum byte
  	mov BYTE PTR scanByte_asm, 0		#retorna 0 em caso de erro
  	ret

END:
	mov BYTE PTR kbc_ih_error_asm, 0	#retorna variavel a indicar que foi lido um byte
  	mov scanByte_asm, al 				#retorna byte lido
  	ret
