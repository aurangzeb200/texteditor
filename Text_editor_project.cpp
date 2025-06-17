#include <iostream>
#include <vector>
#include <cstdlib>
#include <conio.h>
#include <deque>  
#include <string>
#include <fstream> 

using namespace std;
struct Node {
    char data;
    Node* next;
    Node* prev;
    Node(char ch) : data(ch), next(nullptr), prev(nullptr) {}
};



struct EditorStatus {
public:
    int cursorColumn;
    int totalLines;
    int index;
    string commandStatus;
    string lastCommand;
    deque<string> commandHistory;
    EditorStatus() : cursorColumn(0), totalLines(1), index(0), lastCommand(""), commandStatus("") {}

    void addCommandToHistory(const string& command) {
        commandHistory.push_back(command);
        lastCommand = command;
    }

    void displayCommandHistory() const {
        if (commandHistory.empty()) {
            cout << "No command history available." << endl;
            return;
        }
        cout << "Command History:" << endl;
        for (int i = 0; i < commandHistory.size(); i++) {
            if (i == index) {
                cout << " > " << commandHistory[i] << " <" << endl;
            }
        }
    }

    void moveToNextCommand() {
        if (commandHistory.empty()) {
            commandStatus = "No command history available.";
            return;
        }

        if (index >= commandHistory.size() - 1) {
            commandStatus = "Already at the most recent command.";
            return;
        }

        index++;
    }

    void moveToPreviousCommand() {
        if (commandHistory.empty()) {
            commandStatus = "No command history available.";
            return;
        }

        if (index <= 0) {
            commandStatus = "Already at the oldest command.";
            return;
        }
        index--;
    }
};

class Line {
private:
    Node* head;
    Node* cursor;
    bool cursorAtHead;
    bool isWordCharacter(char c) {
        return isalnum(c) || c == '_';
    }
    bool isPunctuation(char c) {
        return ispunct(c) || c == ' ';
    }
public:
    Line() : head(nullptr), cursor(nullptr), cursorAtHead(true) {}
    
    void insertChar(char ch) {
        Node* newNode = new Node(ch);

        if (head == nullptr) {
            head = newNode;
            cursor = newNode;
            cursorAtHead = false;
        }
        else if (cursorAtHead) {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
            cursor = newNode;
            cursorAtHead = false;
        }
        else {
            newNode->next = cursor ? cursor->next : nullptr;
            newNode->prev = cursor;
            if (cursor->next != nullptr) {
                cursor->next->prev = newNode;
            }
            cursor->next = newNode;
            cursor = newNode;
        }
    }

    bool moveRight() {
        if (head == nullptr) {
            return true;
        }
        else if (cursor == nullptr) {
            cursor = head;
            cursorAtHead = false;
        }
        else if (cursor->next != nullptr) {
            cursor = cursor->next;
        }
        else if (cursor->next == nullptr) {
            cursorAtHead = false;
            return true;
        }
        return false;
    }

    bool moveLeft() {
        if (head == nullptr) {
            return true;
        }
        else if (cursor == head) {
            cursor = nullptr;
            cursorAtHead = true;
        }
        else if (cursor == nullptr) {
            cursor = nullptr;
            cursorAtHead = true;
            return true;
        }
        else if (cursor->prev != nullptr) {
            cursor = cursor->prev;
        }
        return false;
    }

    void display(bool displayCursor) const {
        Node* temp = head;
        if (displayCursor && cursorAtHead && cursor == nullptr) {
            cout << "|";
        }
        while (temp != nullptr) {
            cout << temp->data;
            if (displayCursor && temp == cursor) {
                cout << "|";
            }
            temp = temp->next;
        }
        if (displayCursor && cursor == nullptr && !cursorAtHead) {
            cout << "|";
        }
    }

