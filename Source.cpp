#include <iostream>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include <string>
using namespace std;

struct atr {
    string begin;
    string next;
    string True;
    string False;
    string addr;
};

string allidents[100];
int idents = 0;
int idents1 = 0;

bool isalpha(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool isdigit(char c) {
    return (c >= '0' && c <= '9');
}

struct tok {
    string val;
    int state = 0;
};

const int numofKeys = 61;
string keywords[numofKeys] = {
        "asm", "Wagarna", "new", "this", "auto", "enum", "operator", "throw", "Mantiqi", "explicit",
        "private", "True", "break", "export", "protected", "try", "case", "extern", "public", "typedef",
        "catch", "False", "register", "typeid", "Harf", "Ashriya", "typename", "Adadi", "class", "for",
        "Wapas", "union", "const", "dost", "short", "unsigned", "goto", "signed", "using", "continue",
        "Agar", "sizeof", "virtual", "default", "inline", "static", "Khali", "delete", "volatile", "do",
        "long", "struct", "double", "mutable", "switch", "while", "namespace", "template", "Matn",
        "output<-", "input->"
};

char buffer1[25];
char buffer2[25];
int b1 = 0, b2 = 0;
bool eofreached = false;
ifstream file;
ofstream errfile;
int stritr = 0;
int Pstritr = 0;
string* alltokens = new string[1000];

char getnextchar() {
    if (b1 < 25 && buffer1[b1] != '\0') {
        b1++;
        return buffer1[b1 - 1];
    }
    else if (b2 < 25 && buffer2[b2] != '\0') {
        b1 = 0;
        buffer1[b1] = '\0';
        b2++;
        return buffer2[b2 - 1];
    }
    else {
        int x = 0, y = 0;
        b1 = 0;
        b2 = 0;
        bool commentchck1 = false, commentchck2 = false;
        while (x < 25 && eofreached == false) {
            char c;
            if (file.get(c)) {
                if (commentchck1 == true) {
                    if (c == '\n') {
                        commentchck1 = false;
                    }
                }
                else if (commentchck2 == true) {
                    if (c == '*') {
                        char c4;
                        file.get(c4);
                        if (c4 == '/') {
                            commentchck2 = false;
                        }
                    }
                }
                else {
                    if (c == ' ' || c == '\n') {
                        continue;
                    }
                    else if (c == '/') {
                        char c2;
                        file.get(c2);
                        if (c2 == '/') {
                            commentchck1 = true;
                        }
                        else if (c2 == '*') {
                            commentchck2 = true;
                        }
                        else {
                            buffer1[x] = c;
                            x++;
                            if (b1 >= 25) {
                                buffer2[y] = c2;
                                y++;
                            }
                            else {
                                buffer1[x] = c2;
                                x++;
                            }
                        }
                    }
                    else {
                        buffer1[x] = c;
                        x++;
                    }
                }
            }
            else {
                eofreached = true;
                buffer1[x] = '\0';
                buffer2[0] = '\0';
                break;
            }

        }
        while (y < 25 && eofreached == false) {
            char c;
            if (file.get(c)) {
                if (commentchck1 == true) {
                    if (c == '\n') {
                        commentchck1 = false;
                    }
                }
                else if (commentchck2 == true) {
                    if (c == '*') {
                        char c4;
                        file.get(c4);
                        if (c4 == '/') {
                            commentchck2 = false;
                        }
                    }
                }
                else {
                    if (c == ' ' || c == '\n') {
                        continue;
                    }
                    else if (c == '/') {
                        char c2;
                        file.get(c2);
                        if (c2 == '/') {
                            commentchck1 = true;
                        }
                        else if (c2 == '*') {
                            commentchck2 = true;
                        }
                        else {
                            buffer2[y] = c;
                            y++;
                            if (b2 >= 25) {
                                buffer1[x] = c2;
                                x++;
                            }
                            else {
                                buffer2[y] = c2;
                                y++;
                            }
                        }
                    }
                    else {
                        buffer2[y] = c;
                        y++;
                    }
                }
            }
            else {
                eofreached = true;
                buffer2[y] = '\0';
                break;
            }
        }
        if (buffer1[b1] != '\0') {
            b1++;
            return buffer1[b1 - 1];
        }
    }
}

void setpreviouschar() {
    if (b2 > 0) {
        b2--;
    }
    else if (b1 > 0) {
        b1--;
    }
}

bool eofcheck() {
    if (eofreached == true) {
        if (buffer2[b2] == '\0') {
            if (buffer1[b1] == '\0') {
                return true;
            }
            else return false;
        }
        else return false;
    }
    else return false;
}

tok tokenizer(int map[47][32], int advance[47][32], char inputs[], int rows, int cols) {
    int state = 0;
    char ch;
    if (!eofcheck()) {
        ch = getnextchar();
    }
    else {
        tok temp;
        temp.state = -9999;
        return temp;
    }
    int cha;
    string token;
    while (map[state][31] != 1) {
        cha = -1;

        //o1
        if (state == 8 && (ch != '=' && ch != '+')) {
            cha = 24;
        }

        //o2
        else if (state == 14 && (ch != '=' && ch != '>')) {
            cha = 25;
        }

        //o3
        else if (state == 17 && (ch != '=' && ch != '>' && ch != '<')) {
            cha = 26;
        }

        // input keyword
        else if (state == 23 && (token == "input" && ch == '-')) {
            if (!eofcheck()) {
                token += ch;
                ch = getnextchar();
                continue;
            }
            else {
                tok temp;
                temp.state = state;
                temp.val = token;
                return temp;
            }
        }
        else if (state == 23 && (token == "input-" && ch == '>')) {
            tok temp;
            token += ch;
            temp.state = 24;
            temp.val = token;
            return temp;
        }

        // output keyword
        else if (state == 23 && (token == "output" && ch == '<')) {
            if (!eofcheck()) {
                token += ch;
                ch = getnextchar();
                continue;
            }
            else {
                tok temp;
                temp.state = state;
                temp.val = token;
                return temp;
            }
        }
        else if (state == 23 && (token == "output<" && ch == '-')) {
            tok temp;
            token += ch;
            temp.state = 24;
            temp.val = token;
            return temp;
        }

        //o4 keyword
        else if (state == 23 && (!isalpha(ch) && !isdigit(ch) && ch != '_')) {
            cha = 27;
        }

        //o4 id
        else if (state == 25 && (!isalpha(ch) && !isdigit(ch) && ch != '_')) {
            cha = 27;
        }

        //o5
        else if (state == 29 && (!isdigit(ch))) {
            cha = 28;
        }

        //o6 
        else if (state == 28 && (ch != '.' && ch != 'E' && !isdigit(ch))) {
            cha = 29;
        }

        //o6
        else if (state == 33 && (ch != 'E' && !isdigit(ch))) {
            cha = 29;
        }

        //o6
        else if (state == 37 && (ch != '.' && ch != 'E' && !isdigit(ch))) {
            cha = 29;
        }

        //o6
        else if (state == 40 && (ch != 'E' && !isdigit(ch))) {
            cha = 29;
        }

        //o7
        else if (state == 44 && (ch != '"')) {
            cha = 30;
        }

        else if (isalpha(ch) == true && (state != 28 && state != 33 && state != 37 && state != 40)) {
            cha = 21;
        }
        else if (isdigit(ch) == true) {
            cha = 22;
        }

        else {
            for (int i = 0; i < cols; i++) {
                if (ch == inputs[i]) {
                    cha = i;
                    break;
                }
            }
        }

        if (cha == -1) {
            // lexical error, invalid input
            errfile << ch;
            for (int i = 0; i < numofKeys; i++) {
                if (token == keywords[i]) {
                    tok t;
                    t.val = token;
                    t.state = 24;
                    return t;
                }
            }
            if (!token.empty()) {
                tok temp1;
                temp1.val = token;
                temp1.state = 24;
                return temp1;
            }
            else {
                char errchar = getnextchar();
                tok t22;
                t22.state = -9999;
                return t22;
            }
        }

        int newstate = map[state][cha];
        if (advance[state][cha] == 1) {
            state = newstate;
            token += ch;
            if (state == 23) {
                for (int i = 0; i < numofKeys; i++) {
                    if (token == keywords[i]) {
                        tok t;
                        t.val = token;
                        t.state = 24;
                        return t;
                    }
                }
            }

            if (map[state][31] == 1) {
                break;
            }

            if (!eofcheck()) {
                ch = getnextchar();
            }
            else {
                tok temp;
                temp.state = state;
                temp.val = token;
                return temp;
            }

        }
        else if (advance[state][cha] == 0) {
            state = newstate;
            setpreviouschar();
            break;
        }
        else {
            tok tempp;
            tempp.val = token;
            tempp.state = state;
            setpreviouschar();
            return tempp;
        }
    }

    tok t1;
    t1.val = token;
    t1.state = state;
    return t1;
}


void Lexer() {
    const int rows = 47, cols = 32, starStates = 8;

    int states[rows];
    for (int i = 0; i < rows; i++) {
        states[i] = i;
    }
    // inputs
    char inputs[cols] = { '[', ']', '{', '}', '(', ')', ':', '*', '/', '%', '!', '&', '=', '|', '+', '<', '>', '"', '-', '.', '_', 'l', 'd', 'E', '1', '2', '3', '4', '5', '6', '7', 'A' };

    // transition table
    int map[rows][cols] = {
        //      { [,  ],  {,  },  (,  ),  :,  *,  /,  %,  !,  &,  =,  |,  +,  <,  >,  ",  -,  .,  _,  l,  d,  E,  1,  2,  3,  4,  5,  6,  7,  A}
                {21, 21, 21, 21, 21, 21, 46,  1,  1,  1,  2,  4, 12,  6,  8, 17, 14, 44, 29, -1, 41, 23, 28, -1, -1, -1, -1, -1, -1, -1, -1, -1}, //0 start
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //1
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, //2
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //3
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, //4
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //5
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, //6
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //7
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 10, -1,  9, -1, -1, -1, -1, -1, -1, -1, 28, -1, 11, -1, -1, -1, -1, -1, -1, -1}, //8
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //9
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //10
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //11
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, //12
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //13
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 15, -1, -1, -1, 43, -1, -1, -1, -1, -1, -1, -1, -1, 16, -1, -1, -1, -1, -1, -1}, //14
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //15
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //16
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 19, -1, -1, 42, 20, -1, -1, -1, -1, -1, -1, -1, -1, -1, 18, -1, -1, -1, -1, -1}, //17
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //18
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //19
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //20
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //21
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //22
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 25, 23, 27, -1, -1, -1, -1, 24, -1, -1, -1, -1}, //23
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //24
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 25, 25, 25, -1, -1, -1, -1, 26, -1, -1, -1, -1}, //25
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //26
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 25, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, //27
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 32, -1, -1, 28, 35, -1, -1, -1, -1, -1, 31, -1, -1}, //28
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 28, -1, -1, -1, -1, -1, 30, -1, -1, -1}, //29
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //30
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //31
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 33, -1, -1, -1, -1, -1, -1, -1, -1, -1}, //32
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 33, 35, -1, -1, -1, -1, -1, 34, -1, -1}, //33
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //34
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 36, -1, -1, -1, 36, -1, -1, -1, 37, -1, -1, -1, -1, -1, -1, -1, -1, -1}, //35
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 37, -1, -1, -1, -1, -1, -1, -1, -1, -1}, //36
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 39, -1, -1, 37, 35, -1, -1, -1, -1, -1, 38, -1, -1}, //37
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //38
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 40, -1, -1, -1, -1, -1, -1, -1, -1, -1}, //39
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 40, 35, -1, -1, -1, -1, -1, 38, -1, -1}, //40
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 25, 25, -1, -1, -1, -1, -1, -1, -1, -1, -1}, //41
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //42
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //43
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 45, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 44, -1}, //44
                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1}, //45
                {-1, -1, -1, -1, -1, -1, 21, -1, -1, -1, -1, -1, 22, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}  //46
    };

    // advance table
    int stars[starStates] = { 11, 16, 18, 24, 26, 31, 34, 38 };
    int advance[rows][cols];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (map[i][j] != -1) {
                bool check = false;
                for (int k = 0; k < starStates; k++) {
                    if (map[i][j] == stars[k]) {
                        advance[i][j] = 0;
                        check = true;
                        break;
                    }
                }
                if (check == false) {
                    advance[i][j] = 1;
                }
            }
        }
    }

    file.open("Input Code.txt");
    ofstream tokfile, symfile, litfile;
    tokfile.open("Tokens.txt");
    symfile.open("Symbol Table.txt");
    litfile.open("Literal Table.txt");
    errfile.open("Errors.txt");

    tok* lit = new tok[100];
    tok* sym = new tok[200];
    tok* keys = new tok[100];
    int litcount = 0, symcount = 0, keycount = 0;

    int arr[17] = { 1,3,5,7,9,10,11,13,15,16,43,18,19,20,42,21,22 };
    int laststate = 0;
    string datatype;

    while (!eofcheck()) {
        bool noerr = false;
        tok t;
        t = tokenizer(map, advance, inputs, rows, cols);
        for (int i = 0; i < 17; i++) {
            if (t.state == arr[i]) {
                // operator or punctuation
                tokfile << "<";
                tokfile << t.val;
                alltokens[stritr++] = t.val;
                tokfile << ">";
                noerr = true;
                break;
            }
        }
        if (t.state == 45) {
            // literal table
            lit[litcount] = t;
            litcount++;
            string str = t.val;
            str = str.substr(1, str.length() - 2);
            t.val = str;
            litfile << litcount;
            alltokens[stritr++] = "literal";
            litfile << ".\t";
            litfile << t.val;
            litfile << '\n';
            tokfile << "<";
            tokfile << litcount;
            tokfile << ",lit>";
        }
        else if (t.state == 26) {
            // identifier
            bool innercheck = false;
            for (int i = 0; i < symcount; i++) {
                if (t.val == sym[i].val) {
                    // output only token .. identifier exists
                    tokfile << "<";
                    tokfile << i;
                    allidents[idents++] = t.val;
                    alltokens[stritr++] = "identifier";
                    tokfile << ",id>";
                    innercheck = true;
                }
            }
            // identifier does not exist
            if (innercheck == false) {
                sym[symcount] = t;
                symcount++;
                tokfile << "<";
                tokfile << symcount;
                tokfile << ",id>";
                symfile << symcount;
                symfile << ".\t";
                symfile << t.val;
                alltokens[stritr++] = "identifier";
                symfile << "\t";
                if (laststate == 24) {
                    symfile << datatype;
                }
                symfile << "\n";
                allidents[idents++] = t.val;
            }
        }
        else if (t.state == 24) {
            // keywords
            bool found = false;
            for (int i = 0; i < numofKeys; i++) {
                if (t.val == keywords[i]) {
                    bool ex = false;
                    for (int j = 0; j < keycount; j++) {
                        if (t.val == keys[j].val) {
                            // token for keyword exists
                            tokfile << "<";
                            tokfile << j + 1;
                            alltokens[stritr++] = t.val;
                            tokfile << ",keyword>";
                            ex = true;
                            found = true;
                            break;
                        }
                    }
                    // enter in keys array and send token
                    if (ex == false) {
                        keys[keycount] = t;
                        keycount++;
                        tokfile << "<";
                        tokfile << keycount;
                        tokfile << ",keyword>";
                        alltokens[stritr++] = t.val;
                        found = true;
                        break;
                    }
                }
                if (found == true) {
                    break;
                }
            }
            if (found == false) {
                // lexical error no keyword
                errfile << t.val;
                errfile << "\n";
            }
        }
        else if (t.state == 31) {
            // number
            tokfile << "<";
            tokfile << t.val;
            alltokens[stritr++] = "number";
            tokfile << ",number>";
            allidents[idents++] = t.val;
        }
        else if (t.state == 34) {
            // float
            tokfile << "<";
            tokfile << t.val;
            tokfile << ",float>";
            alltokens[stritr++] = "number";
            allidents[idents++] = t.val;
        }
        else if (t.state == 38) {
            // exponent
            tokfile << "<";
            tokfile << t.val;
            tokfile << ",exponent>";
            alltokens[stritr++] = "number";
            allidents[idents++] = t.val;
        }
        else {
            if (noerr == false) {
                errfile << t.val;
                errfile << "\n";
            }
        }
        laststate = t.state;
        datatype = t.val;
    }

    file.close();
    tokfile.close();
    symfile.close();
    litfile.close();
    errfile.close();
    cout << "Lexical Analysis Successful." << endl;
    for (int i = 0; i < 100; i++)
        cout << "_";
    cout << endl << endl;
}

