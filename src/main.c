#include <unistd.h>

#include "inputController.h"
#include "transitionController.h"
#include "tapeController.h"
#include "utils.h"

#define BUFFER_SIZE 4096


int run() {
    int currentStateIndex = startIndex;  // Начальное состояние
    int errorCode = 0;

    while (currentStateIndex != stopIndex) {
        // Получаем переход по текущему состоянию и символу
        struct nodeTransition transition = getTransition(currentStateIndex, currentNode->symbol);
        errorCode = writeSymbol(transition.symbol);
		if (errorCode < 0 ) { 
			return errorCode;
		}
        errorCode = moveHead(transition.direction);
		if (errorCode < 0 ) { 
			return errorCode;
		}
        currentStateIndex = transition.index_trans;
    }
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
