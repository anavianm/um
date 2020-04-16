/*
* execute.c
* Maxwell Anavian, Lawrence Xu
* April 13th, 2020
* HW6: UM
*
* Execution Implementation of UM
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "input.h"
#include "bitpack.h"
#include "assert.h"
#include "execute.h"
#include "um-dis.h"

const int REG_WIDTH = 3;
const int OP_WIDTH = 4;

const int OP_LSB = 28;
const int REG_A_LSB = 6;
const int REG_B_LSB = 3;
const int REG_C_LSB = 0;


void execute(Memory mem, FILE *input)
{
    /* assertions */
    assert (mem->program_counter == 0);

    /* create registers */
    uint32_t registers[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    /* input zero segment instructions */
    create_zero_segment(mem->zero_segment, input);

    /* iterate through zero_segment and complete instructions */
    while (mem->program_counter < Seq_length(mem->zero_segment)) {

        /* get instructions */
        uint32_t instruction = (uint32_t)(uintptr_t)Seq_get(mem->zero_segment,
                                                        mem->program_counter);

        uint32_t opcode = Bitpack_getu(instruction, OP_WIDTH, OP_LSB);

        assert(opcode <= 13);

        /* get registers */
        uint32_t reg_c = Bitpack_getu(instruction, REG_WIDTH, REG_C_LSB);
        uint32_t reg_b = Bitpack_getu(instruction, REG_WIDTH, REG_B_LSB);
        uint32_t reg_a = Bitpack_getu(instruction, REG_WIDTH, REG_A_LSB);

        assert(reg_a < 8);
        assert(reg_b < 8);
        assert(reg_c < 8);

        uint32_t load_reg = Bitpack_getu(instruction, REG_WIDTH, 25);
        uint32_t loaded_val = Bitpack_getu(instruction, 25, 0);

        switch (opcode) {
            case 0: /* CONDITIONAL MOVE */
                registers[reg_a] = condmove(registers[reg_c],
                                            registers[reg_a],
                                            registers[reg_b]);
                break;
            case 1: /* SEGMENTED LOAD */
                registers[reg_a] = segload(mem, registers[reg_b],
                                                registers[reg_c]);
                break;

            case 2: /* SEGMENTED STORE */
                segstore(mem, registers[reg_a], registers[reg_b],
                                                        registers[reg_c]);
                break;

            case 3: /* ADD */
                registers[reg_a] = add(registers[reg_b], registers[reg_c]);
                break;

            case 4: /* MULTIPLICATION */
                registers[reg_a] = multiply(registers[reg_b],
                                            registers[reg_c]);
                break;

            case 5: /* DIVIDE */
                registers[reg_a] = divide(registers[reg_b], registers[reg_c]);
                break;

            case 6: /* BITWISE NAND */
                registers[reg_a] = bitnand(registers[reg_b], registers[reg_c]);
                break;

            case 7: /* HALT */
                return;

            case 8: /* MAP SEGMENT */
                registers[reg_b] = map_segment(mem, registers[reg_c]);
                break;

            case 9: /* UNMAP SEGMENT */
                unmap_segment(mem, registers[reg_c]);
                break;

            case 10: /* output */
                output(registers[reg_c]);
                break;

            case 11:/* INPUT */
                registers[reg_c] = input_instruction();
                break;

            case 12: /* LOAD PROGRAM */
                /* load new program (if needed) */
                if (registers[reg_b] != 0) {
                    /* obtain new program */
                    Seq_T new_program = get_m_segment(mem, registers[reg_b]);

                    /* load program into zero segment */
                    Seq_free(&mem->zero_segment);
                    mem->zero_segment = load_program(new_program);
                    Seq_put(mem->memory_segments, 0, mem->zero_segment);

                }

                /* update program counter */
                mem->program_counter = registers[reg_c] - 1;

                break;

            case 13: /* LOAD VALUE */
                registers[load_reg] = loaded_val;

            default:
                break;
        }

        mem->program_counter ++;
    }

    Seq_free(&mem->zero_segment);

}

uint32_t condmove(uint32_t reg_c, uint32_t reg_a, uint32_t reg_b)
{
    if (reg_c != 0) {
        return reg_b;
    } else {
        return reg_a;
    }
}

uint32_t segload(Memory mem, uint32_t segment_index, uint32_t offset)
{
    return m_get_word(mem, segment_index, offset);
}

void segstore(Memory mem, uint32_t segment_index, uint32_t offset,
                                                  uint32_t value)
{
    m_insert_word(mem, segment_index, offset, value);
}

uint32_t add(uint32_t num1, uint32_t num2)
{
    return (num1 + num2);
}

uint32_t multiply(uint32_t num1, uint32_t num2)
{
    return (num1 * num2);
}

uint32_t divide(uint32_t num1, uint32_t num2)
{
    assert(num2 != 0);
    return num1/num2;
}

uint32_t bitnand(uint32_t num1, uint32_t num2)
{
    return ~(num1 & num2);
}

uint32_t input_instruction()
{
    /* request a character */
    uint32_t value;
    value = fgetc(stdin);

    assert ((int)value <= 255);

    if ((int) value == EOF) {
        return ~0;
    } else {
        return (uint32_t)value;
    }
}

void output(uint32_t value)
{
    assert (value <= 255);

    putchar(value);
}

Seq_T load_program(Seq_T program)
{
    /* duplicate */
    Seq_T duplicate = Seq_new(Seq_length(program));

    /* copy over */
    for (int i = 0; i < Seq_length(program); i++) {
        Seq_addhi(duplicate, Seq_get(program, i));
    }

    return duplicate;
}


/* map_segment
* I: Memory struct, number of words
* O: id of placed segment - put into r[b]
* Purpose: Creates or reuses a segment, inserting a specific amount of words,
            and initializes all of them to 0.
*/

uint32_t map_segment(Memory mem, int num_words)
{
    Seq_T w_seg;
    uint32_t id;

    /* check for freed_segments */
    if (Seq_length(mem->unmapped_segments) == 0) {
        /* create space for a new segment */
        Seq_T new_seq = new_m_segment(num_words);

        /* insert segment into segment sequence */
        Seq_addhi(mem->memory_segments, new_seq);

        /* set memory segment that will be mapped */
        id = Seq_length(mem->memory_segments) - 1;

        w_seg = Seq_get(mem->memory_segments, id); /* get last segment */

        /* insert words initialized as 0 */
        m_create_words(w_seg, num_words);

        return id;
    } else {

        /* find index of reusable segment */
        id = (uint32_t)(uintptr_t)(Seq_remhi(mem->unmapped_segments));

        /* set memory segment that will be mapped */
        Seq_T new_seq = new_m_segment(num_words);

        Seq_put(mem->memory_segments, id, new_seq);

        /* insert words initialized as 0 */
        m_create_words(new_seq, num_words);

        return id;
    }
}

void unmap_segment(Memory mem, uint32_t index)
{
    assert (index != 0);
    m_unmap_segment(mem, index);
}
