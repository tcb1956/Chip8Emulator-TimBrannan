#include "Codes.cpp"
using namespace std;

/************************************************************/
/*                                                          */
/************************************************************/
class OpcodeFactory {

  public:

    OpcodeFactory() {
    }

    ~OpcodeFactory(){
    }

    Opcode* createOp(char *buf)
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
            return new CLS();
          } else {
            if(n1=='0' && n2=='E' && n3=='E') {
              return new RTS();
            } else {
              return new UNK();
            };
          };
          break;
        case 1:
          return new JMP();
          break;
        case 2:
          return new JSR();
          break;
        case 3:
          return new SKEQC();
          break;
        case 4:
          return new SKNEC();
          break;
        case 5:
          if (qual == 0) {
            return new SKEQR();
          } else {
            return new UNK();
          };
          break;
        case 6:
          return new MOVC();
          break;
        case 7:
          return new ADDC();
          break;
        case 8:
          return opLevel2(s, regr, regy);
          break;
        case 9:
          if (qual == 0) {
            return new SKNER();
          } else {
            return new UNK();
          };
          break;
        case 0xa:
          return new MVI();
          break;
        case 0xb:
          return new JMI();
          break;
        case 0xc:
          return new RND();
          break;
        case 0xd:
          if(qual==0) {
            return new XSPRITE();
          } else {
            return new SPRITE();
          };
          break;
        case 0xe:
          //if(qual==14 && regy=='9') {
          if(qual==14) {
            return new SKPR();
          }
          else {
            //if(qual==1 && regy=='a') {
            if(qual==1) {
              return new SKUP();
            }
            else {
              return new UNK();
            };
          };
          break;
        case 0xf:
          return opLevel2b(s, regr, regy);
          break;
        default:
          return new UNK();
      };
    };

  private:

    Opcode* opLevel2(unsigned char qual, unsigned char regr, unsigned char regy)
    {
      //Parse the fourth nibble of the code if the first is an 8

      switch (qual-'0') {
        case 0:
          return new MOVR();
          break;
        case 1:
          return new ORR();
          break;
        case 2:
          return new ANDR();
          break;
        case 3:
          return new XORR();
          break;
        case 4:
          return new ADDR();
          break;
        case 5:
          return new SUBR();
          break;
        case 6:
          if (regy == 0) {
            return new SHR();
          };
          break;
        case 7:
          return new RSB();
          break;
        case 0xe:
          if (regy == 0) {
            return new SHL();
          };
          break;
        default:
          return new UNK();
      };
    }

    Opcode* opLevel2b(unsigned char qual, unsigned char regr, unsigned char regy)
    {
      //Parse the 3rd and 4th nibbles of the code if the first is an 0xf

      switch (regy-'0') {
        case 0:
          if(qual=='7') {
            return new GDELAY();
          }
          else {
            if(qual=='a') {
              return new KEYR();
            }
            else {
              return new UNK();
            }
          };
          break;
        case 1:
          if(qual=='5') {
            return new SDELAY();
          }
          else {
            if(qual=='8') {
              return new SSOUND();
            }
            else {
              return new UNK();
            };
          };
          break;
        case 2:
          if(qual=='9') {
            return new FONTR();
          }
          else {
            return new UNK();
          };
          break;
        case 3:
          if(qual=='0') {
            return new XFONT();
          }
          else {
            if(qual=='3') {
              return new BCD();
            }
            else {
              return new UNK();
            }
          };
          break;
        case 5:
          if(qual=='5') {
            return new STR();
          }
          else {
            return new UNK();
          };
          break;
        case 6:
          if(qual=='5') {
            return new LDR();
          }
          else {
            return new UNK();
          };
          break;
        default:
          return new XFONT();
      };
    }


};
