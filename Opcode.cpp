//#include "pch.h"
#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <ios>
#include <cstdio>
using namespace std;

class Opcode {
  public:

    void disLevel2(unsigned char qual, unsigned char regr, unsigned char regy)
    {
      switch (qual) {
        case 0:
          cout << "MOV V" << regr << ",V" << regy << endl;
        case 1:
          cout << "OR V" << regr << ",V" << regy << endl;
        case 2:
          cout << "AND V" << regr << ",V" << regy << endl;
        case 3:
          cout << "XOR V" << regr << ",V" << regy << endl;
        case 4:
          cout << "ADD V" << regr << ",V" << regy << endl;
        case 5:
          cout << "SUB V" << regr << ",V" << regy << endl;
        case 6:
          if (regy == 0) {
            cout << "SHR V" << regr << endl;
          };
        case 7:
          cout << "RSB V" << regr << ",V" << regy << endl;
        case 0xe:
          if (regy == 0) {
            cout << "SHL V" << regr << endl;
          };
      };
    }


    void disLevel1(char *buf)
    {
      unsigned char n0, n1, n2, n3;
      unsigned char cmd, regr, regy, qual;
      char addr[4], cnst[3];

      //Get the nibbles
      n0 = (buf[0] >> 4) & 0xf;
      n1 = buf[0] & 0xf;
      n2 = (buf[1] >> 4) & 0xf;
      n3 = buf[1] & 0xf;

      cmd = n0;   //Get first nibble of code
      qual = n3;  //Keep qual as number not char

      //Convert to printable characters
      n0 < 10 ? n0 += '0' : n0 += 'A' - 10;
      n1 < 10 ? n1 += '0' : n1 += 'A' - 10;
      n2 < 10 ? n2 += '0' : n2 += 'A' - 10;
      n3 < 10 ? n3 += '0' : n3 += 'A' - 10;

      //Build parameters
      sprintf(addr, "%c%c%c", n1, n2, n3);
      sprintf(cnst, "%c%c", n2, n3);
      regr = n1;
      regy = n2;

      switch (cmd) {
        case 0:
          if(n1=='0' && n2=='E' && n3=='0') {
            cout << "CLS\n";
          }
          if(n1=='0' && n2=='E' && n3=='E') {
            cout << "RTS\n";
          }
        case 1:
          cout << "JMP " << addr << endl;
        case 2:
          cout << "JSR " << addr << endl;
        case 3:
          cout << "SKEQ V" << regr << "," << cnst << endl;
        case 4:
          cout << "SKNE V" << regr << "," << cnst << endl;
        case 5:
          if (qual == 0) {
            cout << "SKEQ V" << regr << ",V" << regy << endl;
          };
        case 6:
          cout << "MOV V" << hex << regr << "," << hex << cnst << endl;
        case 7:
          cout << "ADD V" << regr << ",V" << regy << endl;
        case 8:
          disLevel2(qual, regr, regy);
        case 9:
          if (qual == 0) {
            cout << "SKNE V" << regr << ",V" << regy << endl;
          };
        case 0xa:
          cout << "MVI " << addr << endl;
        case 0xb:
          cout << "JMI " << addr << endl;
        case 0xc:
          cout << "FIX THIS " << endl;
        case 0xd:
          cout << "FIX THIS " << endl;
        case 0xe:
          cout << "FIX THIS " << endl;
        case 0xf:
          cout << "FIX THIS " << endl;
        default:
          cout << "Unknown opcode " << hex << n0 << endl;
      };
    }



    void disassemble(char *fn)
    {
      ifstream in;
      in.open(fn, ios::binary | ios::in);
      if(!in) {
        cout << "Cannot open file\n";
      }
      char buf[2];
      int cnt = 0;
      int c8addr = 0x200;
      while(!in.eof() && ++cnt < 10) {
        in.read(buf, 2);
        cout << hex << c8addr << "   ";
        c8addr += 2;
        disLevel1(buf);
        //cout << "Line " << cnt << endl;
      };
      cout << endl;
      in.close();
    }
};


int main(int argc, char *argv[])
{
  if(argc!=2) {
    cout << "Usage: Opcode <filename>\n";
    return 1;
  }
  Opcode oc;
  oc.disassemble(argv[1]);
  cout << "Done\n";
  return 0;
}
