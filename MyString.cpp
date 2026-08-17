#include <iostream>
#include"MyString.h"

using namespace std;

int Mystring::getLen(const char* str) {
    if (!str) return 0;
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void Mystring::copyStr(char* dest, const char* src, int count) {
    for (int i = 0; i < count; ++i) {
        dest[i] = src[i];
    }
    dest[count] = '\0';
}

Mystring::Mystring() {
    size = 0;
    Cs = new char[1];
    Cs[0] = '\0';
}

Mystring::Mystring(char c) {
    size = 1;
    Cs = new char[2];
    Cs[0] = c;
    Cs[1] = '\0';
}

Mystring::Mystring(const char* p) {
    if (p == nullptr) {
        size = 0;
        Cs = new char[1];
        Cs[0] = '\0';
    }
    else {
        size = getLen(p);
        Cs = new char[size + 1];
        copyStr(Cs, p, size);
    }
}

Mystring::Mystring(int n, char c) {

    size = n;
    Cs = new char[size + 1];
    for (int i = 0; i < size; i++)
        Cs[i] = c;
    Cs[size] = '\0';
}

Mystring::Mystring(int num) {
    Mystring temp = itos(num);
    size = temp.size;
    Cs = new char[size + 1];
    copyStr(Cs, temp.Cs, size);
}

Mystring::~Mystring() {
    delete[] Cs;
}

Mystring::Mystring(const Mystring& other) {
    size = other.size;
    Cs = new char[size + 1];
    copyStr(Cs, other.Cs, size);
}

Mystring& Mystring::operator=(const Mystring& other) {
    if (this != &other) {
        delete[] Cs;
        size = other.size;
        Cs = new char[size + 1];
        copyStr(Cs, other.Cs, size);
    }
    return *this;
}


int Mystring::Length() const {
    return size;
}


int Mystring::stoi() const {
    int result = 0;
    int i = 0;
    bool isNegative = false;


    if (size > 0 && Cs[0] == '-') {
        isNegative = true;
        i++;
    }


    for (; i < size; ++i) {
        if (Cs[i] >= '0' && Cs[i] <= '9') {
            result = result * 10 + (Cs[i] - '0');
        }
        else {
            break;
        }
    }

    return isNegative ? -result : result;
}


Mystring Mystring::itos(int num) {
    if (num == 0) {
        return Mystring("0");
    }

    char buffer[32];
    int i = 0;
    bool isNegative = false;

    long long tempNum = num;
    if (tempNum < 0) {
        isNegative = true;
        tempNum = -tempNum;
    }

    while (tempNum > 0) {
        buffer[i++] = (tempNum % 10) + '0';
        tempNum = tempNum / 10;
    }

    if (isNegative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    int start = 0;
    int end = i - 1;

    while (start < end) {

        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;

        start++;
        end--;
    }

    return Mystring(buffer);
}


void Mystring::ReplaceFirst(char c) {
    if (size > 0 && Cs != nullptr) {
        Cs[0] = c;
    }
}

Mystring Mystring::trim() const {
    if (size == 0) {
        return Mystring("");
    }

    int start = 0;
    while (start < size && (Cs[start] == ' ' || Cs[start] == '\t' || Cs[start] == '\n' || Cs[start] == '\r')) {
        start++;
    }

    int end = size - 1;

    while (end >= start && (Cs[end] == ' ' || Cs[end] == '\t' || Cs[end] == '\n' || Cs[end] == '\r')) {
        end--;
    }

    if (start > end) {
        return Mystring("");
    }

    int newSize = end - start + 1;

    char* temp = new char[newSize + 1];

    for (int i = 0; i < newSize; ++i) {
        temp[i] = Cs[start + i];
    }
    temp[newSize] = '\0';

    Mystring trimmed(temp);
    delete[] temp;

    return trimmed;
}

char Mystring::CharacterAt(int i) const {

    if (i >= 0 && i < size) {
        return Cs[i];
    }
    return '\0';
}

void Mystring::ReplaceAt(int i, char c) {

    if (i >= 0 && i < size) {
        Cs[i] = c;
    }
}

bool Mystring::isEqual(const Mystring& M) const {
    if (size != M.size) {
        return false;
    }

    for (int i = 0; i < size; ++i) {
        if (Cs[i] != M.Cs[i]) {
            return false;
        }
    }

    return true;
}

bool Mystring::isLess(const Mystring& M) const {

    int minLen = (size < M.size) ? size : M.size;

    for (int i = 0; i < minLen; ++i) {
        if (Cs[i] < M.Cs[i]) {
            return true;
        }

        if (Cs[i] > M.Cs[i]) {
            return false;
        }
    }
    return size < M.size;
}

bool Mystring::isGreater(const Mystring& M) const {

    int minLen = (size < M.size) ? size : M.size;


    for (int i = 0; i < minLen; ++i) {
        if (Cs[i] > M.Cs[i]) {
            return true;
        }
        if (Cs[i] < M.Cs[i]) {
            return false;
        }
    }

    return size > M.size;
}

Mystring* Mystring::split(char delimiter, int& count) const {
    if (size == 0) {
        count = 0;
        return nullptr;
    }

    int delimitersCount = 0;
    for (int i = 0; i < size; ++i) {
        if (Cs[i] == delimiter) {
            delimitersCount++;
        }
    }
    count = delimitersCount + 1;

    Mystring* result = new Mystring[count];

    int wordIndex = 0;
    int start = 0;


    for (int i = 0; i <= size; ++i) {

        if (Cs[i] == delimiter || Cs[i] == '\0') {
            int wordLen = i - start;

            char* temp = new char[wordLen + 1];
            for (int j = 0; j < wordLen; ++j) {
                temp[j] = Cs[start + j];
            }
            temp[wordLen] = '\0';

            if (wordIndex < count) {
                result[wordIndex] = Mystring(temp);
                wordIndex++;
            }

            delete[] temp;

            start = i + 1;
        }
    }

    return result;
}

void Mystring::ToUpperInPlace() {
    if (Cs == nullptr)
        return;

    for (int i = 0; i < size; ++i) {

        if (Cs[i] >= 'a' && Cs[i] <= 'z') {
            Cs[i] = Cs[i] - 32;
        }
    }
}

Mystring Mystring::ToUpper() const {
    Mystring temp(*this);

    temp.ToUpperInPlace();

    return temp;
}


void Mystring::ToLowerInPlace() {
    if (Cs == nullptr)
        return;

    for (int i = 0; i < size; ++i) {
        if (Cs[i] >= 'A' && Cs[i] <= 'Z') {
            Cs[i] = Cs[i] + 32;
        }
    }
}

Mystring Mystring::ToLower() const {
    Mystring temp(*this);

    temp.ToLowerInPlace();

    return temp;
}

bool Mystring::isDelimiter(char c, const char* delimList) {
    if (!delimList)
        return false;
    for (int i = 0; delimList[i] != '\0'; i++) {
        if (c == delimList[i]) {
            return true;
        }
    }
    return false;
}

Mystring* Mystring::tokenize(const char* delim, int& count) const {
    if (size == 0 || delim == nullptr) {
        count = 0;
        return nullptr;
    }

    int tokenCount = 0;
    bool insideWord = false;

    for (int i = 0; i < size; ++i) {
        if (!isDelimiter(Cs[i], delim)) {
            if (!insideWord) {
                tokenCount++;
                insideWord = true;
            }
        }
        else {
            insideWord = false;
        }
    }

    count = tokenCount;
    if (count == 0)
        return nullptr;

    Mystring* result = new Mystring[count];
    int tokenIndex = 0;

    int start = -1;
    for (int i = 0; i <= size; ++i) {
        if (i == size || isDelimiter(Cs[i], delim)) {
            if (start != -1) {
                int wordLen = i - start;

                char* temp = new char[wordLen + 1];
                for (int j = 0; j < wordLen; ++j) {
                    temp[j] = Cs[start + j];
                }
                temp[wordLen] = '\0';

                if (tokenIndex < count) {
                    result[tokenIndex] = Mystring(temp);
                    tokenIndex++;
                }

                delete[] temp;
                start = -1;
            }
        }
        else {
            if (start == -1) {
                start = i;
            }
        }
    }
    return result;
}

void Mystring::clear() {
    delete[] Cs;
    size = 0;
    Cs = new char[1];
    Cs[0] = '\0';
}

void Mystring::Print() const {
    cout << Cs;
}
