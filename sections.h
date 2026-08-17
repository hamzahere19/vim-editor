
#pragma once
#include "Paragraph.h"

class Section {
private:
    Paragraph* paragraphs; 
    int numParagraphs;     

public:
   
    Section();
    Section(const Section& other);           
    Section& operator=(const Section& other); 
    ~Section();
   

    void addParagraph(const Paragraph& p);
    void insertParagraphAt(int index, const Paragraph& p);
    void removeParagraphAt(int index);


    int getNumParagraphs() const;
    Paragraph& getParagraph(int index);
    const Paragraph& getParagraph(int index) const;
    void print() const;
};
