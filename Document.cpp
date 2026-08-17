#include "Document.h"
#include <iostream>
using namespace std;


Document::Document() {
    chapters = nullptr;
    numChapters = 0;
}


Document::Document(const Mystring& t) {
    title = t;
    chapters = nullptr;
    numChapters = 0;
}

void Document::doccopy(const Document& other) {
    title = other.title;
    numChapters = other.numChapters;
    if (numChapters > 0 && other.chapters != nullptr) {
        chapters = new Chapter[numChapters];
        for (int i = 0; i < numChapters; i++) {
            chapters[i] = other.chapters[i];
        }
    }
    else {
        chapters = nullptr;
        numChapters = 0;
    }
}

Document::Document(const Document& other) {
    doccopy(other);
}


Document& Document::operator=(const Document& other) {
    if (this != &other) {
        delete[] chapters;

        doccopy(other);
    }
    return *this;
}


Document::~Document() {
    delete[] chapters;
    chapters = nullptr;
    numChapters = 0;
}

void Document::setTitle(const Mystring& t) {
    title = t;
}

Mystring Document::getTitle() const {
    return title;
}

void Document::addChapter(const Chapter& c) {
    Chapter* temp = new Chapter[numChapters + 1];

    for (int i = 0; i < numChapters; i++) {
        temp[i] = chapters[i];
    }
    temp[numChapters] = c;

    delete[] chapters;
    chapters = temp;
    numChapters++;
}


void Document::insertChapterAt(int index, const Chapter& c) {
    if (index < 0 || index > numChapters) return;

    Chapter* temp = new Chapter[numChapters + 1];

    for (int i = 0; i < index; i++) {
        temp[i] = chapters[i];
    }
    temp[index] = c;
    for (int i = index; i < numChapters; i++) {
        temp[i + 1] = chapters[i];
    }

    delete[] chapters;
    chapters = temp;
    numChapters++;
}


void Document::removeChapterAt(int index) {
    if (index < 0 || index >= numChapters || chapters == nullptr) return;

    if (numChapters == 1) {
        delete[] chapters;
        chapters = nullptr;
        numChapters = 0;
        return;
    }

    Chapter* temp = new Chapter[numChapters - 1];

    for (int i = 0, k = 0; i < numChapters; i++) {
        if (i != index) {
            temp[k++] = chapters[i];
        }
    }

    delete[] chapters;
    chapters = temp;
    numChapters--;
}


int Document::getNumChapters() const {
    return numChapters;
}


Chapter& Document::getChapter(int index) {
    return chapters[index];
}


const Chapter& Document::getChapter(int index) const {
    return chapters[index];
}

void Document::print() const {
    cout << "========================================" << endl;
    cout << "DOCUMENT" << endl;
    cout << "========================================" << endl << endl;

    for (int i = 0; i < numChapters; i++) {
        cout << "CHAPTER " << (i + 1) << ":" << endl;
        chapters[i].print();
        cout << "----------------------------------------" << endl;
    }
}