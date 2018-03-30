#include<msgpack.hpp>
#include<vector>
#include<fstream>
#include<queue>
using namespace std;
bool traveled[500][500];
int graph[500][500];
struct step
{
    int i, j, last, step;
};
int main()
{
    ifstream inf("input.txt", ios::binary);
    ofstream outf("output.txt", ios::binary);
    inf.seekg(0, inf.end);
    int bufsize = inf.tellg();
    inf.seekg(0, inf.beg);

    char *buf = new char[bufsize];
    inf.read(buf, bufsize);

    int T;
    size_t off = 0;
    msgpack::object_handle oh;
    oh = msgpack::unpack(buf, bufsize, off);
    oh.get().convert(T);

    while(T--)
    {
        int ans[60000] ={0};
        int row, col, type_num;
        vector<int> qq;
        oh = msgpack::unpack(buf, bufsize, off);
        oh.get().convert(row);
        oh = msgpack::unpack(buf, bufsize, off);
        oh.get().convert(col);
        oh = msgpack::unpack(buf, bufsize, off);
        oh.get().convert(type_num);
        oh = msgpack::unpack(buf, bufsize, off);
        oh.get().convert(qq);

        for(int i = 0, size = row*col; i < size; ++i)
            graph[i/col][i%col] = qq[i];

        memset(traveled, 0, sizeof(traveled));
        queue<step> BFS;
        for(int i = 0; i < row; ++i)
            for(int j = 0; j < col; ++j)
            {
                if(traveled[i][j] || ans[graph[i][j]] == -1) continue;

                step tmp;
                tmp.i = i;
                tmp.j = j;
                tmp.last = -1;
                tmp.step = 0;
                BFS.push(tmp);

                bool flag = true;
                while(1)
                {
                    step it = BFS.front(); BFS.pop();
                    traveled[it.i][it.j] = true;
                    if(graph[it.i+1][it.j] == graph[it.i][it.j] && it.last != 0)
                    {
                        if(traveled[it.i+1][it.j] && flag)
                        {
                            ans[graph[it.i][it.j]] = -1;
                            BFS = queue<step>();
                            break;
                        }
                        tmp.i = it.i + 1;
                        tmp.j = it.j;
                        tmp.last = 1;
                        tmp.step = it.step + 1;
                        BFS.push(tmp);
                    }
                    if(graph[it.i-1][it.j] == graph[it.i][it.j] && it.last != 1)
                    {
                        if(traveled[it.i-1][it.j] && flag)
                        {
                            ans[graph[it.i][it.j]] = -1;
                            BFS = queue<step>();
                            break;
                        }
                        tmp.i = it.i - 1;
                        tmp.j = it.j;
                        tmp.last = 0;
                        tmp.step = it.step + 1;
                        BFS.push(tmp);
                    }
                    if(graph[it.i][it.j+1] == graph[it.i][it.j] && it.last != 2)
                    {
                        if(traveled[it.i][it.j+1] && flag)
                        {
                            ans[graph[it.i][it.j]] = -1;
                            BFS = queue<step>();
                            break;
                        }
                        tmp.i = it.i;
                        tmp.j = it.j + 1;
                        tmp.last = 3;
                        tmp.step = it.step + 1;
                        BFS.push(tmp);
                    }
                    if(graph[it.i][it.j-1] == graph[it.i][it.j] && it.last != 3)
                    {
                        if(traveled[it.i][it.j-1] && flag)
                        {
                            ans[graph[it.i][it.j]] = -1;
                            BFS = queue<step>();
                            break;
                        }
                        tmp.i = it.i;
                        tmp.j = it.j - 1;
                        tmp.last = 2;
                        tmp.step = it.step + 1;
                        BFS.push(tmp);
                    }
                    if(BFS.empty())
                    {
                        if(flag)
                        {
                            flag = false;
                            tmp.i = it.i;
                            tmp.j = it.j;
                            tmp.last = -1;
                            tmp.step = 0;
                            BFS.push(tmp);
                        }
                        else
                        {
                            if(it.step > ans[graph[it.i][it.j]] ) ans[graph[it.i][it.j]] = it.step;
                            break;
                        }
                    }
                }
            }
            char temp[4] = {0};
            vector<string> output(type_num);
            for(int i = 1; i <= type_num; ++i)
                if(ans[i] == -1) output[i-1] = "cycle";
                else
                {
                    if (ans[i] < 10)
                    {
                        temp[0] = '0' + ans[i];
                        temp[1] = 0;
                    }
                    else if (ans[i] < 100)
                    {
                        temp[0] = '0' + ans[i] / 10;
                        temp[1] = '0' + ans[i] % 10;
                        temp[2] = 0;
                    }
                    else
                    {
                        temp[0] = '0' + ans[i]/100;
                        ans[i] %= 100;
                        temp[1] = '0' + ans[i] / 10;
                        temp[2] = '0' + ans[i] % 10;
                        temp[3] = 0;
                    }
                    output[i-1] = string(temp);
                }
            msgpack::sbuffer sbuf;
            msgpack::pack(sbuf , output) ;
            outf.write(sbuf.data() , sbuf.size()) ;
    }
    delete []buf;
    inf.close();
    outf.close();
    return 0;
}