string termval, magval, rval;
int labelnum = 1, tempnum = 1;
ofstream tacfile;
atr stmt, whilestmt, ifstmt;

string newTemp() {
    return ("t" + to_string(tempnum++));
}

string newLabel() {
    return ("L" + to_string(labelnum++));
}

void advance() {
    if (Pstritr < stritr) {
        Pstritr++;
    }
    else {
        return;
    }
}

void ERROR() {
    cout << "Parser Error: Syntax Error found." << endl;
}

void RelOp() {
    if (alltokens[Pstritr] == "==" || alltokens[Pstritr] == "<" || alltokens[Pstritr] == ">" || alltokens[Pstritr] == "<=" || alltokens[Pstritr] == ">=" || alltokens[Pstritr] == "<>") {
        advance();
    }
    else {
        ERROR();
        advance();
    }
}

string topget() {
    return (allidents[idents1++]);
}

void Expr();
void Stmt(atr obj);
void CompStmt(atr obj);
atr Mag();

atr Factor() {
    atr facret;
    if (alltokens[Pstritr] == "identifier") {
        facret.addr = topget();
        advance();
        return facret;
    }
    else if (alltokens[Pstritr] == "number") {
        facret.addr = topget();
        advance();
        return facret;
    }
    else if (alltokens[Pstritr] == "(") {
        advance();
        atr magret = Mag();
        facret.addr = magret.addr;
        if (alltokens[Pstritr] == ")") {
            advance();
        }
        return facret;
    }
    else {
        ERROR();
        advance();
        return facret;
    }
}

