#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <vector>
using namespace std;

#ifndef __PARSING_HPP_INCLUDED__   
#define __PARSING_HPP_INCLUDED__   

//Functions used for parsing.

void print_vec_int (vector<int>& v){
	for (vector<int>::iterator it = v.begin(); it != v.end(); it++){
		cout << *it << endl;
	}
}

int my_stoi(const string& str)
{
        bool parsing = false;
        string c;
        int n = 0;
        for (int i = 0; i < str.length(); i++){
                c = str[i];
                if (isdigit(c[0])){
                        parsing = true;
                        n = (n * 10) + atoi(c.c_str());
                }
                else if (parsing == true){
                        parsing = false;
                        break;
                }
        }
        return n;
}

vector<long double> parse_line (string line){
	bool reading = false;
	int i = 0;
	string a;
	int l_size;
	l_size = line.size();
	vector <long double> res;
	while (i < l_size){
	  if ((isdigit(line[i]) || line[i] == '.') && reading && i < (l_size - 1)){
	  	a += line[i];
	  }
	  if (!isdigit(line[i]) && line[i] != '.'  && reading){
		reading = false;
		res.push_back(stold(a));
		a.clear();
	  }
	  if (isdigit(line[i]) && i == (l_size - 1)){
		reading = false;
		a += line[i];
		res.push_back(stold(a));
		a.clear();
	  }
	
	  if (isdigit(line[i]) && !reading){
		a = line[i];
		reading = true;
	  }		
	  i++;
	}
	return res;
}

vector<int> parse_line2 (string line){
 	bool reading = false;
        int i = 0;
        string a;
        int l_size;
        l_size = line.size();
        vector<int> res;

	while (i < l_size){
          if (isdigit(line[i]) && reading && i < (l_size - 1)){
                a += line[i];
          }
          if (!isdigit(line[i]) && reading){
                reading = false;
                res.push_back(stold(a));
                a.clear();
          }
          if (isdigit(line[i]) && i == (l_size - 1)){
                reading = false;
                a += line[i];
                res.push_back(stold(a));
                a.clear();
          }

          if (isdigit(line[i]) && !reading){
                a = line[i];
                reading = true;
          }
          i++;
        }
        return res;

}

template <class content_type> content_type kahan_sum(vector<content_type> &values){

	content_type c, y, sum, t;
	c = 0;
	y = 0;
	sum = 0;

	for (typename vector<content_type>::iterator it = values.begin(); it != values.end(); ++it){
		y = *it - c;
		t = sum + y;
		c = (t - sum) - y;
		sum = t;
	}
	return sum; 
} 

#endif	