    void resetCursor() {
        cursor = nullptr;
        cursorAtHead = true;
    }
    Node* getCursor() const {
        return cursor;
    }
    Node* getHead()  const{
        return head;
    }
    void deleteAfterCursor() {
        Node* temp;
        if (cursor == nullptr) {
            temp = head;
            head = nullptr;
        }
        else {
            temp = cursor->next;
            cursor->next = nullptr;
        }

        while (temp) {
            Node* toDelete = temp;
            temp = temp->next;
            delete toDelete;
        }
    }
    void cursorAtEnd() {
        if (head != nullptr) {
            if (cursor == nullptr) {
                cursor = head;
                cursorAtHead = false;
            }
            while (cursor->next != nullptr) {
                cursor = cursor->next;
            }
        }
        else {
            cursor = nullptr;
            cursorAtHead = true;
        }
    }
    void deleteChAtCursor() {
        if (cursor == head) {
            Node* toDelete = head;
            head = head->next;
            if (head != nullptr) {
                head->prev = nullptr;
            }
            cursor = nullptr;
            cursorAtHead = true;
            delete toDelete;
        }
        else {
            Node* toDelete = cursor;
            if (cursor->prev != nullptr) {
                cursor->prev->next = cursor->next;
            }
            if (cursor->next != nullptr) {
                cursor->next->prev = cursor->prev;
                cursor = cursor->prev;
            }
            else {
                cursor = cursor->prev;
            }
            delete toDelete;
        }
    }
    void setCursor(Node* c) {
        cursor = c;
    }
    void sethead(bool s) {
        if (s) cursorAtHead = true;
        else cursorAtHead = false;
    }
    void mtnw() {
        if (head == nullptr) return;
        if (cursor == nullptr) {
            cursor = head;
            cursorAtHead = false;
        }
        while (cursor->next != nullptr && !isPunctuation(cursor->next->data)) {
            cursor = cursor->next;
        }
        while (cursor->next != nullptr && isPunctuation(cursor->next->data)) {
            cursor = cursor->next;
        }
    }
    void mtpw() {
        if (head == nullptr) return;

        if (cursor == head || cursor == nullptr) {
            cursorAtHead = true;
            cursor = nullptr;
            return;
        }
        while (isWordCharacter(cursor->data)) {
            cursor = cursor->prev;
            if (cursor == head) {
                cursorAtHead = true;
                cursor = nullptr;
                return;
            }
        }
        while (isPunctuation(cursor->data)) {
            cursor = cursor->prev;
            if (cursor == head) {
                cursorAtHead = true;
                cursor = nullptr;
                return;
            }
        }
        while (isWordCharacter(cursor->data)) {
            cursor = cursor->prev;
            if (cursor == head) {
                cursorAtHead = true;
                cursor = nullptr;
                return;
            }
        }
    }
    void mtwe() {
        if (head == nullptr) return;
        if (cursor == nullptr) {
            cursor = head;
            cursorAtHead = false;
        }
        while (cursor->next != nullptr && isPunctuation(cursor->next->data)) {
            cursor = cursor->next;
        }
        while (cursor->next != nullptr && !isPunctuation(cursor->next->data)) {
            cursor = cursor->next;
        }
    }
    Node* getTail() {
        Node* temp = cursor;
        cursorAtEnd();
        Node* temp1 = cursor;
        cursor = temp;
        return temp1;
    }
    int getLength() {
        Node* temp = head;
        int length = 0;
        while (temp != nullptr) { 
            temp = temp->next;
            length++;
        }
        return length; 
    }
};


class TextEditor {
private:
    vector<Line> lines;
    int currentLine;
    bool insertMode;
    bool commandMode;
    string copyBuffer;
    string copyBuffer1;
    EditorStatus status;
    string lastPattern;
    int lastMatchLine;
    int lastMatchColumn;
    int colno;
public:
    TextEditor() : currentLine(0), insertMode(false), commandMode(0), lastMatchLine(0), lastMatchColumn(0), colno(0) {
        lines.emplace_back();
    }
    void toggleCommandMode() {
        commandMode = !commandMode;
    }
    bool isCommandMode() const {
        return commandMode;
    }

    void updateDisplay() {
        system("cls");
        for (int i = 0; i < lines.size(); i++) {
            if (i < 9) {
                cout << i + 1 << "  | ";
            }
            else cout << i + 1 << " | ";
            lines[i].display(i == currentLine);
            cout << endl;
        }
        updateStatusLine();
    }
    class FileManager {
    private:
        string currentFileName;
        bool modified;

    public:
        FileManager() : currentFileName(""), modified(false) {}

        bool loadFile(const string& filename, vector<Line>& lines) {
            ifstream file(filename);
            if (!file.is_open()) {
                return false;
            }
            lines.clear();
            string line;
            while (getline(file, line)) {
                Line lineObj;
                for (char c : line) {
                    lineObj.insertChar(c);
                }
                lines.push_back(lineObj);
            }
            file.close();
            return true;
        }

        bool saveFile(string& filename, vector<Line>& lines) {
            ofstream file(filename);
            if (!file) {
                return false;
            }

            for (auto& line : lines) {
                Node* temp = line.getHead();
                while (temp != nullptr) {
                    file << temp->data;
                    temp = temp->next;
                }
                file << '\n';
            }

            file.close();
            currentFileName = filename;
            modified = false;
            return true;
        }

        bool hasUnsavedChanges() const {
            return modified;
        }

        void markAsModified() {
            modified = true;
        }

        string getCurrentFileName() const {
            return currentFileName;
        }
    };
    void setLastCommand(const string& cmd) {
        status.lastCommand = cmd;
        status.addCommandToHistory(cmd);
    }
    FileManager fileManager;
    bool isWordCharacter(char c) {
        return isalnum(c) || c == '_';
    }
    bool isPunctuation(char c) {
        return ispunct(c) || " ";
    }

    void updateStatusLine() {
        checkCol();
        status.totalLines = lines.size();
        cout << "---------------------------" << endl;
        cout << (isInsertMode() ? "INSERT" : "NORMAL") << " | ";
        string filename = fileManager.getCurrentFileName();
        if (!filename.empty()) {
            cout << filename;
        }
        else {
            cout << "Untitled";
        }

        if (fileManager.hasUnsavedChanges()) {
            cout << " [+]";
        }
        cout << " | Line: " << currentLine + 1 << ", Col: " << status.cursorColumn;
        cout << " | Total Lines: " << status.totalLines;
        cout << " | Last Command: " << status.lastCommand << endl;
        cout << "Command Status: " << status.commandStatus << endl;
        status.displayCommandHistory();
        cout << "---------------------------" << endl;
    }




    void insertChar(char ch) {
        lines[currentLine].insertChar(ch);
        fileManager.markAsModified();
        setLastCommand("Insert Char: " + string(1, ch));
    }

