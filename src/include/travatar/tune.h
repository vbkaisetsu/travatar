#ifndef TUNE_H__
#define TUNE_H__

#include <vector>
#include <cfloat>
#include <boost/shared_ptr.hpp>
#include <travatar/sparse-map.h>
#include <travatar/sentence.h>
#include <travatar/util.h>
// #include <boost/thread.hpp>
// #include <tr1/unordered_map>
// #include <travatar/sparse-map.h>

// #include <travatar/thread-pool.h>
// #include <travatar/eval-measure.h>

namespace travatar {

class TuningExample;

// Performs MERT
class Tune {

public:

    // **** Non-static Members ****
    Tune() : gain_threshold_(0.000001) {
        ranges_[-1] = std::pair<double,double>(-DBL_MAX, DBL_MAX);
    }

    // Tune weights
    virtual double RunTuning(SparseMap & weights) = 0;

    // Find gradient range
    std::pair<double,double> FindGradientRange(
                                const SparseMap & weights,
                                WordId feat);
    std::pair<double,double> FindGradientRange(
                                const SparseMap & weights,
                                const SparseMap & gradient,
                                std::pair<double,double> range);

    // Getters/Setters
    void SetExamples(const std::vector<boost::shared_ptr<TuningExample> > & examps) { examps_ = examps; }
    int NumExamples() { return examps_.size(); }
    void SetGainThreshold(double thresh) { gain_threshold_ = thresh; }
    double GetGainThreshold() { return gain_threshold_; }
    void SetRange(int id, double min, double max) {
        ranges_[id] = std::pair<double,double>(min,max);
    }
    void AddExample(const boost::shared_ptr<TuningExample> & examp) {
        examps_.push_back(examp);
    }
    std::vector<boost::shared_ptr<TuningExample> > & GetExamples() { return examps_; }
    const std::vector<boost::shared_ptr<TuningExample> > & GetExamples() const { return examps_; }
    TuningExample & GetExample(int id) {
        return *SafeAccess(examps_, id);
    }

protected:

    // A feature must create a gain of more than this to be added
    double gain_threshold_;

    // The range of the weights
    typedef std::tr1::unordered_map<WordId, std::pair<double,double> > RangeMap;
    RangeMap ranges_;

    // The examples to use
    std::vector<boost::shared_ptr<TuningExample> > examps_;

};

}

#endif
