#pragma once
#include "MyString.h" 

class Line {
private:
    Mystring text;

public:

    Line();
    Line(const char* str);
    Line(const Mystring& str);


    int getLength() const;
    Mystring getText() const;
    void print() const;

    void insertChar(int index, char c);   
    void removeChar(int index);         
    void deleteFrom(int index);         
    void toggleCase(int index);          
};