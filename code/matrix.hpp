#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <iostream> 
#include <vector>
#include <cmath>
#include <tuple>
#include "parsing.hpp"

//Matrix is store as one vector.

using namespace std;

template <class content_type>
class matrix {

        public:
          matrix(int colnum, int rownum);	
	  ~matrix(); 
          void add_elem(content_type elem);
          int index_of(int i, int j);
          content_type get_value(int i , int j);
	  pair<int, int> get_size();
          void show ();
	  content_type sum_col(int i);
	  content_type sum_row(int i);
        
	private:
          vector<content_type>* values = new vector<content_type>;
          int row_num;
          int col_num;
};

using namespace std;

template <class content_type>  matrix<content_type> :: matrix (int row, int col){
	values->reserve (row * col);
	row_num = row;
	col_num = col;
}

template <class content_type> matrix<content_type> :: ~matrix(void){
	values->clear();
	delete values;
} 


//Returns size of the matrix.
template <class content_type> pair<int, int> matrix<content_type>::get_size(){
	pair<int, int> p(row_num, col_num);
	return p;
	}

//Adds one element into the matrix.
template <class content_type> void matrix<content_type> :: add_elem(content_type elem){
	if (values->size() < row_num * col_num) values->push_back(elem);
	else {
	   cerr << "Out of bounds! \n";
	   return;
	   values->resize((row_num + 1) * col_num);
	   values->push_back(elem);
	}
}

//Recounts the indeces.
template <class content_type> int matrix<content_type> :: index_of (int i, int j){
	
	if (j > col_num || i > row_num){
	  cerr << "Out of bounds! \n";
	  return -1;
	} 
	else
	 return ((i * col_num) + j);
}

//Returns value Matrix[i][j].
template <class content_type> content_type matrix<content_type> :: get_value (int i , int j){
	int index;
	index = index_of (i, j);
	if (index == - 1){ 
		cerr << "Out of bounds! \n";
	}
	else return (values->at(index));
}

//Prints the matrix.
template <class content_type> void matrix<content_type> :: show(){
	int i = 0;
	for (typename vector<content_type>::iterator it = values->begin(); it != values->end(); it++ ){
		i++;
		cout << ' ' << *it;
		if (i >= col_num) {cout << endl << endl; i = 0;}
	}
	cout << endl;
}


//Returns colnum sum of i-th colnum.
//Using function kahan_sum, which can lower the error while summing doubles.
template <class content_type> content_type matrix<content_type> :: sum_col (int i){
	vector<content_type> col;

	for ( int j = index_of(0,i); j <= index_of(row_num - 1, i); j = j + col_num )
		col.push_back(values->at(j));
	return (kahan_sum<content_type>(col));
	
}

//Retuns row sum of the i-th row.
template <class content_type> content_type matrix<content_type> :: sum_row (int i){
	vector<content_type> row;
        for ( int j = index_of(i,0); j <= index_of(i, col_num - 1); j++ )
		row.push_back(values->at(j));
	return (kahan_sum<>(row));
}
#endif
