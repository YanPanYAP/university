#include "stdafx.h"

char** DivideTexT(char* source, int size) {
    char** words = new char* [MAX_NUMBER_OF_WORDS];
    int stroke = 0;
    int column = 0;
    int countApostr = 0;
    int countQuotes = 0;
    int realColumn = 1;
    In::IN letters;

    for (int i = 0; i < MAX_NUMBER_OF_WORDS; i++) {
        words[i] = new char[MAX_LENGTH_OF_WORD] { NULL };
    }

    bool flag = false;

    for (int i = 0; i < size; i++) {
        if (source[i] == '\'') {
            words[stroke][column++] = source[i++];
            countApostr++;
            int startColumn = realColumn;
            while (i < size && source[i] != '\'') {
                if (source[i] == '\n') {
                    throw ERROR_THROW(321);
                }
                words[stroke][column++] = source[i];
                i++;
                realColumn++;
            }
            if (i >= size || source[i] != '\'') {
                throw ERROR_THROW(321);
            }
            words[stroke][column++] = source[i];
            countApostr++;
            words[stroke++][column] = '\0';
            column = 0;
            realColumn++;
            continue;
        }
        else if (source[i] == '\"') {
            words[stroke][column++] = source[i++];
            countQuotes++;
            int startColumn = realColumn;
            while (i < size && source[i] != '\"') {
                if (source[i] == '\n') {
                    throw ERROR_THROW(321);
                }
                words[stroke][column++] = source[i];
                i++;
                realColumn++;
            }
            if (i >= size || source[i] != '\"') {
                throw ERROR_THROW(321);
            }
            words[stroke][column++] = source[i];
            countQuotes++;
            words[stroke++][column] = '\0';
            column = 0;
            realColumn++;
            continue;
        }
        else if (letters.code[(int)source[i]] != In::IN::S) {
            words[stroke][column++] = source[i];
            flag = true;
        }
        else {
            if (source[i] != ' ' && flag) {
                words[stroke++][column] = '\0';
                column = 0;
                words[stroke][column++] = source[i];
                words[stroke++][column] = '\0';
                column = 0;
            }
            else if (!flag && source[i] != ' ') {
                words[stroke][column++] = source[i];
                words[stroke++][column] = '\0';
                column = 0;
            }
            else if (flag) {
                words[stroke++][column] = '\0';
                column = 0;
            }
            flag = false;
        }
        realColumn++;
    }

    if (countApostr % 2 == 1 || countQuotes % 2 == 1) {
        throw ERROR_THROW(609);
    }

    words[stroke] = NULL;
    return words;
}
