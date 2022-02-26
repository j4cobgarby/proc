MAR2RAM = 1<<0
MDR_OE = 1<<1
PC_OE = 1<<2
MDR_WE = 1<<3
MAR_WE = 1<<4
PC_WE = 1<<5
IR_WE = 1<<6
R1_OE = 1<<7
R2_OE = 1<<8
R1_WE = 1<<9
R2_WE = 1<<10
A_WE = 1<<11
MDR_DB = 1<<12
RAM_WE = 1<<13
RAM_OE = 1<<14
PC_INC = 1<<15
ALU_A_WE = 1<<16
ALU_B_WE = 1<<17
ALU_OE = 1<<18
HLT = 1<<19

def find_index(step, opcode): # step is 0->7, opcode is 0->15
    return step + (opcode << 3)

eeprom = [0] * 256 # 24-bit output eeprom

inst = 0
fetch_cycle = PC_OE | IR_WE | RAM_OE | PC_INC

nop = [
    fetch_cycle,
    0,0,0,0,0,0,0
]

op1 = [ # M[R2] -> R1
    fetch_cycle,
    MAR_WE | R2_OE,
    MAR2RAM | MDR_WE | RAM_OE,
    MDR_OE | R1_WE,
    0,0,0,0
]

op2 = [ # R1 -> M[R2]
    fetch_cycle,
    MAR_WE | R2_OE,
    MDR_WE | R1_OE,
    MAR2RAM | MDR_OE | MDR_DB | RAM_WE,
    0,0,0,0
]

op3 = [ # R1 + R2 -> A
    fetch_cycle,
    R1_OE | ALU_A_WE,
    R2_OE | ALU_B_WE,
    A_WE | ALU_OE,
    0,0,0,0,
]
op4 = op3 # R1 - R2 -> A
op5 = op3 # R1 OR R2 -> A
op6 = op3 # R1 AND R2 -> A
op7 = op3 # R1 XOR R2 -> A
op8 = op3 # R1 LSH R2 -> A
op9 = op3 # R1 RSH R2 -> A

op10 = [ # R1 -> R2
    fetch_cycle,
    R1_OE | R2_WE,
    0,0,0,0,0,0
]
op11 = [ # M[PC] -> PC
    fetch_cycle,
    MDR_WE | RAM_OE,
    MDR_OE | PC_WE,
    0,0,0,0,0
]
op12 = [ # M[PC] -> R; PC++
    fetch_cycle,
    MDR_WE | RAM_OE,
    MDR_OE | R1_WE,
    PC_INC,
    0,0,0,0
]
ops = [nop, op1, op2, op3, op4, op5, op6, op7, op8, op9, op10, op11, op12, nop, nop, nop]

hexfile = open("ucoderom.hex", "w")
hexfile.write("v3.0 hex words plain\n")

for i, op in enumerate(ops):
    [hexfile.write("{:06x} ".format(x)) for x in op]
    hexfile.write("\n")

hexfile.close()

#print(eeprom)
