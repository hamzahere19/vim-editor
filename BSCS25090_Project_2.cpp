#include <iostream>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include <algorithm>

#include "MyString.h"
#include "Line.h"
#include "Paragraph.h"
#include "sections.h"
#include "Chapter.h"
#include "Document.h"

using namespace std;

bool showLineNumbers = false;
void SetConsoleSettings() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTitleA("VIM Text Editor - OOP Project");

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = 20;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);


    SetConsoleTextAttribute(hOut,
        0 |                                                                            
        BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY     
    );
}
void gotoRowCol(int rpos, int cpos) {
    COORD scrn;
    scrn.X = (SHORT)cpos;
    scrn.Y = (SHORT)rpos;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), scrn);
}

int getLineNumOffset(int currentR) {
    if (!showLineNumbers)
        return 0;
    int num = currentR + 1;
    if (num < 10) 
        return 2;
    if (num < 100)
        return 3;
    return 4;
}

void refreshDocument(const Document& doc) {
    system("cls");
    if (doc.getNumChapters() == 0) return;

    const Paragraph& p = doc.getChapter(0).getSection(0).getParagraph(0);
    int total = p.getNumLines();

    for (int i = 0; i < total; i++) {
        gotoRowCol(i, 0);

        if (showLineNumbers) {
            int num = i + 1;
            cout << num << " ";
        }
        p.getLine(i).print();
    }
}

void refreshLine(int r, int c, const Line& line) {
    gotoRowCol(r, 0);
    cout << "                                                                                ";

    gotoRowCol(r, 0);
    if (showLineNumbers) {
        int num = r + 1;
        cout << num << " ";
    }
    line.print();

    int offset = getLineNumOffset(r);
    gotoRowCol(r, c + offset);
}

void clearCommandBar() {
    gotoRowCol(24, 0);
    cout << "                                                                                ";
}

void printStatus(const Mystring& msg) {
    clearCommandBar();
    gotoRowCol(24, 0);
    msg.Print();
}

void saveToFile(const Document& doc, const Mystring& filename) {
    ofstream outFile;

    char fileBuf[256];
    int i = 0;
    for (; i < filename.Length(); i++) {
        fileBuf[i] = filename.CharacterAt(i);
    }
    fileBuf[i] = '\0';

    outFile.open(fileBuf);
    if (!outFile) {
        printStatus("Error: Could not save file!");
        return;
    }

    if (doc.getNumChapters() == 0) 
        return;

    const Section& sec = doc.getChapter(0).getSection(0);
    const Paragraph& p = sec.getParagraph(0);
    int totalLines = p.getNumLines();

    for (int lineIdx = 0; lineIdx < totalLines; lineIdx++) {
        Line l = p.getLine(lineIdx);
        for (int j = 0; j < l.getLength(); j++) {
            outFile << l.getText().CharacterAt(j);
        }
        if (lineIdx < totalLines - 1)
            outFile << "\n";
    }

    outFile.close();
    printStatus("File written successfully.");
}

void loadFromFile(Document& doc, const Mystring& filename) {
    char fileBuf[256];
    int idx = 0;
    for (; idx < filename.Length(); idx++) {
        fileBuf[idx] = filename.CharacterAt(idx);
    }
    fileBuf[idx] = '\0';

    ifstream inFile(fileBuf);
    if (!inFile) {
        printStatus("Error: File not found!");
        return;
    }

    Document newDoc;
    Chapter newChap;
    Paragraph p;
    char tempBuffer[1024];

    while (inFile.getline(tempBuffer, 1024)) {
        p.addLine(Line(tempBuffer));
    }

    if (p.getNumLines() == 0) {
        p.addLine(Line(""));
    }

    Section s;
    s.addParagraph(p);
    newChap.addSection(s);
    newDoc.addChapter(newChap);

    doc = newDoc;
    inFile.close();
    refreshDocument(doc);
    printStatus("File loaded successfully.");
}


