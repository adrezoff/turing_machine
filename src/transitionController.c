#include "transitionController.h"
#include "utils.h"


// Таблица переходов
struct nodeTransition transitionTable[TABLE_ROW][TABLE_COLUMN];

// Таблица отображения состояний
char stateTable[TABLE_ROW][50]; // Максимальная длина имени состояния — 50 символов

// Индексы начального и конечного состояний
int startIndex = -1;
int stopIndex = -1;

// Инициализация таблицы
void initializeTransitionTable() {
    for (int i = 0; i < TABLE_ROW; i++) {
        for (int j = 0; j < TABLE_COLUMN; j++) {
            transitionTable[i][j].index_trans = -1;
            transitionTable[i][j].symbol = '_';    
            transitionTable[i][j].direction = '.'; 
        }
    }
    my_memset((char *)stateTable, 0, TABLE_ROW * 50);
}

// Добавление состояния
int addState(const char *state) {
    for (int i = 0; i < TABLE_ROW; i++) {
        if (stateTable[i][0] == '\0') {
            my_strcpy(stateTable[i], state);
            if (my_strcmp(state, "start") == 0) startIndex = i;
            if (my_strcmp(state, "stop") == 0) stopIndex = i;
            return i;
        }
        if (my_strcmp(stateTable[i], state) == 0) {
            return i; 
        }
    }
    return -201; 
}

// Получение индекса состояния по его имени
int getStateIndex(const char *state) {
    for (int i = 0; i < TABLE_ROW; i++) {
        if (my_strcmp(stateTable[i], state) == 0) {
            return i; 
        }
    }
    return -202; 
}


// Добавление перехода
int addTransition(const char *currentState, char currentSymbol,
                  const char *newState, char newSymbol, char direction) {
    int currentStateIndex = getStateIndex(currentState);
    int newStateIndex = getStateIndex(newState);
    if (currentStateIndex == -1 || newStateIndex == -1) {
        return -203;
    }

    int symbolIndex = currentSymbol - ASCII_OFFSET;
    if (symbolIndex < 0 || symbolIndex >= TABLE_COLUMN) {
        return -204; 
    }

    transitionTable[currentStateIndex][symbolIndex].index_trans = newStateIndex;
    transitionTable[currentStateIndex][symbolIndex].symbol = newSymbol;
    transitionTable[currentStateIndex][symbolIndex].direction = direction;

    return 0; 
}

// Получение перехода
struct nodeTransition getTransition(int stateIndex, char currentSymbol) {
    int symbolIndex = currentSymbol - ASCII_OFFSET;
    if (symbolIndex < 0 || symbolIndex >= TABLE_COLUMN) {
        struct nodeTransition invalidTransition = {-1, '_', '.'};
        return invalidTransition;
    }
    return transitionTable[stateIndex][symbolIndex];
}