void TermP() {
    if (alltokens[Pstritr] == "*" || alltokens[Pstritr] == "/") {
        string op = alltokens[Pstritr];
        advance();
        atr facret = Factor();
        string temp = newTemp();
        tacfile << temp << " = " << termval << " " << op << " " << facret.addr << endl;
        termval = temp;
        TermP();
    }
    else {
        return;
    }
}

atr Term() {
    atr facret = Factor();
    termval = facret.addr;
    TermP();
    atr newTerm;
    newTerm.addr = termval;
    return newTerm;
}

void MagP() {
    if (alltokens[Pstritr] == "+" || alltokens[Pstritr] == "-") {
        string op = alltokens[Pstritr];
        advance();
        atr termret = Term();
        string temp = newTemp();
        tacfile << temp << " = " << magval << " " << op << " " << termret.addr << endl;
        magval = temp;
        MagP();
    }
    else {
        return;
    }
}

atr Mag() {
    atr termret = Term();
    magval = termret.addr;
    MagP();
    atr newmag;
    newmag.addr = magval;
    return newmag;
}

void RvalueP(atr obj) {
    if (alltokens[Pstritr] == "==" || alltokens[Pstritr] == "<" || alltokens[Pstritr] == ">" || alltokens[Pstritr] == "<=" || alltokens[Pstritr] == ">=" || alltokens[Pstritr] == "<>") {
        string op = alltokens[Pstritr];
        RelOp();
        atr magret = Mag();
        tacfile << "if " << rval << " " << op << " " << magret.addr << " goto " << obj.True << endl;
        tacfile << "goto " << obj.False << endl;
        rval = magret.addr;
        RvalueP(obj);
    }
    else {
        return;
    }
}