const int MAX_HISTORY = 50;
Document undoStack[MAX_HISTORY];
int undoTop = -1;
Document redoStack[MAX_HISTORY];
int redoTop = -1;

void saveState(const Document& doc) {
    if (undoTop < MAX_HISTORY - 1) {
        undoStack[++undoTop] = doc;
    }
    else {
        for (int i = 0; i < MAX_HISTORY - 1; i++) {
            undoStack[i] = undoStack[i + 1];
        }
        undoStack[MAX_HISTORY - 1] = doc;
    }
    redoTop = -1;
}

bool performForwardSearch(const Document& doc, const Mystring& pattern, int startR, int startC, int& foundR, int& foundC) {
    int patLen = pattern.Length();
    if (patLen == 0 || doc.getNumChapters() == 0) 
        return false;

    const Paragraph& p = doc.getChapter(0).getSection(0).getParagraph(0);
    int totalLines = p.getNumLines();

    for (int r = startR; r < totalLines; r++) {
        Line l = p.getLine(r);
        int lineLen = l.getLength();
        int colStart = (r == startR) ? startC : 0;

        for (int c = colStart; c <= lineLen - patLen; c++) {
            bool match = true;
            for (int k = 0; k < patLen; k++) {
                if (l.getText().CharacterAt(c + k) != pattern.CharacterAt(k)) {
                    match = false;
                    break;
                }
            }
            if (match) {
                foundR = r;
                foundC = c;
                return true;
            }
        }
    }
    return false;
}

bool performBackwardSearch(const Document& doc, const Mystring& pattern, int startR, int startC, int& foundR, int& foundC) {
    int patLen = pattern.Length();
    if (patLen == 0 || doc.getNumChapters() == 0)
        return false;

    const Paragraph& p = doc.getChapter(0).getSection(0).getParagraph(0);

    for (int r = startR; r >= 0; r--) {
        Line l = p.getLine(r);
        int lineLen = l.getLength();
        int colStart = (r == startR) ? min(startC, lineLen - patLen) : lineLen - patLen;

        for (int c = colStart; c >= 0; c--) {
            bool match = true;
            for (int k = 0; k < patLen; k++) {
                if (l.getText().CharacterAt(c + k) != pattern.CharacterAt(k)) {
                    match = false;
                    break;
                }
            }
            if (match) {
                foundR = r;
                foundC = c;
                return true;
            }
        }
    }
    return false;
}

void performGlobalReplace(Document& doc, const Mystring& cmd) {
    if (cmd.Length() < 5 || cmd.CharacterAt(0) != '%' || cmd.CharacterAt(1) != 's' || cmd.CharacterAt(2) != '/')
    {
        printStatus("Invalid replace format. Use: :%s/old/new/g");
        return;
    }

    int len = cmd.Length();
    int p1 = 2; 
    int p2 = -1;
    int p3 = -1;

    for (int i = p1 + 1; i < len; i++) {
        if (cmd.CharacterAt(i) == '/') {
            if (p2 == -1) p2 = i;
            else if (p3 == -1) p3 = i;
        }
    }

    if (p2 == -1) {
        printStatus("Invalid replace format. Use: :%s/old/new/g");
        return;
    }

    char oldBuf[256];
    int oIdx = 0;
    for (int i = p1 + 1; i < p2; i++) oldBuf[oIdx++] = cmd.CharacterAt(i);
    oldBuf[oIdx] = '\0';
    Mystring oldStr(oldBuf);

    char newBuf[256];
    int nIdx = 0;
    int endNew = (p3 == -1) ? len : p3;
    for (int i = p2 + 1; i < endNew; i++) newBuf[nIdx++] = cmd.CharacterAt(i);
    newBuf[nIdx] = '\0';
    Mystring newStr(newBuf);

    int oldLen = oldStr.Length();
    int newLen = newStr.Length();

    if (oldLen == 0) {
        printStatus("Search string cannot be empty.");
        return;
    }

    saveState(doc);

    Paragraph& p = doc.getChapter(0).getSection(0).getParagraph(0);
    int totalLines = p.getNumLines();

    for (int r = 0; r < totalLines; r++) {
        Line& currentLine = p.getLine(r);
        int c = 0;

        while (c <= currentLine.getLength() - oldLen) {
            bool match = true;
            for (int k = 0; k < oldLen; k++) {
                if (currentLine.getText().CharacterAt(c + k) != oldStr.CharacterAt(k)) {
                    match = false;
                    break;
                }
            }

            if (match) {
                for (int k = 0; k < oldLen; k++) {
                    currentLine.removeChar(c);
                }
                for (int k = 0; k < newLen; k++) {
                    currentLine.insertChar(c + k, newStr.CharacterAt(k));
                }
                c += newLen;
            }
            else {
                c++;
            }
        }
    }

    refreshDocument(doc);
    printStatus("Replaced occurrences successfully.");
}

