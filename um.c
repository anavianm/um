/*
* um.c
* Maxwell Anavian, Lawrence Xu
* April 13th, 2020
* HW6: UM
*
* Emulator of the Universal Machine, takes in UM Instructions
*/

#include <stdio.h>
#include <stdlib.h>
#include "seq.h"
#include "bitpack.h"
#include "assert.h"
#include "execute.h"


int main(int argc, char *argv[])
{
    (void) argc;
    /* Any Use case without 2 instructions */
    if (argc == 1 || argc > 2) {
        fprintf(stderr, "%s\n",
                    "Incorrect Usage. Run with ./um [filename.um]");
        exit(EXIT_FAILURE);
    }

    /* case when user a um binary */
    else if (argc == 2) {
        FILE *fp = fopen(argv[1], "rb");
        /* Calls memory and creates memory struct */
        Memory mem;
        mem = new_memory();

        /* check to make sure that file is not null */
        assert(fp != NULL);

        /* Calls execute with struct mem and file */
        execute(mem, fp);

        /* Frees all memory */
        free_memory(mem);

        /* Closes file pointer */
        fclose(fp);
    }


    return 0;

}
