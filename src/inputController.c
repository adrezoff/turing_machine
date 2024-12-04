#include <fcntl.h>
#include <unistd.h>

#include "inputController.h"
#include "transitionController.h"
#include "tapeController.h"
#include "utils.h"


// Функция для парсинга файла машины Тьюринга
int parseMachineFile(const char *filename, int bufferSize) {
	int errorCode = 0;
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        return -2;
    }

    char fileReadBuffer[bufferSize];
    char dataBuffer[3 * bufferSize];
    my_memset(dataBuffer, 0, sizeof(dataBuffer));

    int bytesRead = 0, fileIndex = 0, dataIndex = 0;
    int stateCount = 0, transitionCount = 0;
    int currentLine = 0; // Состояние парсинга (номер текущей строки в файле)

    while ((bytesRead = read(fd, fileReadBuffer, bufferSize)) > 0) {
        fileIndex = 0;

        while (fileIndex < bytesRead) {
            while (fileIndex < bytesRead && fileReadBuffer[fileIndex] != '\n') {
                dataBuffer[dataIndex++] = fileReadBuffer[fileIndex++];
            }

            if (fileIndex < bytesRead && fileReadBuffer[fileIndex] == '\n') {
                fileIndex++; // Пропускаем \n
                dataBuffer[dataIndex] = '\0';

                if (currentLine == 0) {
                    // Парсим количество состояний
                    stateCount = strToInt(dataBuffer);
                } else if (currentLine == 1) {
                    // Парсим состояния
                    int stateIndex = 0;
                    char state[50];
                    for (int i = 0; dataBuffer[i] != '\0'; i++) {
                        if (dataBuffer[i] == ' ') {
                            state[stateIndex] = '\0';
							errorCode = addState(state);
							if (errorCode < 0 ) { 
								close(fd);
								return errorCode;
							}
							stateCount --;
							if (stateCount < 0 ) { 
								close(fd);
								return -3;
							}
                            stateIndex = 0;
                        } else {
                            state[stateIndex++] = dataBuffer[i];
                        }
                    }
                    if (stateIndex > 0) {
                        state[stateIndex] = '\0';
						errorCode = addState(state);
						if (errorCode < 0 ) { 
							close(fd);
							return errorCode;
						}
                    }
                } else if (currentLine == 2) {
                    // Парсим количество переходов
                    transitionCount = strToInt(dataBuffer);
                } else {
                    // Парсим переход
                    char startState[50], endState[50];
                    char symbol = '\0', newSymbol = '\0', direction = '\0';
                    int i = 0, j = 0;

                    // Считываем стартовое состояние
                    if (dataBuffer[i] == '(') i++;
                    while (dataBuffer[i] != ',' && dataBuffer[i] != '\0') {
                        startState[j++] = dataBuffer[i++];
                    }
                    startState[j] = '\0';
                    i += 2; // Пропускаем ', '

                    // Считываем символ
                    symbol = dataBuffer[i++];
                    i += 6; // Пропускаем ") -> ("

                    // Считываем конечное состояние
                    j = 0;
                    while (dataBuffer[i] != ',' && dataBuffer[i] != '\0') {
                        endState[j++] = dataBuffer[i++];
                    }
                    endState[j] = '\0';
                    i += 2; // Пропускаем ', '

                    // Считываем новый символ
                    newSymbol = dataBuffer[i++];
                    i += 2; // Пропускаем ", "

                    // Считываем направление
                    direction = dataBuffer[i];

                    // Добавляем переход
					errorCode = addTransition(startState, symbol, endState, newSymbol, direction);
					transitionCount--;
					if (transitionCount < 0 ) { 
						close(fd);
						return -7;
					}
					if (errorCode < 0 ) { 
						close(fd);
						return errorCode;
					}
                }

                // Переходим к следующей строке
                dataIndex = 0;
                currentLine++;
            }
        }
    }

    if (bytesRead < 0) {
        close(fd);
        return -4; 
    }

    close(fd);
    return 0; 
}

// Функция для парсинга начального состояния ленты
int parseTapeFile(const char *filename, int bufferSize) {
	int errorCode = 0;
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        return -5; 
    }
    
    char buffer[bufferSize];
    int bytesRead;

    errorCode = initializeTape();
    if (errorCode < 0 ) {
		close(fd);
		return errorCode;
	}

    // Чтение файла и обработка данных
    while ((bytesRead = read(fd, buffer, bufferSize)) > 0) {
        int index = 0;
        while (index < bytesRead) {
            if (buffer[index] == '\n') {
                index++;
                continue;
            }
            errorCode = writeSymbol(buffer[index]);
			if (errorCode < 0 ) { 
				close(fd);
				return errorCode;
			}
			errorCode = moveHead('>');
			if (errorCode < 0 ) { 
				close(fd);
				return errorCode;
			}
            index++;
        }
    }

    if (bytesRead < 0) {
        close(fd);
        return -6;
    }

    close(fd);
    
	errorCode = resetHead();
	if (errorCode < 0 ) { 
		close(fd);
		return errorCode;
	}

    return 0; 
}
