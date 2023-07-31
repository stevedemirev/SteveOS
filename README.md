Welcome to SteveOS!

This OS runs on i386 architecture so please be sure to create or use a cross-compiler in order to run this OS.
Here is a resource on how to build your own cross-compiler if you do not have one: 
https://github.com/cfenollosa/os-tutorial/tree/master/11-kernel-crosscompiler

Before running the OS you will need nasm and qemu:
sudo apt-get install nasm
sudo apt-get install qemu

The Makefile attached will include the path of the i386 compiler, so be sure to change the path variables for $CC,$LD, and $GDB in order for the Makefile to work.

After completing this, the Makefile should be ready to run. You can do this by entering the /asm directory and running "make run". After running this command, the qemu emulator should appear and the OS should load. You can use the command "make clean" to clean all object and binary files.

Once the OS is loaded, we have a few commands you are able to run:
    > HELP - Shows the available commands
    > LEN <user input> - Prints the length of any strings that the user has input (spaces included).
    > ASCII <user input> - Prints the ASCII value equivalent of the string the user has entered (spaces not included).
    > REV <user input> - Prints the user input string in reverse.
    > PAGE - Requests and Allocates Memory from the Kernel
    > END - Deallocates all Memory and Shuts down the CPU

Thank you for trying out SteveOS!
