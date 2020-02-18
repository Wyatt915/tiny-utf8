/*
 * tinyutf8.c
 * Copyright © 2020 Wyatt Sheffield
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int int_to_utf8(uint32_t val){
    //trivial case - this is just ASCII. We're done.
    if (val < 0x80){
        putchar(val);
        return 0;
    }
    else if (val > 0x80000000){
        fprintf(stderr, "Illegal codepoint.\n");
        return 1;
    }
    // The width of the output character in bytes can be computed with a bunch of if...else
    // statements. However, as you can see, there is a definite pattern to how the width is
    // determined based on the codepoint. That is, one more byte is required for each reduction of
    // leading zeroes by 5 in the binary representation of the codepoint.
    //else if (val < 0x00000800)  width = 2; // 20 leading zeroes
    //else if (val < 0x00010000)  width = 3; // 15 leading zeroes
    //else if (val < 0x00200000)  width = 4; // 10 leading zeroes
    //else if (val < 0x04000000)  width = 5; // 5  leading zeroes
    //else if (val < 0x80000000)  width = 6; // 0  leading zeroes
    // This means that the width is 6 - (leadingzeroes-1/5)
    // Lots of architectures have instructions for counting leading zeroes, and they can be accessed
    // by the gcc builtin function __builtin_clz (also supported by clang).
    uint8_t  width = 6 - ((__builtin_clz(val)-1)/5);

    uint8_t firstbits = 7 - width;
    // Each remaining byte starts with '10' and contains 6 bits of character data
    uint8_t totalbits = firstbits + (width - 1) * 6;
    //This bitwise hacking produces a byte with a number of leading 1s equal to the value <width>.
    //For example, if width == 5, then msb_mask will be 0b11111000.
    uint8_t msb_mask = ~((1 << (firstbits + 1)) - 1);

    //The first byte is the leading 1s defined by msb_mask followed by the first few bits of <val>.
    putchar(msb_mask | (val >> (totalbits - firstbits)));
    //The remaining bytes are prefixed with 0b10 and contain 6 bits of data.
    for (int i = width - 2; i >= 0; i--){
        //0x3f isolates the six least significant bits, 0x80 makes each byte start with '10'
        putchar((0x3f & (val >> 6 * i)) | 0x80);
    }
    return 0;
}

int main(int argc, char** argv){
    char input[16];
    fgets(input, 15, stdin);
    uint32_t val = strtol(input, NULL, 16);
    return int_to_utf8(val);
}
