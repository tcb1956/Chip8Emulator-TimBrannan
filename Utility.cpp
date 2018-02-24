#include <iostream>

namespace Mine {

  struct my_registers {
    int V[16];
    int PC;
    int I;
    int DT;
    int ST;
  };

}

using namespace std;
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
      if(SP == 0xEA0) {
        cout << "Stack full" << endl;
      } else {
        stk[--SP] = val;
      }
    }

    char pop() {
      if(SP == 0x1000) {
        cout << "Stack underflow" << endl;
        return -1;
      } else {
        return stk[SP++];
      }
    }
};

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
