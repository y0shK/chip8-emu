#include <stdio.h>
#include <stdint.h>
#include <string.h>

/*
Resources 
https://austinmorlan.com/posts/chip8_emulator/#class-members

*/

//registers = [0x00] * 16 # 8 bit
//memory = [0x000] * 4096 # 16-bit
//index = 0x000 # 16-bit
//pc = 0x000 # program counter
//stack = [0x000] * 16
//stack_pointer = 0x000
//timer = 0
//input_keys = [0x0] * 16
//video = [0x000] * (64 * 32)
//opcode = 0x00 # 16-bit

uint8_t registers[16];
uint16_t memory[4096];
uint16_t index_loc;
uint16_t pc;
uint16_t stack;
uint8_t sp;
uint8_t delay_timer;
uint8_t sound_timer;
uint8_t input[16];
uint32_t display[64*32];
uint16_t opcode;

// font from 0x050 to 0x09F
/*
registers[050] = 0xF0,
registers[051] = 0x90, ...,
registers[09F] = 0x80
*/

// reserve from 0x000 to 0x1FF

uint16_t initial_font = 0x050;
uint16_t font_limit = 0x09F - 0x050;

uint16_t font_storage[80] = {0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
0x20, 0x60, 0x20, 0x20, 0x70, // 1
0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
0x90, 0x90, 0xF0, 0x10, 0x10, // 4
0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
0xF0, 0x10, 0x20, 0x40, 0x40, // 7
0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
0xF0, 0x90, 0xF0, 0x90, 0x90, // A
0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
0xF0, 0x80, 0x80, 0x80, 0xF0, // C
0xE0, 0x90, 0x90, 0x90, 0xE0, // D
0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
0xF0, 0x80, 0xF0, 0x80, 0x80};  // F

// start loading into memory buffer at 0x200
// given a filename,
// read in binary and store in memory
uint16_t start_addr = 0x200;

/*
memset(buffer, 0, sizeof(buffer));
    num = fread( buffer, sizeof( char ), NUM_ALPHA, stream );
    if ( num ) {  fread success
      printf( "Number of characters has been read = %i\n", num );
      printf( "buffer = %s\n", buffer );
      fclose( stream );
*/

// rom is *.ch8
void load_rom() {

    //const char* rom;

    // https://www.geeksforgeeks.org/fread-function-in-c/
    // https://www.ibm.com/docs/en/i/7.2?topic=functions-fread-read-items
    // https://stackoverflow.com/questions/24019624/why-the-ftell-returns-0-in-this-function
    // https://www.geeksforgeeks.org/fseek-in-c-with-example/

    FILE* rom_ptr = fopen("helloWorldROM.txt", "r");
    fseek(rom_ptr, 0, SEEK_END); // move pointer to end to see stream length
    long rom_size = ftell(rom_ptr); // get stream length

    fseek(rom_ptr, 0, 0); // move pointer back to the beginning

    printf("Rom size\n");
    printf("%d\n", rom_size);

    //char buffer[rom_size+1];
    char buffer[rom_size];

    // printf("%s", &rom_ptr);

    while (!feof(rom_ptr)) {
        
        fread(buffer, sizeof(buffer), 10, rom_ptr);
        // Print the read data
        printf("%s", buffer);
    }

    printf("\n");

    fclose(rom_ptr);

    for (int i = 0; i < rom_size; ++i) {
        printf("buffer ");
        printf("%c\n", buffer[i]);
        memory[start_addr + i] = buffer[i];
    }

    printf("Memory\n");
    printf("%c", memory[start_addr]); // first character in filename provided
    
}

int main() {
    
    for (int i = 0; i < font_limit; i++) {
        memory[i + initial_font] = font_storage[i]; 
    }

    printf("%d\n", memory[0x050]); // expected 0xF0 (240) - received 240

    load_rom(); // expected "h" - received "h"

    return 0;

}

// output
/*
240
Rom size
10
helloWorld
buffer h
buffer e
buffer l
buffer l
buffer o
buffer W
buffer o
buffer r
buffer l
buffer d
Memory
h
*/