void Rvalue(atr obj) {
    atr magret = Mag();
    rval = magret.addr;
    atr rvalP;
    rvalP.True = obj.True;
    rvalP.False = obj.False;
    RvalueP(rvalP);
}

void Expr() {
    if (alltokens[Pstritr] == "identifier") {
        string id = topget();
        advance();
        if (alltokens[Pstritr] == ":=") {
            advance();
            atr magret = Mag();
            tacfile << id << " = " << magret.addr << endl;
        }
    }
}

void ElsePart(atr obj) {
    if (alltokens[Pstritr] == "Wagarna") {
        advance();
        atr stmt;
        stmt.begin = newLabel();
        stmt.next = obj.next;
        Stmt(stmt);
    }
}

void IfStmt(atr obj) {
    if (alltokens[Pstritr] == "Agar") {
        advance();
    }
    if (alltokens[Pstritr] == "(") {
        advance();
        atr rvalo;
        rvalo.True = newLabel();
        rvalo.False = newLabel();
        Rvalue(rvalo);
        if (alltokens[Pstritr] == ")") {
            advance();
        }
        tacfile << rvalo.True << ":" << endl;
        atr stmt;
        stmt.next = newLabel();
        Stmt(stmt);
        tacfile << "goto " << stmt.next << endl;
        tacfile << rvalo.False << ":" << endl;
        ElsePart(stmt);
        tacfile << stmt.next << ":" << endl;
    }
}

