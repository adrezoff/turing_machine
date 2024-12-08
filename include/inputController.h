#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

// Получение пути до файла
int readFileName(char **fileName, int bufferSize);

// Функция для парсинга файла машины Тьюринга
int parseMachineFile(int bufferSize); 

// Функция для парсинга файла ленты
int parseTapeInput(int bufferSize);

#endif
