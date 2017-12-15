#include "sudoku.hpp"

Sudoku::Sudoku()
{
    //По индексу каждой клетки таблицы ставим ей в соответствие номер строки, столбца и блока, в которых она стоит
    //square - индекс клетки
    int square = 0;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
        {
            square = 9 * i + j;
            inRow[square] = i;
            inCol[square] = j;
            inBlock[square] = (i / 3) * 3 + ( j / 3);
        }
    
    //Записываем в sequence индексы всех клеток, обнуляем entry
    for (int square = 0; square < 81; square++)
    {
        sequence[square] = square;
        entry[square] = 0;
    }
    
    //Ставим в соответствие каждой ячейке "бинарный вектор" кандидатов в нее
    for (int i = 0; i < 9; i++)
    {
        block[i] = initVec;
        row[i] = initVec;
        col[i] = initVec;
    }
}

Sudoku::~Sudoku()
{}

void Sudoku::swap(int s1, int s2)
{
    int tmp = sequence[s2];
    sequence[s2] = sequence[s1];
    sequence[s1] = tmp;
}


//val  - значение, записанное в клетке, i и j - ее индексы;
bool Sudoku::init_entry(int i, int j, int val)
{
    int square = 9 * i + j; //индекс текущей клетки
    //!!!!!!!!побитовый сдвиг 1 на val позиций
    //valbit - это значение val "звкодированое" в битовый вектор
    int valbit = 1 << val;
    int seqPtr2;
    
    //в entry храним числа, сдвинутые влево на val позиций
    entry[square] = valbit; //5 кодируется в 32 (0...0101)
    //в векторах массивов row/block/col в valbit - бите будет 0, а остальные биты i-го блока будут равны 1, что означает, что в блоке занята определенная цифра, а
    //а остальные свободны; 0 в векторе означает занятую цифру
    int tmp_b = block[inBlock[square]], tmp_c = col[inCol[square]], tmp_r = row[inRow[square]];
    block[inBlock[square]] &= ~valbit;
    col[inCol[square]] &= ~valbit;
    row[inRow[square]] &= ~valbit;
    //если за проход не произошло изменений, значит мы добавили уже существующую цифру => цифры в ряду/столбце/блоке исходной таблицы повторются => входное условие некорректно
    if (tmp_b == block[inBlock[square]] || tmp_c == col[inCol[square]] || tmp_r == row[inRow[square]])
        return false;
    
    seqPtr2 = seqPtr; //так мы экономим такты работы, чтобы не перебирать все сначала; seqPtr означает кол-во данных по условию чисел (чисел в таблице в данный момент - при инициализации)
    while (seqPtr2 < 81 && sequence[seqPtr2] != square)   //seqPtr2=square - нельзя, т к в sequenсe индексы расположены не попроядку - из-за для удобного хранения индексов заполненных ячеек в начале массива sequence
        seqPtr2++ ;
    
    swap(seqPtr, seqPtr2); //seqPtr2 указывает на индекс новой добавленной ячейки; seqPtr - число заполненных ячеек, т о в sequence seqPtr указывает на индекс первой попавшейся незаполненной ячейки
    seqPtr++; //сдвигаем "указатель"
    return true;
}

void Sudoku::print_array_to_str()
{
    int valbit, square;
    char ch;
    square = 0;
    fileOut = "";
    for (int i = 0; i < 9; i++)
    {
        if (i % 3 == 0)
            fileOut += '\n';  //переход на новую строку i+1 = номер строки
        for (int j = 0; j < 9; j++)
        {
            if (j % 3 == 0)
                fileOut += ' ';  //отступы-разделители между тройками цифр
            valbit = entry[square++];
            if (valbit == 0)
                ch = '-';
            else
            {
                for (int val = 1; val <= 9; val++)
                    if (valbit == (1 << val))
                    {
                        ch = '0' + val;
                        break;
                    }
            }
            fileOut += ch;
        }
        
        fileOut += '\n'; //после того, как вывели 9 символов, переходим на новую строку
        
    }
}


