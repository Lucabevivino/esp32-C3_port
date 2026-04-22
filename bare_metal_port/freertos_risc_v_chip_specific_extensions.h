
#ifndef __FREERTOS_RISC_V_EXTENSIONS_H__
#define __FREERTOS_RISC_V_EXTENSIONS_H__

#define portasmHAS_SIFIVE_CLINT           0
#define portasmHAS_MTIME                  0
#define portasmADDITIONAL_CONTEXT_SIZE    0

.macro portasmSAVE_ADDITIONAL_REGISTERS
/* No additional registers to save, so this macro does nothing. */
.endm

.macro portasmRESTORE_ADDITIONAL_REGISTERS
/* No additional registers to restore, so this macro does nothing. */
.endm

.macro portupdate_mtimer_compare_register
.endm

#endif /* __FREERTOS_RISC_V_EXTENSIONS_H__ */
