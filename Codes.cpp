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
  C8mem *emem = new C8mem();

  virtual void disassemble() = 0;
  virtual void emulate(struct my_registers *r, C8mem *mem) = 0;
  void buildParams(char* buf) {

  //Parse the first nibble of the Opcode
    //and form the parameters

    //Get the nibbles
    n0 = (buf[0] >> 4) & 0xf;
    n1 = buf[0] & 0xf;
    n2 = (buf[1] >> 4) & 0xf;
    n3 = buf[1] & 0xf;

    regr_n = n1;
    regy_n = n2;
    addr_n = n1 * 0xff + n2 * 0xf + n3;
    cnst_n = n2 * 0xf + n3;
    cmd = n0;   //Get first nibble of code
    qual = n3;  //Keep qual as number not char

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
  void disassemble() {cout << "CLS" << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {};
};
class RTS : public Opcode {
  void disassemble() {cout << "RTS" << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {
    r->PC = stk->pop();
  };
};
class JMP : public Opcode {
  void disassemble() {cout << "JMP " << addr << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {
    r->PC = (int)addr;
  };
};
class JSR : public Opcode {
  void disassemble() {cout << "JSR " << addr << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {
    stk->push(r->PC+2);
    r->PC = (int)addr;
  };
};
class SKEQC : public Opcode {
  void disassemble() {cout << "SKEQ V" << regr << "," << cnst << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {
    if(r->V[regr] == (int)cnst) {
      r->PC += 2;
    }
  };
};
class SKNEC : public Opcode {
  void disassemble() {cout << "SKNE V" << regr << "," << cnst << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {
    if(r->V[regr] != (int)cnst) {
      r->PC += 2;
    }
  };
};
class SKEQR : public Opcode {
  void disassemble() {cout << "MOV V" << regr << ",V" << regy << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {
    if(r->V[regr] == r->V[regy]) {
      r->PC += 2;
    }
  };
};
class MOVC : public Opcode {
  void disassemble() {cout << "MOV V" << hex << regr << "," << hex << cnst << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {
    cout << "cnst=" << cnst_n << "; regr=" << regr;
    r->V[regr_n] = (int)cnst_n;
    cout << "; r->V[regr]=" << r->V[regr_n] << endl;
  };
};
class ADDR : public Opcode {
  void disassemble() {cout << "ADD V" << hex << regr << "," << hex << cnst << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {
    r->V[regr] = r->V[regr] + (int)cnst;
  };
};
class MOVR : public Opcode {
  void disassemble() {cout << "MOV V" << regr << ",V" << regy << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {
    r->V[regr] = r->V[regy];
  };
};
class ORR : public Opcode {
  void disassemble() {cout << "OR V" << regr << ",V" << regy << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {
    r->V[regr] = r->V[regr] | r->V[regy];
  };
};
class ANDR : public Opcode {
  void disassemble() {cout << "AND V" << regr << ",V" << regy << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {};
};
class XORR : public Opcode {
  void disassemble() {cout << "XOR V" << regr << ",V" << regy << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {
    r->V[regr] = r->V[regr] & r->V[regy];
  };
};
class ADDC : public Opcode {
  void disassemble() {cout << "ADD V" << regr << ",V" << regy << endl;};
  // carry in Vf
  void emulate(struct my_registers *r, C8mem *mem) {
    r->V[regr] = r->V[regr] + r->V[regy];
  };
};
class SUBR : public Opcode {
  void disassemble() {cout << "SUB V" << regr << ",V" << regy << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {
    //Vf = 1 if borrow !!!
    r->V[regr] = r->V[regr] - r->V[regy];
  };
};
class SHR : public Opcode {
  void disassemble() {cout << "SHR V" << regr << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {
    //bit 0 to Vf !!!
    r->V[regr] = r->V[regr] >> 1;
  };
};
class RSB : public Opcode {
  void disassemble() {cout << "RSB V" << regr << ",V" << regy << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {
    //Vf = 1 if borrow !!!
    r->V[regr] = r->V[regy] - r->V[regr];
  };
};
class SHL : public Opcode {
  void disassemble() {cout << "SHL V" << regr << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {
    // bit 7 to Vf !!!
    r->V[regr] = r->V[regr] << 1;
  };
};
class SKNER : public Opcode {
  void disassemble() {cout << "SKNE V" << regr << ",V" << regy << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {
    if(r->V[regr] != r->V[regy]) {
      r->PC += 2;
    }
  };
};
class MVI : public Opcode {
  void disassemble() {cout << "MVI " << addr << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {
    r->I = (int)addr;
  };
};
class JMI : public Opcode {
  void disassemble() {cout << "JMI " << addr << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {
    r->PC = (int)addr + r->V[0];
  };
};
class RND : public Opcode {
  void disassemble() {cout << "RAND V" << hex << regr << "," << hex << addr << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {
    r->V[regr] = rand() % (int)addr;
  };
};
class SPRITE : public Opcode {
  void disassemble() {cout << "SPRITE " << hex << regr << "X," << hex << regy << "Y," << hex << s << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {};
};
class XSPRITE : public Opcode {
  void disassemble() {cout << "XSPRITE " << hex << regr << "X," << hex << regy << "Y" << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {};
};
class SKPR : public Opcode {
  void disassemble() {cout << "SKPR " << hex << regr << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {};
};
class SKUP : public Opcode {
  void disassemble() {cout << "SKUP " << hex << regr << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {};
};
class GDELAY : public Opcode {
  void disassemble() {cout << "GDELAY V" << regr << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {};
};
class KEYR : public Opcode {
  void disassemble() {cout << "KEY V" << regr << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {};
};
class SDELAY : public Opcode {
  void disassemble() {cout << "SDELAY V" << regr << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {};
};
class SSOUND : public Opcode {
  void disassemble() {cout << "SSOUND V" << regr << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {};
};
class ADI : public Opcode {
  void disassemble() {cout << "ADI V" << regr << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {
    r->I = r->I + r->V[regr];
  };
};
class FONTR : public Opcode {
  void disassemble() {cout << "FONT V" << regr << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {};
};
class XFONT : public Opcode {
  void disassemble() {cout << "XFONT V" << regr << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {};
};
class BCD : public Opcode {
  void disassemble() {cout << "BCD V" << regr << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {};
};
class STR : public Opcode {
  void disassemble() {cout << "STR V0-V" << regr << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {
    for(int i=0; i<regr; i++) {
      emem->set(r->I+i, r->V[i]);
    }
  };
};
class LDR : public Opcode {
  void disassemble() {cout << "LDR V0-V" << regr << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {
    for(int i=0; i<regr; i++) {
      r->V[i] = emem->get(r->I+i);
    }
  };
};
class UNK : public Opcode {
  void disassemble() {cout << "Unknown opcode " << endl;};
  void emulate(struct my_registers *r, C8mem *mem) {};
};
