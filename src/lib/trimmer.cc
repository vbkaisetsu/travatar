#include <travatar/trimmer.h>
#include <travatar/hyper-graph.h>
#include <travatar/util.h>
#include <boost/foreach.hpp>
#include <map>

using namespace std;
using namespace travatar;
using namespace boost;

void Trimmer::AddId(std::map<int,int> & id_map, int id) {
    if(id_map.find(id) == id_map.end())
        id_map.insert(make_pair(id, id_map.size()));
}

HyperGraph * Trimmer::TransformGraph(const HyperGraph & hg) const {
    std::map<int,int> active_nodes, active_edges;
    FindActive(hg, active_nodes, active_edges);
    HyperGraph * ret = new HyperGraph(hg);
    // Make the new edge/node arrays
    ret->DeleteEdges();
    ret->GetEdges().resize(active_edges.size());
    ret->DeleteNodes();
    ret->GetNodes().resize(active_nodes.size());
    // Add the active edges/nodes
    BOOST_FOREACH(const HyperEdge * edge, hg.GetEdges()) {
        std::map<int,int>::const_iterator it = active_edges.find(edge->GetId());
        if(it != active_edges.end())
            ret->GetEdges()[it->second] = new HyperEdge(*edge);
    }
    BOOST_FOREACH(const HyperNode * node, hg.GetNodes()) {
        std::map<int,int>::const_iterator it = active_nodes.find(node->GetId());
        if(it != active_nodes.end())
            ret->GetNodes()[it->second] = new HyperNode(*node);
    }
    // Replace the links
    BOOST_FOREACH(HyperEdge * edge, ret->GetEdges()) {
        vector<HyperNode*> new_tails;
        BOOST_FOREACH(HyperNode* tail, edge->GetTails())
            new_tails.push_back(ret->GetNodes()[active_nodes[tail->GetId()]]);
        edge->SetTails(new_tails);
        edge->SetHead(ret->GetNodes()[active_nodes[edge->GetHead()->GetId()]]);
    }
    BOOST_FOREACH(HyperNode * node, ret->GetNodes()) {
        vector<HyperEdge*> new_edges;
        BOOST_FOREACH(HyperEdge* edge, node->GetEdges())
            if(active_edges.find(edge->GetId()) != active_edges.end())
                new_edges.push_back(ret->GetEdges()[active_edges[edge->GetId()]]);
        node->SetEdges(new_edges);
    }
    // Replace the IDs
    BOOST_FOREACH(HyperEdge * edge, ret->GetEdges())
        edge->SetId(active_edges[edge->GetId()]);
    BOOST_FOREACH(HyperNode * node, ret->GetNodes())
        node->SetId(active_nodes[node->GetId()]);
    return ret;
}
