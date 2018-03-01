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
  unsigned int regr_n, regy_n, addr_n, cnst_n, s_n;
  char addr[4], cnst[3];
  //Stack *stk = new Stack();
  Delay *del;
  Sound *snd;
  
  Opcode() {
	Delay *del = new Delay();
	Sound *snd = new Sound();
  };
  ~Opcode() {};

  virtual void disassemble() = 0;
  //virtual void emulate(struct my_registers *r) = 0;
  virtual void emulate(struct my_registers *r) {
	cout << "Unimplemented instruction" << endl;
  };
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
    s_n = n3;
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
  void emulate(struct my_registers *r) {
    for(int row=0; row<64; row++) {
      for(int col=0; col<32; col++) {
        r->disp->set(row, col, 0);
      }
    }
  };
};
class RTS : public Opcode {
  void disassemble() {cout << "RTS";};
  void emulate(struct my_registers *r) {
    r->PC = (r->stk)->pop();
    r->SP += 2;
  };
};
class JMP : public Opcode {
  void disassemble() {cout << "JMP " << addr;};
  void emulate(struct my_registers *r) {
    r->PC = addr_n;
  };
};
class JSR : public Opcode {
  void disassemble() {cout << "JSR " << addr << "  ";};
  void emulate(struct my_registers *r) {
    (r->stk)->push(r->PC+2);  //return Address
    //should I also push registers??
    r->PC = addr_n;
    r->SP -= 2;
  };
};
class SKEQC : public Opcode {
  void disassemble() {cout << "SKEQ V" << regr << "," << cnst;};
  void emulate(struct my_registers *r) {
    if(r->V[regr_n] == cnst_n) {
      r->PC += 2;
    }
  };
};
class SKNEC : public Opcode {
  void disassemble() {cout << "SKNE V" << regr << "," << cnst;};
  void emulate(struct my_registers *r) {
    if(r->V[regr_n] != cnst_n) {
      r->PC += 2;
    }
  };
};
class SKEQR : public Opcode {
  void disassemble() {cout << "MOV V" << regr << ",V" << regy;};
  void emulate(struct my_registers *r) {
    if(r->V[regr_n] == r->V[regy_n]) {
      r->PC += 2;
    }
  };
};
class MOVC : public Opcode {
  void disassemble() {cout << "MOV V" << hex << regr << "," << hex << cnst;};
  void emulate(struct my_registers *r) {
    r->V[regr_n] = cnst_n;
  };
};
class ADDR : public Opcode {
  void disassemble() {cout << "ADD V" << hex << regr << "," << hex << cnst;};
  void emulate(struct my_registers *r) {
    r->V[regr_n] = r->V[regr_n] + cnst_n;
  };
};
class MOVR : public Opcode {
  void disassemble() {cout << "MOV V" << regr << ",V" << regy;};
  void emulate(struct my_registers *r) {
    r->V[regr_n] = r->V[regy_n];
  };
};
class ORR : public Opcode {
  void disassemble() {cout << "OR V" << regr << ",V" << regy;};
  void emulate(struct my_registers *r) {
    r->V[regr_n] = r->V[regr_n] | r->V[regy_n];
  };
};
class ANDR : public Opcode {
  void disassemble() {cout << "AND V" << regr << ",V" << regy;};
  void emulate(struct my_registers *r) {
    r->V[regr_n] = r->V[regr_n] & r->V[regy_n];
  };
};
class XORR : public Opcode {
  void disassemble() {cout << "XOR V" << regr << ",V" << regy;};
  void emulate(struct my_registers *r) {
    r->V[regr_n] = r->V[regr_n] ^ r->V[regy_n];
  };
};
class ADDC : public Opcode {
  void disassemble() {cout << "ADD V" << regr << ",V" << regy;};
  // carry in Vf
  void emulate(struct my_registers *r) {
    int tmp = r->V[regr_n] + r->V[regy_n];
    r->V[regr_n] = tmp;
    r->V[0xf] = tmp > 255 ? 1 : 0;
  };
};
class SUBR : public Opcode {
  void disassemble() {cout << "SUB V" << regr << ",V" << regy;};
  void emulate(struct my_registers *r) {
    //Vf = 1 if borrow !!!
    int tmp = r->V[regr_n] - r->V[regy_n];
    r->V[regr_n] = tmp;
    r->V[0xf] = tmp < 0 ? 1 : 0;
  };
};
class SHR : public Opcode {
  void disassemble() {cout << "SHR V" << regr;};
  void emulate(struct my_registers *r) {
    //bit 0 to Vf !!!
    int tmp = r->V[regr_n] & 1;
    r->V[regr_n] = r->V[regr_n] >> 1;
    r->V[0xf] = tmp;
  };
};
class RSB : public Opcode {
  void disassemble() {cout << "RSB V" << regr << ",V" << regy;};
  void emulate(struct my_registers *r) {
    //Vf = 1 if borrow !!!
    r->V[regr_n] = r->V[regy_n] - r->V[regr_n];
  };
};
class SHL : public Opcode {
  void disassemble() {cout << "SHL V" << regr;};
  void emulate(struct my_registers *r) {
    // bit 7 to Vf !!!
    int tmp = r->V[regr_n] & 0x80 >> 7;
    r->V[regr_n] = r->V[regr_n] << 1;
    r->V[0xf] = tmp;
  };
};
class SKNER : public Opcode {
  void disassemble() {cout << "SKNE V" << regr << ",V" << regy;};
  void emulate(struct my_registers *r) {
    if(r->V[regr_n] != r->V[regy_n]) {
      r->PC += 2;
    }
  };
};
class MVI : public Opcode {
  void disassemble() {cout << "MVI " << addr << "  ";};
  void emulate(struct my_registers *r) {
    r->I = addr_n;
  };
};
class JMI : public Opcode {
  void disassemble() {cout << "JMI " << addr << "  ";};
  void emulate(struct my_registers *r) {
    r->PC = addr_n + r->V[0];
  };
};
class RND : public Opcode {
  void disassemble() {cout << "RAND V" << hex << regr << "," << hex << addr;};
  void emulate(struct my_registers *r) {
    r->V[regr_n] = (rand() % 255) & addr_n;
  };
};
class SPRITE : public Opcode {
  void disassemble() {cout << "SPRITE " << hex << regr << "X," << hex << regy << "Y," << hex << s;};
  void emulate(struct my_registers *r) {
    bool chg = 0;
    for(int row=0; row<s_n; row++) {
      //get byte
      unsigned char tmp = r->mem->get(r->I+row);
      unsigned char msk = 0x80;
      for(int col=7; col>-1; col--) {
        //get single bit out of byte
        bool tmp2 = (bool)((tmp & msk) >> col);
        //XOR with value in display
        bool in = r->disp->get(regy_n+col, regr_n+row);
        bool out = in ^ tmp2;
	   if(out != in) {r->V[0xf] = 1;} //set Vf if a bit changes
        r->disp->set(regy_n+col, regr_n+row, out);
        //cout << "tmp=" << hex << tmp << "; tmp2=" << (tmp2==1?1:0) << "; in=" << (in==1?1:0) << "; msk=" << hex << msk << "; out=" << (out==1?1:0) << endl;
        msk = msk >> 1;
      }
    }
  };
};
class SKPR : public Opcode {
  void disassemble() {cout << "SKPR " << hex << regr;};
  void emulate(struct my_registers *r) {
     if (kbhit()) {
         ch = getch();
         if (ch == regr_n) {r->PC += 2;};
     }
  };
};
class SKUP : public Opcode {
  void disassemble() {cout << "SKUP " << hex << regr;};
  void emulate(struct my_registers *r) {
     if (!kbhit()) {
         ch = getch();
         if (ch != regr_n) {r->PC += 2;};
     }
  };
};
class GDELAY : public Opcode {
  void disassemble() {cout << "GDELAY V" << regr;};
  void emulate(struct my_registers *r) {
     // From Emulator101
     r->V[regr_n] = del->get();
  };
};
class KEYR : public Opcode {
  void disassemble() {cout << "KEY V" << regr;};
  void emulate(struct my_registers *r) {
     while(!kbhit()) {};
     ch = getch();
	r->V[regr_n] = ch;
  };
};
class SDELAY : public Opcode {
  void disassemble() {cout << "SDELAY V" << regr;};
  void emulate(struct my_registers *r) {
     // From Emulator101
     del->set(r->V[regr_n]);
  };
};
class SSOUND : public Opcode {
  void disassemble() {cout << "SSOUND V" << regr;};
  void emulate(struct my_registers *r) {
     // From Emulator101
     snd->set(r->V[regr_n]);
  };
};
class ADI : public Opcode {
  void disassemble() {cout << "ADI V" << regr << "  ";};
  void emulate(struct my_registers *r) {
    r->I = r->I + r->V[regr_n];
  };
};
class FONTR : public Opcode {
  void disassemble() {cout << "FONT V" << regr << "  ";};
  void emulate(struct my_registers *r) {
	Opcode::emulate();
  };
};
class XFONT : public Opcode {
  void disassemble() {cout << "XFONT V" << regr << "  ";};
  void emulate(struct my_registers *r) {
	Opcode::emulate();
  };
};
class BCD : public Opcode {
  void disassemble() {cout << "BCD V" << regr << "  ";};
  void emulate(struct my_registers *r) {
    uint8_t ones, tens, hundreds;
    uint8_t value = r->V[regr_n];
    ones = value % 10;
    value = value / 10;
    tens = value % 10;
    hundreds = value / 10;
    (r->mem)->set(r->I, hundreds);
    (r->mem)->set(r->I+1, tens);
    (r->mem)->set(r->I+2, ones);

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
  void emulate(struct my_registers *r) {
    for(int i=0; i<regr_n; i++) {
      (r->mem)->set(r->I+i, r->V[i]);
    }
  };
};
class LDR : public Opcode {
  void disassemble() {cout << "LDR V0-V" << regr;};
  void emulate(struct my_registers *r) {
    for(int i=0; i<regr_n; i++) {
      r->V[i] = (r->mem)->get(r->I+i);
    }
  };
};
class UNK : public Opcode {
  void disassemble() {cout << "Unknown opcode ";};
  void emulate(struct my_registers *r) {
    r->PC = 0;
  };
};
