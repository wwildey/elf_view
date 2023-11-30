# elf_view
QT app to view elf file details

Project requires capstone library for disassembly which can be found at:
https://github.com/capstone-engine/capstone

This library was available as an install package for the Opensuse distribution that was used for development.

capstone exsample from http://www.capstone-engine.org/lang_c.html
// capstone example make file
# capstone library name (without prefix 'lib' and suffix '.so')
LIBNAME = capstone

test1: test1.o
	${CC} $< -O3 -Wall -l$(LIBNAME) -o $@

%.o: %.c
	${CC} -c $< -o $@


// capstone example c code
/* test1.c */
#include <stdio.h>
#include <inttypes.h>

#include <capstone/capstone.h>

#define CODE "\x55\x48\x8b\x05\xb8\x13\x00\x00"

int main(void)
{
	csh handle;
	cs_insn *insn;
	size_t count;

	if (cs_open(CS_ARCH_X86, CS_MODE_64, &handle) != CS_ERR_OK)
		return -1;
	count = cs_disasm(handle, CODE, sizeof(CODE)-1, 0x1000, 0, &insn);
	if (count > 0) {
		size_t j;
		for (j = 0; j < count; j++) {
			printf("0x%"PRIx64":\t%s\t\t%s\n", insn[j].address, insn[j].mnemonic,
					insn[j].op_str);
		}

		cs_free(insn, count);
	} else
		printf("ERROR: Failed to disassemble given code!\n");

	cs_close(&handle);

    return 0;
}

// compile example
$ make
cc -c test1.c -o test1.o
cc test1.o -O3 -Wall -lcapstone -o test1

// Output
$ ./test1
0x1000:	push		rbp
0x1001:	mov		rax, qword ptr [rip + 0x13b8]