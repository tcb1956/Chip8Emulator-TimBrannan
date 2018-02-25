#include <iostream>
#include <fstream>
#include <ostream>
#include <cctype>
#include <iomanip>
#include <ios>
#include <cstdio>
#include "stdlib.h"
#include "Utility.cpp"
using namespace std;
using namespace Mine;
/*
struct my_registers {
  int V[16];
  int PC;
  int I;
  int DT;
  int ST;
} my_regs;
*/
class Opcode {
  public:

  unsigned char n0, n1, n2, n3;
  unsigned char cmd, regr, regy, qual, s;
  unsigned int regr_n, regy_n, addr_n, cnst_n;
  char addr[4], cnst[3];
  Stack *stk = new Stack();

  virtual void disassemble() = 0;
  virtual void emulate(struct my_registers *r, C8mem *mem, Stack *stk) = 0;
  void buildParams(char* buf) {

  //Parse the first nibble of the Opcode
    //and form the parameters

    //Get the nibbles
    n0 = (buf[0] >> 4) & 0xf;
    n1 = buf[0] & 0xf;
    n2 = (buf[1] >> 4) & 0xf;
    n3 = buf[1] & 0xf;

    //Build parameters as numbers
    regr_n = n1;
    regy_n = n2;
    addr_n = n1 * 0x100 + n2 * 0x10 + n3;
    cnst_n = n2 * 0x10 + n3;
    //cmd = n0;   //Get first nibble of code
    //qual = n3;  //Keep qual as number not char

    //Convert nibbles to printable characters
    n0 < 10 ? n0 += '0' : n0 += 'A' - 10;
    n1 < 10 ? n1 += '0' : n1 += 'A' - 10;
    n2 < 10 ? n2 += '0' : n2 += 'A' - 10;
    n3 < 10 ? n3 += '0' : n3 += 'A' - 10;

    //Build parameters
    sprintf(addr, "%c%c%c", n1, n2, n3);
    sprintf(cnst, "%c%c", n2, n3);
    regr = n1;
    regy = n2;
    s = n3;
  };
};

