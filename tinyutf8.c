#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char** argv){
    char* input = malloc(10*sizeof(char));
    fgets(input, 10, stdin);
    uint32_t val = strtol(input, NULL, 16);
    free(input);
    input = NULL;

    int numbytes = 0;

    if (val < 0x80){
        //trivial case - this is just ASCII. We're done.
        putchar(val);
        return 0;
    }
    else if (val < 0x00000800)  numbytes = 2;
    else if (val < 0x00010000)  numbytes = 3;
    else if (val < 0x00200000)  numbytes = 4;
    else if (val < 0x04000000)  numbytes = 5;
    else if (val < 0x80000000)  numbytes = 6;
    else{
        fprintf(stderr, "Illegal codepoint.\n");
        return 1;
    }

    uint8_t firstbits = 7 - numbytes;
    uint8_t totalbits = firstbits + (numbytes - 1) * 6;
    uint8_t msb_mask = 0xff & ~((1 << (firstbits + 1)) - 1);

    //Do the weird case of the first byte...
    putchar(msb_mask | (val >> (totalbits - firstbits)));
    //now the rest of the bytes are the same.
    char byte;
    for (int i = numbytes - 2; i >= 0; i--){
        byte = 0x3f & (val >> 6 * i); //0x3f isolates the six least significant bits
        byte |= 0x80;
        putchar(byte);
    }

    return 0;
}
