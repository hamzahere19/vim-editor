#include "Chapter.h"
#include <iostream>
using namespace std;

Chapter::Chapter() {
    sections = nullptr;
    numSections = 0;
}

void Chapter::copy(const Chapter& other) {
    numSections = other.numSections;
    if (numSections > 0 && other.sections != nullptr) {
        sections = new Section[numSections];
        for (int i = 0; i < numSections; i++) {
            sections[i] = other.sections[i];
        }
    }
    else {
        sections = nullptr;
        numSections = 0;
    }
}
Chapter::Chapter(const Chapter& other) {
    copy(other);
}

Chapter& Chapter::operator=(const Chapter& other) {
    if (this != &other) {
        delete[] sections;

        copy(other);
    }
    return *this;
}


Chapter::~Chapter() {
    delete[] sections;
    sections = nullptr;
    numSections = 0;
}


void Chapter::addSection(const Section& s) {
    Section* temp = new Section[numSections + 1];

    for (int i = 0; i < numSections; i++) {
        temp[i] = sections[i];
    }
    temp[numSections] = s;

    delete[] sections;
    sections = temp;
    numSections++;
}

void Chapter::insertSectionAt(int index, const Section& s) {
    if (index < 0 || index > numSections) 
        return;

    Section* temp = new Section[numSections + 1];

    for (int i = 0; i < index; i++) {
        temp[i] = sections[i];
    }
    temp[index] = s;
    for (int i = index; i < numSections; i++) {
        temp[i + 1] = sections[i];
    }

    delete[] sections;
    sections = temp;
    numSections++;
}

void Chapter::removeSectionAt(int index) {
    if (index < 0 || index >= numSections || sections == nullptr)
        return;

    if (numSections == 1) {
        delete[] sections;
        sections = nullptr;
        numSections = 0;
        return;
    }

    Section* temp = new Section[numSections - 1];

    for (int i = 0, k = 0; i < numSections; i++) {
        if (i != index) {
            temp[k++] = sections[i];
        }
    }

    delete[] sections;
    sections = temp;
    numSections--;
}

int Chapter::getNumSections() const {
    return numSections;
}

Section& Chapter::getSection(int index) {
    return sections[index];
}

const Section& Chapter::getSection(int index) const {
    return sections[index];
}

void Chapter::print() const {
    for (int i = 0; i < numSections; i++) {
        sections[i].print();
        cout << endl;
    }
}