class CLS : public Opcode {
  void disassemble() {cout << "CLS";};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {};
};
class RTS : public Opcode {
  void disassemble() {cout << "RTS";};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    r->PC = stk->pop();
    r->SP += 2;
  };
};
class JMP : public Opcode {
  void disassemble() {cout << "JMP " << addr;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    r->PC = addr_n;
  };
};
class JSR : public Opcode {
  void disassemble() {cout << "JSR " << addr << "  ";};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    stk->push(r->PC+2);  //return Address
    //should I also push registers??
    r->PC = addr_n;
    r->SP -= 2;
  };
};
class SKEQC : public Opcode {
  void disassemble() {cout << "SKEQ V" << regr << "," << cnst;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    if(r->V[regr_n] == cnst_n) {
      r->PC += 2;
    }
  };
};
class SKNEC : public Opcode {
  void disassemble() {cout << "SKNE V" << regr << "," << cnst;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    if(r->V[regr_n] != cnst_n) {
      r->PC += 2;
    }
  };
};
class SKEQR : public Opcode {
  void disassemble() {cout << "MOV V" << regr << ",V" << regy;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    if(r->V[regr_n] == r->V[regy_n]) {
      r->PC += 2;
    }
  };
};
class MOVC : public Opcode {
  void disassemble() {cout << "MOV V" << hex << regr << "," << hex << cnst;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    //cout << "cnst=" << cnst << "; cnst_n=" << cnst_n << "; regr=" << regr << "; regr_n=" << regr_n;
    r->V[regr_n] = cnst_n;
    //cout << "; r->V[regr]=" << r->V[regr_n];
  };
};
class ADDR : public Opcode {
  void disassemble() {cout << "ADD V" << hex << regr << "," << hex << cnst;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    r->V[regr_n] = r->V[regr_n] + cnst_n;
  };
};
class MOVR : public Opcode {
  void disassemble() {cout << "MOV V" << regr << ",V" << regy;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    r->V[regr_n] = r->V[regy_n];
  };
};
class ORR : public Opcode {
  void disassemble() {cout << "OR V" << regr << ",V" << regy;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    r->V[regr_n] = r->V[regr_n] | r->V[regy_n];
  };
};
class ANDR : public Opcode {
  void disassemble() {cout << "AND V" << regr << ",V" << regy;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    r->V[regr_n] = r->V[regr_n] & r->V[regy_n];
  };
};
class XORR : public Opcode {
  void disassemble() {cout << "XOR V" << regr << ",V" << regy;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    r->V[regr_n] = r->V[regr_n] ^ r->V[regy_n];
  };
};
class ADDC : public Opcode {
  void disassemble() {cout << "ADD V" << regr << ",V" << regy;};
  // carry in Vf
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    r->V[regr_n] = r->V[regr_n] + r->V[regy_n];
  };
};
class SUBR : public Opcode {
  void disassemble() {cout << "SUB V" << regr << ",V" << regy;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    //Vf = 1 if borrow !!!
    r->V[regr_n] = r->V[regr_n] - r->V[regy_n];
  };
};
class SHR : public Opcode {
  void disassemble() {cout << "SHR V" << regr;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    //bit 0 to Vf !!!
    r->V[regr_n] = r->V[regr_n] >> 1;
  };
};
class RSB : public Opcode {
  void disassemble() {cout << "RSB V" << regr << ",V" << regy;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    //Vf = 1 if borrow !!!
    r->V[regr_n] = r->V[regy_n] - r->V[regr_n];
  };
};
class SHL : public Opcode {
  void disassemble() {cout << "SHL V" << regr;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    // bit 7 to Vf !!!
    r->V[regr_n] = r->V[regr_n] << 1;
  };
};
class SKNER : public Opcode {
  void disassemble() {cout << "SKNE V" << regr << ",V" << regy;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    if(r->V[regr_n] != r->V[regy_n]) {
      r->PC += 2;
    }
  };
};
class MVI : public Opcode {
  void disassemble() {cout << "MVI " << addr << "  ";};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    //cout << "cnst=" << cnst << "; cnst_n=" << cnst_n << "; regr=" << regr << "; regr_n=" << regr_n;
    r->I = addr_n;
  };
};
class JMI : public Opcode {
  void disassemble() {cout << "JMI " << addr << "  ";};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    r->PC = addr_n + r->V[0];
  };
};
class RND : public Opcode {
  void disassemble() {cout << "RAND V" << hex << regr << "," << hex << addr;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    r->V[regr_n] = rand() % addr_n;
  };
};
class SPRITE : public Opcode {
  void disassemble() {cout << "SPRITE " << hex << regr << "X," << hex << regy << "Y," << hex << s;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    int loc = r->I;

  };
};
class XSPRITE : public Opcode {
  void disassemble() {cout << "XSPRITE " << hex << regr << "X," << hex << regy << "Y";};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {};
};
class SKPR : public Opcode {
  void disassemble() {cout << "SKPR " << hex << regr;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {};
};
class SKUP : public Opcode {
  void disassemble() {cout << "SKUP " << hex << regr;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {};
};
class GDELAY : public Opcode {
  void disassemble() {cout << "GDELAY V" << regr;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    /* From Emulator101
    static void OpF(Chip8State *state, uint8_t *code)
    {
     int reg = code[0]&0xf;
     switch (code[1])
     {
         case 0x07: state->V[reg] = state->delay; break;  //MOV VX, DELAY
         case 0x15: state->delay = state->V[reg]; break;  //MOV DELAY, VX
         case 0x18: state->sound = state->V[reg]; break;  //MOV SOUND, VX
     }
   } */
  };
};
class KEYR : public Opcode {
  void disassemble() {cout << "KEY V" << regr;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {};
};
class SDELAY : public Opcode {
  void disassemble() {cout << "SDELAY V" << regr;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    /* From Emulator101
    static void OpF(Chip8State *state, uint8_t *code)
    {
     int reg = code[0]&0xf;
     switch (code[1])
     {
         case 0x07: state->V[reg] = state->delay; break;  //MOV VX, DELAY
         case 0x15: state->delay = state->V[reg]; break;  //MOV DELAY, VX
         case 0x18: state->sound = state->V[reg]; break;  //MOV SOUND, VX
     }
   } */
  };
};
class SSOUND : public Opcode {
  void disassemble() {cout << "SSOUND V" << regr;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {};
};
class ADI : public Opcode {
  void disassemble() {cout << "ADI V" << regr << "  ";};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    r->I = r->I + r->V[regr_n];
  };
};
class FONTR : public Opcode {
  void disassemble() {cout << "FONT V" << regr << "  ";};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {};
};
class XFONT : public Opcode {
  void disassemble() {cout << "XFONT V" << regr << "  ";};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {};
};
class BCD : public Opcode {
  void disassemble() {cout << "BCD V" << regr << "  ";};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    uint8_t ones, tens, hundreds;
    uint8_t value = r->V[regr_n];
    ones = value % 10;
    value = value / 10;
    tens = value % 10;
    hundreds = value / 10;
    mem->set(r->I, hundreds);
    mem->set(r->I+1, tens);
    mem->set(r->I+2, ones);

    /* From Emulator101
    int reg = code[0]&0xf;
    uint8_t ones, tens, hundreds;
    uint8_t value=state->V[reg];
    ones = value % 10;
    value = value / 10;
    tens = value % 10;
    hundreds = value / 10;
    state->memory[state->I] = hundreds;
    state->memory[state->I+1] = tens;
    state->memory[state->I+2] = ones;
    */
  };
};
class STR : public Opcode {
  void disassemble() {cout << "STR V0-V" << regr;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    for(int i=0; i<regr_n; i++) {
      mem->set(r->I+i, r->V[i]);
    }
  };
};
class LDR : public Opcode {
  void disassemble() {cout << "LDR V0-V" << regr;};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    for(int i=0; i<regr_n; i++) {
      r->V[i] = mem->get(r->I+i);
    }
  };
};
class UNK : public Opcode {
  void disassemble() {cout << "Unknown opcode ";};
  void emulate(struct my_registers *r, C8mem *mem, Stack *stk) {
    r->PC = 0;
  };
};
