#include <iostream>
#include <cstdio>
#include <algorithm>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <vector>
using namespace std;
//#define endl '\n'
#define ll long long
#define pi pair<int, int>
#define f first
#define s second

pid_t pid;
int inpipefd[2];
int outpipefd[2];
char buf[256];
int status;

void runcmd(string s = "/bin/script", vector<string> arg = {"--return", "--quiet", "-c", "", "/dev/null"}){
        pid = 0;
        pipe(inpipefd);
        pipe(outpipefd);
        pid = fork();
        if(pid == 0){
                dup2(outpipefd[0], STDIN_FILENO);
                dup2(inpipefd[1], STDOUT_FILENO);
                dup2(inpipefd[1], STDERR_FILENO);

                prctl(PR_SET_PDEATHSIG, SIGTERM);

                const char* sc = s.c_str();
                const char** argc = new const char* [arg.size() + 2];
                argc[0] = sc;
                for(int i = 0; i < arg.size(); i++) argc[i + 1] = arg[i].c_str();
                argc[arg.size() + 1] = NULL;

                execv(sc, (char **)argc);

                exit(1);
        }

        close(outpipefd[0]);
        close(inpipefd[1]);

        signal(SIGPIPE, SIG_IGN);
        fcntl(inpipefd[0], F_SETFL, O_NONBLOCK);
}


bool alive(){
	waitpid(pid, &status, WNOHANG);
	return !status;
}

void killcmd(){
	close(outpipefd[1]);
	close(inpipefd[0]);
	kill(pid, SIGKILL);
	waitpid(pid, &status, 0);
}

void delay(int milliseconds){
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}

string cmdin(){
	delay(200);
	memset(buf, 0, sizeof(buf));
	read(inpipefd[0], buf, 256);
	return string(buf);
}

int cmdint(){
	int ret = 0;
	while(1){
		read(inpipefd[0], buf, 1);
		if(buf[0] < '0' || buf[0] > '9') break;
		ret = 10 * ret + buf[0] - '0';
	}
	return ret;
}

void cmdout(string s){
	s += '\n';
	write(outpipefd[1], s.c_str(), (int)s.size());
}

void cmdout(int x){
	cmdout(to_string(x));
}

void interact(){
	string s;
	while(1){
		cout << cmdin() << flush;
		if(getline(cin, s)){
			if(s == "die") break;
			cmdout(s);
		}
	}
}

string adr(ll x, int y = 8){
	string ret;
	while(y--){
		ret += (char)(x % 256);
		x /= 256;
	}
	return ret;
}

ll hex(string s, int y = 8){
	ll ret = 0;
	y = min(y, (int)s.size());
	while(y--){
		ret = 256 * ret + (256 + s[y]) % 256;
	}
	return ret;
}

//format '\x00' correctly
int main(){
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	
	
	return 0;
}
