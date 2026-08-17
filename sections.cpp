#include "sections.h"
#include <iostream>
using namespace std;


Section::Section() {
    paragraphs = nullptr;
    numParagraphs = 0;
}


Section::Section(const Section& other) {
    numParagraphs = other.numParagraphs;
    if (numParagraphs > 0 && other.paragraphs != nullptr) {
        paragraphs = new Paragraph[numParagraphs];
        for (int i = 0; i < numParagraphs; i++) {
            paragraphs[i] = other.paragraphs[i];
        }
    }
    else {
        paragraphs = nullptr;
        numParagraphs = 0;
    }
}

Section& Section::operator=(const Section& other) {
    if (this != &other) {
        delete[] paragraphs;

        numParagraphs = other.numParagraphs;
        if (numParagraphs > 0 && other.paragraphs != nullptr) {
            paragraphs = new Paragraph[numParagraphs];
            for (int i = 0; i < numParagraphs; i++) {
                paragraphs[i] = other.paragraphs[i];
            }
        }
        else {
            paragraphs = nullptr;
            numParagraphs = 0;
        }
    }
    return *this;
}


Section::~Section() {
    delete[] paragraphs;
    paragraphs = nullptr;
    numParagraphs = 0;
}


void Section::addParagraph(const Paragraph& p) {
    Paragraph* temp = new Paragraph[numParagraphs + 1];

    for (int i = 0; i < numParagraphs; i++) {
        temp[i] = paragraphs[i];
    }
    temp[numParagraphs] = p;

    delete[] paragraphs;
    paragraphs = temp;
    numParagraphs++;
}

void Section::insertParagraphAt(int index, const Paragraph& p) {
    if (index < 0 || index > numParagraphs) 
        return;

    Paragraph* temp = new Paragraph[numParagraphs + 1];

    for (int i = 0; i < index; i++) {
        temp[i] = paragraphs[i];
    }
    temp[index] = p;
    for (int i = index; i < numParagraphs; i++) {
        temp[i + 1] = paragraphs[i];
    }

    delete[] paragraphs;
    paragraphs = temp;
    numParagraphs++;
}


void Section::removeParagraphAt(int index) {
    if (index < 0 || index >= numParagraphs || paragraphs == nullptr) 
        return;

    if (numParagraphs == 1) {
        delete[] paragraphs;
        paragraphs = nullptr;
        numParagraphs = 0;
        return;
    }

    Paragraph* temp = new Paragraph[numParagraphs - 1];

    for (int i = 0, k = 0; i < numParagraphs; i++) {
        if (i != index) {
            temp[k++] = paragraphs[i];
        }
    }

    delete[] paragraphs;
    paragraphs = temp;
    numParagraphs--;
}

int Section::getNumParagraphs() const {
    return numParagraphs;
}

Paragraph& Section::getParagraph(int index) {
    return paragraphs[index];
}

const Paragraph& Section::getParagraph(int index) const {
    return paragraphs[index];
}

void Section::print() const {
    for (int i = 0; i < numParagraphs; i++) {
        paragraphs[i].print();
        cout << endl;
    }
}
