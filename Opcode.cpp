#include <windows.h>
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

    void disassemble(char *fn)
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
        cout << "createOp done...";
        oc->buildParams(buf);
        cout << "buildParams done...";
        oc->disassemble();
        cout << "disassemble done...";
        //disLevel1(buf);
        //Disassembler::dmem->set(c8addr, buf[0]);
        dmem->set(c8addr++, buf[0]);
        dmem->set(c8addr++, buf[1]);
        cout << "dmem->set done\n";
      };
      cout << endl;
      in.close();
    }
};

/************************************************************/
/*                                                          */
/************************************************************/

class Emulator {
  public:

    struct my_registers my_regs;
    struct my_registers *r;

    // memory space
    C8mem *emem;

    void printStat(void) {
      for(int i=0; i<16; i++) {
        cout << "V[" << i << "]=" << r->V[i] << ", ";
      };
      cout << endl;
      cout << "PC=" << r->PC << ", I=" << r->I << ", DT=" << r->DT << ", ST=" << r->ST << endl;
      r->PC += 2;
    }

    Emulator(C8mem *mem) {
      // Address space
      emem = mem;
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
      r->I = 0;
      r->DT = 0;
      r->ST = 0;
      char buf[2];
      r->PC = 0x200;
      int rtn = 0;
      Opcode *oc;
      OpcodeFactory *ocf = new OpcodeFactory();
      //Stack *stk = new Stack();
      while(!rtn) {
        buf[0] = emem->get(r->PC);
        buf[1] = emem->get(r->PC+1);
        oc = ocf->createOp(buf);
        oc->buildParams(buf);
        oc->disassemble();
        oc->emulate(r, emem);
        printStat();
        r->PC += 2;
        Sleep(2);
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

  //if(argv[2] == 0 || argv[2] == 1) {
  if(!strcmp(argv[2], "0") || !strcmp(argv[2], "1")) {
    C8mem *mem = new C8mem();
    Disassembler *dis = new Disassembler(mem);
    dis->disassemble(argv[1]);
    cout << "Done\n";
    /*for(int i=0x200;i<0x210;i++) {
      cout << "c8mem @ " << i << " is " << hex << mem->get(i) << endl;
    }*/
    //if(argv[2] == 1) {
    if(!strcmp(argv[2], "1")) {
      Emulator *em = new Emulator(mem);
      em->emulate();
    }
  } else {
    cout << argv[2] << " is not a valid option"  << endl;
  }
  return 0;
}
