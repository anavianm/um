/*
* execute.h
* Maxwell Anavian, Lawrence Xu
* April 13th, 2020
* HW6: UM
*
* Execution Interface for UM
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "assert.h"
#include "fmt.h"
#include "seq.h"
#include "memory.h"
#include "input.h"

void execute(Memory mem, FILE *input);

void output(uint32_t value);
uint32_t condmove(uint32_t reg_c, uint32_t reg_a, uint32_t reg_b);
uint32_t segload(Memory mem, uint32_t segment_index, uint32_t offset);
void segstore(Memory mem, uint32_t segment_index, uint32_t offset,
                                                  uint32_t value);
uint32_t add(uint32_t num1, uint32_t num2);
uint32_t multiply(uint32_t num1, uint32_t num2);
uint32_t divide(uint32_t num1, uint32_t num2);
uint32_t bitnand(uint32_t num1, uint32_t num2);
uint32_t input_instruction();
void output(uint32_t value);
Seq_T load_program(Seq_T zero_segment);
uint32_t map_segment(Memory mem, int num_words);
void unmap_segment(Memory mem, uint32_t index);
