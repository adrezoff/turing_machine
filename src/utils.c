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

void *global_base = NULL;

// Найти свободный блок подходящего размера
Block *findFreeBlock(Block **last, unsigned int size) {
    Block *current = global_base;
    while (current && !(current->free && current->size >= size)) {
        *last = current;
        current = current->next;
    }
    return current;
}

// Выделить новый блок
Block *requestSpace(Block *last, unsigned int size) {
    Block *block = sbrk(0); // Получить текущую брейк-точку
    void *request = sbrk(size + BLOCK_SIZE); // Сдвинуть брейк-точку
    if (request == (void*)-1) { // Проверить ошибку
        return NULL;
    }

    if (last) {
        last->next = block;
    }

    block->size = size;
    block->free = 0;
    block->next = NULL;
    return block;
}

// Выделение памяти
void *my_malloc(unsigned int size){
    if (size <= 0) {
        return NULL;
    }

    Block *block;
    if (!global_base) { // Если это первый вызов malloc
        block = requestSpace(NULL, size);
        if (!block) {
            return NULL;
        }
        global_base = block;
    } else {
        Block *last = global_base;
        block = findFreeBlock(&last, size);
        if (!block) { // Если свободного блока нет, выделяем новый
            block = requestSpace(last, size);
            if (!block) {
                return NULL;
            }
        } else { // Если блок найден, переиспользуем его
            block->free = 0;
        }
    }

    return (block + 1); // Вернуть указатель на память после заголовка
}

void my_free(void *ptr) {
    if (!ptr) {
        return; // Ничего не делаем, если указатель NULL
    }

    // Получаем заголовок блока
    Block *block = (Block*)ptr - 1;

    // Помечаем блок как свободный
    block->free = 1;

    // Попробуем объединить соседние свободные блоки
    Block *current = global_base;

    while (current) {
        if (current->free && current->next && current->next->free) {
            // Объединяем текущий блок с следующим
            current->size += BLOCK_SIZE + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}


