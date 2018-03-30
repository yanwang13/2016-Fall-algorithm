#include<iostream>
#include<fstream>
#include<vector>
#include<queue>
#include<string>
#include<msgpack.hpp>
using namespace std;

class state{
	public:
		int rr;
		int cc;
		int dir;
		int len;
};

int main(){
	fstream input, output;
	input.open("input.txt",ios::in|ios::binary);
	output.open("output.txt", ios::out|ios::binary);
	
	input.seekg(0,input.end);
	int bufsize = input.tellg();
	input.seekg(0, input.beg);
	
	char* buffer = new char[bufsize];
	input.read(buffer, bufsize);
	input.close();
	
	size_t offset = 0;
	msgpack::object_handle oh = msgpack::unpack(buffer, bufsize, offset);
	
	int case_num = 0;
	oh.get().convert(case_num);
	
	msgpack::sbuffer sbuf;
	vector<string>out;
	
	int row, col, types;
	vector<int> graph;
	queue<state> qq;
	int ans[60000]={0};
	bool visit[250000]={0};
	for(;case_num>0;--case_num){
		oh = msgpack::unpack(buffer, bufsize, offset);
		oh.get().convert(row); //get row
		oh = msgpack::unpack(buffer, bufsize, offset);
		oh.get().convert(col); //get col
		oh = msgpack::unpack(buffer, bufsize, offset);
		oh.get().convert(types); //get number of types
		
		oh = msgpack::unpack(buffer, bufsize, offset);
		oh.get().convert(graph);
		
		int map[500][500];
		for(int i=0;i<row*col;++i)
			map[i/col][i%col] = graph[i];

		for(int k=0;k<types+1;++k)
			ans[k] = 0;
		
		//do the BFS
		for(int i=0;i<row;++i){
			for(int j=0;j<col;++j){
				
				int tp = map[i][j];
				if(ans[tp]==-1||visit[i*col+j]) continue;
				
				for(int k=0;k<row*col;++k)//initial visit
					visit[k] = 0;
				
				state root;
				root.rr = i;
				root.cc = j;
				root.len = 0;
				root.dir = -1;
				qq.push(root);
				visit[root.rr*col+root.cc] = true;
				
				while(!qq.empty()){
				
					// up>>0 down>>1 left>>2 right>>3
					state pre = qq.front();
					qq.pop();
					//cout << "pre ("<<pre.rr<<","<<pre.cc<<")"<<endl;
					state tmp;

					//up (i-1)*col + j ff>=col
					if( pre.rr>0 && map[pre.rr-1][pre.cc]==tp){
						if(visit[(pre.rr-1)*col+pre.cc] && pre.dir!=1){ // the vertex is reachable but already visited
							ans[tp] = -1;
							break;
						}
						else if(!visit[(pre.rr-1)*col+pre.cc]){
							tmp.rr = pre.rr-1;
							tmp.cc = pre.cc;
							tmp.len = pre.len+1;
							tmp.dir = 0;
							qq.push(tmp);
							//cout << "push up ("<<tmp.rr<<","<<tmp.cc<<")"<<endl;
							visit[tmp.rr*col+tmp.cc] = true;
						}
					}
					//down (i+1)*col + j
					if( pre.rr<row-1 && map[pre.rr+1][pre.cc]==tp){
						if(visit[(pre.rr+1)*col+pre.cc] && pre.dir!=0){
							ans[tp] = -1;
							break;
						}
						else if(!visit[(pre.rr+1)*col+pre.cc]){
							tmp.rr = pre.rr+1;
							tmp.cc = pre.cc;
							tmp.len = pre.len+1;
							tmp.dir = 1;
							qq.push(tmp);
							//cout << "push down ("<<tmp.rr<<","<<tmp.cc<<")"<<endl;
							visit[tmp.rr*col+tmp.cc] = true;
						}
					}
					//left i*col + (j-1)
					if( pre.cc>0 && map[pre.rr][pre.cc-1]==tp){
						if(visit[pre.rr*col+pre.cc-1] && pre.dir!=3){
							ans[tp] = -1;
							break;
						}
						else if(!visit[pre.rr*col+pre.cc-1]){
							tmp.rr = pre.rr;
							tmp.cc = pre.cc-1;
							tmp.len = pre.len+1;
							tmp.dir = 2;
							qq.push(tmp);
							//cout << "push left ("<<tmp.rr<<","<<tmp.cc<<")"<<endl;
							visit[tmp.rr*col+tmp.cc] = true;
						}
					}
					//right i*col + (j+1)
					if( pre.cc<col-1 && map[pre.rr][pre.cc+1]==tp){
						if(visit[pre.rr*col+pre.cc+1] && pre.dir!=2){
							ans[tp] = -1;
							break;
						}
						else if(!visit[pre.rr*col+pre.cc+1]){
							tmp.rr = pre.rr;
							tmp.cc = pre.cc+1;
							tmp.len = pre.len+1;
							tmp.dir = 3;
							qq.push(tmp);
							//cout << "push right ("<<tmp.rr<<","<<tmp.cc<<")"<<endl;
							visit[tmp.rr*col+tmp.cc] = true;
						}
					}
					
					//cout<<"tmp.len = " <<tmp.len<<endl;
					if(ans[tp]>=0 && pre.len>ans[tp])
						ans[tp] = pre.len;
				}
				while(!qq.empty()) // clear the queue
					qq.pop();
			}
		}

		for(int i=1;i<types+1;++i){
			if(ans[i]==-1)
				out.push_back("cycle");
			else
				out.push_back(to_string(ans[i]));
		}
		msgpack::pack(sbuf, out);
		graph.clear();
		out.clear();
	}
	delete [] buffer;

	output.write(sbuf.data(), sbuf.size());
	output.close();
	
	return 0;
}
