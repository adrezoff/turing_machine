#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

// Функция для парсинга файла машины Тьюринга
int parseMachineFile(const char *filename, int bufferSize); 

// Функция для парсинга файла ленты
int parseTapeFile(const char *filename, int bufferSize);

#endif
