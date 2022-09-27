# A logic circuit + kicad schematic for a 16 bit processor

## Architecture

This is a 16-bit processor; the data bus is 16 bits wide, all the instructions act on 16 bit data, etc. Within the processor are five general purpose registers named A-E. There's a program counter (PC), instruction register (IR), memory address register (MAR), and memory data register (MDR).

A-E can be used by the programmer for anything they want. PC points to the address of the word in memory _after_ the currently executing instruction. IR holds the word of the currently executing instruction. MAR and MDR are used to communicate with memory; data is put into MDR and an address is put into MAR when writing to memory, and when reading an address is put into MAR and data is read from MDR. The specific memory access mechanism is something the programmer doesn't need to worry about, though.

The format of an instruction in memory is `0xBAxO`, written as a hexadecimal number, where O refers to the opcode, x is unused bits, A is register 1 (R1), and B is register 2 (R2).

Instructions act on generic registers R1 and R2 to allow the programmer a lot more freedom in what instructions can do, without introducing a lot more instructions. R1 and R2 can each refer to any of A-E, or PC. To select a specific register as R1 or R2, replace the B or A in the format with 0-4 to correspond to registers A-E, or 5 for PC. The available opcodes are as follows:

|------|--------|
|opcode|meaning |
|------|--------|
|0x0   |NOP     |
|0x1   |[R2] -> R1|
|0x2   |R1 -> [R2]|
|0x3   |R1 + R2 -> A|
|0x4   |R1 - R2 -> A|
|0x5   |R1 OR R2 -> A|
|0x6   |R1 AND R2 -> A|
|0x7   |R1 XOR R2 -> A|
|0x8   |R1 << R2 -> A|
|0x9   |R1 >> R2 -> A|
|0xa   |R1 -> R2|
|0xb   |[PC] -> PC|
|0xc   |[PC] -> R1; PC++|
|------|-------|

Note that when a register is surrounded by square brackets [], the instruction is referring to the data at the memory address of that register's value. This is similar to the `*` operator in C.

The last two opcodes may be unintuitive at first. `0xb` can be used to start executing instructions from a different (constant) address in memory. Since PC points to the word immediately _after_ the currently executing instruction, so there should be a word of constant data in the program file directly after an `0xb` or `0xc` instruction. Where `0xb` can be used to jump to a different location, `0xc` is used to load a constant value into a register. The program counter is incremented after such an instruction so that the data word is skipped and not executed as if it was an instruction.

## Example programs

### Fibonacci sequence generator

Equivalent program in C:

```c
int a = 0; // a will store the current value in the sequence.
int b = 1;
int c = 1;

while (1) {
    a = b + c;
    c = b;
    b = a;
}
```

Assembly:

```
[PC] -> Bl PC++
1
[PC] -> C; PC++
1
B + C -> A
B -> C
A -> B
[PC] -> PC
4
```

Machine Code Words:

```
010c
0001
020c
0001
2103
210a
100a
000b
0004
```

Each line of the assembly and machine code versions map directly 1-to-1 to each other.