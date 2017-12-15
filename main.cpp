#include "sudoku.hpp"
#include <iostream>
#include <fstream>

std::string read_from_file(char* argv[])
{
    std::string buf = "";
    std::string data = "";
    std::ifstream file_in(argv[1]);
    while (!file_in.eof())
    {
        file_in >> buf;
        data+=buf;
    }
    file_in.close();
    return data;
}

void push_to_file(char* argv[], std::string data)
{
    std::ofstream file_out(argv[2], std::ios::app);
    file_out << data;
    file_out.close();
}

int main(int argc, char* argv[])
{
    //argv[1]=="input.txt"
    //argv[2]=="output.txt"
    
    Sudoku table;
    if (argc == 3)
    {
        
        if (table.enter_data(read_from_file(argv)))
        {
            //push_to_file(argv, table.get_data());
            table.place(table.seq_ptr());
            push_to_file(argv, table.get_data());
        }
        else
            push_to_file(argv, table.get_data());
    }
    
    return 0;
}


