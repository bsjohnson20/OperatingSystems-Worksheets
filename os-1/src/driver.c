int __attribute__((cdecl)) asm_main( void );

/**
 * @file driver.c
 * @brief Entry point for the program.
 *
 * The main purpose of this file is to provide a entry point for the program.
 * It calls the assembly main function and returns its status.
 */

/**
 * @fn int main(void)
 * @brief Entry point for the program.
 *
 * Calls the assembly main and returns its status.
 *
 * @return The status returned by the assembly main.
 */
int main() {
    int ret_status;
    ret_status = asm_main();
    return ret_status;
}
