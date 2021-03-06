
#include <cmath>
#include <boost/foreach.hpp>
#include <travatar/sparse-map.h>
#include <travatar/dict.h>

using namespace std;

namespace travatar {

bool operator==(const SparseMap & lhs, const SparseMap & rhs) {
    // cerr << "Comparing " << lhs << " to " << rhs << endl;
    if(lhs.size() != rhs.size())
        return false;
    BOOST_FOREACH(const SparsePair & kv, lhs) {
        SparseMap::const_iterator it = rhs.find(kv.first);
        if(it == rhs.end() || abs(it->second - kv.second) > 1e-6) return false;
    }
    return true;
}
bool operator!=(const SparseMap & lhs, const SparseMap & rhs) {
    return !(lhs == rhs);
}

std::ostream & operator<<(std::ostream & out, const SparseMap & rhs) {
    out << "{";
    int count = 0;
    BOOST_FOREACH(const SparsePair & kv, rhs) {
        if(kv.second != 0) {
            if(count++ != 0) out << ", ";
            out << "\"" << Dict::WSym(kv.first) << "\": " << kv.second;
        }
    }
    out << "}";
    return out;
}

SparseMap & operator+=(SparseMap & lhs, const SparseMap & rhs) {
    BOOST_FOREACH(const SparsePair & val, rhs)
        if(val.second != 0) {
            lhs[val.first] += val.second;
            if(lhs[val.first] == 0) lhs.erase(val.first);
        }
    return lhs;
}

SparseMap operator+(const SparseMap & lhs, const SparseMap & rhs) {
    SparseMap ret;
    BOOST_FOREACH(const SparsePair & val, rhs)
        if(val.second != 0)
            ret[val.first] += val.second;
    BOOST_FOREACH(const SparsePair & val, lhs)
        if(val.second != 0) {
            ret[val.first] += val.second;
            if(ret[val.first] == 0) ret.erase(val.first);
        }
    return ret;
}

SparseMap operator-(const SparseMap & lhs, const SparseMap & rhs) {
    SparseMap ret;
    BOOST_FOREACH(const SparsePair & val, lhs)
        if(val.second != 0)
            ret[val.first] += val.second;
    BOOST_FOREACH(const SparsePair & val, rhs) {
        if(val.second != 0) {
            ret[val.first] -= val.second;
            if(ret[val.first] == 0) ret.erase(val.first);
        }
    }
    return ret;
}

double operator*(const SparseMap & lhs, const SparseMap & rhs) {
    double ret = 0;
    if(lhs.size() <= rhs.size()) {
        BOOST_FOREACH(const SparsePair & val, lhs) {
            SparseMap::const_iterator it = rhs.find(val.first);
            if(it != rhs.end())
                ret += val.second * it->second;
        }
    } else {
        BOOST_FOREACH(const SparsePair & val, rhs) {
            SparseMap::const_iterator it = lhs.find(val.first);
            if(it != lhs.end())
                ret += val.second * it->second;
        }
    }
    return ret;
}

SparseMap operator*(const SparseMap & lhs, double rhs) {
    SparseMap ret;
    BOOST_FOREACH(const SparsePair & val, lhs)
        ret[val.first] = val.second * rhs;
    return ret;
}

void NormalizeL1(SparseMap & weights, double denom) {
    double curr = 0;
    BOOST_FOREACH(const SparsePair & val, weights)
        curr += abs(val.second);
    if(curr == 0) return;
    denom = denom/curr;
    BOOST_FOREACH(SparseMap::value_type & val, weights)
        val.second *= denom;
}

}
