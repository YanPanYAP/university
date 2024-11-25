#include "stdafx.h"
#include <cctype>
using namespace std;

namespace In
{
    int removeExtraSpaces(std::string& input) {
        int lol = 0;
        bool insideQuotes = false;
        const std::string operators = ";,}{()=+-/*";

        // Подсчёт пробелов в начале строки
        size_t startSpaces = 0;
        while (startSpaces < input.length() && input[startSpaces] == ' ') {
            ++startSpaces;
            lol++;
        }

        // Удаление пробелов в начале строки
        if (startSpaces > 0) {
            input.erase(0, startSpaces);
        }

        // Удаление лишних пробелов между словами
        size_t i = 0;
        while (i < input.length()) {
            if (input[i] == '\'') {
                insideQuotes = !insideQuotes;
            }
            if (!insideQuotes && input[i] == ' ') {
                size_t spacesToRemove = 0;

                // Считаем количество пробелов
                while (i + spacesToRemove < input.length() && input[i + spacesToRemove] == ' ') {
                    ++spacesToRemove;
                }

                // Если пробелов больше одного, удаляем лишние
                if (spacesToRemove > 1) {
                    lol += spacesToRemove - 1;
                    input.erase(i + 1, spacesToRemove - 1);
                    continue;
                }
            }

            // Удаляем пробелы вокруг операторов
            if (operators.find(input[i]) != std::string::npos) {
                // Удаляем пробелы перед оператором
                while (i > 0 && std::isspace(input[i - 1])) {
                    input.erase(i - 1, 1);
                    lol++;
                    --i;
                }

                // Удаляем пробелы после оператора
                while (i + 1 < input.length() && std::isspace(input[i + 1])) {
                    input.erase(i + 1, 1);
                    lol++;
                }
            }

            i++;
        }

        return lol;
    }

    IN getin(wchar_t infile[]) // Ôóíêöèÿ äëÿ ÷òåíèÿ äàííûõ èç ôàéëà è èõ îáðàáîòêè.
    {
        ifstream fin;
        char* outFile = new char[wcslen(infile) + 1]; // Ñîçäàåì áóôåð äëÿ èìåíè ôàéëà â ôîðìàòå char.
        wcstombs_s(NULL, outFile, wcslen(infile) + 1, infile, wcslen(infile) + 1); // Ïðåîáðàçóåì èìÿ ôàéëà èç wchar_t â char.
        fin.open(infile);
        if (fin.bad()) // Ïðîâåðÿåì, íå ïðîèçîøëî ëè îøèáêè ïðè îòêðûòèè ôàéëà.
        {
            throw ERROR_THROW(116);
        }
        if (!fin.is_open())
        {
            throw ERROR_THROW(110);
        }

        IN resultIn; // Ñîçäàåì ñòðóêòóðó äëÿ õðàíåíèÿ äàííûõ èç ôàéëà.
        resultIn.size = 0; //íà÷àëüíûé ðàçìåð òåêñòà.
        resultIn.lines = 0; //êîëè÷åñòâî ñòðîê.
        int position = 0;
        int ch; // Ïåðåìåííàÿ äëÿ õðàíåíèÿ òåêóùåãî ñèìâîëà.
        std::string currentLine; // Ñòðîêà äëÿ íàêîïëåíèÿ ñèìâîëîâ.
        std::string buffer; // Áóôåð äëÿ ñ÷èòûâàíèÿ ñòðîê èç ôàéëà.
        while (std::getline(fin, buffer)) // Ñ÷èòûâàåì ñòðîêó èç ôàéëà.
        {
            resultIn.lines++;
            for (int i = 0; i < buffer.length(); i++)
            {
                ch = buffer[i]; // Ïîëó÷àåì òåêóùèé ñèìâîë.
                /*if (ch == ' ') {
                    cout << "hjsdbsdsdfhvkdsagyduysgfgdasfylgsdlfygdsluyfgldysagflydss";
                }*/
                if (ch == fin.eof())
                {
                    break;
                }
                switch (resultIn.code[(unsigned char)ch])
                {
                case IN::F: // Åñëè ñèìâîë ÿâëÿåòñÿ áóêâîé, öèôðîé èëè çíàêîì ïîä÷åðêèâàíèÿ, òî ýòî èäåíòèôèêàòîð.
                {
                    resultIn.text.push_back(currentLine); // Äîáàâëÿåì òåêóùóþ ñòðîêó â ðåçóëüòàò.
                    throw ERROR_THROW_IN(111, resultIn.lines, ++position, buffer, resultIn.text); // Âûáðàñûâàåì èñêëþ÷åíèå ïðè îøèáêå.
                    break;
                }
                case IN::I:
                {
                    resultIn.ignor++;
                    position++;
                    break;
                }
                case IN::T: // Åñëè ñèìâîë ÿâëÿåòñÿ òåêñòîâûì ñèìâîëîì.
                {
                    currentLine += ch; // Äîáàâëÿåì ñèìâîë ê òåêóùåé ñòðîêå.
                    position++;
                    resultIn.size++;
                    break;
                }
                default:
                {
                    currentLine += resultIn.code[(unsigned char)ch]; // Äîáàâëÿåì ñïåöèàëüíûé ñèìâîë.
                    resultIn.size++;
                    position++;
                    break;
                }
                }
            }

            if (!currentLine.empty())
            {
                resultIn.size++;
                position++;
                position = 0; // Ñáðàñûâàåì ïîçèöèþ ïîñëå îêîí÷àíèÿ ñòðîêè.
                currentLine += resultIn.code[IN_CODE_ENDL]; // Äîáàâëÿåì ñèìâîë êîíöà ñòðîêè.
                int pobeda = removeExtraSpaces(currentLine); // Óäàëÿåì ëèøíèå ïðîáåëû â ñòðîêå.
                resultIn.text.push_back(currentLine); // Äîáàâëÿåì òåêóùóþ ñòðîêó â ðåçóëüòàò.
                currentLine.clear(); // Î÷èùàåì ñòðîêó äëÿ ñëåäóþùåé èòåðàöèè.

                resultIn.ignor += pobeda;
            }
        }

        fin.close();

        return resultIn;
    }
}