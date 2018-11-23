#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <bitset>
#include "parsing.hpp"
#include "matrix_partition.hpp"
#include "matrix.hpp"

using namespace std;

map <Key, int> hyper_edges;

int main(int argc, char *argv[]){

int n, k, p, s;
string s_n, s_k, s_p, s_s;
vector<int> row;
vector<int> h_i;

ifstream fin;
string line1, line2, line;

if (argc == 2) fin.open(argv[1]);       //Input file name is a parameter.
else{
	cerr << "No input file.\n";
	cerr << argv[1];
	return 0;
};

getline (fin, line1);                    //Read parameters n, k
stringstream sline1(line1);
sline1 >> s_n >> s_k;
n = my_stoi(s_n);
k = my_stoi(s_k);

getline (fin, line2);			//Read parameters p, s
stringstream sline2(line2);
sline2 >> s_s;

s = my_stoi(s_s);
p = 1;

cerr << "Read: n: " << n << "k:  " << k << " s: " << s << " p:  " << p << endl; 
matrix<int> m(p, s);			//Original matrix a_ij

if (fin.good()){
	while (getline(fin, line)){
		cerr << "Read line: " << line << endl;
		row = parse_line2(line);
		h_i.push_back(k);
		for (vector<int>::iterator it = row.begin(); it != row.end(); ++it){
			cerr << "Into matrix added " << *it <<endl;
			m.add_elem(*it);
		}
	row.clear();
	}
	fin.close();
}

else{
	 cout << "Reading file error\n";
	 return 0;
}
cerr << "Call gen_all_matrices\n";
clock_t start;
double duration;
start = clock();
gen_all_matrices(n, n, m, h_i);
duration = (clock() - start) / (double) CLOCKS_PER_SEC;
cout << "Time: " << duration << endl;
}

