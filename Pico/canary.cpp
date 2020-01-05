#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <string.h>
using namespace std;
//#define endl '\n'
#define pi pair<int, int>

string call(string cmd) {

    string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];

    stream = popen(cmd.c_str(), "r");
    if (stream) {
    while (!feof(stream))
	    if (fgets(buffer, max_buffer, stream) != NULL)
	    data.append(buffer);
	    pclose(stream);
    }
    return data;
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	
	system("/problems/canary_3_257a2a2061c96a7fb8326dbbc04d0328/vuln");
	string s = call("echo $!");

	freopen(("/proc/" + s + "/fd/0").c_str(), "r", stdin);
	freopen(("/proc/" + s + "/fd/1").c_str(), "w", stdin);
	
	cout << 5 << endl;
	cin >> s;
	cout << s << endl;

	return 0;
}
