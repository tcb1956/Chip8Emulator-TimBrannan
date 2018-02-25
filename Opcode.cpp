#include <windows.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <ios>
#include <cstdio>
#include "OpcodeFactory.cpp"
using namespace std;

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

    int disassemble(char *fn)
    {
      ifstream in;
      in.open(fn, ios::binary | ios::in);
      if(!in) {
        cout << "Cannot open file\n";
      }
      char buf[2];
      int c8addr = 0x200;
      Opcode *oc;
      OpcodeFactory *ocf = new OpcodeFactory();
      while(!in.eof()) {  // && ++cnt < 10) {
        in.read(buf, 2);
        cout << hex << c8addr << "   ";
        oc = ocf->createOp(buf);
        oc->buildParams(buf);
        oc->disassemble();
        cout << endl;
        //disLevel1(buf);
        //Disassembler::dmem->set(c8addr, buf[0]);
        dmem->set(c8addr++, buf[0]);
        dmem->set(c8addr++, buf[1]);
      };
      cout << endl;
      in.close();
      return c8addr;
    }
};

/************************************************************/
/*                                                          */
/************************************************************/

class Emulator {
  public:

    struct my_registers my_regs;
    struct my_registers *r;

    // memory space and stack
    C8mem *emem;
    Stack *stk;

    void printStat(void) {
      cout << "\t\tPC=" << r->PC << ", SP=" << r->SP << ", I=" << r->I << ", DT=" << r->DT << ", ST=" << r->ST << endl;
      cout << "\t\t\t";
      for(int i=0; i<8; i++) {
        cout << "V[" << i << "]=" << r->V[i] << ", ";
      };
      cout << endl << "\t\t\t";
      for(int i=8; i<16; i++) {
        cout << "V[" << i << "]=" << r->V[i] << ", ";
      };
      cout << endl;
    }

    Emulator(C8mem *mem) {
      // Address space
      emem = mem;
      stk = new Stack();
    }

    ~Emulator(){
      //release mem
    }

    void emulate(void)
    {
      r = &my_regs;
      for(int i=0; i<17; i++) {
        r->V[i] = 0;
      }
      r->PC = 0x200;
      r->SP = 0xF00;
      r->I = 0;
      r->DT = 0;
      r->ST = 0;
      char buf[2];
      int rtn = 0;
      char ch;
      bool stp = 0;
      Opcode *oc;
      OpcodeFactory *ocf = new OpcodeFactory();
      while(!stp && r->PC>=0x200) {
        buf[0] = emem->get(r->PC);
        buf[1] = emem->get(r->PC+1);
        oc = ocf->createOp(buf);
        oc->buildParams(buf);
        oc->disassemble();
        oc->emulate(r, emem, stk);
        printStat();
        r->PC += 2;
        //if((ch = GetAsyncKeyState(VK_SPACE)) == 27) {stp = 1;};
        if (kbhit()) {
            ch = getch();
            if (ch == 27) {stp = 1;};
        }
      };
    }
};

/************************************************************/
/*                                                          */
/************************************************************/
int main(int argc, char *argv[])
{
  if(argc!=3) {
    cout << "Usage: Opcode <filename> <option>" << endl;
    cout << "Where option=0 for disassembler; 1 for emulator" << endl;
    return 1;
  }

  C8mem *mem = new C8mem();
  Disassembler *dis = new Disassembler(mem);

  int bad = 1;
  if(!strcmp(argv[2], "0")) {
    dis->disassemble(argv[1]);
    cout << "Done" << endl;
    bad = 0;
  }
  if(!strcmp(argv[2], "1")) {
    Emulator *em = new Emulator(mem);
    dis->disassemble(argv[1]);
    em->emulate();
    bad = 0;
  }
  if(bad == 1) {
    cout << argv[2] << " is not a valid option"  << endl;
  }
  return 0;
}
