# Lauren Glaser
# PS 05
# CS 461

This is the README the for Simple Shell.

Find the folder contains your src/SimpleShell.cpp, include/SimpleShell.h and Makefile files and point to that in your linux terminal.
Your terminal should look something like:

    laurennikolle@LAPTOP-SPL29JA8:~/SimpleShell$

Check that SimpleShell has the correct files (Makefile, src, include, README) by writing "ls".
It should look something like:

    laurennikolle@LAPTOP-SPL29JA8:~/SimpleShell$ ls
    Makefile  include  src

To compile the program, write "make".
If you get an error when making the Makefile, ensure that the Makefile tabs are actually tabs and not spaces. Sometimes
copying the file can change that tabs to spaces.
It should look something like:

    laurennikolle@LAPTOP-SPL29JA8:~/SimpleShell$ make
    g++ -Iinclude -std=c++11 -c src/SimpleShell.cpp -o src/SimpleShell.o
    g++ -Iinclude -std=c++11 -o bin/SimpleShell src/SimpleShell.o
    ./bin/SimpleShell

You should now be in the loop. To kill the program, select CTRL + Z.

You can now use the commands:
    cat src/SimpleShell.cpp | grep main
    ls | wc

If you get an error about permission denied, use sudo before your command (sudo mkdir foo). Errors related to permissions look similar to: 

    "mkdir: cannot create directory ‘foo’: Permission denied"

To clean up the complied files, write "make clean".
It should look something like:

    laurennikolle@LAPTOP-SPL29JA8:~/SimpleShell$ make clean
        rm -f src/*.o
        rm -rf bin

Good luck!

```bash
make  # Compiles the program and then runs the executable
make compile  # Compiles the source code into an executable located in the bin/ directory
make run  # Executes the compiled program
make clean  # Deletes the object files and the executable from the bin/ directory