//Функция для ввода данных
bool Sudoku::enter_data(std::string fileIn)
{
    bool mistakeInInput = false;
    if (fileIn.size() > 81)
    {
        fileOut+="Недопустимый размер Судоку! Превышение допустимого размера таблицы.";
        return false;
    }
    if (fileIn.size() < 81)
    {
        fileOut+="Недопустимый размер Судоку! Таблица слишком маленькая.";
        return false;
    }
    

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            char ch = fileIn[9*i+j];
            if (ch >= '1' && ch <= '9')
            {
                if (init_entry(i, j, ch - '0'))
                {}
                else
                {
                    mistakeInInput = true;
                    break;
                }
            }
        }
    }
    
    if (mistakeInInput)
    {
        fileOut+="Ошибка во входном файле! Данная таблица не соответствует правилам Судоку.";
        return false;
    }
    //если таблица была изначально пустая, нам незачем "прогонять" ее через функцию заполнения. При этом мы экономим на вызовах place()
    if (seqPtr == 0)
    {
        //fileOut+="\n --- --- ---\n --- --- ---\n --- --- ---\n --- --- ---\n --- --- ---\n --- --- ---\n --- --- ---\n --- --- ---\n --- --- ---\n\n";
        fileOut+="\n 123 456 789\n 456 789 123\n 789 123 456\n\n 261 934 875\n 834 517 692\n 597 268 314\n\n 312 675 948\n 645 891 237\n 978 342 561\n\n";
        return false;
    }
    
    //print_array_to_str();  //печатаем данную по условию таблицу
    
    //если таблица была изначально полная, нам незачем "прогонять" ее через функцию заполнения. Экономим на вызове place().
    if (seqPtr == 81)
    {
        fileOut+="Исходная таблица заполнена!";
        return false;
    }
        
    return true;

}


//S к концу выполнения функции не меняется
//возвращает S2: min индекс ячейки в sequence с минимально возможным числом вариантов заполнения
int Sudoku::next_seq(int s)
{
    int s2 = 0, square = 0, possibles = 0, bitCount; //S2 = 0
    int minBitCount = 100;     //minBitCount - наименьшее кол-во кандидатов в клетку
    
    for (int k = s; k < 81; k++)
    {//начиная с k-го индекса в sequence хранятся индексы всех незаполненных ячеек; все заполненные ячейки находятся в начале sequence
        square = sequence[k]; //в sequence все незаполненные индексы. В square зап-ли индкс первой попавшейся незаполненной клетки - в ой итерации цикла
        //определяем, вектор, который задает цифры, которыми можно заполнить ячейку
        //По индексам едениц в векторе posibles определяем цифры-кандицаты в ячейку
        possibles = block[inBlock[square]] & row[inRow[square]] & col[inCol[square]];
        bitCount = 0;
        //Если possibles == 0, то у нас нет кандидатов для ячейки
        //Считает число едениц в векторе possibles, то есть определяет кол-во кандидатов в ячейку, и зап-ет рез-т в bitCount
        while (possibles)
        {
            possibles &= ~(possibles & -possibles);
            bitCount++;
        }
        
        if (bitCount == 1)
        {
            s2 = k;
            break;
        }
        if (bitCount < minBitCount)
        {
            minBitCount = bitCount;
            //в S2 храним индекс ячейки с минимально возможным числом вариантов заполнения
            s2 = k;
        }
    }
    
    return s2;
}


void Sudoku::place(int s)
{
    //таблица заполнена, задача решена
    if (s >= 81)
    {
        print_array_to_str();
        return;
    }
    
    int s2 = next_seq(s); //S2 - индекс ячейки в sequence с минимально возможным числом вариантов заполнения!!!
    swap(s, s2); //Делаем свап: кидаем индекс вершины, с кот будем работать в массив индексов с заполненными ячейками
    
    
    int square = sequence[s]; //берем индекс клетки, с кот будем работать, а именно с той, у которой минимальное число вариантов заполнения (первой попавшейся из sequence среди таких), т е сюда войдут и случаи с единственно возможным кандидатом в ячейку
    //получаем номер блока/строки/столбца ячейки
    int blockIndex = inBlock[square],
    rowIndex = inRow[square],
    colIndex = inCol[square];
    
    int possibles = block[blockIndex] & row[rowIndex] & col[colIndex]; //получаем возможные варианты заполнения ячейки
    //работаем, пока есть возможность выбора цифры для ячеки: при успехе рекурсивно вызываем place()
    while (possibles)
    { //если Possibles становится == 0 до входа в цикл, то вызова place(S+1) не будет, будет выполняться "обратный свап" до тех пор, пока possibles!=0 //wiki (possibles&(~possibles+1))
        int valbit = (possibles&(~possibles+1)); // Берем самый "маленький" кандидат в ячейку -  младший 1 бит в Possibles //~(possibles-1)&possibles//
        possibles &= ~valbit; //убираем из вектора ячейки взятую цифру
        entry[square] = valbit; //записываем взятую цифру в ячейку судоку
        block[blockIndex] &= ~valbit; //записываем в блок взятую цифру, то есть удаляем ее из блока
        row[rowIndex] &= ~valbit;
        col[colIndex] &= ~valbit;
        
        place(s + 1); //рекурсивно вызываем для следующей незаполненной ячейки, увеличиваем s
        
        //"откат" изменений
        entry[square] = 0;
        block[blockIndex] |= valbit; //побитовое ИЛИ отменяет действие побитового И, возвращаем вектор в исходное состояние
        row[rowIndex] |= valbit;
        col[colIndex] |= valbit;
    }
    swap(s, s2);
    //
}


int Sudoku::seq_ptr()
{
    return seqPtr;
}

std::string Sudoku::get_data()
{
    return fileOut;
}
