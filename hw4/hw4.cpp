#include<iostream>
#include<fstream>
#include<msgpack.hpp>
#include<sstream>
#include<string>
#include<vector>

using namespace std;

class card{
	public:
		int type;
		int attack;
		
		card operator+(const card& b){
			card c;
			c.type = this->type + b.type;
			c.attack = this->attack + b.attack;
			return c;
		}
		card(){
			type = 0;
			attack = 0;
		}
};

card funct(int W, int* wt, card* val,int n){ 
	int i, w;
	card K[W+1];
	
	for(int i=0; i<n; ++i){
		for(int j=W; j-wt[i]>=0; --j){

			card tmp = K[j-wt[i]] + val[i];
			if(tmp.type==1 && K[j-wt[i]].type==0) tmp.attack +=2; //if this is the first follower;
			
			if((tmp.type<=5 && K[j].attack<tmp.attack)||(K[j].attack==tmp.attack && tmp.type < K[j].type)) 
				K[j] = tmp;
			//else if(K[j].attack==tmp.attack) K[j] = (tmp.type > K[j].type)? K[j] : tmp;
		}
	}
    return K[W];
}

int main(){
	
	fstream input, output;
	input.open("input.txt",ios::in|ios::binary);
	output.open("output.txt", ios::out);
	
	input.seekg(0,input.end);
	int bufsize = input.tellg();
	input.seekg(0, input.beg);
	
	char* buffer = new char[bufsize];
	input.read(buffer, bufsize);
	input.close();
	
	size_t offset = 0;
	msgpack::object_handle oh = msgpack::unpack(buffer, bufsize, offset);
	
	int num = 0;
	oh.get().convert(num);
	
	//where to store informations
	card* record = nullptr;
	int* cost = nullptr;
	vector<string> text;

	for(;num > 0;--num){
		int card_num = 0;
		int play_points = 0;
		oh = msgpack::unpack(buffer, bufsize, offset);
		oh.get().convert(card_num);
		oh = msgpack::unpack(buffer, bufsize, offset);
		oh.get().convert(play_points);
		
		//card* record = new card[card_num];
		record = new card[card_num];
		//int* cost = new int[card_num];
		cost = new int[card_num];
		oh = msgpack::unpack(buffer, bufsize, offset);
		//vector<string> text;
		oh.get().convert(text);
		
		stringstream ss;
		string tmp;
		for(int i=0;i<card_num;++i){ //read cards
			ss << text[i];
			ss >> tmp; //read type
			record[i].type = (tmp=="spell")? 0 : 1;
			ss >> cost[i]; //read cost
			ss >> record[i].attack; //read attack
			
			ss.str("");
			ss.clear();
			tmp.clear();
		}
		
		//sort the cards by attack
		for(int m=1;m<card_num;++m){
			card tmp = record[m];
			int tmp_cost = cost[m];
			int n;
			for(n=m-1;n>=0 && tmp.attack > record[n].attack;--n){
				record[n+1] = record[n];
				cost[n+1] = cost[n];
			}
		record[n+1]	= tmp;
		cost[n+1] = tmp_cost;
		}
		
		
		card out = funct(play_points, cost, record, card_num);
		output << out.attack <<endl;
		
		text.clear();
		delete [] record;
		delete [] cost;
	}
	delete [] buffer;
	output.close();

	return 0;
}