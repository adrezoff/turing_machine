#include <unistd.h>

#include "tapeController.h"
#include "utils.h"


TapeNode *currentNode = NULL;
TapeNode *leftmostNode = NULL;
TapeNode *rightmostNode = NULL;

// Создание нового узла ленты
static TapeNode* createNode(int index, char symbol) {
    TapeNode *newNode = (TapeNode *)my_malloc(sizeof(TapeNode));
    if (!newNode) {
        return NULL; 
    }
    newNode->index = index;
    newNode->symbol = symbol;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

// Инициализация ленты: создается центральный узел с индексом 1
int initializeTape() {
    currentNode = createNode(1, '_');
    if (!currentNode) {
        return -100; 
    }
    leftmostNode = currentNode;
    rightmostNode = currentNode;
    return 0; 
}

// Запись символа в текущую ячейку ленты
int writeSymbol(char symbol) {
    if (!currentNode) {
        return -101; 
    }

    currentNode->symbol = symbol;

    // Обновление границ ленты
    if (symbol != '_') {
        if (currentNode->index < leftmostNode->index) {
            leftmostNode = currentNode;
        }
        if (currentNode->index > rightmostNode->index) {
            rightmostNode = currentNode;
        }
    }
    return 0; 
}

// Перемещение головки в указанном направлении
int moveHead(char direction) {
    if (!currentNode) {
        return -102;
    }

    switch (direction) {
        case '<': 
            if (!currentNode->prev) {
                // Создаем новый узел слева
                currentNode->prev = createNode(currentNode->index - 1, '_');
                if (!currentNode->prev) {
                    return -103; 
                }
                currentNode->prev->next = currentNode;
            }
            currentNode = currentNode->prev;
            break;

        case '>':
            if (!currentNode->next) {
                // Создаем новый узел справа
                currentNode->next = createNode(currentNode->index + 1, '_');
                if (!currentNode->next) {
                    return -104; 
                }
                currentNode->next->prev = currentNode;
            }
            currentNode = currentNode->next;
            break;

        case '.':
            break;

        default:
            return -105;
    }
    return 0;
}

// Удаление пустых символов (_), начиная с правого конца
int cleanRightSide() {
    if (!rightmostNode) {
        return -106; 
    }

    TapeNode *temp = rightmostNode;
    while (temp && temp->symbol == '_') {
        TapeNode *toDelete = temp;
        temp = temp->prev;

        if (toDelete != leftmostNode) {
            my_free(toDelete);
        }
    }

    if (temp && temp->symbol != '_') {
        rightmostNode = temp;
    } else {
        rightmostNode = NULL;
    }
    return 0;
}

// Удаление пустых символов (_), начиная с левого конца
int cleanLeftSide() {
    if (!leftmostNode) {
        return -107; 
    }

    TapeNode *temp = leftmostNode;
    while (temp && temp->symbol == '_') {
        TapeNode *toDelete = temp;
        temp = temp->next;

        if (toDelete != rightmostNode) {
            my_free(toDelete);
        }
    }

    if (temp && temp->symbol != '_') {
        leftmostNode = temp;
    } else {
        leftmostNode = NULL;
    }
    return 0;
}

// Сброс головки в центральную позицию (индекс 1)
int resetHead() {
    if (!currentNode) {
        return -102; 
    }

    while (currentNode->index > 1) {
        moveHead('<');
    }
    while (currentNode->index < 1) {
        moveHead('>');
    }
    return 0; 
}

// Печать ленты
int printTape() {
    if (!currentNode) {
        return -108; 
    }

    cleanLeftSide();
    cleanRightSide();

    TapeNode *temp = leftmostNode;
    while (temp) {
        write(1, &temp->symbol, 1);
        if (temp == rightmostNode) break;
        temp = temp->next;
    }
    
	write(1, "\n", 1);
	
    return 0;
}
