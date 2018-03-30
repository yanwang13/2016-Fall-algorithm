#include<iostream>
#include<fstream>
#include<vector>
//#include<string>
#include <algorithm>
using namespace std;

class node{
	public:
		float rst; //resistance
		float cap; //capacity
		//node(float rst, float cap):rst(rst), cap(cap){};
};

class buf{
	public:
		float rst; //resistance
		float cap; //capacity
		float cost; //cost
		//buffer(float rst, float cap, float cost):rst(rst), cap(cap), cost(cost){};
};

class state{
	public:
		float cost;
		float delay;
		float dscap;
		int cc[20];
};
bool cmp_ans(state a, state b){
	if(a.cost != b.cost)
		return a.cost < b.cost;
	else
        return a.delay < b.delay;
}
bool compare(state a, state b){
    if(a.delay != b.delay)
        return a.delay < b.delay;
	else if(a.cost != b.cost)
		return a.cost < b.cost;
    else
        return a.dscap < b.dscap;
}

int main(){
	fstream input;
	input.open("input.txt",ios::in);
	fstream output;
	output.open("output.txt", ios::out);
	
	int num;
	input >> num;

	for(;num > 0; --num){
		int max_delay , node_num, buf_num;
		input >> max_delay;
		input >> node_num;
		vector<node> nn;
		//node nn[node_num]={};
		int n;
		for(int i=0;i<node_num;++i){
			node tmp;
			input >> n;
			input >> tmp.rst;
			input >> tmp.cap;
			nn.push_back(tmp);
		}
		
		input >> buf_num;
		vector<buf> bb;
		//buffer bb[buf_num]={};
		for(int i=0;i<buf_num;++i){
			buf tmp;
			input >> n;
			input >> tmp.rst;
			input >> tmp.cap;
			input >> tmp.cost;
			bb.push_back(tmp);
		}
		
		
		vector<state> pre;
		state init;
		init.cost = 0;
		init.dscap = 0;
		init.delay = 0;
		pre.push_back(init);
		vector<state> cur;
		for(int i=node_num-1;i>=0;--i){ //from node_n to node_0
			
			for(int k=0;k < pre.size();++k){
					
					for(int j=0;j<=buf_num;++j){
						
						if(j==0){ //no buffer
							float tmp_delay = pre[k].delay + nn[i].rst * (nn[i].cap + pre[k].dscap);
							if(tmp_delay > max_delay)continue;
							
							cur.push_back(pre[k]);
							vector<state>::iterator it = cur.end()-1;
							it->delay = tmp_delay;
							//cost remain
							it->dscap += nn[i].cap;
							it->cc[i] = 0;
						}
						else{
							float tmp_delay = pre[k].delay + nn[i].rst*bb[j-1].cap + bb[j-1].rst*(nn[i].cap + pre[k].dscap);
							if(tmp_delay > max_delay)continue;
							
							cur.push_back(pre[k]);
							vector<state>::iterator it = cur.end()-1;
							it->delay = tmp_delay;
							it->cost += bb[j-1].cost;
							it->dscap = bb[j-1].cap;
							it->cc[i] = j;
						}
					}
				}
			/*if(i==node_num-1){
				for(int j=0;j<=buf_num;++j){
					if(j==0){
						state tmp;
						tmp.delay = nn[i].rst * nn[i].cap;
						if(tmp.delay > max_delay) continue;
						tmp.cost = 0;
						tmp.dscap = nn[node_num-1].cap;
						tmp.cc[i] = 0;
						cur.push_back(tmp);
					}
					else{
						state tmp;
						tmp.delay = nn[i].rst * bb[j-1].cap + bb[j-1].rst * nn[i].cap;
						if(tmp.delay > max_delay)continue;
						tmp.cost = bb[j-1].cost;
						tmp.dscap = bb[j-1].cap;
						tmp.cc[i] = j;
						cur.push_back(tmp);
					}
				}
			}
			
			else{
				if(pre.size()==0)break;
				for(int k=0;k < pre.size();++k){
					
					for(int j=0;j<=buf_num;++j){
						
						if(j==0){ //no buffer
							float tmp_delay = pre[k].delay + nn[i].rst * (nn[i].cap + pre[k].dscap);
							if(tmp_delay > max_delay)continue;
							
							cur.push_back(pre[k]);
							vector<state>::iterator it = cur.end()-1;
							it->delay = tmp_delay;
							//cost remain
							it->dscap += nn[i].cap;
							it->cc[i] = 0;
						}
						else{
							float tmp_delay = pre[k].delay + nn[i].rst*bb[j-1].cap + bb[j-1].rst*(nn[i].cap + pre[k].dscap);
							if(tmp_delay > max_delay)continue;
							
							cur.push_back(pre[k]);
							vector<state>::iterator it = cur.end()-1;
							it->delay = tmp_delay;
							it->cost += bb[j-1].cost;
							it->dscap = bb[j-1].cap;
							it->cc[i] = j;
						}
					}
				}
			}*/
			
			pre.clear();
			
			/*for(int m=1;m<cur.size();++m){
				state temp = cur[m];
				int n;
				for(n=m-1;n>=0 && cur[n].delay > temp.delay ;--n){
					cur[n+1] = cur[n];
				}
				cur[n+1]= temp;
			}*/
			sort(cur.begin(), cur.end(), compare);
			
			/*for (int i = 0; i < cur.size(); i++)
            {
                cout << cur[i].delay << " " << cur[i].dscap << " " << cur[i].cost << endl;
            }*/
			for(int m=0;m<cur.size();++m){
                pre.push_back(cur[m]);
                for(int n=m+1;n<cur.size();++n){
                    if(cur[m].delay <= cur[n].delay && cur[m].cost <= cur[n].cost && cur[m].dscap <= cur[n].dscap){
                        cur.erase(cur.begin()+n);
                        --n;
                    }
                }
            }
			cur.clear();
		}
		
		if(pre.size()==0)
			output << "NO SOLUTION";
		else{
			//sort the ans
			/*for(int m=1;m<pre.size();++m){
				state temp = pre[m];
				int n = m-1;
				for(;n>=0;--n){
					if( pre[n].cost > temp.cost)
						pre[n+1]= pre[n];
					else if(temp.cost == pre[n].cost && pre[n].delay > temp.delay)
						pre[n+1] = pre[n];
				}
				pre[n+1] = temp;
			}*/
			sort(pre.begin(), pre.end(), cmp_ans);
			
			for(int k=0;k<node_num;++k)
				if(pre[0].cc[k]!=0)
					output << k+1 << " " << pre[0].cc[k] << " ";
		}
		output << endl;
	}
	input.close();
	output.close();
	return 0;
}