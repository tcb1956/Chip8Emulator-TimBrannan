#include <iostream>
#include <fstream>
#include <ostream>
#include <cctype>
#include <iomanip>
#include <ios>
#include <cstdio>
using namespace std;

class Opcode {
public:

  unsigned char n0, n1, n2, n3;
  unsigned char cmd, regr, regy, qual, s;
  char addr[4], cnst[3];

  virtual void disassemble() = 0;
  virtual void emulate() = 0;
  void buildParams(char* buf) {
    //Parse the first nibble of the Opcode
    //and form the parameters

    //Store in address space for Chip8Emulator
    //c8mem[PC++] = buf[0];
    //c8mem[PC++] = buf[1];

    //Get the nibbles
    n0 = (buf[0] >> 4) & 0xf;
    n1 = buf[0] & 0xf;
    n2 = (buf[1] >> 4) & 0xf;
    n3 = buf[1] & 0xf;

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
  void emulate(void) {};
};
class RTS : public Opcode {
  void disassemble() {cout << "RTS" << endl;};
  void emulate(void) {};
};
class JMP : public Opcode {
  void disassemble() {cout << "JMP " << addr << endl;};
  void emulate(void) {};
};
class JSR : public Opcode {
  void disassemble() {cout << "JSR " << addr << endl;};
  void emulate(void) {};
};
class SKEQC : public Opcode {
  void disassemble() {cout << "SKEQ V" << regr << "," << cnst << endl;};
  void emulate(void) {};
};
class SKNEC : public Opcode {
  void disassemble() {cout << "SKNE V" << regr << "," << cnst << endl;};
  void emulate(void) {};
};
class SKEQR : public Opcode {
  void disassemble() {cout << "MOV V" << regr << ",V" << regy << endl;};
  void emulate(void) {};
};
class MOVC : public Opcode {
  void disassemble() {cout << "MOV V" << hex << regr << "," << hex << cnst << endl;};
  void emulate(void) {};
};
class ADDR : public Opcode {
  void disassemble() {cout << "ADD V" << hex << regr << "," << hex << cnst << endl;};
  void emulate(void) {};
};
class MOVR : public Opcode {
  void disassemble() {cout << "MOV V" << regr << ",V" << regy << endl;};
  void emulate(void) {};
};
class ORR : public Opcode {
  void disassemble() {cout << "OR V" << regr << ",V" << regy << endl;};
  void emulate(void) {};
};
class ANDR : public Opcode {
  void disassemble() {cout << "AND V" << regr << ",V" << regy << endl;};
  void emulate(void) {};
};
class XORR : public Opcode {
  void disassemble() {cout << "XOR V" << regr << ",V" << regy << endl;};
  void emulate(void) {};
};
class ADDC : public Opcode {
  void disassemble() {cout << "ADD V" << regr << ",V" << regy << endl;};
  // carry in Vf
  void emulate(void) {};
};
class SUBR : public Opcode {
  void disassemble() {cout << "SUB V" << regr << ",V" << regy << endl;};
  void emulate(void) {};
};
class SHR : public Opcode {
  void disassemble() {cout << "SHR V" << regr << endl;};
  void emulate(void) {};
};
class RSB : public Opcode {
  void disassemble() {cout << "RSB V" << regr << ",V" << regy << endl;};
  void emulate(void) {};
};
class SHL : public Opcode {
  void disassemble() {cout << "SHL V" << regr << endl;};
  void emulate(void) {};
};
class SKNER : public Opcode {
  void disassemble() {cout << "SKNE V" << regr << ",V" << regy << endl;};
  void emulate(void) {};
};
class MVI : public Opcode {
  void disassemble() {cout << "MVI " << addr << endl;};
  void emulate(void) {};
};
class JMI : public Opcode {
  void disassemble() {cout << "JMI " << addr << endl;};
  void emulate(void) {};
};
class RND : public Opcode {
  void disassemble() {cout << "RAND V" << hex << regr << "," << hex << addr << endl;};
  void emulate(void) {};
};
class SPRITE : public Opcode {
  void disassemble() {cout << "SPRITE " << hex << regr << "X," << hex << regy << "Y," << hex << s << endl;};
  void emulate(void) {};
};
class XSPRITE : public Opcode {
  void disassemble() {cout << "XSPRITE " << hex << regr << "X," << hex << regy << "Y" << endl;};
  void emulate(void) {};
};
class SKPR : public Opcode {
  void disassemble() {cout << "SKPR " << hex << regr << endl;};
  void emulate(void) {};
};
class SKUP : public Opcode {
  void disassemble() {cout << "SKUP " << hex << regr << endl;};
  void emulate(void) {};
};
class GDELAY : public Opcode {
  void disassemble() {cout << "GDELAY V" << regr << endl;};
  void emulate(void) {};
};
class KEYR : public Opcode {
  void disassemble() {cout << "KEY V" << regr << endl;};
  void emulate(void) {};
};
class SDELAY : public Opcode {
  void disassemble() {cout << "SDELAY V" << regr << endl;};
  void emulate(void) {};
};
class SSOUND : public Opcode {
  void disassemble() {cout << "SSOUND V" << regr << endl;};
  void emulate(void) {};
};
class ADI : public Opcode {
  void disassemble() {cout << "ADI V" << regr << endl;};
  void emulate(void) {};
};
class FONTR : public Opcode {
  void disassemble() {cout << "FONT V" << regr << endl;};
  void emulate(void) {};
};
class XFONT : public Opcode {
  void disassemble() {cout << "XFONT V" << regr << endl;};
  void emulate(void) {};
};
class BCD : public Opcode {
  void disassemble() {cout << "BCD V" << regr << endl;};
  void emulate(void) {};
};
class STR : public Opcode {
  void disassemble() {cout << "STR V0-V" << regr << endl;};
  void emulate(void) {};
};
class LDR : public Opcode {
  void disassemble() {cout << "LDR V0-V" << regr << endl;};
  void emulate(void) {};
};
class UNK : public Opcode {
  void disassemble() {cout << "Unknown opcode " << endl;};
  void emulate(void) {};
};
