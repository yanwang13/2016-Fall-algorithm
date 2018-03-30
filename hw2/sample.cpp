#include<iostream>
#include<msgpack.hpp>
#include<vector>
#include<fstream>

using namespace std;
void write_msg_file(){
    int x[] = {5,5,6,6};
    int y[] = {9,4,8,7};
    vector<int> arr1(x, y+sizeof(x)/sizeof(x[0])); //initialize vector<int> arr1: 5,5,6,6
    vector<int> arr2(y, y+sizeof(y)/sizeof(y[0])); //initialize vector<int> arr2: 9,4,8,7
    
    msgpack::sbuffer sbuf;
    cout<<"Encode vector to msgpack object"<<endl;
    msgpack::pack(sbuf, x);
    msgpack::pack(sbuf, y);
    fstream outf;
    cout<<"Write to hw2.txt"<<endl;
    outf.open("hw2.txt", ios::out | ios::binary);
    outf.write(sbuf.data(), sbuf.size() );
    outf.close();
}

void read_msg_file(){
    fstream inf;
    inf.open("hw2.txt", ios::in | ios::binary ); 
    size_t off;
    int bufsize;
    char *buf = NULL;
    vector<int> arr;
    msgpack::object_handle oh;
    //To get the file size or you can use POSIX: stat 
    inf.seekg(0, ios::end);
    bufsize = inf.tellg();
    inf.seekg(0);  //reset the postion
    
    //create a buf size
    buf = new char[bufsize];
    inf.read(buf, bufsize);
    
    off = 0;
    cout<<"The buf offset: "<<off<<endl;
    oh = msgpack::unpack(buf, bufsize, off); //unpack the data to get the object handler
    oh.get().convert(arr); //get the vector<int> type
    cout<<"After the unpack function buf offset: "<<off<<endl;  //you can see the offset change
    cout<<"arr1 content"<<endl;
    for(int i = 0 ; i < arr.size() ; i++){
        cout<<arr[i]<<" ";
    }
    cout<<endl; 
    oh = msgpack::unpack(buf, bufsize, off);
    oh.get().convert(arr);
    cout<<"After the unpack function buf offset: "<<off<<endl;  //you can see the offset change
    cout<<"arr2 content"<<endl;
    for(int i = 0 ; i < arr.size() ; i++){
        cout<<arr[i]<<" ";
    }
}
int main(){
    cout<<"Writeing file"<<endl; 
    write_msg_file(); 
    cout<<endl<<"====================================="<<endl<<endl;
    cout<<"Reading file"<<endl;
    read_msg_file();
    return 0;
}
