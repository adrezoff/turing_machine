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

// Удаление узла по ссылке
void deleteNode(TapeNode **nodeToDelete) {
    if (!nodeToDelete || !(*nodeToDelete)) {
        return;
    }

    TapeNode *node = *nodeToDelete;

    // Если узел был первым в ленте
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        leftmostNode = node->next;
    }

    // Если узел был последним в ленте
    if (node->next) {
        node->next->prev = node->prev;
    } else {
        rightmostNode = node->prev;
    }

    my_free(node);
    *nodeToDelete = NULL;

}

// Инициализация ленты
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
    return 0;
}

// Перемещение головки
int moveHead(char direction) {
    if (!currentNode) {
        return -102;
    }


    switch (direction) {
        case '<': {
            if (!currentNode->prev) {
                TapeNode *newNode = createNode(currentNode->index - 1, '_');
                if (!newNode) {
                    return -103;
                }
                newNode->next = currentNode;
                currentNode->prev = newNode;
                leftmostNode = newNode;

                if (currentNode->symbol == '_') {
					TapeNode *temp = currentNode;
					currentNode = currentNode->prev;
					deleteNode(&temp);
				} else {
					currentNode = currentNode->prev;
				}
            }

            else if (currentNode->index - currentNode->prev->index == 1) {
				if (currentNode->symbol == '_') {
					TapeNode *temp = currentNode;
					currentNode = currentNode->prev;
					deleteNode(&temp);
				} else {
					currentNode = currentNode->prev;
				}
			}
			else {
				TapeNode *newNode = createNode(currentNode->index - 1, '_');
				if (!newNode) {
					return -104;
				}
				newNode->next = currentNode;
				newNode->prev = currentNode->prev;
                currentNode->prev = newNode;

                if (currentNode->symbol == '_') {
					TapeNode *temp = currentNode;
					currentNode = currentNode->prev;
					deleteNode(&temp);
				} else {
					currentNode = currentNode->prev;
				}
			}

            break;
        }

        case '>': {
            if (!currentNode->next) {
                TapeNode *newNode = createNode(currentNode->index + 1, '_');
                if (!newNode) {
                    return -104;
                }
                newNode->prev = currentNode;
                currentNode->next = newNode;
                rightmostNode = newNode;

                if (currentNode->symbol == '_') {
					TapeNode *temp = currentNode;
					currentNode = currentNode->next;
					deleteNode(&temp);
				} else {
					currentNode = currentNode->next;
				}
            }

			else if (currentNode->next->index - currentNode->index == 1) {
				if (currentNode->symbol == '_') {
					TapeNode *temp = currentNode;
					currentNode = currentNode->next;
					deleteNode(&temp);
				} else {
					currentNode = currentNode->next;
				}
			}

			else {
				TapeNode *newNode = createNode(currentNode->index + 1, '_');
				if (!newNode) {
					return -104;
				}
				newNode->prev = currentNode;
				newNode->next = currentNode->next;
                currentNode->next = newNode;
                if (currentNode->symbol == '_') {
					TapeNode *temp = currentNode;
					currentNode = currentNode->next;
					deleteNode(&temp);
				} else {
					currentNode = currentNode->next;
				}

			}
            break;
        }

        case '.': {
            break;
        }

        default:
            return -105;
    }
    return 0;
}

// Сброс головки
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
    if (!leftmostNode || !rightmostNode) {
        return -108;
    }

    TapeNode *temp = leftmostNode;
    int expectedIndex = leftmostNode->index;

    while (temp) {
        while (expectedIndex < temp->index) {
            write(1, "_", 1);
            expectedIndex++;
        }
        write(1, &temp->symbol, 1);
        expectedIndex++;
        temp = temp->next;
    }

    write(1, "\n", 1);
    return 0;
}
