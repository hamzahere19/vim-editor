#pragma once
#include "Chapter.h"
#include "MyString.h"

class Document {
private:
    Mystring title;       
    Chapter* chapters;   
    int numChapters;      

public:
    
    Document();
    Document(const Mystring& t);
    Document(const Document& other);           
    Document& operator=(const Document& other); 
    ~Document();
    void doccopy(const Document& other);


    void setTitle(const Mystring& t);
    Mystring getTitle() const;

    void addChapter(const Chapter& c);
    void insertChapterAt(int index, const Chapter& c);
    void removeChapterAt(int index);

    int getNumChapters() const;
    Chapter& getChapter(int index);
    const Chapter& getChapter(int index) const;
    void print() const;
};