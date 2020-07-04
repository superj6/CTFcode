#include <iostream>
#include <cstdio>
#include <algorithm>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <pty.h>
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
int master;
char buf[256];
int status;

void runcmd(string s = "", vector<string> arg = {}, vector<string> env = {}){
        pid = forkpty(&master, NULL, NULL, NULL);
        struct termios tios;
        tcgetattr(master, &tios);
        tios.c_lflag = 0;
        tcsetattr(master, TCSANOW, &tios);

        if(pid == 0){
                prctl(PR_SET_PDEATHSIG, SIGTERM);

                const char* sc = s.c_str();
                const char** argc = new const char* [arg.size() + 2];
                argc[0] = sc;
                for(int i = 0; i < arg.size(); i++) argc[i + 1] = arg[i].c_str();
                argc[arg.size() + 1] = NULL;
		const char** envc = new const char* [env.size() + 1];
                for(int i = 0; i < env.size(); i++) envc[i] = env[i].c_str();
                envc[env.size()] = NULL;

                execve(sc, (char **)argc, (char **)envc);

                exit(1);
        }

        signal(SIGPIPE, SIG_IGN);
}


bool alive(){
        waitpid(pid, &status, WNOHANG);
        return !status;
}

void killcmd(){
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
        memset(buf, 0, sizeof(buf));
        read(master, buf, 256);
        return string(buf);
}

ll cmdint(){
        ll ret = 0;
        while(1){
                read(master, buf, 1);
                if(buf[0] < '0' || buf[0] > '9') break;
                ret = 10 * ret + buf[0] - '0';
        }
        return ret;
}

ll cmdhex(bool f = 0){
        if(f){
                string s = "0x";
                for(int i = 0; i < s.size(); i++){
                        read(master, buf, 1);
                        if(buf[0] != s[i]) return 0;
                }
        }
	ll ret = 0;
        while(1){
                read(master, buf, 1);
                if(!(buf[0] >= '0' && buf[0] <= '9') && !(buf[0] >= 'a' && buf[0] <= 'f')) break;
                ret = 16 * ret + (buf[0] > '9' ? buf[0] - 'a' + 10 : buf[0] - '0');
        }
        return ret;
}


void cmdout(string s){
        s += '\n';
        write(master, s.c_str(), (int)s.size());
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

//format special chars correctly
//keep large adresses unsigned
//compile with '-lutil'
int main(){
        ios::sync_with_stdio(false);
        cin.tie(NULL);
	

        return 0;
}
