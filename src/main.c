#include <unistd.h>

#include "inputController.h"
#include "transitionController.h"
#include "tapeController.h"
#include "utils.h"

#define BUFFER_SIZE 4096


int run() {
    int currentStateIndex = startIndex;  // Начальное состояние
    char currentSymbol = '_';  // Начальный символ на ленте
    int transitionFound = 0;   // Флаг, чтобы отслеживать, были ли найдены переходы

    while (currentStateIndex != stopIndex) {
        // Получаем текущий символ
        if (currentNode) {
            currentSymbol = currentNode->symbol;
        }

        // Получаем переход по текущему состоянию и символу
        struct nodeTransition transition = getTransition(currentStateIndex, currentSymbol);

        if (transition.index_trans != -1) {
            // Выполняем переход
            writeSymbol(transition.symbol);
            moveHead(transition.direction);
            currentStateIndex = transition.index_trans;
            transitionFound = 1; 
        } else {
            transitionFound = 0;
            break;  
        }
    }

    // Возвращаем код ошибки, если переходов не было
    if (!transitionFound) {
        return -301; 
    }

    // Печать ленты только если был найден хотя бы один переход
    printTape(); 
    return 0; 
}


// Основная точка входа
int main(int argc, char *argv[]) {
	int errorCode = 0;
    if (argc != 3) {
		errorCode = -1;
		printErrorCode(errorCode);
		return 0;
    }
    errorCode = parseMachineFile(argv[1], BUFFER_SIZE);
	if (errorCode < 0 ) { 
		printErrorCode(errorCode);
		return 0;
	}

    errorCode = parseTapeFile(argv[2], BUFFER_SIZE);
	if (errorCode < 0 ) { 
		printErrorCode(errorCode);
		return 0;
	}

    errorCode = run();
    printErrorCode(errorCode);
    return 0;
    
}
