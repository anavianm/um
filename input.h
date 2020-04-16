/*
* input.h
* Maxwell Anavian, Lawrence Xu
* April 13th, 2020
* HW6: UM
*
* File IO and unpacking of instructions. Providing input for UM
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "seq.h"

void create_zero_segment(Seq_T zero_segment, FILE *input);
