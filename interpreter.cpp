#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stack>

#define MAX_MEMSIZE 30000

unsigned char memory[MAX_MEMSIZE] = {0};
char instruction_memory[MAX_MEMSIZE] = {0};

int main(int argc, char* argv[])
{
  if(argc < 2) {
    std::cout << "Please provide bf input (./interpret filename.bf)" << std::endl;
    return 0;
  }
  std::ifstream in(argv[1]);
  if(!in.is_open()) {
    std::cout << "Failed to open file: " << argv[1] << std::endl;
    return 0;
  }

  char c = 0;
  unsigned int instructionPointer = 0;
  while(!in.eof()) {
    in.get(c);
    if(c=='<'||c=='>'||c==','||c=='.'||c=='['||c==']'||c=='-'||c=='+') {
      instruction_memory[instructionPointer] = c;
      ++instructionPointer;
    }
  }
  unsigned int instrCnt = instructionPointer;
  instructionPointer = 0;
  unsigned int memPointer = 0;
  std::stack<unsigned int> loops;

  for(int i = 0; i < instrCnt; ++i) {
    std::cout << instruction_memory[i];
  }
  std::cout << '\n';
  while(instructionPointer < instrCnt) {
    // std::cout << "Executing " << instruction_memory[instructionPointer] << '\n'
    //           << "Mem address " << memPointer << '\n'
    //           << "mem value " << (int)memory[memPointer] << '\n' << '\n';
    switch(instruction_memory[instructionPointer]) {
      case '<':
        memPointer = (memPointer == 0 ? 0 : memPointer-1);
        break;
      case '>':
        memPointer = (memPointer == MAX_MEMSIZE-1 ? MAX_MEMSIZE-1 : memPointer+1);
        break;
      case '+':
        ++memory[memPointer];
        break;
      case '-':
        --memory[memPointer];
        break;
      case ',':
        memory[memPointer] = getchar();
        break;
      case '.':
        std::cout << memory[memPointer];
        break;
      case '[':
        if(memory[memPointer] != 0) {
          loops.push(instructionPointer);
        } else {
          int br = 0;
          int i = instructionPointer+1;
          while(i < instrCnt) {
            if(instruction_memory[i] == '[') {
              ++br;
            } else {
              if(br-- == 0) {
                instructionPointer = i+1;
                break;
              }
            }
            ++i;
          }
        }
        break;
      case ']':
        if(memory[memPointer] != 0) {
          instructionPointer = loops.top()-1;
          loops.pop();
        }
        break;
    }
    ++instructionPointer;
  }
  
  std::cout << std::endl;
  in.close();
  return 0;
}
