/*
* w_segment.h
* Maxwell Anavian, Lawrence Xu
* April 13th, 2020
* HW6: UM
*
* Word Segment Implementation of UM:
* Module takes care of the manipulation of the offsets within each segment.
* We refer to them as word segments.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void s_create_words(Seq_T word_segment, int num_words);

void s_free_segment(Seq_T word_segment);

void s_insert_word(Seq_T word_segment, uint32_t word, int offset);

uint32_t s_get_word(Seq_T word_segment, int offset);
