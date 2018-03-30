#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>

class node{
	public:
		float rst; //resistance
		float cap; //capacity
		node(float rst, float cap):rst(rst), cap(cap){};
};

class buffer{
	public:
		float rst; //resistance
		float cap; //capacity
		float cost; //cost
		buffer(float rst, float cap, float cost):rst(rst), cap(cap), cost(cost){};
};

int main(){
	fstream input;
	input.open("input.txt",ios::in);
	
	stringstream ss;
	char buffer = new char[10000];
	input.getline(buffer, 10000);
	ss << buffer;
	
	int num;
	ss >> num;
	
	for(;num > 0; --num){
		int max_delay , node_num, buf_num;
		ss >> max_delay;
		ss >> node_num;
		node nn[node_num];
		for(int i=0;i<node_num;++i){
			ss >> nn[i].rst;
			ss >> nn[i].cap;
		}
		
		ss >> buf_num;
		buffer bb[buf_num];
		for(int i=0;i<buf_num;++i){
			ss >> bb[i].rst;
			ss >> bb[i].cap;
			ss >> bb[i].cost;
		}
		
		cout << max_delay <<endl;
		for(int i=0;i<node_num;++i){
			cout << nn[i].rst << " " << nn[i].cap <<endl;
		}
		cout <<endl;
		for(int i=0;i<buf_num;++i){
			cout << bb[i].rst << " " << bb[i].cap << " " << bb[i].cost <<endl;
		}
	}
	return 0;
}