    void moveRight() {
        if (lines[currentLine].moveRight()) {
            if (currentLine < lines.size() - 1) {
                currentLine++;
                lines[currentLine].resetCursor();
            }
        }
    }

    void moveLeft() {
        if (lines[currentLine].moveLeft()) {
            if (currentLine > 0) {
                currentLine--;
                lines[currentLine].cursorAtEnd();
            }
        }
    }

    void moveUp() {
        if (currentLine > 0) {
            currentLine--;
        }
    }

    void moveDown() {
        if (currentLine < lines.size() - 1) {
            currentLine++;
        }
    }

    void newLine() {
        status.commandStatus = "New Line Added Successfully!!!!";
        fileManager.markAsModified();

        if (currentLine <= -1) {
            lines.emplace_back();
            currentLine = 0;
        }

        else {
            Line newLine;
            Node* cursor = lines[currentLine].getCursor();
            if (cursor == nullptr) {
                cursor = lines[currentLine].getHead();
                if (cursor != nullptr) {
                    newLine.insertChar(cursor->data);
                }
            }

            while (cursor && cursor->next) {
                cursor = cursor->next;
                newLine.insertChar(cursor->data);
            }

            lines[currentLine].deleteAfterCursor();
            lines.insert(lines.begin() + currentLine + 1, newLine);
            currentLine++;
        }

        lines[currentLine].resetCursor();
    }

    void enterInsertMode() {
        insertMode = true;
    }

    void exitInsertMode() {
        insertMode = false;
    }

    bool isInsertMode() const {
        return insertMode;
    }

    void deleteCurrentLine() {
        if (currentLine < 0 || currentLine >= lines.size()) {
            status.commandStatus = "Failed To Delete Line.";
            return;
        }
        fileManager.markAsModified();

        if (lines.size() == 1) {
            lines[currentLine].setCursor(nullptr);
            lines[currentLine].deleteAfterCursor();
            return;
        }

        if (currentLine == 0) {
            lines.erase(lines.begin());
            currentLine = 0;
        }
        else {
            lines.erase(lines.begin() + currentLine);
            currentLine--;
        }

        if (!lines.empty()) {
            lines[currentLine].cursorAtEnd();
        }
        status.commandStatus = "Line Deleted Successfully!";
    }


    void deleteToEndOfLine() {
        fileManager.markAsModified();
        lines[currentLine].deleteAfterCursor();
        status.commandStatus = "Deleted Successfully!";
    }
    void deleteCharacterAtCursor() {
        fileManager.markAsModified();
        Node* cursor = lines[currentLine].getCursor();
        if (lines[currentLine].getHead() == nullptr) {
            deleteCurrentLine();
        }
        else if (cursor == nullptr && currentLine > 0) {
            lines[currentLine - 1].cursorAtEnd();
            cursor = lines[currentLine].getHead();
            lines[currentLine - 1].insertChar(cursor->data);
            while (cursor && cursor->next) {
                cursor = cursor->next;
                lines[currentLine - 1].insertChar(cursor->data);
            }
            deleteCurrentLine();
        }
        else if (cursor == nullptr && currentLine == 0) {}
        else lines[currentLine].deleteChAtCursor();
        status.commandStatus = "Deleted Character Successfully!";
    }
    void backspace() {
        deleteCharacterAtCursor();
    }
    void yankLine(bool append = false) {
        if (append == true) {
            if (currentLine + 1 >= 0 && currentLine + 1 < lines.size()) {
                Node* temp = lines[currentLine + 1].getHead();
                copyBuffer1.clear();

                while (temp != nullptr) {
                    copyBuffer1 += temp->data;
                    temp = temp->next;
                }
            }
            status.commandStatus = "Copying Successfully!";
        }
        else {
            if (currentLine >= 0 && currentLine < lines.size()) {
                Node* temp = lines[currentLine].getHead();
                copyBuffer.clear();

                while (temp != nullptr) {
                    copyBuffer += temp->data;
                    temp = temp->next;
                }
            }
            status.commandStatus = "Copying Successfully!";
        }
    }
    void pasteAfter(bool paste = false) {
        fileManager.markAsModified();
        if (paste) {
            Line line;
            if (copyBuffer1.empty()) {
                newLine();
                return;
            }
            for (char ch : copyBuffer1) {
                line.insertChar(ch);
            }
            if (currentLine + 1 < lines.size()) {
                lines.insert(lines.begin() + currentLine + 1, line);
            }
            else {
                lines.push_back(line);
            }
            currentLine++;
            status.commandStatus = "Pasting After Successfully!";
        }
        else {
            Line line;
            if (copyBuffer.empty()) {
                newLine();
                return;
            }
            for (char ch : copyBuffer) {
                line.insertChar(ch);
            }
            if (currentLine + 1 < lines.size()) {
                lines.insert(lines.begin() + currentLine + 1, line);
            }
            else {
                lines.push_back(line);
            }
            currentLine++;
            status.commandStatus = "Pasting After Successfully!";
        }
    }
    void pasteBefore(bool paste = false) {
        fileManager.markAsModified();
        if (paste) {
            Line line;
            if (copyBuffer.empty()) {
                newLine();
                moveUp();
                return;
            }
            for (char ch : copyBuffer) {
                line.insertChar(ch);
            }
            lines.insert(lines.begin() + currentLine, line);
            currentLine++;
            lines[currentLine].cursorAtEnd();
            status.commandStatus = "Pasting Before Successfully!";
        }
        else {
            Line line;
            if (copyBuffer1.empty()) {
                newLine();
                moveUp();
                return;
            }
            for (char ch : copyBuffer1) {
                line.insertChar(ch);
            }
            lines.insert(lines.begin() + currentLine, line);
            currentLine++;
            lines[currentLine].cursorAtEnd();
            status.commandStatus = "Pasting Before Successfully!";
        }
    }
    void moveToStartOfLine() {
        lines[currentLine].resetCursor();
        status.commandStatus = "Successfully Reached At Start.";
    }
    void moveToEndOfLine() {
        lines[currentLine].cursorAtEnd();
        status.commandStatus = "Successfully Reached At End.";
    }