int main() {
    SetConsoleSettings();

    Document activeDoc;
    Chapter activeChapter;
    Paragraph p;
    p.addLine(Line(""));
    Section s;
    s.addParagraph(p);
    activeChapter.addSection(s);
    activeDoc.addChapter(activeChapter);

    int r = 0, c = 0;
    int mode = 0;         
    bool isRunning = true;
    char prevKey = 0;

    enum LastAction { NONE, DELETE_CHAR, DELETE_LINE, DELETE_TO_END, TOGGLE_CASE, PASTE_AFTER, PASTE_BEFORE };
    LastAction lastCmd = NONE;

    bool isSelecting = false;
    int selStartC = -1;

    char copyBuf[1024] = "";
    int copyBufLen = 0;

    Mystring currentFileName("file.txt");

    char cmdBufArray[256] = "";
    int cmdBufLen = 0;

    Mystring lastSearchPattern("");
    bool lastSearchIsForward = true;

    system("cls");
    refreshDocument(activeDoc);

    while (isRunning) {
        if (mode != 2) {
            int offset = getLineNumOffset(r);
            gotoRowCol(r, c + offset);
        }

        if (_kbhit()) {
            int key = _getch();

            Line& currentLine = activeDoc.getChapter(0).getSection(0).getParagraph(0).getLine(r);

  
            if (mode == 0) {
                if (key == 'l') {
                    if (c < currentLine.getLength()) 
                        c++;
                }
                else if (key == 'h') {
                    if (c > 0) c--;
                }
                else if (key == 'j') {
                    int totalLines = activeDoc.getChapter(0).getSection(0).getParagraph(0).getNumLines();
                    if (r < totalLines - 1) 
                        r++;
                }
                else if (key == 'k') {
                    if (r > 0)
                        r--;
                }
                else if (key == '0') {
                    c = 0;
                }
                else if (key == '$') { 
                    c = currentLine.getLength();
                }
                else if (key == 'w') {
                    int len = currentLine.getLength();
                    while (c < len && currentLine.getText().CharacterAt(c) != ' ') c++;
                    while (c < len && currentLine.getText().CharacterAt(c) == ' ') c++;
                }
                else if (key == 'b') {
                    if (c > 0) {
                        c--;
                        while (c > 0 && currentLine.getText().CharacterAt(c) == ' ') c--;
                        while (c > 0 && currentLine.getText().CharacterAt(c - 1) != ' ') c--;
                    }
                }
                else if (key == 'g' && prevKey == 'g') {
                    r = 0; c = 0;
                    prevKey = 0;
                }
                else if (key == 'G') {
                    int totalLines = activeDoc.getChapter(0).getSection(0).getParagraph(0).getNumLines();
                    r = totalLines - 1;
                    c = 0;
                }
                else if (key == 4) { 
                    int totalLines = activeDoc.getChapter(0).getSection(0).getParagraph(0).getNumLines();
                    r = min(r + 5, totalLines - 1);
                }
                else if (key == 21) { 
                    r = max(r - 5, 0);
                }
                else if (key == 'x') {
                    if (c < currentLine.getLength()) {
                        saveState(activeDoc);
                        currentLine.removeChar(c);
                        refreshLine(r, c, currentLine);
                        lastCmd = DELETE_CHAR;
                    }
                }
                else if (key == 'd' && prevKey == 'd') {
                    saveState(activeDoc);
                    currentLine = Line("");
                    refreshDocument(activeDoc);
                    c = 0;
                    prevKey = 0;
                    lastCmd = DELETE_LINE;
                }
                else if (key == 'D') {
                    if (c < currentLine.getLength()) {
                        saveState(activeDoc);
                        currentLine.deleteFrom(c);
                        refreshLine(r, c, currentLine);
                        lastCmd = DELETE_TO_END;
                    }
                }
                else if (key == 'y' && prevKey == 'y') {
                    copyBufLen = 0;
                    for (int i = 0; i < currentLine.getLength(); i++) {
                        copyBuf[copyBufLen++] = currentLine.getText().CharacterAt(i);
                    }
                    copyBuf[copyBufLen] = '\0';
                    printStatus("Line copied .");
                    prevKey = 0;
                }
                else if (key == 'p') {
                    if (copyBufLen > 0) {
                        saveState(activeDoc);
                        r++;
                        activeDoc.getChapter(0).getSection(0).getParagraph(0).insertLineAt(r, Line(copyBuf));
                        refreshDocument(activeDoc);
                        c = 0;
                        lastCmd = PASTE_AFTER;
                    }
                }
                else if (key == 'P') {
                    if (copyBufLen > 0) {
                        saveState(activeDoc);
                        activeDoc.getChapter(0).getSection(0).getParagraph(0).insertLineAt(r, Line(copyBuf));
                        refreshDocument(activeDoc);
                        c = 0;
                        lastCmd = PASTE_BEFORE;
                    }
                }
                else if (key == '~') {
                    if (c < currentLine.getLength()) {
                        saveState(activeDoc);
                        currentLine.toggleCase(c);
                        c++;
                        refreshLine(r, c, currentLine);
                        lastCmd = TOGGLE_CASE;
                    }
                }
                else if (key == 'u') {
                    if (undoTop >= 0) {
                        if (redoTop < MAX_HISTORY - 1) {
                            redoStack[++redoTop] = activeDoc;
                        }
                        activeDoc = undoStack[undoTop--];
                        refreshDocument(activeDoc);

                        int totalLines = activeDoc.getChapter(0).getSection(0).getParagraph(0).getNumLines();
                        r = min(r, totalLines - 1);
                        Line& restoredLine = activeDoc.getChapter(0).getSection(0).getParagraph(0).getLine(r);
                        if (c > restoredLine.getLength()) c = restoredLine.getLength();
                    }
                }
                else if (key == 18) { 
                    if (redoTop >= 0) {
                        if (undoTop < MAX_HISTORY - 1) {
                            undoStack[++undoTop] = activeDoc;
                        }
                        activeDoc = redoStack[redoTop--];
                        refreshDocument(activeDoc);

                        int totalLines = activeDoc.getChapter(0).getSection(0).getParagraph(0).getNumLines();
                        r = min(r, totalLines - 1);
                        Line& restoredLine = activeDoc.getChapter(0).getSection(0).getParagraph(0).getLine(r);
                        if (c > restoredLine.getLength()) c = restoredLine.getLength();
                    }
                }
                else if (key == '.') {
                    if (lastCmd == DELETE_CHAR && c < currentLine.getLength()) {
                        saveState(activeDoc);
                        currentLine.removeChar(c);
                        refreshLine(r, c, currentLine);
                    }
                    else if (lastCmd == DELETE_LINE) {
                        saveState(activeDoc);
                        currentLine = Line("");
                        refreshDocument(activeDoc);
                        c = 0;
                    }
                    else if (lastCmd == DELETE_TO_END && c < currentLine.getLength()) {
                        saveState(activeDoc);
                        currentLine.deleteFrom(c);
                        refreshLine(r, c, currentLine);
                    }
                    else if (lastCmd == TOGGLE_CASE && c < currentLine.getLength()) {
                        saveState(activeDoc);
                        currentLine.toggleCase(c);
                        c++;
                        refreshLine(r, c, currentLine);
                    }
                }
                else if (key == 'i') {
                    mode = 1;
                    clearCommandBar();
                    printStatus("-- INSERT MODE --");
                }
                else if (key == ':') {
                    mode = 2;
                    cmdBufLen = 0;
                    cmdBufArray[0] = '\0';
                    clearCommandBar();
                    gotoRowCol(24, 0);
                    cout << ":";
                }
                else if (key == 'q') { isRunning = false; }

                if (key == 'd' || key == 'g' || key == 'y') prevKey = key;
                else prevKey = 0;
            }

            else if (mode == 1) {
                if (key == 27) { 
                    mode = 0;
                    isSelecting = false;
                    clearCommandBar();
                }
                else if (key == 13) { 
                    saveState(activeDoc);
                    r++;
                    activeDoc.getChapter(0).getSection(0).getParagraph(0).insertLineAt(r, Line(""));
                    refreshDocument(activeDoc);
                    c = 0;
                    isSelecting = false;
                }
                else if (key == 8) { 
                    if (c > 0) {
                        saveState(activeDoc);
                        c--;
                        currentLine.removeChar(c);
                        refreshLine(r, c, currentLine);
                    }
                    isSelecting = false;
                }
                else if (key == 9) { 
                    saveState(activeDoc);
                    for (int i = 0; i < 4; i++) {
                        currentLine.insertChar(c, ' ');
                        c++;
                    }
                    refreshLine(r, c, currentLine);
                    isSelecting = false;
                }
                else if (key == 0 || key == 224) {
                    int extKey = _getch();

          
                    if (extKey == 59) {
                        isSelecting = !isSelecting;
                        if (isSelecting) {
                            selStartC = c;
                            printStatus("-- SELECTING --");
                        }
                        else {
                            clearCommandBar();
                        }
                    }
             
                    else if (extKey == 60) {
                        if (isSelecting && selStartC != -1 && selStartC != c) {
                            int start = min(selStartC, c);
                            int end = max(selStartC, c);
                            copyBufLen = 0;
                            for (int i = start; i < end; i++) {
                                copyBuf[copyBufLen++] = currentLine.getText().CharacterAt(i);
                            }
                            copyBuf[copyBufLen] = '\0';
                            isSelecting = false;
                            selStartC = -1;
                            printStatus("Text Copied.");
                        }
                    }
     
                    else if (extKey == 61) {

                        if (isSelecting && selStartC != -1 && selStartC != c) {

                            saveState(activeDoc);
                            int start = min(selStartC, c);
                            int end = max(selStartC, c);
                            copyBufLen = 0;
                            for (int i = start; i < end; i++) {
                                copyBuf[copyBufLen++] = currentLine.getText().CharacterAt(i);
                            }
                            copyBuf[copyBufLen] = '\0';
                            for (int i = start; i < end; i++) {
                                currentLine.removeChar(start);
                            }
                            c = start;
                            refreshLine(r, c, currentLine);
                            isSelecting = false;
                            selStartC = -1;
                            printStatus("Text Cut.");
                        }
                    }
     
                    else if (extKey == 62) {
                        if (copyBufLen > 0) {
                            saveState(activeDoc);
                            for (int i = 0; i < copyBufLen; i++) {
                                currentLine.insertChar(c, copyBuf[i]);
                                c++;
                            }
                            refreshLine(r, c, currentLine);
                        }
                    }
               
                    else if (extKey == 63) {
                        saveState(activeDoc);
                        int spacesToRemove = 0;
                        while (spacesToRemove < 4 && c > 0 && currentLine.getText().CharacterAt(c - 1) == ' ') {
                            currentLine.removeChar(c - 1);
                            c--;
                            spacesToRemove++;
                        }
                        if (spacesToRemove > 0) {
                            refreshLine(r, c, currentLine);
                        }
                    }
             
                    else if (extKey == 75) { 
                        if (c > 0) c--;
                    }
                    else if (extKey == 77) { 
                        if (c < currentLine.getLength()) c++;
                    }
                    else if (extKey == 72) { 
                        if (r > 0) {
                            r--;
                            Line& prevLine = activeDoc.getChapter(0).getSection(0).getParagraph(0).getLine(r);
                            if (c > prevLine.getLength()) c = prevLine.getLength();
                        }
                    }
                    else if (extKey == 80) { 
                        int totalLines = activeDoc.getChapter(0).getSection(0).getParagraph(0).getNumLines();
                        if (r < totalLines - 1) {
                            r++;
                            Line& nextLine = activeDoc.getChapter(0).getSection(0).getParagraph(0).getLine(r);
                            if (c > nextLine.getLength()) c = nextLine.getLength();
                        }
                    }
                    else if (extKey == 83) { 
                        if (c < currentLine.getLength()) {
                            saveState(activeDoc);
                            currentLine.removeChar(c);
                            refreshLine(r, c, currentLine);
                        }
                    }
                }
                else if (key >= 32 && key <= 126) {
                    saveState(activeDoc);
                    currentLine.insertChar(c, (char)key);
                    c++;
                    refreshLine(r, c, currentLine);
                }
            }

            else if (mode == 2) {
                if (key == 27) { 
                    mode = 0;
                    clearCommandBar();
                }
                else if (key == 13) { 
                    mode = 0;
                    cmdBufArray[cmdBufLen] = '\0';
                    Mystring commandBuf(cmdBufArray);

                    if (commandBuf.isEqual(Mystring("q!")) || commandBuf.isEqual(Mystring("q"))) {
                        isRunning = false;
                    }
                    else if (commandBuf.isEqual(Mystring("w"))) {
                        saveToFile(activeDoc, currentFileName);
                    }
                    else if (commandBuf.isEqual(Mystring("wq"))) {
                        saveToFile(activeDoc, currentFileName);
                        isRunning = false;
                    }
                    else if (commandBuf.isEqual(Mystring("set number")) || commandBuf.isEqual(Mystring("set nu"))) {
                        showLineNumbers = true;
                        refreshDocument(activeDoc);
                        clearCommandBar();
                    }
                    else if (commandBuf.isEqual(Mystring("set nonumber")) || commandBuf.isEqual(Mystring("set nonu"))) {
                        showLineNumbers = false;
                        refreshDocument(activeDoc);
                        clearCommandBar();
                    }
                    else if (cmdBufLen >= 4 && commandBuf.CharacterAt(0) == '%' && commandBuf.CharacterAt(1) == 's') {
                        performGlobalReplace(activeDoc, commandBuf);
                    }
                    else if (cmdBufLen > 2 && commandBuf.CharacterAt(0) == 'w' && commandBuf.CharacterAt(1) == ' ') {
                        char nameBuf[256];
                        int nIdx = 0;
                        for (int i = 2; i < cmdBufLen; i++) {
                            nameBuf[nIdx++] = commandBuf.CharacterAt(i);
                        }
                        nameBuf[nIdx] = '\0';
                        currentFileName = Mystring(nameBuf);
                        saveToFile(activeDoc, currentFileName);
                    }
                    else if (cmdBufLen > 2 && commandBuf.CharacterAt(0) == 'e' && commandBuf.CharacterAt(1) == ' ') {
                        char nameBuf[256];
                        int nIdx = 0;
                        for (int i = 2; i < cmdBufLen; i++) {
                            nameBuf[nIdx++] = commandBuf.CharacterAt(i);
                        }
                        nameBuf[nIdx] = '\0';
                        currentFileName = Mystring(nameBuf);
                        loadFromFile(activeDoc, currentFileName);
                        r = 0; c = 0;
                    }
                    else if (cmdBufLen > 1 && commandBuf.CharacterAt(0) == '/') {
                        char searchBuf[256];
                        int sIdx = 0;
                        for (int i = 1; i < cmdBufLen; i++) {
                            searchBuf[sIdx++] = commandBuf.CharacterAt(i);
                        }
                        searchBuf[sIdx] = '\0';
                        lastSearchPattern = Mystring(searchBuf);
                        lastSearchIsForward = true;

                        int foundR = -1, foundC = -1;
                        if (performForwardSearch(activeDoc, lastSearchPattern, r, c + 1, foundR, foundC)) {
                            r = foundR;
                            c = foundC;
                            clearCommandBar();
                        }
                        else {
                            printStatus("Pattern not found.");
                        }
                    }
                    else if (cmdBufLen > 1 && commandBuf.CharacterAt(0) == '?') {
                        char searchBuf[256];
                        int sIdx = 0;
                        for (int i = 1; i < cmdBufLen; i++) {
                            searchBuf[sIdx++] = commandBuf.CharacterAt(i);
                        }
                        searchBuf[sIdx] = '\0';
                        lastSearchPattern = Mystring(searchBuf);
                        lastSearchIsForward = false;

                        int foundR = -1, foundC = -1;
                        if (performBackwardSearch(activeDoc, lastSearchPattern, r, c - 1, foundR, foundC)) {
                            r = foundR;
                            c = foundC;
                            clearCommandBar();
                        }
                        else {
                            printStatus("Pattern not found.");
                        }
                    }
                    else if (commandBuf.isEqual(Mystring("n"))) {
                        if (lastSearchPattern.Length() > 0) {
                            int foundR = -1, foundC = -1;
                            bool success = false;
                            if (lastSearchIsForward) {
                                success = performForwardSearch(activeDoc, lastSearchPattern, r, c + 1, foundR, foundC);
                            }
                            else {
                                success = performBackwardSearch(activeDoc, lastSearchPattern, r, c - 1, foundR, foundC);
                            }

                            if (success) {
                                r = foundR;
                                c = foundC;
                                clearCommandBar();
                            }
                            else {
                                printStatus("No more occurrences.");
                            }
                        }
                        else {
                            printStatus("No previous search pattern.");
                        }
                    }
                    else if (commandBuf.isEqual(Mystring("N"))) {
                        if (lastSearchPattern.Length() > 0) {
                            int foundR = -1, foundC = -1;
                            bool success = false;
                            if (lastSearchIsForward) {
                                success = performBackwardSearch(activeDoc, lastSearchPattern, r, c - 1, foundR, foundC);
                            }
                            else {
                                success = performForwardSearch(activeDoc, lastSearchPattern, r, c + 1, foundR, foundC);
                            }

                            if (success) {
                                r = foundR;
                                c = foundC;
                                clearCommandBar();
                            }
                            else {
                                printStatus("No more occurrences.");
                            }
                        }
                        else {
                            printStatus("No previous search pattern.");
                        }
                    }
                }
                else if (key == 8) { 
                    if (cmdBufLen > 0) {
                        cmdBufLen--;
                        cmdBufArray[cmdBufLen] = '\0';
                        clearCommandBar();
                        gotoRowCol(24, 0);
                        cout << ":" << cmdBufArray;
                    }
                }
                else if (key >= 32 && key <= 126) { 
                    if (cmdBufLen < 255) {
                        cmdBufArray[cmdBufLen++] = (char)key;
                        cmdBufArray[cmdBufLen] = '\0';
                        gotoRowCol(24, cmdBufLen);
                        cout << (char)key;
                    }
                }
            }
        }
    }

    return 0;
}