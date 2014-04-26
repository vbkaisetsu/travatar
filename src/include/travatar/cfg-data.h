#ifndef CFG_DATA_H__
#define CFG_DATA_H__

#include <iostream>
#include <vector>
#include <boost/foreach.hpp>
#include <travatar/sentence.h>
#include <travatar/util.h>

namespace travatar {

class CfgData {
public:
    Sentence words;
    WordId label;
    Sentence syms;

    CfgData(const Sentence & _words = Sentence(),
            WordId _label = -1,
            const Sentence & _syms = Sentence())
        : words(_words), label(_label), syms(_syms) { }
    
    void AppendChild(const CfgData & child) {
        BOOST_FOREACH(WordId wid, child.words)
            words.push_back(wid);
        syms.push_back(child.label);
    }

    bool operator==(const CfgData & rhs) const {
        return words == rhs.words && label == rhs.label && syms == rhs.syms;
    }

    std::vector<int> GetNontermPositions() {
        std::vector<int> ret;
        for(int i = 0; i < (int)words.size(); i++)
            if(words[i] < 0)
                ret.push_back(i);
        return ret;
    }

    void Print(std::ostream & out) const;

    WordId GetSym(int id) {
        return (id < (int)syms.size() ? syms[id] : -1);
    }

};

inline std::ostream & operator<<(std::ostream & out, const CfgData & data) {
    data.Print(out);
    return out;
}

typedef std::vector<CfgData> CfgDataVector;

}

#endif