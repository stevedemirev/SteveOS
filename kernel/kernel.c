#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include <stdint.h>

uint32_t* page_ptr = 0;

void kernel_main() {
    isr_install();
    irq_install();

    kprint("Welcome to SteveOS!\n"
           "Enter 'HELP' to view available commands,\n"
           "'PAGE' to request Memory Allocation,\n"
           "'END' to deallocate Memory and shut down CPU\n> ");
}

void user_input(char *input) {
    if (strcmp(input, "END") == 0) {
        // Print the memory address before deallocation (if it's not NULL)
        if (page_ptr) {
            kprint("Before deallocation: ");
            char page_ptr_str[9]; // Assuming 32-bit address (8 characters + null terminator)
            hex_to_ascii((uint32_t)page_ptr, page_ptr_str);
            kprint(page_ptr_str);
            kprint("\n");
        }
        // Deallocate the memory
        kfree((void*)page_ptr);
        // After deallocation, set page_ptr to NULL
        page_ptr = NULL;
        // Print the memory address after deallocation (which should be NULL)
        kprint("After deallocation: ");
        char after_dealloc_str[9]; // Assuming 32-bit address (8 characters + null terminator)
        hex_to_ascii((uint32_t)page_ptr, after_dealloc_str);
        kprint(after_dealloc_str);
        kprint("\n");
        kprint("Thank you for trying SteveOS. Bye!\n");
        asm volatile("hlt");
    } 
    else if (strcmp(input, "PAGE") == 0) {
        /* Lesson 22: Code to test kmalloc, the rest is unchanged */
        uint32_t phys_addr;
        page_ptr = kmalloc(1000, 1, &phys_addr);
        char page_str[16] = "";
        hex_to_ascii(page_ptr, page_str);
        char phys_str[16] = "";
        hex_to_ascii(phys_addr, phys_str);
        kprint("Page: ");
        kprint(page_str);
        kprint(", physical address: ");
        kprint(phys_str);
        kprint("\n");
    }
    else if (cmp_arg(input, "REV ", 4) == 0) {
        // Skip the first three characters (the "REV" string)
        char* reversed_input = input + 3;
        reverse(reversed_input);
        // Print the reversed input
        kprint("Reversed input: ");
        kprint(reversed_input);
        kprint("\n");
    }
    else if (cmp_arg(input, "ASCII ", 6) == 0){
        // Convert the input to ASCII codes separated by spaces
        char converted_str[256] = ""; // Adjust the buffer size as needed
        convert_to_ascii(input, converted_str);

        // Print the converted string
        kprint("ASCII codes: ");
        kprint(converted_str);
        kprint("\n");
    }
    else if (cmp_arg(input, "LEN ", 4) == 0){
        // Skip the first three characters (the "LEN" command)
        char* input2 = input + 4;
        // Calculate and print the length of the input string
        int length = strlen(input2);
        char length_str[16]; // Adjust buffer size as needed
        int_to_ascii(length, length_str);
        kprint("Length of input: ");
        kprint(length_str);
        kprint("\n");
    }
    else if(strcmp(input, "HELP") == 0){
        kprint("Commands Available are:\n'REV'- Reverse the input string\n");
        kprint("'ASCII'- Print the ASCII value of the input string\n");
        kprint("'LEN'- Print the total length taken by the input string (Spaces included)\n");
        kprint("'PAGE'- Request Memory allocation from the kernel\n");
        kprint("'END'- Deallocate memory and shut down CPU\n");
    }
    else{
        kprint("You said '");
        kprint(input);
        kprint("' but it is not a valid command\n");
    }
    kprint(">");
}