#include <unistd.h>

#include "inputController.h"
#include "transitionController.h"
#include "tapeController.h"
#include "utils.h"

#define BUFFER_SIZE 1024


int run() {
    int currentStateIndex = startIndex;  // Начальное состояние
    int errorCode = 0;
    int i = 0;

    while (currentStateIndex != stopIndex) {
        // Получаем переход по текущему состоянию и символу
        struct nodeTransition transition = getTransition(currentStateIndex, currentNode->symbol);
        if (transition.index_trans == -1){
			return -205;
		}

        errorCode = writeSymbol(transition.symbol);
		if (errorCode < 0 ) { 
			return errorCode;
		}
        errorCode = moveHead(transition.direction);
		if (errorCode < 0 ) { 
			return errorCode;
		}
        currentStateIndex = transition.index_trans;
		if (i >= 100000) {
			errorCode = -206;
			printTape(); 
			return errorCode;
		}
        i++;
        
    }
    return 0; 
}


// Основная точка входа
int main() {
	int errorCode = 0;
    
    errorCode = parseMachineFile(BUFFER_SIZE);
	if (errorCode < 0 ) { 
		printErrorCode(errorCode);
		return 0;
	}

    errorCode = parseTapeInput(BUFFER_SIZE);
	if (errorCode < 0 ) { 
		printErrorCode(errorCode);
		return 0;
	}

    errorCode = run();
    printTape(); 
    printErrorCode(errorCode);
    return 0;
    
}
