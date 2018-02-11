Chip8Emulator-TimBrannan

Chip-8 Emulator
Create a cpu emulator for the Chip-8 processor. The Chip-8 is a processor from
the 70's that is an excellent introduction to hardware emulation as it has a
limited instruction set and is very well documented. Writing the emulator from
scratch will give you some very good exposure to the type of software we deal
with. Here is a decent sequence of events...

Research Chip8; gather data sheets / instruction set info; gather some sample
Chip8 ROMs

Create a disassembler and make sure it produces sensible output for a known good
ROM

Emulate the CPU (i.e. handle sequences of opcodes with your virtual processor)

Emulate the peripheral devices like user input, sound / video output (This part
is less important, so feel free to cheat on this part from existing code on the
internet)

Requirements:
The emulator shall be able of disassembling a Chip-8 ROM and producing source
assembly listings.

The emulator shall be capable of executing a Chip-8 ROM without crashing.

The emulator shall be able to accept user input via keyboard and produce a
 graphical display.

Notes:
Feel free to reference online resources for learning the fundamentals of
 emulation, but remember this exercise is to sharpen your skills so don't cheat.
  A good place to start is http://emulator101.com/.

Check your code in frequently. This allows me to see your progression and leave
 you notes along the way.

Feel free to ask me questions. I want to see you succeed and will be happy to
 help.

Evaluate whether this type of coding is fun to you. While we do not operate
 against Chip-8 (yet!), this class of work will be similar to the work you could
  be asked to do. Use this exercise as an opportunity to see if you enjoy this
   type of coding.
