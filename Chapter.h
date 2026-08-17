#pragma once
#include "sections.h"

class Chapter {
private:
    Section* sections;  
    int numSections;    

public:
  
    Chapter();
    Chapter(const Chapter& other);            
    Chapter& operator=(const Chapter& other); 
    ~Chapter();
    void copy(const Chapter& other);

 
    void addSection(const Section& s);
    void insertSectionAt(int index, const Section& s);
    void removeSectionAt(int index);


    int getNumSections() const;
    Section& getSection(int index);
    const Section& getSection(int index) const;
    void print() const;
};