    void moveToNextWord() {
        if (currentLine >= lines.size() - 1 && lines[currentLine].getCursor()->next == nullptr) {
            status.commandStatus = "No Next Word Exist.";
            return;
        }
        status.commandStatus = "Successfully Moved To Next Word.";
        if (lines[currentLine].getCursor() != nullptr && lines[currentLine].getHead() != nullptr) {
            if (currentLine < lines.size() - 1 && lines[currentLine].getCursor()->next == nullptr) {
                moveDown();
                lines[currentLine].resetCursor();
                if (isWordCharacter(lines[currentLine].getHead()->data)) {
                    return;
                }
            }
        }
        lines[currentLine].mtnw();
    }
    void moveToPreviousWord() {
        if (lines[currentLine].getCursor() == nullptr) {
            if (currentLine > 0) {
                moveUp();
                lines[currentLine].cursorAtEnd();
                status.commandStatus = "Successfully Moved To Previous Word.";
                return;
            }
            else {
                status.commandStatus = "No Previous Word Exist.";
            }
        }
        lines[currentLine].mtpw();
        status.commandStatus = "Successfully Moved To Previous Word.";
    }
    void moveToWordEnd() {
        if (lines[currentLine].getCursor() != nullptr && lines[currentLine].getHead() != nullptr) {
            if (currentLine < lines.size() - 1 && lines[currentLine].getCursor()->next == nullptr) {
                moveDown();
                lines[currentLine].resetCursor();
                if (isWordCharacter(lines[currentLine].getHead()->data)) {
                    status.commandStatus = "Successfully Moved To The End Of Word.";
                    return;
                }
            }
            else status.commandStatus = "No Next Word Exist.";
        }
        lines[currentLine].mtwe();
        status.commandStatus = "Successfully Moved To The End Of Word.";
    }
    void checkCol() {
        if (lines[currentLine].getCursor() == lines[currentLine].getHead() || lines[currentLine].getCursor() == nullptr) {
            status.cursorColumn = 1;
            return;
        }
        status.cursorColumn = 0;
        Node* realpos = lines[currentLine].getCursor();
        lines[currentLine].mtnw();
        Node* reachpos = lines[currentLine].getCursor();
        lines[currentLine].setCursor(lines[currentLine].getHead());
        while (lines[currentLine].getCursor() != reachpos) {
            lines[currentLine].mtnw();
            status.cursorColumn += 1;
        }
        if (realpos == nullptr) {
            lines[currentLine].resetCursor();
            return;
        }
        lines[currentLine].setCursor(realpos);
    }

    void handleFileError(const string& operation, const string& filename) {
        if (operation == "saveFile") {
            if (errno == ENOENT) {
                status.commandStatus = "Error: File not found: " + filename;
            }
            else if (errno == EACCES) {
                status.commandStatus = "Error: Permission denied: " + filename;
            }
            else if (errno == ENOSPC) {
                status.commandStatus = "Error: Disk full. Cannot save file: " + filename;
            }
            else if (errno == EEXIST) {
                status.commandStatus = "Error: File already exists: " + filename;
            }
            else {
                status.commandStatus = "Error saving file to " + filename;
            }
        }
        else if (operation == "loadFile") {
            if (errno == ENOENT) {
                status.commandStatus = "Error: File not found: " + filename;
            }
            else if (errno == EACCES) {
                status.commandStatus = "Error: Permission denied: " + filename;
            }
            else {
                status.commandStatus = "Error loading file: " + filename;
            }
        }
    }

    bool handleFileCommand(const string& cmd) {
        if (cmd == ":q") {
            if (fileManager.hasUnsavedChanges()) {
                status.commandStatus = "Unsaved changes. Use :q! to quit without saving.";
                return false;
            }
            exit(0);
        }
        else if (cmd == ":q!") {
            status.commandStatus = "Force quit without saving.";
            exit(0);
        }
        else if (cmd.find(":w ") == 0) {
            string filename = cmd.substr(3);
            if (fileManager.saveFile(filename, lines)) {
                status.commandStatus = "File saved to " + filename;
            }
            else {
                status.commandStatus = "saveFile", filename;
            }
        }
        else if (cmd == ":w") {
            string filename = fileManager.getCurrentFileName();
            if (filename.empty()) {
                status.commandStatus = "No file name specified. Use :w <filename> to save.";
            }
            else if (fileManager.saveFile(filename, lines)) {
                status.commandStatus = "File saved to " + filename;
            }
            else {
                status.commandStatus = "saveFile", filename;
            }   
        }
        else if (cmd == ":wq") {
            string filename = fileManager.getCurrentFileName();
            if (filename.empty()) {
                status.commandStatus = "No file name specified. Use :w <filename> to save.";
            }
            else if (fileManager.saveFile(filename, lines)) {
                status.commandStatus = "File saved. Exiting...";
                exit(0);
            }
            else {
                status.commandStatus = "saveFile", filename;
            }
        }
        else if (cmd.find(":e ") == 0) {
            string filename = cmd.substr(3);
            if (fileManager.loadFile(filename, lines)) {
                status.commandStatus = "File loaded successfully.";
            }
            else {
                status.commandStatus = "loadFile", filename;
            }
        }
        else {
            status.commandStatus = "Unknown command: " + cmd;
            return false;
        }
        return true;
    }