void WhileStmt(atr obj) {
    if (alltokens[Pstritr] == "while") {
        advance();
    }
    if (alltokens[Pstritr] == "(") {
        advance();
        atr rvalo;
        rvalo.True = newLabel();
        rvalo.False = obj.next;
        tacfile << obj.begin << ":" << endl;
        Rvalue(rvalo);
        if (alltokens[Pstritr] == ")") {
            advance();
        }
        tacfile << rvalo.True << ":" << endl;
        atr stmt;
        stmt.next = obj.begin;
        Stmt(stmt);
        tacfile << "goto " << obj.begin << endl;
        tacfile << obj.next << ":" << endl;
    }
}

void Stmt(atr obj) {
    if (alltokens[Pstritr] == "while") {
        atr whileobj;
        whileobj.begin = obj.begin;
        whileobj.next = obj.next;
        WhileStmt(whileobj);
    }
    else if (alltokens[Pstritr] == "Agar") {
        atr ifobj;
        ifobj.begin = obj.begin;
        ifobj.next = obj.next;
        IfStmt(ifobj);
    }
    else if (alltokens[Pstritr] == "{") {
        atr comobj;
        comobj.begin = obj.begin;
        comobj.next = obj.next;
        CompStmt(comobj);
    }
    else if (alltokens[Pstritr] == "identifier") {
        Expr();
        if (alltokens[Pstritr] == "::") {
            advance();
        }
    }
}

