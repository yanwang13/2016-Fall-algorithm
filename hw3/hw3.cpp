#include<iostream>
#include<vector>
#include<fstream>
#include<msgpack.hpp>
#include<sstream>
#include<utility>
#include<string>
using namespace std;

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
	
	vector<string> text;
	size_t offset = 0;
	msgpack::object_handle oh = msgpack::unpack(buffer, bufsize, offset);
	oh.get().convert(text);
	delete [] buffer;
	
	stringstream ss;
	ss << text[0];
	int num = 0;
	ss >> num;
	
	
	//cout <<num <<endl;
	
	msgpack::sbuffer sbuf;
	vector<pair<string, string>> data_set;
	vector<string> out;
	
	for(int i=1;i<=num;++i){
		int x, n;
		ss.str("");
		ss.clear();
		ss << text[2*i-1];
		ss >> x; // x is the number of element of each dataset
		
		for(int j=0;j<x;++j){
			string name, phone;
			ss >> name >> phone;
			data_set.push_back(make_pair(name, phone));
		}
		
		//sorting 
		for(int m=1;m<x;++m){
			int tmp = atoi(data_set[m].second.c_str()); //convert string into int
			pair<string, string> tmp_set = data_set[m];
			
			int n;
			for(n=m-1;n>=0 && tmp < atoi(data_set[n].second.c_str());--n)
				data_set[n+1] = data_set[n];	
			data_set[n+1] = tmp_set;
		}
		/*cout << "data_set" << i <<endl;
		for(int i=0;i<data_set.size();++i){
			cout << data_set[i].first << " " << data_set[i].second << " ";
		}
		cout <<endl;*/
		
		// find pattern
		//===================================================
		ss.str("");
		ss.clear();
		ss << text[2*i];
		ss >> n;
		string tmp; //the answer fo every pattern
		for(int j=0;j<n;++j){
			string pattern;
			ss >> pattern;
			for(std::vector<pair<string, string>>::iterator it = data_set.begin();it!=data_set.end();++it){
				if(it->second.find(pattern) != std::string::npos){
					tmp += it->first;
					tmp.push_back(' ');
					tmp += it->second;
					tmp.push_back(' ');
					//cout << it->first << " " << it->second <<" ";
				}
			}
			
		}
		//cout << endl;
		tmp.pop_back();
		//tmp.push_back('\n');
		out.push_back(tmp);
		data_set.clear();
	}
	
	msgpack::pack(sbuf, out);
	output.write(sbuf.data(), sbuf.size());
	output.close();

	return 0;
}