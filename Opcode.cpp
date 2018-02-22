//#include "pch.h"
#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <ios>
#include <cstdio>
using namespace std;

/************************************************************/
/*                                                          */
/************************************************************/
class C8mem {
  private:
    char *mem;

  public:

    C8mem() {
      try {
        mem = new char [0xfff];
        for(int i=0; i<0x1000; i++) {mem[i] = 0;}
      } catch (bad_alloc xa) {
        cout << "Allocation Failure\n";
      }
    }

    ~C8mem() {
      delete [] mem;
    }

    void set(unsigned int indx, char val) {
      mem[indx] = val;
    }
    char get(unsigned int indx) {
      return mem[indx];
    }
};

/************************************************************/
/*                                                          */
/************************************************************/
class Stack {
  private:
    char *stk;
    int SP = 0xF00;

  public:

    Stack(){
      try {
        stk = new char [96];
        for(int i=0; i<0x1000; i++) {stk[i] = 0;}
      } catch (bad_alloc xa) {
        cout << "Allocation Failure\n";
      }
    }

    ~Stack(){
      delete [] stk;
    }

    // Put in boundary checks !!
    void push(char val) {
      stk[--SP] = val;
    }

    char pop() {
      return stk[SP++];
    }
};

/************************************************************/
/*                                                          */
/************************************************************/
class Disassembler {
  C8mem *dmem;
  public:

    Disassembler(C8mem *mem) {
      dmem = mem;
    }

    ~Disassembler(){
      //release mem
    }

    void disLevel2(unsigned char qual, unsigned char regr, unsigned char regy)
    {
      //Parse the fourth nibble of the code if the first is an 8

      switch (qual-'0') {
        case 0:
          cout << "MOV V" << regr << ",V" << regy << endl;
          break;
        case 1:
          cout << "OR V" << regr << ",V" << regy << endl;
          break;
        case 2:
          cout << "AND V" << regr << ",V" << regy << endl;
          break;
        case 3:
          cout << "XOR V" << regr << ",V" << regy << endl;
          break;
        case 4:
          cout << "ADD V" << regr << ",V" << regy << endl;
          break;
        case 5:
          cout << "SUB V" << regr << ",V" << regy << endl;
          break;
        case 6:
          if (regy == 0) {
            cout << "SHR V" << regr << endl;
          } else {
            cout << "Unknown opcode " << endl;
          };
          break;
        case 7:
          cout << "RSB V" << regr << ",V" << regy << endl;
          break;
        case 0xe:
          if (regy == 0) {
            cout << "SHL V" << regr << endl;
          } else {
            cout << "Unknown opcode " << endl;
          };
          break;
        default:
          cout << "Unknown opcode " << endl;
      };
    }

    void disLevel2b(unsigned char qual, unsigned char regr, unsigned char regy)
    {
      //Parse the 3rd and 4th nibbles of the code if the first is an 0xf

      switch (regy-'0') {
        case 0:
          if(qual=='7') {cout << "GDELAY V" << regr << endl;}
          else {
            if(qual=='a') {cout << "KEY V" << regr << endl;}
            else {cout << "Unknown opcode 0?" << endl;};
          };
          break;
        case 1:
          if(qual=='5') {cout << "SDELAY V" << regr << endl;}
          else {
            if(qual=='8') {cout << "SSOUND V" << regr << endl;}
            else {cout << "Unknown opcode 1?" << endl;};
          };
          break;
        case 2:
          if(qual=='9') {cout << "FONT V" << regr << endl;}
          else {cout << "Unknown opcode 2?" << endl;
          };
          break;
        case 3:
          if(qual=='0') {cout << "XFONT V" << regr << endl;}
          else {
            if(qual=='3') {cout << "BCD V" << regr << endl;}
            else {cout << "Unknown opcode 3?" << endl;};
          };
          break;
        case 5:
          if(qual=='5') {cout << "STR V0-V" << regr << endl;}
          else {cout << "Unknown opcode 5?" << endl;
          };
          break;
        case 6:
          if(qual=='5') {cout << "LDR V0-V" << regr << endl;}
          else {cout << "Unknown opcode 6?" << endl;
          };
          break;
        default:
          cout << "Unknown opcode (default)" << endl;
      };
    }