    void joinLines() {
        fileManager.markAsModified();
        if (currentLine == -1) {
            return;
        }
        if (currentLine < lines.size() - 1) {
            Node* cursor = lines[currentLine].getHead();
            lines[currentLine + 1].cursorAtEnd();
            cursor = lines[currentLine].getHead();
            lines[currentLine + 1].insertChar(cursor->data);
            while (cursor && cursor->next) {
                cursor = cursor->next;
                lines[currentLine + 1].insertChar(cursor->data);
            }
            deleteCurrentLine();
            status.commandStatus = "Joined Successfully";
        }
        else status.commandStatus = "No More Lines To Join";
    }
    void indentLine() {
        Node* cursor = lines[currentLine].getCursor();
        lines[currentLine].resetCursor();
        char ch = ' ';
        lines[currentLine].insertChar(ch);
        lines[currentLine].insertChar(ch);
        lines[currentLine].setCursor(cursor);
        status.commandStatus = "Indentation Successfully";
    }
    void UnindentLine() {
        Node* cursor = lines[currentLine].getCursor();
        Node* cursor1 = lines[currentLine].getHead();
        lines[currentLine].setCursor(cursor1);
        moveToNextWord();
        if (cursor1->data == ' ') {
            deleteCharacterAtCursor();
            deleteCharacterAtCursor();
        }
        lines[currentLine].setCursor(cursor);
        status.commandStatus = "Unindentation Successfully";
    }
    void deleteLineNumber(int lineNum) {
        int line = currentLine;
        currentLine = lineNum - 1;
        deleteCurrentLine();
        if (currentLine == 0) {
            currentLine = 0;
            return;
        }
        currentLine = line - 1;
        status.commandStatus = "Deleting " + to_string(lineNum) + " Successfully";
    }

    bool search(const string& pattern) {
        if (pattern.empty()) {
            return false;
        }

        char first = pattern[0];
        int i = 0;
        for (int k = 0; k < lines.size(); k++) {

            Node* originalCursor = lines[k].getCursor();

            lines[k].setCursor(lines[k].getHead());

            Node* cursor = lines[k].getCursor();
            while (cursor != nullptr) {
                i++;
                if (cursor->data == first) {
                    Node* startMatchCursor = cursor;
                    Node* tempCursor = cursor;
                    int matchIndex = 0;

                    while (matchIndex < pattern.length() &&
                        tempCursor != nullptr &&
                        tempCursor->data == pattern[matchIndex]) {
                        tempCursor = tempCursor->next;
                        matchIndex++;
                    }

                    if (matchIndex == pattern.length()) {
                        if (lines[k].getHead() != startMatchCursor) {
                            startMatchCursor = startMatchCursor->prev;
                            lines[k].setCursor(startMatchCursor);
                        }
                        else {
                            lines[k].resetCursor();
                        }
                        checkCol();
                        currentLine = k;
                        lastMatchLine = k + 1;
                        colno = i - 1;
                        lastMatchColumn = status.cursorColumn;
                        return true;
                    }
                }
                cursor = cursor->next;
            }

            lines[k].setCursor(originalCursor);
        }
        return false;
    }
    void searchpattern(const string& pattern) {
        lastPattern = pattern;
        string s;
        if (search(pattern)) {
            s += "line: ";
            s += to_string(lastMatchLine);
            s += " column: ";
            s += to_string(lastMatchColumn);
            status.commandStatus = s;
        }
        else {
            s = pattern;
            s += " not found";
            status.commandStatus = s;
        }
    }
    
