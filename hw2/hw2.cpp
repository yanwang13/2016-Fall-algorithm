#include<iostream>
#include<fstream>
#include<msgpack.hpp>
#include<vector>
#include<algorithm>
using namespace std;

int main(){
	
	ifstream input("input.txt", ios::in|ios::binary);
	
	//get the file size
	input.seekg(0,input.end);
	int bufsize = input.tellg();
	input.seekg(0, input.beg);
	
	char *buffer = new char[bufsize]; //buffer to store the content
	input.read(buffer, bufsize);
	
	msgpack::sbuffer sbuf;
	
	size_t offset = 0;
	vector<int> ans;
	int seq_num = 0;
	
	msgpack::object_handle oh = msgpack::unpack(buffer, bufsize, offset);
	oh.get().convert(seq_num);
	
	
	while(seq_num!=0){
		ans.clear();
		msgpack::object_handle oh = msgpack::unpack(buffer, bufsize, offset);
		oh.get().convert(ans);
		sort(ans.begin(), ans.end());
		
		msgpack::pack(sbuf, ans);
		--seq_num;
	}
	
	input.close();
	delete [] buffer;
	
	ofstream output("output.txt", ios::out|ios::binary);
	output.write(sbuf.data(), sbuf.size());
	output.close();
	
	return 0;
}