/*
 * tinyutf8.c
 * Copyright © 2020 Wyatt Sheffield
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h> // memset
#include <unistd.h> // read()

uint8_t get_width(uint32_t val){
    // The width of the output character in bytes can be computed with a bunch
    // of if...else statements. However, there is a definite pattern to how the
    // width is determined based on the codepoint. That is, one more byte is
    // required for each reduction of leading zeroes by 5 in the binary
    // representation of the codepoint. This means that the width is
    // 6 - ((leadingzeroes-1)/5) Lots of architectures have instructions for
    // counting leading zeroes, and they can be accessed by the gcc builtin
    // function __builtin_clz (also supported by clang).
#if defined __has_builtin
#   if __has_builtin (__builtin_clz)
#       pragma message "This machine supports __builtin_clz"
        return 6 - ((__builtin_clz(val)-1)/5);
#   endif
#else
#   pragma message "This machine does not support __builtin_clz"
#endif
    // note that the trivial case for widh = 1 byte has been addressed by the
    // first statement in int_to_utf8.
    if      (val < 0x00000800)  return 2; // 20 leading zeroes
    else if (val < 0x00010000)  return 3; // 15 leading zeroes
    else if (val < 0x00200000)  return 4; // 10 leading zeroes
    else if (val < 0x04000000)  return 5; // 5  leading zeroes
    else if (val < 0x80000000)  return 6; // 0  leading zeroes
}

int int_to_utf8(uint32_t val){
    //trivial case - this is just ASCII. We're done.
    if (val < 0x80){
        putchar(val);
        putchar('\n');
        return 0;
    }
    else if (val >= 0x80000000){
        fprintf(stderr, "Illegal codepoint.\n");
        return 1;
    }
    uint8_t width = get_width(val);

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
    putchar('\n');
    return 0;
}

int utf8_to_codepoint(const uint8_t character[], int len){
    uint8_t width;
    uint64_t codepoint = 0;
    if (character[0] < 0x80){
        printf("U+%04X\n", character[0]);
        return 0;
    }
    else if (character[0] < 0xE0)  width = 2;
    else if (character[0] < 0xF0)  width = 3;
    else if (character[0] < 0xF8)  width = 4;
    else if (character[0] < 0xFC)  width = 5;
    else if (character[0] < 0xFE)  width = 6;
    else if (character[0] == 0xFE) width = 7;
    else{
        fprintf(stderr, "Bad UTF-8 character\n");
        return 1;
    }
    if (len < width){
        fprintf(stderr, "Bad UTF-8 character\n");
        return 1;
    }
    uint64_t msb_mask = 0xFF << (7 - width); // we do 7 instead of 8 since the leading 1s are followed by a single 0
    codepoint |= ((uint64_t)character[0] & ~msb_mask) << (6 * (width - 1));
    for (uint8_t i = 1; i < width; i++){
        codepoint |= ((uint64_t)character[i] & 0x3f) << (6 * (width - i - 1));
    }
    printf("U+%04lX\n", codepoint);
    return 0;
}

int main(int argc, char** argv){
    uint8_t buf[256];
    uint8_t* endptr;
    memset(buf, 0, sizeof(buf));
    if (argc == 1){
        int len = read(STDIN_FILENO, buf, sizeof(buf));
        if (len < 1){
            return 0;
        }
        else if (len > 2 && (buf[0] == 'U' || buf[0] == 'u') && buf[1] == '+'){
            uint32_t val = strtol(buf + 2, &endptr, 16);
            if (errno){
                perror("strtol");
                return EXIT_FAILURE;
            }
            if (endptr == buf + 2){
                fprintf(stderr, "No digits found after U+\n");
                return EXIT_FAILURE;
            }
            return int_to_utf8(val);
        }
        return utf8_to_codepoint(buf, len);
    }
    //uint32_t val = strtol(argv[1], NULL, 16);
    //return int_to_utf8(val);
}