    void disLevel1(char *buf)
    {
      //Parse the first nibble of the Opcode
      //and form the parameters

      unsigned char n0, n1, n2, n3;
      unsigned char cmd, regr, regy, qual, s;
      char addr[4], cnst[3];

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

      //Parse first nibble
      switch (cmd) {
        case 0:
          if(n1=='0' && n2=='E' && n3=='0') {
            cout << "CLS\n";
          } else {
            if(n1=='0' && n2=='E' && n3=='E') {
              cout << "RTS\n";
            } else {
              cout << "Unknown opcode " << endl;
            };
          };
          break;
        case 1:
          cout << "JMP " << addr << endl;
          break;
        case 2:
          cout << "JSR " << addr << endl;
          break;
        case 3:
          cout << "SKEQ V" << regr << "," << cnst << endl;
          break;
        case 4:
          cout << "SKNE V" << regr << "," << cnst << endl;
          break;
        case 5:
          if (qual == 0) {
            cout << "SKEQ V" << regr << ",V" << regy << endl;
          } else {
            cout << "Unknown opcode " << endl;
          };
          break;
        case 6:
          cout << "MOV V" << hex << regr << "," << hex << cnst << endl;
          break;
        case 7:
          cout << "ADD V" << hex << regr << "," << hex << cnst << endl;
          break;
        case 8:
          disLevel2(s, regr, regy);
          break;
        case 9:
          if (qual == 0) {
            cout << "SKNE V" << regr << ",V" << regy << endl;
          } else {
            cout << "Unknown opcode " << endl;
          };
          break;
        case 0xa:
          cout << "MVI " << addr << endl;
          break;
        case 0xb:
          cout << "JMI " << addr << endl;
          break;
        case 0xc:
          cout << "RAND V" << hex << regr << "," << hex << addr << endl;
          break;
        case 0xd:
          if(qual==0) {
            cout << "XSPRITE " << hex << regr << "X," << hex << regy << "Y" << endl;
          } else {
            cout << "SPRITE " << hex << regr << "X," << hex << regy << "Y," << hex << s << endl;
          };
          break;
        case 0xe:
          //if(qual==14 && regy=='9') {cout << "SKPR " << hex << regr << endl;}
          if(qual==14) {cout << "SKPR " << hex << regr << endl;}
          else {
            //if(qual==1 && regy=='a') {cout << "SKUP " << hex << regr << endl;}
            if(qual==1) {cout << "SKUP " << hex << regr << endl;}
            else {cout << "Unknown opcode " << endl;};
          };
          break;
        case 0xf:
          disLevel2b(s, regr, regy);
          break;
        default:
          cout << "Unknown opcode " << hex << n0 << endl;
      };
    };


    void disassemble(char *fn)
    {
      ifstream in;
      in.open(fn, ios::binary | ios::in);
      if(!in) {
        cout << "Cannot open file\n";
      }
      char buf[2];
      //int cnt = 0;
      int c8addr = 0x200;
      while(!in.eof()) {  // && ++cnt < 10) {
        in.read(buf, 2);
        cout << hex << c8addr << "   ";
        c8addr += 2;
        disLevel1(buf);
        Disassembler::dmem->set(c8addr, buf[0]);
      };
      cout << endl;
      in.close();
    }
};

/************************************************************/
/*                                                          */
/************************************************************/
class Emulator {
  private:

