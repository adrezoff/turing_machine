#ifndef TAPE_CONTROLLER_H
#define TAPE_CONTROLLER_H

typedef struct TapeNode {
    int index;
    char symbol;
    struct TapeNode *next;
    struct TapeNode *prev;
} TapeNode;

// Указатели на текущую головку и центральный блок
extern TapeNode *currentNode;

int initializeTape();
int writeSymbol(char symbol);
int moveHead(char direction);
int cleanRightSide();
int cleanLeftSide(); 
int resetHead(); // вернуть currentNode в позицию с индексом 1 (перемещением головки)
int printTape();

#endif
