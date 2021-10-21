# JD1618

JD1618 is a basic 8 bit VProcessor(Virtual Processor) which has 64KB memory, 16 bit address bus, 4 registers.
It has 4 instructions and 18 Opcodes. The fastest instruction has a clock cycle of 3. It has auto clock cycle manager for managing the number of clock cycles wanted for an instruction for executing. This is a processor made like the 6502 processor. It has a built in program loader to load a file to memory.

There is a sample file you can use to load into the emulator in the root folder of the repo.

## Build

To build this project execute :-

```shell
make JD1618
```

To debug the program, execute :-

```shell
make Debug
```

## Bugs

* After the memory location FFFF, you cannot load the program
