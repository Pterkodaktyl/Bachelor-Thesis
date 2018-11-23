//Dinic algorithm
#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <queue>
#include <cassert>

#ifndef DINIC_H
#define DINIC_H

using namespace std;
typedef long long LL;

//rcap - residual capacity
//rev_edge for the edge uv - pointer to the edge  vu - we need them while counting residual capacities

struct Edge {
  int from, to, cap, flow, demand, index;
  struct Edge *rev_edge;
  Edge(int from, int to, LL cap, LL flow, int demand, int index) :
    from(from), to(to), cap(cap), flow(flow), demand(demand), index(index) { rev_edge = NULL; }

  LL rcap(int N) { 
    if ( from < to || rev_edge == NULL) return (cap - flow);
    else if (from == N - 2) return (cap - flow);
    else return (rev_edge->flow - rev_edge->demand); 
  }
};

struct Dinic {
    int N; //Number of vertices.
    int P; //parameter: if P == 1, then there exist pointers to rev_edge.
    vector<vector<Edge> > G;
    vector<list<Edge *> > Zb; //Rest of the edges in the level graph.
    vector<int> layer;
  
    Dinic(int N) : N(N), G(N), P(0) {
	layer.resize(N,-1);
	for (int i = 0; i < N; i++)
	{
	    list<Edge*> elist; //empty list
	    Zb.push_back(elist);
	}
    }

    //Constructor for the original network.
    //push_back can change the pointers... 
    Dinic(int N, int P) : N(N), G(N), P(P) {
  	for_each(G.begin(), G.end(), [&N](vector<Edge> &v) {v.reserve(2*N);});
	layer.resize(N,-1);
	for (int i = 0; i < N; i++)
	{
	    list<Edge*> elist; //empty list
	    Zb.push_back(elist);
	}
    }
	
    //Adding edges into the network.
    //Updating rev_edge pointers.
    void AddEdge(int from, int to, int cap, int flow, int demand) {
	if (from == to) return;
	G[from].push_back(Edge(from, to, cap, flow, demand, G[to].size()));
	G[to].push_back(Edge(to, from, flow, 0, 0, G[from].size() - 1));
	
	if (P == 1){ 
	    G[from].back().rev_edge = &(G[to].back());
	    G[to].back().rev_edge = &(G[from].back());
	}
    }

    //Update flow on the edge e and on the edge rev_e.
    void AssignFlow(Edge *e, LL flow) {
	e->flow += flow;
	G[e->to][e->index].flow -= flow;
    }
    
    //Counting layers in the residual network. Return false, if the sink is unreachable.
    bool ComputeLayers(int source, int sink) {
	queue<int> Q;

	for (int i = 0; i < N; i++)
	{
	    layer[i] = -1;
	    Zb[i].clear();
	}
	
	layer[source] = 0; Q.push(source);

	//Two stop conditions: Q is empty or the sink has been already processed.
	while ((!Q.empty()) && (Q.front() != sink)) {
	    int cur = Q.front(); Q.pop();
	    vector<Edge>::iterator it = G[cur].begin();
	    while (it != G[cur].end()) {
		if ( (layer[it->to] == -1 || (layer[it->to] == layer[cur]+1)) && (it->rcap(N) > 0)) {
		    if (layer[it->to] == -1)
		    {
			Q.push(it->to);
			layer[it->to] = layer[cur]+1;
		    }
		    Zb[cur].push_back(&(*it));
		    
		}
		it++;
	    }
	}

	return (layer[sink] != -1);
    }

    //Sent one path in DFS then clean the network.
    LL SendFlow(int from, int sink, LL flowLimit) {
	LL flowSent = 0;
	list<Edge *>& outgoing = Zb[from];
	list<Edge *>::iterator it = outgoing.begin();

	if (it == outgoing.end())
	{
	    //cerr << "Vertex doesn't have any outgoing edges." << endl;
	}
	while (it != outgoing.end())
	{
	    int next = (*it)->to;
	    assert((layer[next] != -1));
	    assert(layer[next] == layer[from] + 1);

	    //Sink case.
	    if (next == sink)
	    {
		flowSent = min(flowLimit, (*it)->rcap(N));
	    } else {
		flowSent = SendFlow(next, sink, min(flowLimit, (*it)->rcap(N)));
	    }
	    
	    //If we weren't able to sent anything then clean and continue.
	    if (flowSent == 0) {
		//cerr << "Deleting edge" << endl;
		it = outgoing.erase(it); //iterator++
	    } else {
		//Update flow/residual capacities:
		//cerr << "Sending flow: " << flowSent << endl;
		AssignFlow(*it, flowSent);
		if ((*it)->rcap(N) == 0) {
		    //cerr << "Edge is saturated, delete." << endl;
		    outgoing.erase(it);
		}
		break; //We don't need to check rest of the edges.
	    }
	}

	return flowSent;
    }

    LL GetMaxFlow(int s, int t) {
	LL totalFlow = 0;
	while (ComputeLayers(s,t))
	{
	    //cerr << "Layer of the sink: " << layer[t] << endl;
	    LL update = SendFlow(s,t, LLONG_MAX);
	    while(update != 0)
	    {
		totalFlow += update;
 	        //cerr << "Added flow: " << update << endl;
		update = SendFlow(s,t, LLONG_MAX);
	    }
	}
	//cout << "Total flow: " << totalFlow << endl;
	return totalFlow;
    }
    
};

#endif