    void findNextOccurrence() {
        string s;
        if (lastPattern.empty()) {
            s = lastPattern + " not found";
            status.commandStatus = s;
            return;
        }
        if (lastMatchLine < 0 || lastMatchColumn < 0) {
            s = "Invalid col and line";
            status.commandStatus = s;
            return;
        }
        int originalLine = lastMatchLine - 1;
        currentLine = lastMatchLine - 1;
        Node* originalCursor = lines[currentLine].getCursor();
        lines[currentLine].setCursor(lines[currentLine].getHead());
        Node* cursor = lines[currentLine].getCursor();
        for (int col = 0; col < colno; col++) {
            cursor = cursor->next;
        }
        for (int i = 0; i < lastPattern.length() && cursor != nullptr; i++) {
            cursor = cursor->next;
            colno++;
        }
        lines[currentLine].setCursor(cursor);
        char firstChar = lastPattern[0];
        while (cursor != nullptr) {
            colno++;
            if (cursor->data == firstChar) {
                Node* startMatchCursor = cursor;
                Node* tempCursor = cursor;
                int matchIndex = 0;

                while (matchIndex < lastPattern.length() &&
                    tempCursor->data == lastPattern[matchIndex]) {
                    tempCursor = tempCursor->next;
                    matchIndex++;
                }

                if (matchIndex == lastPattern.length()) {
                    if (lines[currentLine].getHead() != startMatchCursor) {
                        startMatchCursor = startMatchCursor->prev;
                        lines[currentLine].setCursor(startMatchCursor);
                    }
                    else {
                        lines[currentLine].resetCursor();
                    }
                    checkCol();
                    lastMatchLine = currentLine + 1;
                    currentLine = originalLine;
                    colno--;
                    lastMatchColumn = status.cursorColumn;
                    s += "line: ";
                    s += to_string(lastMatchLine);
                    s += " column: ";
                    s += to_string(lastMatchColumn);
                    status.commandStatus = s;
                    return;
                }
            }
            cursor = cursor->next;
        }
        lines[currentLine].setCursor(originalCursor);
        for (int k = currentLine + 1; k < lines.size(); k++) {
            colno = 0;
            Node* originalCursor = lines[k].getCursor();
            lines[k].setCursor(lines[k].getHead());
            Node* cursor = lines[k].getCursor();
            while (cursor != nullptr) {
                colno++;
                if (cursor->data == firstChar) {
                    Node* startMatchCursor = cursor;
                    Node* tempCursor = cursor;
                    int matchIndex = 0;

                    while (matchIndex < lastPattern.length() &&
                        tempCursor->data == lastPattern[matchIndex]) {
                        tempCursor = tempCursor->next;
                        matchIndex++;
                    }

                    if (matchIndex == lastPattern.length()) {
                        currentLine = k;
                        if (lines[currentLine].getHead() != startMatchCursor) {
                            startMatchCursor = startMatchCursor->prev;
                            lines[currentLine].setCursor(startMatchCursor);
                        }
                        else {
                            lines[currentLine].resetCursor();
                        }
                        checkCol();
                        lastMatchLine = currentLine + 1;
                        lastMatchColumn = status.cursorColumn;
                        colno--;
                        s += "line: ";
                        s += to_string(lastMatchLine);
                        s += " column: ";
                        s += to_string(lastMatchColumn);
                        status.commandStatus = s;
                        return;
                    }
                }
                cursor = cursor->next;
            }
        }
        lines[currentLine].setCursor(originalCursor);
        currentLine = lastMatchLine - 1;
        s = lastPattern;
        s += " not found";
        status.commandStatus = s;
        return;
    }
    void findPreviousOccurrence() {
        string s;
        if (lastPattern.empty()) {
            s = lastPattern + " not found";
            status.commandStatus = s;
            return;
        }
        if (lastMatchLine < 0 || lastMatchColumn < 0) {
            s = "Invalid col and line";
            status.commandStatus = s;
            return;
        }

        int originalLine = lastMatchLine - 1;
        currentLine = lastMatchLine - 1;
        Node* originalCursor = lines[currentLine].getCursor();
        lines[currentLine].setCursor(lines[currentLine].getHead());
        Node* cursor = lines[currentLine].getCursor();

        for (int col = 0; col < colno - 1 && cursor != nullptr; col++) {
            cursor = cursor->next;
        }
        lines[currentLine].setCursor(cursor);

        while (colno > 0) {
            colno--;
            if (cursor != nullptr && cursor->data == lastPattern.back()) {
                Node* startMatchCursor = cursor;
                Node* tempCursor = cursor;
                int matchIndex = lastPattern.length() - 1;

                while (matchIndex >= 0 && tempCursor != nullptr && tempCursor->data == lastPattern[matchIndex]) {
                    tempCursor = tempCursor->prev;
                    matchIndex--;
                }

                if (matchIndex < 0) { 
                    lastMatchLine = currentLine + 1;
                    if (startMatchCursor->prev != nullptr) {
                        lines[currentLine].setCursor(startMatchCursor->prev->prev);
                    }
                    else {
                        lines[currentLine].resetCursor();
                    }
                    checkCol();
                    lastMatchColumn = status.cursorColumn;

                    s = "line: " + to_string(lastMatchLine) +
                        " column: " + to_string(lastMatchColumn);
                    status.commandStatus = s;
                    return;
                }
            }
            cursor = cursor ? cursor->prev : nullptr;
        }
        lines[currentLine].setCursor(originalCursor);

        for (int k = currentLine - 1; k >= 0; k--) {
            colno = lines[k].getLength();
            lines[k].setCursor(lines[k].getTail());
            cursor = lines[k].getCursor();

            while (colno > 0) {
                colno--;
                if (cursor != nullptr && cursor->data == lastPattern.back()) {
                    Node* startMatchCursor = cursor;
                    Node* tempCursor = cursor;
                    int matchIndex = lastPattern.length() - 1;

                    while (matchIndex >= 0 && tempCursor != nullptr && tempCursor->data == lastPattern[matchIndex]) {
                        tempCursor = tempCursor->prev;
                        matchIndex--;
                    }

                    if (matchIndex < 0) { 
                        currentLine = k;
                        lastMatchLine = currentLine + 1;
                        if (startMatchCursor->prev != nullptr) {
                            lines[currentLine].setCursor(startMatchCursor->prev->prev);
                        }
                        else {
                            lines[currentLine].resetCursor();
                        }
                        checkCol();
                        lastMatchColumn = status.cursorColumn;

                        s = "line: " + to_string(lastMatchLine) +
                            " column: " + to_string(lastMatchColumn);
                        status.commandStatus = s;
                        return;
                    }
                }
                cursor = cursor ? cursor->prev : nullptr;
            }
            lines[k].resetCursor();
        }
            currentLine = originalLine;
        lines[currentLine].setCursor(originalCursor);
        s = lastPattern + " not found";
        status.commandStatus = s;
        return;
    }

