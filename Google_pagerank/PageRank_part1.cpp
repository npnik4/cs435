// cs435 MP Nikhil Patel
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include<iterator>
#include <math.h>
#include <iomanip>
#include <sstream>
using namespace std;

struct Node 
{
	vector<int> inDeg;
	int outDegNum = 0; 
	vector<float> rank;
};

ostream & operator<<(ostream & out, const Node & p)
{
    //out << "number of out: " <<p.outDegNum << endl;
    //copy(p.inDeg.begin(), p.inDeg.end(), ostream_iterator<int>(out<<"inDeg: ", " "));
	//out << "\n";
	copy(p.rank.begin(), p.rank.end(), ostream_iterator<float>(out<<"Rank: ", " "));
    return out;
}

int main(int argc, char *argv[])
{	
	int N; // num of vectices
	vector<Node*> nodes; // array of object pointers type Node
	float d = 0.85;
	int edge_num; // num of edges
	int num_iterations = stoi(argv[1]); 
	bool run= false;
	int base = stoi(argv[2]); 
	float initial;
	// read in file through cmd
	ifstream file;
	string arg(argv[3]);
	file.open(arg);
	if( file.is_open() == false ) {
		cout << arg << "FILE NOT FOUND" << endl;
		return 0;
	}
	
	string line;
	bool flag = true;
	while(getline(file, line)){// reading file
		istringstream iss(line);
		int a, b;
		if (!(iss >> a >> b)){ 
			break; 
		} 

		if(flag){
			N = a;
			edge_num = b;
			// N = (int)line[0] - 48;
			// edge_num = (int)line[2] - 48;
			flag = false;
			for(int i=0; i<N; i++)
			{
				nodes.emplace_back(new Node{});
			}
			switch(base)
			{
				case 0: initial= (float)0.0;// base 0
						break;
				case 1: initial= (float)1.0;// base 1
						break;
				case -1: initial= (float)1/N;// base 1/n
						break;
				case -2: initial= (float)pow(N,-0.5) ;// base 1/sq(n)
						break;
				default: cout<< "Invalid Base input"<< endl;
						return 0;
			}
		}
		else{
			nodes[b]->inDeg.push_back(a);
			nodes[a]->outDegNum++;
			// nodes[(int)line[2] - 48]->inDeg.push_back((int)line[0] - 48);
			// nodes[(int)line[0] - 48]->outDegNum++;
		}	
	}
	file.close();

	float error_rate = 0.0;
	if(num_iterations == 0 || N > 10){
		num_iterations = 70;
		error_rate = (float)pow(10, -4);
		if(N>10){
			initial= (float)1/N;
		}
	}
	else if(num_iterations < 0){
		num_iterations = 70;
		error_rate = (float)pow(10, num_iterations);
	}

	int counter= 0;

	for(int x=0; x<=num_iterations; x++)
	{
		float compare;
		bool stop = true;
		for(int i=0;i<N;i++){
			if(x == 0){
				nodes[i]->rank.push_back(initial);
				counter++;
				if(counter == N){
					goto START;
				}
				continue;
			}
			float temp = (1-d)/N;
			for(int z=0; z < (nodes[i]->inDeg.size()); z++){
				if(nodes[nodes[i]->inDeg[z]]->outDegNum != 0)
				{
					temp += (d) * ((nodes[nodes[i]->inDeg[z]]->rank[x-1])/(nodes[nodes[i]->inDeg[z]]->outDegNum));
				}
			}
			nodes[i]->rank.push_back(temp);
			compare = ((nodes[i]->rank[x-1]) - (nodes[i]->rank[x]));
			if(compare > error_rate){ 
				stop = false; // error has met
			}
		}
		if(num_iterations == 70){
			if(stop){
				goto STOP;
			}
		}
		START:
		if(true){
			continue;
		}
	}
	
	STOP:
	if(N<10){
		for(int j=0; j<nodes[0]->rank.size(); j++) // print in his format
		{
			if(j == 0){
				cout << "Base	:  " << j << ": ";
				for(int y=0;y<N;y++){
					cout << setprecision(6) << fixed;
					cout << "P[" << y <<"]=" << nodes[y]->rank[j] << " ";
				}
				cout<< "\n";
			}
			else if(j > 0 && j < 10){
				cout << "Iter	:  " << j << ": ";
				for(int y=0;y<N;y++){
					cout << "P[" << y <<"]=" << nodes[y]->rank[j]<< " ";
				}
				cout<< "\n";
			}
			else{
				cout << "Iter	: " << j << ": ";
				for(int y=0;y<N;y++){
					cout << "P[" << y <<"]=" << nodes[y]->rank[j]<< " ";
				}
				cout<< "\n";
			}
		}
	}
	else 
	{
		cout << "Iter	: " << nodes[0]->rank.size()-1 << ": ";
		for(int y=0;y<N;y++){
			cout << "P[" << y <<"]=" << nodes[y]->rank[nodes[y]->rank.size()-1]<< " ";
		}
		cout<< "\n";
	}
}// end of main