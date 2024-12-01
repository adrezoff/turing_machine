#ifndef UTILS_H
#define UTILS_H

int strToInt(const char *str);
char* intToStr(int num);
void printErrorCode(int returnCode);

// Функции для работы со строками
void my_strcpy(char *dest, const char *src);
int my_strcmp(const char *str1, const char *str2);
int my_strlen(const char *str);
void my_memset(char *buf, int val, int size);

// Функции для работы с памятью
void* my_malloc(int size);
void* my_free(void* ptr);

#endif
