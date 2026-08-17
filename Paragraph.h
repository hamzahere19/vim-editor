#pragma once
#include "Line.h"

class Paragraph {
private:
    Line* lines;
    int numLines;

public:
    Paragraph();
    Paragraph(const Paragraph& other);
    Paragraph& operator=(const Paragraph& other);
    ~Paragraph();

    void addLine(const Line& l);
    void insertLineAt(int index, const Line& l);
    void removeLineAt(int index);

    int getNumLines() const;
    Line& getLine(int index);
    const Line& getLine(int index) const;
    void print() const;
};