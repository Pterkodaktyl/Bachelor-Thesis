#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "matrix.hpp"
#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <climits>
#include "dinic.hpp"

//Functions for building the auxiliary network and the original network.
using namespace std;

class reduction {
 	public:
	  reduction (int row, int col);
	  void process_edge(int from, int to, int demand);
	  void reduce(matrix<long double>& m);
	  int edge_cap (int from, int to, matrix<long double>& m);
	  pair<int, int> demand_rowcol_edge(int from, int to, matrix<long double>& m);
	  pair<int, int> demand_sourcerow_edge(int from, int to, matrix<long double>& m);
	  pair<int, int> demand_colsink_edge(int from, int to, matrix<long double>& m);
	  void show_neig();
	  void create_network (matrix<long double>& m, Dinic *ptr_d); 
	  void cr_maxflow_net (Dinic *ptr_d, Dinic * ptr_n, matrix<long double> &m);
	
	private:
	  vector<int> new_source_neig;   
	  vector<int> new_sink_neig;
	  int new_source;
	  int new_sink;
	
};	

typedef vector<int>::iterator v_it;

reduction::reduction (int row, int col){
	new_source_neig.resize(row+col + 2, 0);
	new_sink_neig.resize(row+col + 2, 0);
	new_source = row+col + 1; 
	new_sink = row+col + 2;
}

void reduction::show_neig(){
	for (v_it it = new_source_neig.begin(); it != new_source_neig.end(); ++it)
		cout << " " << *it;
	cout << endl;
	for (v_it it = new_sink_neig.begin(); it != new_sink_neig.end(); ++it)
		cout << " " << *it;
	cout << endl;
}

//Counts new capacites from the new source to the new sink.
// new_source_neig[i] - capacity of the edge from the new source to the vertex i.
void reduction::process_edge(int from, int to, int demand){
	new_source_neig[to] += demand;
	new_sink_neig[from] += demand;
} 

//Returns capacity of an edge uv.
int reduction::edge_cap (int from, int to, matrix<long double>& m ){
	long double pom;
	pom = ceil(m.get_value(from, to)) - floor(m.get_value(from, to));
	int res = static_cast <int> (pom);
	return res;
}  

//Returns capacity of the row-col edge.
pair<int, int> reduction::demand_rowcol_edge(int from, int to, matrix<long double>& m){
	int demand;
	int cap;
	demand = static_cast <int> (floor(m.get_value(from, to)));
	cap = static_cast <int> (ceil (m.get_value(from, to)));
	pair<int, int> p(demand, cap - demand);
	return p;
}

//Returns capacity of the source-row edge. 
pair<int, int> reduction::demand_sourcerow_edge(int from, int to, matrix<long double>& m ){
	long double sum;
	sum = m.sum_row(to);
	int demand, cap;
	demand = static_cast <int> (floor(sum));
	cap = static_cast <int> (ceil(sum)); 
	pair<int, int> p(demand, cap - demand);
	return p;
}

//Returns capacity of the column-sink edge.
pair<int, int> reduction::demand_colsink_edge(int from, int to, matrix<long double>& m){
	long double sum;
	sum = m.sum_col(from);
	int demand, cap;
	demand = static_cast <int> (floor(sum));
	cap = static_cast <int> (ceil(sum));
	pair<int, int> p(demand, cap - demand);
	return p;
}

//Counting capacities for edges into the new source and from the new sink. 
void reduction::reduce(matrix<long double>& m){
	int row, col;
  	row = m.get_size().first;
	col = m.get_size().second;
	pair<int, int> p;
	//Processing edges from the sink.
	for (int i = 0; i < row; i++){
		p = demand_sourcerow_edge(row + col, i, m);
		process_edge(row + col, i, p.first);	
	}
	//Processing edges to the sink.
	for (int i = 0; i < col; i++){
		p = demand_colsink_edge(i, row + col + 1, m);
		process_edge(i + row, row + col + 1, p.first);
	}
	//Processing row-col edges.
	for (int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			p = demand_rowcol_edge(i, j, m);
			process_edge(i, j + row, p.first);
		}
	}
}

//Building the helper network.
//Add edges using function AddEdge from dinic.h.
void reduction::create_network (matrix <long double>& m, Dinic *ptr_d){
	
	pair <int, int > s;	
	s = m.get_size();
	reduce(m);
	//Adding new source-original edges, edges between original vertices, original vertices-new sink.
	for (int i = 0; i < s.first + s.second + 2; i++){
		ptr_d->AddEdge(s.first + s.second + 2, i, new_source_neig[i], 0, 0);
		ptr_d->AddEdge(i, s.first + s.second + 3, new_sink_neig[i], 0, 0);
		
	}
	
	for (int i = 0; i < s.first; i++){			
	  for(int j = 0; j < s.second; j++ ){
		ptr_d->AddEdge(i, j + s.first, edge_cap(i, j, m), 0, 0);
	  }
	}
	for (int i = 0; i < s.second; i++){
		ptr_d->AddEdge(i + s.first, s.first + s.second + 1, demand_colsink_edge(i, i, m).second, 0, 0);
	}
	
	for (int i = 0; i < s.first; i++){
		ptr_d->AddEdge(s.first + s.second, i, demand_sourcerow_edge(i,i,m).second, 0, 0);
	}
	ptr_d->AddEdge(s.first + s.second + 1, s.first + s.second, INT_MAX, 0, 0 );
}

//Build new network as described in Baranyai's article.
//Set starting flow to the flow found on the helper network. Zero flow isn't always feasible.

void reduction::cr_maxflow_net (Dinic *ptr_d, Dinic * ptr_n, matrix<long double> &m){
	
	int flow = 0;
	int cap = 0;
	int N = ptr_d->N;
	pair <int, int> s;
	s = m.get_size();
	pair<int, int> demand;

	for (int i = 0; i < N - 2; i++) {
                for (vector<Edge>::iterator e = ptr_d->G[i].begin(); e != ptr_d->G[i].end(); ++e){
			cap = 0;
			flow = 0;
                        
                        if (e->from < e->to && e->to < N - 4 && e->from < N - 4){
				demand = demand_rowcol_edge(e->from, e->to - s.first, m); 
                                cap = demand.first + demand.second;
				flow = e->flow + demand.first;
				ptr_n->AddEdge(e->from, e->to, cap, flow, demand.first);
                                
                        }
			else if(e->from == N - 4 && e->to < N - 4){
				demand = demand_sourcerow_edge(e->to, e->to, m);
				cap = demand.first + demand.second;
                                flow = e->flow + demand.first;
                                ptr_n->AddEdge(e->from, e->to, cap, flow, demand.first);
			}
			 else if(e->to == N - 3 && e->from < N - 4){
				demand = demand_colsink_edge(e->from - s.first, e->from - s.first, m);
                                cap = demand.first + demand.second;
                                flow = e->flow + demand.first;
                                ptr_n->AddEdge(e->from, e->to, cap, flow, demand.first);
                        }
		}
	}
}
#endif