void StmtListP(atr obj) {
    if (alltokens[Pstritr] == "while" || alltokens[Pstritr] == "identifier" || alltokens[Pstritr] == "Agar" || alltokens[Pstritr] == "{") {
        atr stmt;
        stmt.next = obj.next;
        stmt.begin = obj.begin;
        Stmt(stmt);
        StmtListP(obj);
    }
    else {
        return;
    }
}

void StmtList(atr obj) {
    atr stlistP;
    stlistP.next = obj.next;
    stlistP.begin = obj.begin;
    StmtListP(stlistP);
}

void CompStmt(atr obj) {
    if (alltokens[Pstritr] == "{") {
        advance();
    }
    else {
        advance();
        ERROR();
    }
    atr stlist;
    stlist.next = obj.next;
    stlist.begin = obj.begin;
    StmtList(stlist);
    if (alltokens[Pstritr] == "}") {
        advance();
    }
    else {
        advance();
        ERROR();
    }
}

void Start() {
    atr obj;
    obj.begin = newLabel();
    obj.next = newLabel();
    Stmt(obj);
}

void Parser_Sementic() {
    tacfile.open("Three Address Code.txt");
    Start();
    cout << "Parser Successful." << endl;
    for (int i = 0; i < 100; i++)
        cout << "_";
    cout << endl << endl;
}

int main() {
    Lexer();
    Parser_Sementic();
    cout << "Three Address Code Generated Successfully." << endl;
    for (int i = 0; i < 100; i++)
        cout << "_";
    cout << endl << endl;
    return 0;
}