/*
* input.c
* Maxwell Anavian, Lawrence Xu
* April 13th, 2020
* HW6: UM
*
* Handles the unpacking of UM instructions, providing the input for UM
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "seq.h"
#include "bitpack.h"
#include "assert.h"
#include "input.h"


void create_zero_segment(Seq_T zero_segment, FILE *input)
{
    assert (!feof(input));

    /* create holders of each 8 bits */
    uint32_t a, b, c, d;

    /* pack each 8 bits until it becomes a 32 bit word */
    while (!feof(input)) {
        uint32_t word = 0;

        a = fgetc(input);
        b = fgetc(input);
        c = fgetc(input);
        d = fgetc(input);

        if((int)a == EOF) {
            break;
        }

        word = Bitpack_newu(word, 8, 24, a);
        word = Bitpack_newu(word, 8, 16, b);
        word = Bitpack_newu(word, 8, 8, c);
        word = Bitpack_newu(word, 8, 0, d);
        Seq_addhi(zero_segment, (void *)(uintptr_t)word);
    }

}
