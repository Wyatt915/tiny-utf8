#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char** argv){
    int exitcode = 0;
    char* input = malloc(10*sizeof(char));
    fgets(input, 10, stdin);
    uint32_t val = strtol(input, NULL, 16);

    int numbytes = 0;

    if (val < 0x80)     numbytes = 1;
    else if (val < 0x00000800)  numbytes = 2;
    else if (val < 0x00010000)  numbytes = 3;
    else if (val < 0x00200000)  numbytes = 4;
    else if (val < 0x04000000)  numbytes = 5;
    else if (val < 0x80000000)  numbytes = 6;
    else{
        fprintf(stderr, "Illegal codepoint.\n");
        exitcode = 1;
        goto cleanup;
    }
    uint8_t msb_mask;
    uint8_t totalbits, firstbits;
    switch (numbytes){
        case 1:
            //trivial case - this is just ASCII. We're done.
            putchar(val);
            goto cleanup;
        case 2:
            msb_mask = 0xc0; //1100 0000
            totalbits = 11; firstbits = 5;
            break;
        case 3:
            msb_mask = 0xe0; //1110 0000
            totalbits = 16; firstbits = 4;
            break;
        case 4:
            msb_mask = 0xf0; //1111 0000
            totalbits = 21; firstbits = 3;
            break;
        case 5:
            msb_mask = 0xf8; //1111 1000
            totalbits = 26; firstbits = 2;
            break;
        case 6:
            msb_mask = 0xfc; //1111 1100
            totalbits = 31; firstbits = 1;
            break;
        default:
            fprintf(stderr, "Unknown error.\n");
            exitcode = 2;
            goto cleanup;
    }

    const uint32_t LSB_6 = 0x3f; //00000000 00000000 00000000 00111111

    //Do the weird case of the first byte...
    putchar(msb_mask | (val >> totalbits - firstbits));
    //now the rest of the bytes are the same.
    char byte;
    for (int i = numbytes - 2; i >= 0; i--){
        byte = LSB_6 & (val >> 6 * i);
        byte |= 0x80;
        putchar(byte);
    }

cleanup:
    free(input);
    input = NULL;
    return exitcode;
}

