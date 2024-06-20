#include "doynax.h"

#include "gcc8_c_support.h"

void *doynaxdepack(const void *input,
                   void *output) { // returns end of output data, input needs to
                                   // be 16-bit aligned!
  register volatile const void *_a0 ASM("a0") = input;
  register volatile void *_a1 ASM("a1") = output;
  __asm volatile("movem.l %%d0-%%d7/%%a2-%%a6,-(%%sp)\n"
                 "jsr _doynaxdepack_vasm\n"
                 "movem.l (%%sp)+,%%d0-%%d7/%%a2-%%a6"
                 : "+rf"(_a0), "+rf"(_a1)
                 :
                 : "cc", "memory");
  return (void *)_a1;
}
