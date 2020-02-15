/*
 * tinyutf8.c
 * Copyright © 2020 Wyatt Sheffield
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int int_to_utf8(uint32_t val){
    int width = 0; //The width of the output character in bytes.
    if (val < 0x80){
        //trivial case - this is just ASCII. We're done.
        putchar(val);
        return 0;
    }
    else if (val < 0x00000800)  width = 2;
    else if (val < 0x00010000)  width = 3;
    else if (val < 0x00200000)  width = 4;
    else if (val < 0x04000000)  width = 5;
    else if (val < 0x80000000)  width = 6;
    else{
        fprintf(stderr, "Illegal codepoint.\n");
        return 1;
    }

    uint8_t firstbits = 7 - width;
    uint8_t totalbits = firstbits + (width - 1) * 6;
    //This bitwise hacking produces a byte with a number of leading 1s equal to the value <width>.
    //For example, if width == 5, then msb_mask will be 0b11111000.
    uint8_t msb_mask = ~((1 << (firstbits + 1)) - 1);

    //The first byte is the leading 1s defined by msb_mask followed by the first few bits of <val>.
    putchar(msb_mask | (val >> (totalbits - firstbits)));
    //The remaining bytes are prefixed with 0b10 and contain 6 bits of data.
    char byte;
    for (int i = width - 2; i >= 0; i--){
        byte = 0x3f & (val >> 6 * i); //0x3f isolates the six least significant bits
        byte |= 0x80;
        putchar(byte);
    }
    return 0;
}

int main(int argc, char** argv){
    char* input = malloc(10*sizeof(char));
    fgets(input, 10, stdin);
    uint32_t val = strtol(input, NULL, 16);
    free(input); input = NULL;
    return int_to_utf8(val);
}
