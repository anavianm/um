/*
* memory.h
* Maxwell Anavian, Lawrence Xu
* April 13th, 2020
* HW6: UM
*
* Implementation of a Segment_T structure
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "seq.h"
#include "w_segment.h"

/* Segment_T struct definition */
typedef struct Memory {
    int program_counter;
    Seq_T memory_segments;
    Seq_T unmapped_segments;
    Seq_T zero_segment;
} *Memory;


Memory new_memory();
void free_memory(Memory mem);
void input_instructions( Memory mem, uint32_t *word);
Seq_T new_m_segment(int hint);
Seq_T get_m_segment(Memory mem, int segment_i);
void m_create_words(Seq_T word_segment, int num_words);
void m_unmap_segment(Memory mem, uint32_t index);
void m_insert_word(Memory mem, int segment_i, int offset, uint32_t word);
uint32_t m_get_word(Memory mem, int segment_i, int offset);
