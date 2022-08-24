# How to build Tesseract
## Part One: Dependencies

(Linux is recommended for building)

Dependencies needed to compile Tesseract are:
- GCC
- nasm
- make
- qemu

## Part Two: Compile the Operating System
### Using the Makefile

Using the Makefile, you need to open a terminal.
What you need to type at the root of the directory is: ```make run```

Once you have compiled using make run, QEMU should normally appear, and there will be no errors.
If this is correct, good job! You have compiled Tesseract successfully.

Make sure to ```make clean``` before compiling again.

### Using the scripts

Go to the Scripts directory using a terminal and execute ```./Compile.sh```.
If there are no errors and QEMU is launching directly, you have successfully compiled Tesseract, good job!

You don't have to execute Clean.sh **unless you are making a PR**, in case you are, make sure to execute Clean.sh before pushing your PR.

# This is the end of this tutorial, thank you for being interested in this project!