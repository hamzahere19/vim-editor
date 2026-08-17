#include "Line.h"
#include <iostream>
using namespace std;

Line::Line() : text("") {}

Line::Line(const char* str) : text(str) {}

Line::Line(const Mystring& str) : text(str) {}


int Line::getLength() const {
    return text.Length();
}

Mystring Line::getText() const {
    return text;
}

void Line::print() const {
    text.Print();
}

void Line::insertChar(int index, char c) {
    int len = text.Length();
    if (index < 0 || index > len)
        return;

    char* temp = new char[len + 2];

    for (int i = 0; i < index; i++) {
        temp[i] = text.CharacterAt(i);
    }
    temp[index] = c;
    for (int i = index; i < len; ++i) {
        temp[i + 1] = text.CharacterAt(i);
    }
    temp[len + 1] = '\0';

    text = Mystring(temp);
    delete[] temp;
}

void Line::removeChar(int index) {
    int len = text.Length();
    if (index < 0 || index >= len || len == 0)
        return;

    char* temp = new char[len];
    if (len == 1) {
        temp[0] = '\0';
    }
    else {
        int k = 0;
        for (int i = 0; i < len; i++) {
            if (i != index) {
                temp[k++] = text.CharacterAt(i);
            }
        }
        temp[k] = '\0';
    }

    text = Mystring(temp);
    delete[] temp;
}

void Line::deleteFrom(int index) {
    int len = text.Length();
    if (index < 0 || index >= len) return;

    char* temp = new char[index + 1];
    for (int i = 0; i < index; i++) {
        temp[i] = text.CharacterAt(i);
    }
    temp[index] = '\0';

    text = Mystring(temp);
    delete[] temp;
}

void Line::toggleCase(int index) {
    int len = text.Length();
    if (index < 0 || index >= len)
        return;

    char c = text.CharacterAt(index);
    if (c >= 'a' && c <= 'z') {
        text.ReplaceAt(index, c - 32);
    }
    else if (c >= 'A' && c <= 'Z') {
        text.ReplaceAt(index, c + 32);
    }
}