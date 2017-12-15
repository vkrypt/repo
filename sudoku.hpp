#ifndef sudoku_hpp
#define sudoku_hpp

#include <cstdio>
#include <string>
#include "stdio.h"


const int initVec = 0x3fe;    //БИНАРНЫЙ ВЕКТОР 1111111110: еденицы соответствуют свободным числам-кандидатам в ячейку от "1" до "9"

class Sudoku
{
private:
    //хранят номер блока/строки/столбца, в котором находится клетка с заданным индексом
    int inBlock[81], inRow[81], inCol[81];
    
    //в entry храним числа таблицы, сдвинутые влево на val позиций
    int entry[81];
    int block[9], row[9], col[9];    //Массивы векторов-кандидатов в соотв блок/строку/столбец
    
    //кол-во данных по условию чисел
    int seqPtr = 0;
    //В начало массива sequence будем записывать индексы заполненных клеток. Если по условию заполнено 17 клеток,
    //то, начиная с sequence[17] будем получать индексы незаполненных клеток (не обязателно упорядоченные)
    int sequence[81]; //Это последовательность индексов, определяющих порядок заполнения ячеек
    std::string fileOut = "";
    
public:
    
    Sudoku();
    ~Sudoku();
    
    void swap(int s1, int s2);
    //val  - значение, записанное в клетке, i и j - ее индексы
    bool init_entry(int i, int j, int val);
    void print_array_to_str();
    //Функция для ввода данных
    bool enter_data(std::string file_in);
    int next_seq(int s);
    void place(int s);
    //возвращает кол-во данных по условию чисел
    int seq_ptr();
    std::string get_data();    
};

#endif /* sudoku_hpp */
