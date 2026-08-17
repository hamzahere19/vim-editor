#include "Paragraph.h"
#include <iostream>
using namespace std;


Paragraph::Paragraph() {
    lines = nullptr;
    numLines = 0;
}


Paragraph::Paragraph(const Paragraph& other) {
    numLines = other.numLines;
    if (numLines > 0 && other.lines != nullptr) {
        lines = new Line[numLines];
        for (int i = 0; i < numLines; i++) {
            lines[i] = other.lines[i];
        }
    }
    else {
        lines = nullptr;
        numLines = 0;
    }
}


Paragraph& Paragraph::operator=(const Paragraph& other) {
    if (this != &other) {
        delete[] lines;

        numLines = other.numLines;
        if (numLines > 0 && other.lines != nullptr) {
            lines = new Line[numLines];
            for (int i = 0; i < numLines; i++) {
                lines[i] = other.lines[i];
            }
        }
        else {
            lines = nullptr;
            numLines = 0;
        }
    }
    return *this;
}


Paragraph::~Paragraph() {
    delete[] lines;
    lines = nullptr;
    numLines = 0;
}


void Paragraph::addLine(const Line& l) {
    Line* temp = new Line[numLines + 1];

    for (int i = 0; i < numLines; i++) {
        temp[i] = lines[i];
    }
    temp[numLines] = l;

    delete[] lines;
    lines = temp;
    numLines++;
}


void Paragraph::insertLineAt(int index, const Line& l) {
    if (index < 0 || index > numLines) return;

    Line* temp = new Line[numLines + 1];


    for (int i = 0; i < index; i++) {
        temp[i] = lines[i];
    }


    temp[index] = l;

    for (int i = index; i < numLines; i++) {
        temp[i + 1] = lines[i];
    }

    delete[] lines;
    lines = temp;
    numLines++;
}


void Paragraph::removeLineAt(int index) {
    if (index < 0 || index >= numLines || lines == nullptr) return;

    if (numLines == 1) {
        delete[] lines;
        lines = nullptr;
        numLines = 0;
        return;
    }

    Line* temp = new Line[numLines - 1];

    for (int i = 0, k = 0; i < numLines; i++) {
        if (i != index) {
            temp[k++] = lines[i];
        }
    }

    delete[] lines;
    lines = temp;
    numLines--;
}

int Paragraph::getNumLines() const {
    return numLines;
}

Line& Paragraph::getLine(int index) {
    return lines[index];
}

const Line& Paragraph::getLine(int index) const {
    return lines[index];
}

void Paragraph::print() const {
    for (int i = 0; i < numLines; i++) {
        lines[i].print();
        cout << endl;
    }
}