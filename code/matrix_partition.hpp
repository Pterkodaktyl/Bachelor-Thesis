#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include "matrix.hpp"
#include "parsing.hpp"
#include "round.hpp"

#ifndef MATRIX_PARTITION_HPP
#define MATRIX_PARTITION_HPP

using namespace std;

class Key{
public:
	int i,j,k;
	Key(int i, int j, int k): i(i), j(j), k(k) {};
	bool operator<(const Key& k) const;
};

bool Key::operator<(const Key& k) const {
	if (k.i != this->i) return (k.i < this->i);
	else if (k.j != this->j) return (k.j < this->j);
	else if (k.k != this->k) return (k.k < this->k);
	else return (k.i < this->i);
}

extern map<Key, int> hyper_edges;
typedef map<Key, int>::iterator it_map;

int pow(int p){
	if (p == 1) return 2;
	if (p == 0) return 1;

	int tmp = pow(p/2);
	if (p % 2 == 0) return (tmp * tmp);
	else return (2 * tmp * tmp);
}

void print_edge(int e){
	int n = 0;
	while (e > 0){
		e >>= 1;	
		if((e % 2) == 1) cout << " " << n;
		n++;
	}
	cout << endl;
	return;
}	

int add_vertex (int edge, int vertex){ 
	return (edge | pow(vertex));
}

void gen_edges_n1_m(matrix<int> &m, vector<int> &h_i){
	for(int i = 0; i < m.get_size().first; i++){
		for(int j = 0; j <  m.get_size().second; j++){
			 Key k1(i,j,m.get_value(i,j));
			 
			 if (h_i[i] == 1 && m.get_value(i,j) == 1) hyper_edges.insert(make_pair(k1, 2));
			 else if (h_i[i] == 1 && m.get_value(i,j) == 0) hyper_edges.insert(make_pair(k1, 0));
			 else if (h_i[i] == 0) hyper_edges.insert(make_pair(k1, 0));
			 else hyper_edges.insert(make_pair(k1, 0)); 
		}
	}
}

void gen_edges_n2_m(matrix<int> &m, matrix<int> &bigger, vector<int> h_i, int n){
	int a;
	it_map it_he;
	for (int j = 0; j< m.get_size().second; j++){
		for (int i = 0; i < m.get_size().first; i++){
			for (int k = 1; k <= m.get_value(i,j); k++){
				if ( k  > (bigger.get_value(i,j))){
					Key k1(i,j,k);
					Key k2(i + m.get_size().first, j, k - bigger.get_value(i,j));
					it_he = hyper_edges.find(k2);
					if (it_he != hyper_edges.end()){
					       	a = add_vertex(it_he->second,n);
						hyper_edges.erase(k2);
					}
					else a = add_vertex(0,n);
					it_he = hyper_edges.find(k1);
					if (it_he != hyper_edges.end()) it_he->second = a;
				        else hyper_edges.insert(make_pair(k1, a));

				}
			}
		}
	}
}

//Matrix result is the epsilon matrix.
void cr_round_eps_m (matrix<int> &m, matrix<int> &result, int n, vector<int> &h_i){
	matrix<long double>* epsilon_m = new matrix<long double>(m.get_size().first, m.get_size().second); 
	long double epsilon = 0;
	cerr << "in cr_round_eps_m\n "; 
	for (int i = 0; i < m.get_size().first; i++){
		for (int j = 0; j < m.get_size().second; j++){
			epsilon = (m.get_value(i,j) * h_i[i]) /(long double) n;
			epsilon_m->add_elem(epsilon);
		}
	}

	round(*epsilon_m, result);
	delete epsilon_m;
}

void cr_bigger_matrix (matrix<int> &m, matrix<int> &epsilon_m, matrix<int> &result_m, vector<int> &h_i, int n){
        for (int i = 0; i < (m.get_size().first * 2); i++){
		for(int j = 0; j < m.get_size().second; j++){
			if (i < m.get_size().first) result_m.add_elem(m.get_value(i,j) - epsilon_m.get_value(i,j));
			else result_m.add_elem(epsilon_m.get_value(i - m.get_size().first, j));
		}
	}
	for (int i = 0; i < m.get_size().first; i++ ){
		if (h_i[i] > 0) h_i.push_back(h_i[i] - 1);
		else h_i.push_back(h_i[i] - 1);
	}	

	return;
}

void gen_all_matrices(int n, int n2, matrix<int> &m, vector<int> &h_i){
	matrix<int>* m_2  = new matrix<int> (m.get_size().first * 2, m.get_size().second);
	matrix<int>* m_2_a = new matrix<int> (m.get_size().first * 2, m.get_size().second); 
	if (n > 1){
		cr_round_eps_m(m, *m_2_a, n, h_i);
		cr_bigger_matrix(m, *m_2_a, *m_2, h_i, n);
		cerr << "Matice pro " << n - 1 << endl;
		delete m_2_a;  	
		gen_all_matrices(n - 1, n2, *m_2, h_i);	
		gen_edges_n2_m(m, *m_2, h_i, n);

	}
	else{
		gen_edges_n1_m(m, h_i);
		delete m_2;
		delete m_2_a;	
	      	return;
	}

	if (n == n2){
		cerr << "n == n2\n"; 
		for(int i = 0; i < m.get_size().second; i++){
        		for(int j = 1; j <= m.get_value(0,i); j++){
				Key k1(0,i,j);
				print_edge(hyper_edges[k1]);			    
                		//cout << std::bitset<32>(hyper_edges[k1]) << endl; //Edges are represented as a k-bit number.
				//cout << hyper_edges[k1] << endl;		    //Bit represention on the edge as a decimal number.
        		}
        		cout << endl;
		}		
	}
	
	delete m_2;
}
#endif
