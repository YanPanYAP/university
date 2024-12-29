#include "FST.h"
#include <vector>
#include <cstring>

namespace FST {
    RELATION::RELATION(char c, short nn) : symbol(c), nnode(nn) {}

    NODE::NODE() : n_relation(0), relations(nullptr) {}

    NODE::NODE(short n, RELATION rel, ...) : n_relation(n), relations(new RELATION[n]) {
        RELATION* p = &rel;
        for (short i = 0; i < n; i++) {
            relations[i] = p[i];
        }
    }

    FST::FST(char* s, short ns, NODE n, ...) : string(s), nstates(ns), nodes(new NODE[ns]), position(-1) {
        NODE* p = &n;
        for (short k = 0; k < ns; k++) {
            nodes[k] = p[k];
        }
        rstates = nullptr;
    }

    bool execute(FST& fst) {
        if (!fst.string) return false;

        const char* str = fst.string;
        short nRStates = 1;

        delete[] fst.rstates;
        fst.rstates = new short[1] { 0 };

        for (size_t i = 0; i < strlen(str); i++) {
            char symb = str[i];
            fst.position++;

            if (isAllowed(symb, fst.rstates, nRStates, fst)) {
                fst.rstates = setRelState(symb, fst, &nRStates, fst.rstates);
                if (i + 1 == strlen(str) && isLastState(fst.rstates, nRStates, fst.nstates)) {
                    return true;
                }
            }
            else {
                return false;
            }
        }
        return false;
    }

    bool isLastState(short* rstates, short length, short countStates) {
        for (short i = 0; i < length; i++) {
            if (rstates[i] == countStates - 1) {
                return true;
            }
        }
        return false;
    }

    bool isAllowed(char symb, short*& rstates, short length, FST& fst) {
        for (short i = 0; i < length; i++) {
            for (short j = 0; j < fst.nodes[rstates[i]].n_relation; j++) {
                if (symb == fst.nodes[rstates[i]].relations[j].symbol) {
                    return true;
                }
            }
        }
        return false;
    }

    short* setRelState(char symb, FST& fst, short* pNRStates, short* rstates) {
        std::vector<short> vrstates;

        for (short i = 0; i < *pNRStates; i++) {
            for (short j = 0; j < fst.nodes[rstates[i]].n_relation; j++) {
                if (fst.nodes[rstates[i]].relations[j].symbol == symb) {
                    vrstates.push_back(fst.nodes[rstates[i]].relations[j].nnode);
                }
            }
        }

        delete[] rstates;

        short* result = new short[vrstates.size()];
        *pNRStates = static_cast<short>(vrstates.size());

        for (size_t i = 0; i < vrstates.size(); i++) {
            result[i] = vrstates[i];
        }

        return result;
    }
}
