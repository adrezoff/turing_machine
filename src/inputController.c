#include <fcntl.h>
#include <unistd.h>

#include "inputController.h"
#include "transitionController.h"
#include "tapeController.h"
#include "utils.h"


// Функция получения пути к файлу через стандартынй ввод
int readFileName(char **fileName, int bufferSize) {
    *fileName = (char *)my_malloc(bufferSize);
    if (*fileName == NULL) {
        return -1;
    }

    int index = 0;
    char ch;

    // Чтение символов из стандартного ввода
    while (read(STDIN_FILENO, &ch, 1) > 0) {
        if (ch == '\n') {
            break;
        }

        // Если буфер заканчивается, увеличиваем его размер
        if (index >= bufferSize - 1) {
            bufferSize *= 2;  // Удваиваем размер буфера
            char *newBuffer = (char *)my_malloc(bufferSize);
            if (newBuffer == NULL) {
                my_free(*fileName); 
                return -8; 
            }
            my_memset(newBuffer, 0, bufferSize);  
            my_strcpy(newBuffer, *fileName);  
            my_free(*fileName); 
            *fileName = newBuffer;
        }

        (*fileName)[index++] = ch; 
    }

    if (index == 0) {
        my_free(*fileName);
        return -5;  
    }

    (*fileName)[index] = '\0';
    return 0; 
}

// Функция для парсинга файла машины
int parseMachineFile(int bufferSize) {
    int errorCode = 0;
    char *filename = 0;
    
    write(1, "INPUT PATH MACHINE FILE:\n", 25);
    
    errorCode = readFileName(&filename, bufferSize);
    if (errorCode < 0) {
		return errorCode;
	}

    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        return -2;
    }

    // Выделяем память для буферов через my_malloc
    char *fileReadBuffer = (char *)my_malloc(bufferSize);
    char *dataBuffer = (char *)my_malloc(bufferSize);
    unsigned int dataBufferSize = bufferSize;

    if (!fileReadBuffer || !dataBuffer) {
        if (fileReadBuffer) my_free(fileReadBuffer);
        if (dataBuffer) my_free(dataBuffer);
        close(fd);
        return -8;
    }

    my_memset(dataBuffer, 0, dataBufferSize);

    int bytesRead = 0, fileIndex = 0, dataIndex = 0;
    int stateCount = 0, transitionCount = 0;
    int currentLine = 0; // Состояние парсинга (номер текущей строки в файле)
	initializeTransitionTable();
	
    while ((bytesRead = read(fd, fileReadBuffer, bufferSize)) > 0) {
        fileIndex = 0;

        while (fileIndex < bytesRead) {
            while (fileIndex < bytesRead && fileReadBuffer[fileIndex] != '\n') {
                if (dataIndex >= dataBufferSize - 1) {
                    // Расширяем dataBuffer с помощью my_malloc и my_free
                    unsigned int newBufferSize = dataBufferSize * 2;
                    char *newBuffer = (char *)my_malloc(newBufferSize);
                    if (!newBuffer) {
                        my_free(fileReadBuffer);
                        my_free(dataBuffer);
                        close(fd);
                        return -9;
                    }
                    my_memset(newBuffer, 0, newBufferSize);
                    my_strcpy(newBuffer, dataBuffer);
                    my_free(dataBuffer);
                    dataBuffer = newBuffer;
                    dataBufferSize = newBufferSize;
                }
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
                            if (errorCode < 0) {
                                my_free(fileReadBuffer);
                                my_free(dataBuffer);
                                close(fd);
                                return errorCode;
                            }
                            stateCount--;
                            if (stateCount < 0) {
                                my_free(fileReadBuffer);
                                my_free(dataBuffer);
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
                        if (errorCode < 0) {
                            my_free(fileReadBuffer);
                            my_free(dataBuffer);
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
                    if (transitionCount < 0) {
                        my_free(fileReadBuffer);
                        my_free(dataBuffer);
                        close(fd);
                        return -7;
                    }
                    if (errorCode < 0) {
                        my_free(fileReadBuffer);
                        my_free(dataBuffer);
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
        my_free(fileReadBuffer);
        my_free(dataBuffer);
        close(fd);
        return -4;
    }

    // Освобождаем память
    my_free(fileReadBuffer);
    my_free(dataBuffer);
    close(fd);
    return 0;
}


// Функция для парсинга ленты
int parseTapeInput(int bufferSize) {
    int errorCode = 0;

    // Используем стандартный ввод (stdin)
    char buffer[bufferSize];
    int bytesRead;

    errorCode = initializeTape();
    if (errorCode < 0) {
        return errorCode;
    }
    
    write(1, "INPUT TAPE:\n", 12);

    // Чтение данных из стандартного ввода
    while ((bytesRead = read(STDIN_FILENO, buffer, 1)) > 0) {
        if (buffer[0] == '\n') {
            break;  
        }

        errorCode = writeSymbol(buffer[0]);
        if (errorCode < 0) {
            return errorCode;
        }

        errorCode = moveHead('>');
        if (errorCode < 0) {
            return errorCode;
        }
    }

    if (bytesRead < 0) {
        return -6;  
    }

    errorCode = resetHead();
    if (errorCode < 0) {
        return errorCode;
    }

    return 0;
}
