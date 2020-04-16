/*
* memory.c
* Maxwell Anavian, Lawrence Xu
* April 13th, 2020
* HW6: UM
*
* Implementation of a Segment_T structure
*/

#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "seq.h"
#include "assert.h"


/* new_memory
* I: instructions: the number of instructions requested by the client
* O: Memory struct that will manage all the segments
* Purpose: Initializes a new Memory struct, which will be used to handle all
*          of the memory and segmenting
*/
Memory new_memory()
{
    Memory mem = malloc(sizeof(struct Memory));

    mem->program_counter = 0;
    /* insert zero segment */
    Seq_T zero = Seq_seq(NULL);
    /* Zero segment becomes 0th index of memory_segments */
    mem->memory_segments = Seq_seq(zero, NULL);
    /* pointer to the zero_segment */
    mem->zero_segment = zero;
    mem->unmapped_segments = Seq_seq(NULL);
    /* memory struct to be used */
    return mem;
}


/* free_memory
* I: struct Memory mem ,
* O: N/A
* Purpose: Frees all allocated memory of the um and the memory struct itself
*/
void free_memory(Memory mem)
{
    mem->program_counter = 0;

    Seq_free(&(mem->unmapped_segments));

    for(int i = 0; i < Seq_length(mem->memory_segments); i++) {
        if (Seq_get(mem->memory_segments, i) != NULL) {
            s_free_segment(Seq_get(mem->memory_segments, i));
        }
    }

    Seq_free(&(mem->memory_segments));
    free(mem);
}


/* new_m_segment
* I: hint: number of words segment will be expected to hold (provided by
*    map instruction)
* O: the created new segment
* Purpose: creates a new segment that will hold a requested amount of words
*/
Seq_T new_m_segment(int hint)
{
    Seq_T new_segment = Seq_new(hint);

    return new_segment;
}

/* get_m_segment
* I: mem: struct that contains all the memory segments
*    segment_i: index of the specific memory segment that is wanted
* O: the sequence that is wanted
* Purpose: returns the specific sequence at the desired index
*/
Seq_T get_m_segment(Memory mem, int segment_i)
{
    /* check if segment is mapped */
    assert(Seq_get(mem->memory_segments, segment_i) != NULL);

    return Seq_get(mem->memory_segments, segment_i);
}

/* m_create_words
* I: Seq_T word_segment : The sequence it will be created in
*    int num_words : length of the new sequence
* O: N/A
* Purpose: Creates a word segment with a length of num_words
*/
void m_create_words(Seq_T word_segment, int num_words)
{
    s_create_words(word_segment, num_words);

}

/* unmap_segment
* I: struct Memory mem ,
*    int index: Index of the segment to be unmapped
* O: N/A
* Purpose: Unamaps the segment specified by the index. It returns it to size of
           0 and adds it to the list of unmapped segments to be used again.
*/
void m_unmap_segment(Memory mem, uint32_t index)
{
    /* free sequence and set to NULL */
    Seq_T unmapped_segment = Seq_get(mem->memory_segments, index);

    /* make sure it is mapped */
    assert (unmapped_segment != NULL);

    /* free segment that was recently unmapped */
    Seq_free(&unmapped_segment);

    /* Put the new memory segment in the old index */
    Seq_put(mem->memory_segments, index, NULL);

    /* Add old unmapped segment to the unmapped_segment seq */
    Seq_addhi(mem->unmapped_segments, (void *)(uintptr_t)index);
}

/* m_insert_word - used by segmented store
* I: mem: struct that contains all the memory segments
*    segment_i: provided index of the segment
*    offset: provided offset in the segment
*    word: the word to be inserted
* O: N/A
* Purpose: inserts word into m[segment_i][offset]
*/
void m_insert_word(Memory mem, int segment_i, int offset, uint32_t word)
{
    /* check if mapped */
    assert(Seq_get(mem->memory_segments, segment_i) != NULL);

    s_insert_word(Seq_get(mem->memory_segments, segment_i), word, offset);
}

/* m_get_word
* I: struct Memory mem ,
*    int Segment_Tindex of memory segment
*    int offset
* O: a uint32_t word from the segment
* Purpose: returns the word at a certian segment and offset at memory location
*          m[segment_i][offset]
*/
uint32_t m_get_word(Memory mem, int segment_i, int offset)
{
    /* check if mapped */
    assert(Seq_get(mem->memory_segments, segment_i) != NULL);

    return s_get_word(Seq_get(mem->memory_segments, segment_i), offset);
}
