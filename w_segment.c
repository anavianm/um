/*
* w_segment.c
* Maxwell Anavian, Lawrence Xu
* April 13th, 2020
* HW6: UM
*
* Word Segment Implementation of UM
*/


#include <stdio.h>
#include <stdlib.h>
#include "seq.h"
#include "w_segment.h"
#include "assert.h"

/* s_create_word
* I: word_segment: the segment that will contain the words
*    num_words: the number of words to map
* O: N/A
* Purpose: Maps the requested amount of words, initializing each word segment
*          (offset) as 0
*/
void s_create_words(Seq_T word_segment, int num_words)
{
    for (int i = 0; i < num_words; i++) {
        /* inserting 0 into the sequence, it is disguised as a pointer, but
        not actually one (doesn't need to be dereferenced) */
        Seq_addhi(word_segment, (void*)(uintptr_t)0);
    }
}

/* free_segment
* I: word_segment: the segment that contains the words
* O: N/A
* Purpose: frees the segment
*/
void s_free_segment(Seq_T word_segment)
{
    assert(word_segment != NULL);
    Seq_free(&word_segment);
}

/* insert_word
* I: word_segment: the segment that will contain the words
*    index: the offset within the segment - the word's location
* O: N/A
* Purpose: inserts word into the requested segment
*/
void s_insert_word(Seq_T word_segment, uint32_t word, int offset)
{
    /* check if within bounds of word segment */
    assert (offset < Seq_length(word_segment));

    Seq_put(word_segment, offset, (void*)(uintptr_t)word);
}


/* s_ get_word
* I: word_segment: the segment that will contain the words
*    offset: index for m location
* O: a uint32_t, which is the word at the offset of the word segment
* Purpose: inserts word into the requested segment
*/
uint32_t s_get_word(Seq_T word_segment, int offset)
{
    /* within bounds of the word_segment */
    assert (offset < Seq_length(word_segment));

    return (uint32_t)(uintptr_t)Seq_get(word_segment, offset);
}
