#ifndef TRANSITION_CONTROLLER_H
#define TRANSITION_CONTROLLER_H

#define TABLE_ROW 256       // Максимальное количество состояний
#define TABLE_COLUMN 95     // Количество печатаемых символов ASCII
#define ASCII_OFFSET 32     // Смещение для кодов символов ASCII

struct nodeTransition {
    int index_trans;   // Индекс нового состояния
    char symbol;       // Новый символ
    char direction;    // Направление ('<', '.', '>')
};

extern char stateTable[TABLE_ROW][50]; // Максимальная длина имени состояния — 50 символов
extern int startIndex;
extern int stopIndex;

// Индексы начального и конечного состояний
extern int startIndex;
extern int stopIndex;

void initializeTransitionTable();
int addState(const char *state);
int addTransition(const char *currentState, char currentSymbol,
                  const char *newState, char newSymbol, char direction);
struct nodeTransition getTransition(int stateIndex, char currentSymbol);

#endif
