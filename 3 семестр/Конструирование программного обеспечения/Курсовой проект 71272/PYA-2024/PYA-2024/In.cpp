#include "stdafx.h"
namespace In {
    IN getin(wchar_t infile[]) {
        IN f = {};
        static unsigned char l[IN_MAX_LEN_TEXT + 1] = "";
        int line = 0, position = 0, ignor = 0, i = 0;
        bool expres = false;

        std::ifstream in(infile);
        if (!in.is_open()) {
            throw ERROR_THROW(110);
        }

        char c;
        while (in.get(c)) {
            unsigned char ch = static_cast<unsigned char>(c);

            if (i >= IN_MAX_LEN_TEXT) {
                throw ERROR_THROW_IN(111, line + 1, position + 1);
            }

            switch (f.code[ch]) {
            case IN::S:
            case IN::T:
                if (ch == '\n') {
                    position = 0;
                    line++;
                    l[i++] = '\n';
                }
                else {
                    if (ch == '\'' || ch == '"') {
                        expres = !expres;
                    }
                    l[i++] = ch;
                    position++;
                }
                break;
            case IN::F:
                throw ERROR_THROW_IN(111, line + 1, position + 1);
            case IN::I:
                ignor++;
                break;
            default:
                if (ch == ' ' && !expres) {
                    if (i > 0 && l[i - 1] != ' ') {
                        l[i++] = ' ';
                    }
                }
                else {
                    l[i++] = ch;
                }
                position++;
                break;
            }
        }

        if (i == 0) {
            throw ERROR_THROW(112);
        }
        l[i] = '\0';

        in.close();

        f.size = i;
        f.text = l;
        f.lines = line + 1;
        f.ignor = ignor;

        return f;
    }
};
