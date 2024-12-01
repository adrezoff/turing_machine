#include <unistd.h> 

#include "utils.h"


// Функция перевода из строки в число
int strToInt(const char *str) {
    int result = 0;
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}

// Функция перевода из числа в строку
char* intToStr(int num) {
    int isNegative = 0;
    if (num < 0) {
        isNegative = 1;
        num = -num; 
    }

    char* str = (char*)my_malloc(20 * sizeof(char));  
    if (str == NULL) {
        return NULL; 
    }

    int i = 0;
    do {
        str[i++] = (num % 10) + '0'; 
        num /= 10;
    } while (num > 0);

    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    for (int j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }

    return str;
}

// Функция печати кода возрата
void printErrorCode(int returnCode) {
    char* message = "DEBUG CODE: ";
    write(1, message, 12); 

    char* returnCodeStr = intToStr(returnCode);  
    if (returnCodeStr != NULL) {
        int len = 0;
        while (returnCodeStr[len] != '\0') {
            len++;
        }
        write(1, returnCodeStr, len); 
        my_free(returnCodeStr); 
    }

    write(1, "\n", 1); 
}

// Копирование строки
void my_strcpy(char *dest, const char *src) {
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
}

// Сравнение строк
int my_strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

// Вычисление длины строки
int my_strlen(const char *str) {
    const char *s = str;
    while (*s) s++;
    return s - str;
}

// Заполнение памяти
void my_memset(char *buf, int val, int size) {
    for (int i = 0; i < size; i++) {
        buf[i] = val;
    }
}

// Выделение памяти
void* my_malloc(int size) {
    return (void *)sbrk(size);
}

// Отчистка памяти
void* my_free(void* ptr) {
	if (ptr != NULL) {
        sbrk(-1 * sizeof(ptr)); 
    }
    return 0;
}

