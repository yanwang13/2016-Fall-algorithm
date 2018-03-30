#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<string>
using namespace std;

int main(){
	
	ifstream input("input.txt", ios::in);
	ofstream output("output.txt", ios::out);
	
	if(!input)
		cout << "input file cannot be open.\n";
	if(!output)
		cout << "output file cannot be open.\n";
	
	int num;
	input >> num;
	input.ignore(100,'\n');
	string str;
	vector<int> ans;
	
	while(num){
		int tmp = 0;
		int n =0; //to count how many numbers to sort
		
		str.clear();
		ans.clear();
		
		getline(input, str);

		for(string::iterator it=str.begin();it!=str.end();++it){
			if(*it>='0' && *it <= '9')
				tmp = tmp*10 + (*it - '0');
			else{
				ans.push_back(tmp);
				tmp = 0;
				++n;
			}
		}
		if(tmp!=0)
			ans.push_back(tmp);
		
		sort(ans.begin(), ans.begin()+n);
		
		for(int i=0;i<n-1;++i){
			output<< ans[i] << " ";
		}
		output << ans[n-1] <<"\n";
		--num;
	}
	
	input.close();
	output.close();
	
	return 0;
}