    // Registers
    int V[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int PC = 0x200, SP = 0xEFF, I = 0, DT = 0, ST = 0;

    // memory space
    C8mem *c8mem;

    void printStat(void) {
      for(int i=0; i<16; i++) {
        cout << "V[" << i << "]=" << V[i] << ", ";
      };
      cout << endl;
      cout << "PC=" << PC << ", SP=" << SP << ", I=" << I << ", DT=" << DT << ", ST=" << ST << endl;
      PC += 2;
    }

  public:

    Emulator(C8mem *mem) {
      // Address space
      C8mem *c8mem = mem;
    }

    ~Emulator(){
      //release mem
    }

    int emLevel2(unsigned char qual, unsigned char regr, unsigned char regy)
    {
      //Parse the fourth nibble of the code if the first is an 8

      switch (qual-'0') {
        case 0:
          V[regr] = V[regy];
          printStat();
          break;
        case 1:
          //cout << "OR V" << regr << ",V" << regy << endl;
          V[regr] = V[regr] | V[regy];
          printStat();
          break;
        case 2:
          //cout << "AND V" << regr << ",V" << regy << endl;
          V[regr] = V[regr] & V[regy];
          printStat();
          break;
        case 3:
          //cout << "XOR V" << regr << ",V" << regy << endl;
          V[regr] = V[regr] ^ V[regy];
          printStat();
          break;
        case 4:
          //cout << "ADD V" << regr << ",V" << regy << endl;
          V[regr] = V[regr] + V[regy];
          printStat();
          break;
        case 5:
          //cout << "SUB V" << regr << ",V" << regy << endl;
          //Vf = 1 if borrow !!!
          V[regr] = V[regr] - V[regy];
          printStat();
          break;
        case 6:
          if (regy == 0) {
            //cout << "SHR V" << regr << endl;
            //bit 0 to Vf !!!
            V[regr] = V[regr] >> 1;
            printStat();
          } else {
            cout << "Unknown opcode " << endl;
            return 1;
          };
          break;
        case 7:
          //cout << "RSB V" << regr << ",V" << regy << endl;
          //Vf = 1 if borrow !!!
          V[regr] = V[regy] - V[regr];
          printStat();
          break;
        case 0xe:
          if (regy == 0) {
            //cout << "SHL V" << regr << endl;
            // bit 7 to Vf !!!
            V[regr] = V[regr] << 1;
            printStat();
          } else {
            cout << "Unknown opcode " << endl;
            return 1;
          };
          break;
        default:
          cout << "Unknown opcode " << endl;
          return 1;
      };
      return 0;
    }

    int emLevel2b(unsigned char qual, unsigned char regr, unsigned char regy)
    {
      //Parse the 3rd and 4th nibbles of the code if the first is an 0xf

      switch (regy-'0') {
        case 0:
          if(qual=='7') {
            // << "GDELAY V" << regr << endl;
            V[regr] = DT;
            printStat();
          }
          else {
            if(qual=='a') {
              //cout << "KEY V" << regr << endl;
              // wait for key press !!!
              //V[regr] = Key;
              printStat();
            }
            else {
              cout << "Unknown opcode 0?" << endl;};
              return 1;
          };
          break;
        case 1:
          if(qual=='5') {
            //cout << "SDELAY V" << regr << endl;
            DT = V[regr];
            printStat();
          }
          else {
            if(qual=='8') {
              //cout << "SSOUND V" << regr << endl;
              ST = V[regr];
              printStat();
            }
            else {
              cout << "Unknown opcode 1?" << endl;
              return 1;
            };
          };
          break;
        case 2:
          if(qual=='9') {
            //cout << "FONT V" << regr << endl;
            printStat();
          }
          else {
            cout << "Unknown opcode 2?" << endl;
            return 1;
          };
          break;
        case 3:
          if(qual=='0') {
            //cout << "XFONT V" << regr << endl;
            printStat();
          }
          else {
            if(qual=='3') {
              //cout << "BCD V" << regr << endl;
              printStat();
            }
            else {
              cout << "Unknown opcode 3?" << endl;};
              return 1;
          };
          break;
        case 5:
          if(qual=='5') {
            //cout << "STR V0-V" << regr << endl;
            printStat();
          }
          else {
            cout << "Unknown opcode 5?" << endl;
            return 1;
          };
          break;
        case 6:
          if(qual=='5') {
            //cout << "LDR V0-V" << regr << endl;
            printStat();
          }
          else {
            cout << "Unknown opcode 6?" << endl;
            return 1;
          };
          break;
        default:
          cout << "Unknown opcode (default)" << endl;
          return 1;
      };
      return 0;
    }

    int emLevel1(char *buf)
    {
      //Parse the first nibble of the Opcode
      //and form the parameters

      unsigned char n0, n1, n2, n3;
      unsigned char cmd, regr, regy, qual, s;
      char addr[4], cnst[3];

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

      //Parse first nibble
      switch (cmd) {
        case 0:
          if(n1=='0' && n2=='E' && n3=='0') {
            cout << "CLS\n";
          } else {
            if(n1=='0' && n2=='E' && n3=='E') {
              cout << "RTS\n";
            } else {
              cout << "Unknown opcode " << endl;
              return 1;
            };
          };
          break;
        case 1:
          //cout << "JMP " << addr << endl;
          //PC = addr;
          break;
        case 2:
          //cout << "JSR " << addr << endl;
          // push PC + 2
          //PC = addr;
          break;
        case 3:
          //cout << "SKEQ V" << regr << "," << cnst << endl;
          if(V[regr] == cnst) {
            PC += 2;
          }
          break;
        case 4:
          //cout << "SKNE V" << regr << "," << cnst << endl;
          if(V[regr] != cnst) {
            PC += 2;
          }
          break;
        case 5:
          if (qual == 0) {
            //cout << "SKEQ V" << regr << ",V" << regy << endl;
            if(V[regr] == V[regy]) {
              PC += 2;
            }
          } else {
            cout << "Unknown opcode " << endl;
            return 1;
          };
          break;
        case 6:
          //cout << "MOV V" << hex << regr << "," << hex << cnst << endl;
          V[regr] = cnst;
          break;
        case 7:
          //cout << "ADD V" << hex << regr << "," << hex << cnst << endl;
          V[regr] = V[regr] + cnst;
          break;
        case 8:
          return emLevel2(s, regr, regy);
          break;
        case 9:
          if (qual == 0) {
            //cout << "SKNE V" << regr << ",V" << regy << endl;
            if(V[regr] != V[regy]) {
              PC += 2;
            }
          } else {
            cout << "Unknown opcode " << endl;
            return 1;
          };
          break;
        case 0xa:
          //cout << "MVI " << addr << endl;
          //I = addr;
          break;
        case 0xb:
          //cout << "JMI " << addr << endl;
          //PC = addr + V[0];
          break;
        case 0xc:
          //cout << "RAND V" << hex << regr << "," << hex << addr << endl;
          //V[regr] = rand(addr);
          break;
        case 0xd:
          if(qual==0) {
            //cout << "XSPRITE " << hex << regr << "X," << hex << regy << "Y" << endl;
          } else {
            //cout << "SPRITE " << hex << regr << "X," << hex << regy << "Y," << hex << s << endl;
          };
          break;
        case 0xe:
          //if(qual==14 && regy=='9') {cout << "SKPR " << hex << regr << endl;}
          if(qual==14) {
            //cout << "SKPR " << hex << regr << endl;
          }
          else {
            //if(qual==1 && regy=='a') {cout << "SKUP " << hex << regr << endl;}
            if(qual==1) {
              //cout << "SKUP " << hex << regr << endl;
            }
            else {
              cout << "Unknown opcode " << endl;
              return 1;
            };
          };
          break;
        case 0xf:
          return emLevel2b(s, regr, regy);
          break;
        default:
          cout << "Unknown opcode " << hex << n0 << endl;
          return 1;
      };
      return 0;
    };


    void emulate(void)
    {
      char buf[2];
      PC = 0x200;
      int rtn = 0;
      while(!rtn) {
        buf[1] = c8mem->get(PC);
        buf[2] = c8mem->get(PC+1);
        rtn = emLevel1(buf);
        printStat();
        PC += 2;
      };
    }
};

/************************************************************/
/*                                                          */
/************************************************************/
int main(int argc, char *argv[])
{
  if(argc!=2) {
    cout << "Usage: Opcode <filename>\n";
    return 1;
  }

  C8mem *mem = new C8mem();
  Disassembler *dis = new Disassembler(mem);
  dis->disassemble(argv[1]);
  Emulator *em = new Emulator(mem);
  dis->disassemble(argv[1]);
  cout << "Done\n";
  return 0;
}
