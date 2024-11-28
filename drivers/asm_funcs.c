// Used to store some functions to run from the assembly code.
#include "asm_funcs.h"

extern void make_square(){
    terminal_writestring("##########\n");
    terminal_writestring("#        #\n");
    terminal_writestring("#        #\n");
    terminal_writestring("#        #\n");
    terminal_writestring("#        #\n");
    terminal_writestring("#        #\n");
    terminal_writestring("##########\n");
}

extern void make_triangle(){
    terminal_writestring("    #    \n");
    terminal_writestring("   ###   \n");
    terminal_writestring("  #####  \n");
    terminal_writestring(" ####### \n");
    terminal_writestring("#########\n");
}

extern void make_circle(){
    terminal_writestring("  #####  \n");
    terminal_writestring(" #     # \n");
    terminal_writestring("#       #\n");
    terminal_writestring("#       #\n");
    terminal_writestring(" #     # \n");
    terminal_writestring("  #####  \n");
}

extern void make_diamond(){
    terminal_writestring("    #    \n");
    terminal_writestring("   ###   \n");
    terminal_writestring("  #####  \n");
    terminal_writestring(" ####### \n");
    terminal_writestring("  #####  \n");
    terminal_writestring("   ###   \n");
    terminal_writestring("    #    \n");
}

