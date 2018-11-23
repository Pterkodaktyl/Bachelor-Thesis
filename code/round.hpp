#include "matrix.hpp"
#include "parsing.hpp"
#include "network.hpp"
#include "dinic.hpp"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <cstdio>

//Function rounds the matrix m.
void round (matrix<long double> &m, matrix<int> &result){
	Dinic *ptr_dinic = NULL;         
	Dinic *ptr_n = NULL;
	int n_row, n_col;
        n_row = m.get_size().first; n_col = m.get_size().second;	
	
	reduction network(n_row, n_col);
	Dinic d(n_row + n_col + 4);
	ptr_dinic = &d;

	network.create_network(m, ptr_dinic);
	ptr_dinic->GetMaxFlow(n_row + n_col + 2, n_row + n_col + 3);

	Dinic d2(n_row + n_col + 2, 1);                          
	ptr_n = &d2;                                                //Starting flow is a max. flow in the auxiliary network.

	network.cr_maxflow_net(ptr_dinic, ptr_n, m);
	ptr_n->GetMaxFlow(n_row + n_col, n_row + n_col + 1);        //Counting max flow in the original network.

	for (int i = 0; i < n_row; i++){
		 for (vector<Edge>::iterator it = ptr_n->G[i].begin(); it != ptr_n->G[i].end(); ++it) {
			 if (it->from < it->to && it->to < n_row + n_col){
				 result.add_elem(it->flow);
			 }
		  }
	}

return;

} 