    bool replace(const string& old, const string& newStr, bool global = false) {
        if (old.empty()) {
            return false; 
        }
        bool replaced = false;
        while (search(old)) {
            replaced = true;
            for (int i = 0; i < old.size(); i++) {
                moveRight();
            }
            for (int i = 0; i < old.size(); i++) {
                lines[currentLine].deleteChAtCursor();
            }
            for (int i = 0; i < newStr.size(); i++) {
                lines[currentLine].insertChar(newStr[i]);
            }
            if (!global) {
                return true; 
            }
        }
        return replaced; 
    }
    EditorStatus& getEditorStatus() {
        return status; 
    }
};


int getChar() {
    int ch = _getch();
    if (ch == 0 || ch == 224) {
        ch = _getch();
        switch (ch) {
        case 72: return 65;  
        case 80: return 66;  
        case 77: return 67;  
        case 75: return 68;  
        }
    }
    return ch;
}

void printCommandPrompt(const string& command) {
    system("cls");  
    cout << "=====================================\n";
    cout << "          Text Editor Command       \n";
    cout << "=====================================\n";
    cout << "Command: " << command << "\n";
    cout << "Press ESC to cancel, ENTER to execute.\n";
    cout << "=====================================\n";
}

int main() {
    TextEditor editor;
    int cmd;
    bool paste = false;
    int numberPrefix = 0;
    string lastSearchPattern;
    bool inCommandMode = false; 
    EditorStatus& status = editor.getEditorStatus();
    while (true) {
        if (!inCommandMode) {
            editor.updateDisplay();
        }
        cmd = getChar();

        if (cmd == ':') {
            string command;
            command += static_cast<char>(cmd);
            printCommandPrompt(command);  
            inCommandMode = true;  

            while (inCommandMode) {
                cmd = getChar();
                if (cmd == 27) {  
                    status.commandStatus = "Command canceled. Returning to editor.";
                    inCommandMode = false;
                    break;
                }

                if (cmd == 13) {  
                    status.commandStatus = "Executing command: " + command;
                    editor.handleFileCommand(command);
                    inCommandMode = false;
                    break;
                }
                command += static_cast<char>(cmd);
                printCommandPrompt(command);
            }
            if (command[1] == 'h' && cmd == 13) {
                inCommandMode = false;
                while (true) {
                    system("cls");
                    editor.updateDisplay();
                    cmd = getChar();
                    if (cmd == 27) {
                        status.commandStatus = "Exiting history navigation. Returning to editor.";
                        editor.setLastCommand("Exiting From Command History....");
                        break;
                    }
                    if (cmd == 66) {  
                        status.moveToPreviousCommand();
                        status.lastCommand = "Moving To Previous Command.";
                    }
                    else if (cmd == 65) {  
                        status.moveToNextCommand();
                        status.lastCommand = "Moving To Next Command.";
                    }
                }
            }
            
            if (cmd == 13) {
                if (command.find(":s/") == 0) {
                    size_t firstSlash = command.find('/', 2);
                    size_t secondSlash = command.find('/', firstSlash + 1);

                    if (firstSlash != string::npos && secondSlash != string::npos) {
                        size_t thirdSlash = command.find('/', secondSlash + 1);

                        string oldStr = command.substr(firstSlash + 1, secondSlash - firstSlash - 1);
                        string newStr;
                        bool global = false;

                        if (thirdSlash != string::npos) {
                            newStr = command.substr(secondSlash + 1, thirdSlash - secondSlash - 1);
                            string remainder = command.substr(thirdSlash + 1);
                            global = (remainder.find("g") != string::npos);
                        }
                        else {
                            newStr = command.substr(secondSlash + 1);
                            size_t gFlagPos = newStr.find("/g");
                            if (gFlagPos != string::npos) {
                                newStr = newStr.substr(0, gFlagPos); 
                                global = true;
                            }
                        }

                        if (!oldStr.empty()) {
                            editor.setLastCommand("Replacing " + oldStr + " with " + newStr);
                            bool success = editor.replace(oldStr, newStr, global);
                            if (success) {
                                status.commandStatus = "Replacement successful.";
                            }
                            else {
                                status.commandStatus = "Word not found. No replacements made.";
                            }
                        }
                        else {
                            status.commandStatus = "Invalid substitution: old string is empty.";
                        }
                    }
                    else {
                        status.commandStatus = "Invalid substitution command syntax. Correct format: :s/old/new/[g]";
                    }
                }
                editor.handleFileCommand(command);
                inCommandMode = false;  
                continue;
            }
        }

        if (cmd == 27) { 
            editor.setLastCommand("Exiting Insert Mode....");
            editor.exitInsertMode();
            inCommandMode = false;  
        }

        while (cmd >= '1' && cmd <= '9') {
            numberPrefix = numberPrefix * 10 + (cmd - '0');
            cmd = getChar();
        }

        if (numberPrefix == 0) {
            numberPrefix = 1;
        }
        
        if (editor.isInsertMode() && !inCommandMode) {
            switch (cmd) {
            case 65: editor.moveUp(); editor.setLastCommand("Move Up."); break;
            case 66: editor.moveDown(); editor.setLastCommand("Move Down."); break;
            case 67: editor.moveRight(); editor.setLastCommand("Move Right."); break;
            case 68: editor.moveLeft(); editor.setLastCommand("Move Left."); break;
            case 13: editor.newLine(); editor.setLastCommand("New Line."); break;
            case 8: editor.backspace(); editor.setLastCommand("Backspace."); break;
            default: editor.insertChar(static_cast<char>(cmd)); break;
            }
        }
        else {
            if (cmd == '/') {
                string pattern;
                cmd = getChar();
                while (cmd != '\n' && cmd != 27) {
                    pattern += static_cast<char>(cmd);
                    cmd = getChar();
                }
                if (!pattern.empty()) {
                    lastSearchPattern = pattern;
                    editor.searchpattern(pattern);
                }
            }

            if (cmd == 'n') {
                editor.setLastCommand("Find Next Occurrence.");
                if (!lastSearchPattern.empty()) {
                    editor.findNextOccurrence();
                }
            }

            if (cmd == 'N') {
                editor.setLastCommand("Find Previous Occurrence.");
                if (!lastSearchPattern.empty()) {
                    editor.findPreviousOccurrence();
                }
            }

            if (cmd == 'd') {
                cmd = getChar();
                if (cmd == 'd') {
                    numberPrefix == 2 ? editor.setLastCommand("Deleting 2 Lines.") : editor.setLastCommand("Deleting Line.");
                    for (int i = 0; i < numberPrefix; ++i) {
                        editor.deleteCurrentLine();
                    }
                }
            }

            if (cmd == 'y') {
                cmd = getChar();
                if (cmd == 'y') {
                    if (numberPrefix == 2) {
                        paste = true;
                        editor.yankLine();
                        editor.yankLine(true);
                        editor.setLastCommand("Copying 2 Lines.");
                    }
                    else {
                        editor.yankLine();
                        paste = false;
                        editor.setLastCommand("Copy Line.");
                    }
                }
            }

            if (cmd == 'j') {
                editor.setLastCommand("Moving Down 5 Times.");
                for (int i = 0; i < numberPrefix - 1; ++i) {
                    editor.moveDown();
                }
            }

            switch (cmd) {
            case 'J': editor.joinLines(); editor.setLastCommand("Joining Lines."); break;
            case '>': cmd = getChar(); if (cmd == '>') { for (int i = 0; i < numberPrefix; ++i) editor.indentLine(); } editor.setLastCommand("Indentation."); break; 
            case '<': cmd = getChar(); if (cmd == '<') { for (int i = 0; i < numberPrefix; ++i) editor.UnindentLine(); } editor.setLastCommand("Unindentation."); break;
            case 'm': editor.deleteToEndOfLine(); editor.setLastCommand("Delete To End Of Line."); break;
            case 'x': editor.deleteCharacterAtCursor(); editor.setLastCommand("Delete Character At Cursor."); break;
            case 'p': 
                if (paste) {
                    editor.pasteAfter();
                    editor.pasteAfter(true);
                    editor.setLastCommand("Pasting 2 lines After."); break;
                }
                else { 
                    editor.pasteAfter(); 
                    editor.setLastCommand("Paste After."); break;
                }

                break;
            case 'P': 
                if (paste) {
                    editor.setLastCommand("Pasting 2 lines Before."); break;
                    editor.pasteBefore(true);
                    editor.pasteBefore();
                }
                else {
                    editor.pasteBefore();
                    editor.setLastCommand("Paste Before."); break;
                }
                break;
            case '0': editor.moveToStartOfLine(); editor.setLastCommand("Move To The Start Of Line."); break;
            case '$': editor.moveToEndOfLine(); editor.setLastCommand("Move To The End Of Line"); break;
            case 'w': editor.moveToNextWord(); editor.setLastCommand("Move To The Next Word."); break;
            case 'b': editor.moveToPreviousWord(); editor.setLastCommand("Move To The Previous Word."); break;
            case 'e': editor.moveToWordEnd(); editor.setLastCommand("Move To The Word End."); break;
            case 'q': editor.setLastCommand("Quting From Editor...."); return 0; break;
            case 'i': editor.enterInsertMode(); editor.setLastCommand("Entering Insert Mode...."); break;
            case 65: editor.moveUp(); editor.setLastCommand("Move Up."); break;
            case 66: editor.moveDown(); editor.setLastCommand("Move Down."); break;
            case 67: editor.moveRight(); editor.setLastCommand("Move Right."); break;
            case 68: editor.moveLeft(); editor.setLastCommand("Move Left."); break; 
            default: break;
            }
        }

        numberPrefix = 0;
    }
    return 0;
}