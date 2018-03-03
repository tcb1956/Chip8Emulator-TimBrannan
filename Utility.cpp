#include <iostream>

using namespace std;

/************************************************************/
/*                                                          */
/************************************************************/
class Stack {
  private:
    int *stk;
    int SP = 0xF00;

  public:

    Stack(){
      try {
        stk = new int [96];
        for(int i=0; i<96; i++) {stk[i] = 0;}
      } catch (bad_alloc xa) {
        cout << "Allocation Failure\n";
      }
    }

    ~Stack(){
      delete [] stk;
    }

    void push(int val) {
      if(SP == 0xEA0) {
        cout << endl << "Stack full" << endl;
      } else {
        SP--;
        stk[SP] = val;
      }
    }

    int pop() {
      if(SP == 0xF02) {
        cout << endl << "Stack underflow" << endl;
        return -1;
      } else {
        int tmp = SP;
        SP++;
        return stk[tmp];
      }
    }
};

/************************************************************/
/*                                                          */
/************************************************************/
class C8mem {
  private:
    unsigned char *mem;

  public:

    C8mem() {
      try {
        //mem = calloc(1024*4, 1);
        mem = new unsigned char [0x1000];
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
/*           THIS NEEDS WORK                                */
/************************************************************/
class Keypad {
  private:
    char *keys;

  public:

    Keypad() {
      try {
        keys = new char [0xf];
        for(int i=0; i<0x10; i++) {keys[i] = 0;}
      } catch (bad_alloc xa) {
        cout << "Allocation Failure\n";
      }
    }

    ~Keypad() {
      delete [] keys;
    }

    char get(unsigned int indx) {
      return keys[indx];
    }
};

/************************************************************/
/*           THIS NEEDS WORK                                */
/************************************************************/
class Display {
  public:
    bool disp[64][32];

    Display() {
      for(int i=0; i<64; i++) {
        for(int j=0; j<32; j++) {
          disp[i][j] = 0;
        }
      }
    }

    ~Display() {
      //delete [] disp;
    }

    void set(unsigned int row, unsigned int col, char val) {
      disp[row][col] = val;
    }
    char get(unsigned int row, unsigned int col) {
      return disp[row][col];
    }

};

struct my_registers {
  int V[16];
  int PC;
  int SP;
  int I;
  int DT;
  int ST;
  Keypad *kp;
  C8mem *mem;
  Stack *stk;
  Display *disp;
};

/************************************************************/
/*           THIS NEEDS WORK                                */
/************************************************************/
class my_Delay {
  public:
    struct my_registers *r;
  	my_Delay(struct my_registers *r_in) {
      r = r_in;
    };
  	~my_Delay() {};
  	void set(int val) {
  		r->DT = val;
  		//Start timer to count down at 60Hz
  	}
  	int get() {
  		return r->DT;
  	}
};

/************************************************************/
/*           THIS NEEDS WORK                                */
/************************************************************/
class my_Sound {
  public:
    struct my_registers *r;
	  my_Sound (struct my_registers *r_in) {
		    r = r_in;
	  }
	  ~my_Sound () {};
  	void set(int val) {
  		r->ST = val;
  		//Start timer to count down at 60Hz
  		//If ST > 0 sound buzzer
  	}
};
