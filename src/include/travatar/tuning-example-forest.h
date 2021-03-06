#ifndef TUNING_EXAMPLE_FOREST_H__
#define TUNING_EXAMPLE_FOREST_H__

#include <boost/shared_ptr.hpp>
#include <travatar/tuning-example.h>
#include <travatar/sentence.h>
#include <travatar/hyper-graph.h>
#include <set>
#include <cfloat>

namespace travatar {

class EvalMeasure;
class MertHull;
class MertHullWeightFunction;

class TuningExampleForest : public TuningExample {

public:

    // Constructor tanking the forest and the reference
    TuningExampleForest(EvalMeasure * measure,
                        const Sentence & ref,
                        int id,
                        double mult) : 
                            measure_(measure),
                            ref_(ref), oracle_score_(mult),
                            curr_score_(-DBL_MAX), id_(id), mult_(mult) {
    }

    virtual ~TuningExampleForest() { }

    // Find the featutres that are active in this forest
    void FindActiveFeatures();

    // Calculate the oracle of this sentence
    void CalculateOracle();

    // Calculate the gain that could be achieved by each feature
    // for this particular forest (oracle-current best)
    virtual SparseMap CalculatePotentialGain(const SparseMap & weights);

    // Count weights
    virtual void CountWeights(SparseMap & weights);

    // Calculate the convex hull for this example given the current weights
    // and gradients
    virtual ConvexHull CalculateConvexHull(
                                const SparseMap & weights,
                                const SparseMap & gradient) const;
    
    // Add a forest hypothesis
    void AddHypothesis(const boost::shared_ptr<HyperGraph> & forest);

protected:

    // Recursively calculate the MERT hull
    const MertHull & CalculateMertHull(
                            const MertHullWeightFunction & func,
                            std::vector<boost::shared_ptr<MertHull> > & hulls, 
                            int node_id) const;

    EvalMeasure * measure_;
    boost::shared_ptr<HyperGraph> forest_;
    Sentence ref_;
    // The score that the best hypothesis in the forest achieves
    double oracle_score_;
    // The score that the forest achieves with the current weights
    double curr_score_;
    // A set of features that are active in the forest
    std::set<int> active_;
    // The ID of this example
    int id_;
    // Multiplier
    double mult_;

};

}

#endif
