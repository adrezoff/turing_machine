#ifndef TAPE_CONTROLLER_H
#define TAPE_CONTROLLER_H

typedef struct TapeNode {
    int index;
    char symbol;
    struct TapeNode *next;
    struct TapeNode *prev;
} TapeNode;

// Указатели на текущую блок (головку)
extern TapeNode *currentNode;

int initializeTape();
int writeSymbol(char symbol);
int moveHead(char direction);
int cleanRightSide();
int cleanLeftSide(); 
int resetHead(); 
int printTape();

#endif
