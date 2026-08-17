#pragma once


class Mystring
{
    int size;
    char* Cs;
    static int getLen(const char* str);
    static void copyStr(char* dest, const char* src, int count);
    static bool isDelimiter(char c, const char* delimList);

public:


    Mystring();
    Mystring(char c);
    Mystring(const char* p);
    Mystring(int n, char c);
    Mystring(int num);
    Mystring(const Mystring& other);

    Mystring& operator=(const Mystring& other);


    ~Mystring();

    void Print() const;
    int  Length() const;
    void clear();

    int stoi() const;
    static Mystring itos(int num);

    Mystring trim() const;
    void ReplaceFirst(char c);
    char CharacterAt(int i) const;
    void ReplaceAt(int i, char c);

    bool isEqual(const Mystring& M) const;
    bool isLess(const Mystring& M) const;
    bool isGreater(const Mystring& M) const;


    Mystring* split(char delim, int& count) const;
    Mystring* tokenize(const char* delim, int& count) const;

    void ToUpperInPlace();
    Mystring ToUpper() const;

    void ToLowerInPlace();
    Mystring ToLower() const;

};
