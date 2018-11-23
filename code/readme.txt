==About==
Source codes attached to the bachelor thesis Hamiltonian Cycles in Kneser Graphs. 
Program computes Baranyai's partition of a complete uniform hypergraph.
It is a straightforward implementation of the proof on the existence of the Baranyai's partition. 
In can be found in [1].
This partition was used in [2] and [3] to construct Hamiltonian cycles in the Kneser graphs.

==Source code files==
Source code computing the partition itself is in the file baranyai_partition.cpp
As a subroutine program rounds real matrices in away, that the row sums and column sum differs from the original ones at most by one.
This function can be found in the file round.hpp.  

==Input format==
Program needs name of the input file in the following format.
First line contains two numbers n, k separated by a space. N is a number of elements in the hypergraf. K is a size of the hyperedge. 
Second line contains number of columns of the matrix. 
The last line contains sizes of the partition sets.

==Output format==
Program writes edges in the partition classes. Each class is separated by a newline.
Edges are written as k-tuples of numbers from 0 to n-1. 
On the last line program writes time needed to compute the partition.
 
==References==
[1] Z.s. Baranyai, On the factorization of the complete uniform hypergraph, Colloq. Math. Soc. Janos Bolyai, 10 (1975), pp. 91-108
[2] Ya-Chen Chen, Triangle-free Hamiltonian Kneser graphs, Journal of Combinatorial Theory, Series B, Volume 89, Issue 1, 2003, Pages 1-16, 
https://www.sciencedirect.com/science/article/pii/S0095895603000406
[3] Ya-Chen Chen, Kneser Graphs Are Hamiltonian For n >= 3k, Journal of Combinatorial Theory, Series B, Volume 80, Issue 1, 2000, Pages 69-79,
https://www.sciencedirect.com/science/article/pii/S0095895600919695

