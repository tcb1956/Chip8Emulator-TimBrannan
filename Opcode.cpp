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

    void dismore(char *buf)
    {
      unsigned char n0, n1, n2, n3;
      unsigned char cmd, regr, regy, qual;
      char addr[4], cnst[3];

      //Get the nibbles
      n0 = (buf[0] >> 4) & 0xf;
      n1 = buf[0] & 0xf;
      n2 = (buf[1] >> 4) & 0xf;
      n3 = buf[1] & 0xf;

      //Convert to printable characters
      n1 < 10 ? n1 += '0' : n1 += 'A' - 10;
      n2 < 10 ? n2 += '0' : n2 += 'A' - 10;
      n3 < 10 ? n3 += '0' : n3 += 'A' - 10;

      cmd = n0; //Get first nibble of code

      //Build all possible parameters
      sprintf(addr, "%c%c%c", n1, n2, n3);
      sprintf(cnst, "%c%c", n2, n3);
      regr = n1;
      regy = n2;
      qual = n3;

      switch (cmd) {
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
          break;
        case 6:
          cout << "MOV V" << hex << regr << "," << hex << cnst << endl;
        case 7:
          cout << "ADD V" << regr << ",V" << regy << endl;
        case 8:
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
              break;
            case 7:
              cout << "RSB V" << regr << ",V" << regy << endl;
            case 0xe:
              if (regy == 0) {
                cout << "SHL V" << regr << endl;
              };
              break;
          };
          case 9:
            if (qual == 0) {
              cout << "SKNE V" << regr << ",V" << regy << endl;
            };
            break;
          case 0xa:
            cout << "MVI " << addr << endl;
          case 0xb:
            cout << "JMI " << addr << endl;
          case 0xc:
          case 0xd:
          case 0xe:
          case 0xf:
            break;
          default:
            cout << "Unknown opcode " << hex << cmd << endl;
            //exit(1);
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
      while(!in.eof()) {

        in.read(buf, 2);

        unsigned char cmd = buf[0] >> 4;

        switch (cmd) {
          case 0x00e0:
            if(buf[1]==0 && buf[2]==14 && buf[3]==0) {
              cout << "CLS\n";
            }
            if(buf[1]==0 && buf[2]==14 && buf[3]==14) {
              cout << "RTS\n";
            }
          default:
            dismore(buf);
            //cout << '.';
        };
      }
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
