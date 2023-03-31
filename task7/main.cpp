#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <signal.h>
#include <fcntl.h>
#include <ctime>
#include "lgraph.cpp"
#define PORTNUM 8081
using namespace std;
class client{
    char *concatenation(const char *buf, char *name){
        int size = strlen("User \0") + strlen(name) + strlen(": \0") + 1;
        if(buf != NULL) size += strlen(buf);
        char *result = (char*)malloc(size*sizeof(char)); // +1 for the null-terminator
        strcpy(result, "User ");
        strcat(result, name);
        strcat(result, ": \0");
        if(buf != NULL) strcat(result, buf);
        result[size-1] = '\0';
        return result;
    }
    char **privates;
    int cur_privates;
    int size_privates;
    typedef enum {Nickname, Normal, Password} States;
    States state;
    char admin_password[strlen("12345")+1] = {"12345"};
public:
    int admin;
    int socket;
    char *name;
    int send_message;
    client(){
        this->socket = -1;
        this->admin = 0;
        send_message = 0;
        privates = NULL;
        name = NULL;
        cur_privates = 0;
        size_privates = 0;
    }
    int accept_client(int ls, client *desk, int *cur_desk){
        this->socket = accept(ls, NULL, NULL);
        cout<<"socket_desc = "<<this->socket<<endl;
        write(this->socket, "Enter Nickname: \n", strlen("Enter Nickname: \n"));
        state = Nickname;
        cout<<"socket client: "<<this->socket<<endl;
        if(this->socket == -1){
            cout<<"error: accept is bad"<<endl;
            return -1;
        }
       (*cur_desk)++;
        return 0;
    }
    int check_state(){
        return this->state == Nickname;
    }
    int read_client(char ***mass, char *buf, int *sizelist, int *curlist, int *sizebuf, int *curbuf){
        int k = 0;
        if(state == Nickname){
            name = (char*)malloc(strlen((*mass)[0]) + 1);
            strcpy(name,(*mass)[0]);
            name[strlen((*mass)[0])] = '\0';
            state = Normal;
            clearlist(*mass, sizelist, curlist);
            k = 6;
            return k;
        }
        if(state == Password){
            char *password = (char*)malloc(strlen((*mass)[0]) + 1);
            strcpy(password,(*mass)[0]);
            password[strlen((*mass)[0])] = '\0';
            if(strcmp(password,admin_password) == 0) this->admin = 1;
            else write(this->socket,"Incorrect password\n", strlen("Incorrect password\n"));
            state = Normal;
            clearlist(*mass, sizelist, curlist);
            free(password);
            k = -1;
            return k;
        }
        if((*mass)[0][0] == '\\'){
            char *str = (char*)malloc(strlen((*mass)[1])+1);
            strcpy(str,(*mass)[1]);
            str[strlen(str)] = '\0';
            if(strcmp(str,"users\0") == 0) k = 1;
            if(strcmp(str,"quit\0") == 0) k = 2;
            if(strcmp(str,"private\0") == 0) k = 3;
            if(strcmp(str,"help\0") == 0) k = 4;
            if(strcmp(str,"privates\0") == 0) k = 5;
            if(strcmp(str,"admin\0") == 0) k = 7;
            if(strcmp(str,"ban\0") == 0) k = 8;
            if(strcmp(str,"kick\0") == 0) k = 9;
            if(strcmp(str,"nick\0") == 0) k = 10;
            if(strcmp(str,"shutdown\0") == 0) k = 11;
            free(str);
        }
        return k;
    }
    int write_client(char ***mass, int *sizelist, int *curlist, char *name){
        if(this->state == Nickname || this->state == Password) return 0;
        char *str = concatenation(NULL, name);
        int f = open("log",O_WRONLY |O_APPEND | O_CREAT, 0777);
        write(this->socket, str, strlen(str));
        write(f, str, strlen(str));
        printlist(mass, sizelist, this->socket, f);
        close(f);
        free(str);
        return 0;
    }
    int add_privates(char *name){
        if(cur_privates > size_privates - 1){
            privates = (char**)realloc(privates,(size_privates+=SIZE)*sizeof(char*));
        }
        char *str = new char[strlen(name)+1];
        strcpy(str, name);
        str[strlen(name)] = '\0';
        privates[cur_privates++] = str;
        return 0;
    }
    int print_privates(){
        int i;
        write(this->socket, "Sent private message to users: ", strlen("Sent private message to users: "));
        if(cur_privates == 0){
            write(this->socket, "You have not sent private messages to anyone\n", strlen("You have not sent private messages to anyone\n"));
            return 0;
        }
        for(i = 0; i < cur_privates; i++){
            write(this->socket, privates[i], strlen(privates[i]));
            write(this->socket, " ", 1);
        }
        write(this->socket, "\n", 1);
        return 0;
    }
    int private_message(char **mass, int sizelist, int curlist, char *name, client *address){
        int i;
        int f = open("log",O_WRONLY |O_APPEND | O_CREAT, 0777);
        this->add_privates(address->name);
        char *str = concatenation("Private message: ", name);
        write(address->socket, str, strlen(str));
        write(f, str, strlen(str));
        for(i = 3; i < sizelist-1; i++){
            write(address->socket,mass[i] , strlen(mass[i]));
            write(address->socket," ", 1);
            write(f, mass[i], strlen(mass[i]));
            write(f," ", 1);
        }
        write(address->socket,"\n", 1);
        write(f, "\n", 1);
        close(f);
        free(str);
        return 0;
    }
    int qiut_client(char **mass, int sizelist, int curlist, char *name) {
        int j = 0, i = 0;
        int f = open("log",O_WRONLY |O_APPEND | O_CREAT, 0777);
        char *str = concatenation("Leaves the channel: ", name);
        write(this->socket, str, strlen(str));
        write(f, str, strlen(str));
        for(i = 2; i<sizelist-1; i++){
            write(this->socket,mass[i] , strlen(mass[i]));
            write(f,mass[i] , strlen(mass[i]));
            write(f," ", 1);
            write(this->socket," ", 1);
        }
        write(this->socket,"\n", 1);
        write(f,"\n", 1);
        close(f);
        free(str);
        return 0;
    }
    int entry_client(char *name){
        if(this->state != Normal) return 0;
        int f = open("log",O_WRONLY | O_APPEND | O_CREAT, 0777);
        char *str = concatenation("Walked in the room\n", name);
        write(this->socket, str, strlen(str));
        write(f, str, strlen(str));
        free(str);
        close(f);
        return 0;
    }
    int change_nickname(char ***mass, int *sizelist, int *curlist, client *user = NULL){
        if(admin && *curlist > 3 && user->admin){
            char str[] = {"You do not have sufficient rights to execute this command\n"};
            write(this->socket, str, strlen(str));
            clearlist(*mass, sizelist, curlist);
            return -1;
        }
        if(admin && *curlist > 3){
            free(user->name);
            user->name = (char*)malloc(strlen((*mass)[3])+1);
            strcpy(user->name,(*mass)[3]);
            user->name[strlen(user->name)] = '\0';
            clearlist(*mass, sizelist, curlist);
            return 0;
        }
        free(name);
        name = (char*)malloc(strlen((*mass)[2]) + 1);
        strcpy(name,(*mass)[2]);
        name[strlen((*mass)[2])] = '\0';
        clearlist(*mass, sizelist, curlist);
        return 0;
    }
    int ban_client(char **mass, int sizelist, int curlist, client *ban_client, int flag_kick){
        if(flag_kick) cout<<"flag_kick"<<endl;
        if(!admin || ban_client->admin){
            char str[] = {"You do not have sufficient rights to execute this command\n"};
            write(this->socket, str, strlen(str));
            return -1;
        }
        char str[] = {"you are banned\n"};
        int f = open("log",O_WRONLY |O_APPEND | O_CREAT, 0777);
        write(ban_client->socket, str, strlen(str));
        write(f, str, strlen(str));
        for(int i = 3; i<sizelist-1; i++){
            write(ban_client->socket, mass[i] , strlen(mass[i]));
            write(f, mass[i] , strlen(mass[i]));
            write(f," ", 1);
            write(ban_client->socket, " ", 1);
        }
        write(ban_client->socket, "\n", 1);
        write(f,"\n", 1);
        close(f);
        return 0;
    }
    int set_admin(){
        this->state = Password;
        char str[] = {"Enter admin password: \n"};
        write(this->socket, str, strlen(str));
        return 0;
    }
    int get_messages(){
        return send_message;
    }
    int clear(){
        cout<<"clear client "<<this->name<<endl;
        for(int i = 0; i < cur_privates; i++) delete []privates[i];
        size_privates = 0;
        cur_privates = 0;
        free(privates);
        free(name);
        return 0;
    }
};
class channel{
    char *name;
    char *password;
    client *desk;
    int size_desk;
    char **ban_nick;
    int size_ban;
    int cur_ban;
public:
    int cur_desk;
    channel(){
        name = NULL;
        password = NULL;
        desk = NULL;
        ban_nick = NULL;
        cur_ban = 0;
        size_ban = 0;
        size_desk = 0;
        cur_desk = 0;
    }
    int add_client(int socket){
        client reader;
        if(cur_desk > size_desk - 1){
            desk = (client*)realloc(desk,(size_desk+=SIZE)*sizeof(client));
        }
        desk[cur_desk] = reader;
        desk[cur_desk].accept_client(socket, desk, &cur_desk);
        return 0;
    }
    int update(client **a, int *cur){
        *a = desk;
        *cur = cur_desk;
        return 0;
    }
    int delit_client(client *a){
        client* desk_new = (client*)malloc(size_desk*sizeof(client));
        int i,j=0;
        for(i = 0; i<cur_desk; i++){
            if(desk[i].name != a->name){
                desk_new[j] = desk[i];
                j++;
            }
        }
        a->clear();
        free(desk);
        cur_desk--;
        desk = desk_new;
        return 0;
    }
    int check_nick(client *user, char *name){
        int k = 0, i;
        char *str = new char[strlen(name)+1];
        strcpy(str, name);
        str[strlen(str)] = '\0';
        if(user->check_state()){
            for(i = 0; i < cur_desk-1; i++) if(strcmp(str,desk[i].name) == 0) k = 1;
        }
        delete []str;
        return k;
    }
    int check_ban(client *user, char *name){
        int k = 0;
        char *str = (char*)malloc(strlen(name)+1);
        strcpy(str, name);
        str[strlen(str)] = '\0';
        if(user->check_state()){
            for(int i = 0; i < cur_ban; i++){
                k = !strcmp(str, ban_nick[i]);
                if(k) break;
            }
        }
        free(str);
        return k;
    }
    int add_ban_nick(char *nick){
        if(cur_ban > size_ban - 1){
            ban_nick = (char**)realloc(ban_nick,(size_ban+=SIZE)*sizeof(char*));
        }
        char *nickname = (char*)malloc(strlen(nick)+1);
        strcpy(nickname, nick);
        nickname[strlen(nick)] = '\0';
        ban_nick[cur_ban++] = nickname;
        return 0;
    }
    ~channel(){
        for(int i = 0; i < cur_desk; i++) desk[i].clear();
        for(int i = 0; i < cur_ban; i++) free(ban_nick[i]);
        free(ban_nick);
        free(desk);
        cur_ban = 0;
        size_ban = 0;
        cur_desk = 0;
        size_desk = 0;
        cout<<"delit chat"<<endl;
    }
};
class server {
    channel *chat;
    int size_chat;
    int cur_chat;
public:
    int main_socket;
    server(){
        chat = NULL;
        size_chat = 0;
        cur_chat = 0;
        this->main_socket = socket(AF_INET, SOCK_STREAM, 0);
        cout<<this->main_socket<<endl;
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(PORTNUM);
        addr.sin_addr.s_addr = INADDR_ANY;
        if (bind(this->main_socket, (struct sockaddr *) &addr, sizeof(addr)) != 0) {
            cout<<"error: bind is bad"<<endl;
            exit(0);
        }
        if (listen(this->main_socket, 5) == -1) {
            cout<<"error: listen is bad"<<endl;
            exit(0);
        }
    }
    int add_channel(channel *ch){
        if(cur_chat > size_chat - 1){
            chat = (channel*)realloc(chat,(size_chat+=SIZE)*sizeof(channel));
        }
        chat[cur_chat++] = *ch;
        return 0;
    }
    int get_socket(){
        return main_socket;
    }
    ~server(){
        free(chat);
        cur_chat = 0;
        size_chat = 0;
        shutdown(this->main_socket, 2);
        cout<<"delit server"<<endl;
    }
};

void exit(int i){
    cout<<"here\n";
};
int main(int argc, char **argv){
    int i, k, opt = 1, cur_desk = 0, buf_size = N, tr = 1, j = 0, flag = 1;
    int f = open("log", O_WRONLY | O_TRUNC | O_CREAT, 0777);
    close(f);
    char help[] = {"Valid Commands: \\help, \\users, \\quit <message>, \\private, \\privates, \\nick <new name>, \\admin\n"};
    char **mass = NULL, **mass2 = NULL;
    char *buf = NULL;
    char num[12];
    time_t times = time(NULL);
    int sizelist = 0, curlist = 0, sizebuf = 0, curbuf = 0, sizelist2 = 0;
    channel chat;
    server ls;
    client *desk;
    ls.add_channel(&chat);
    signal(SIGINT, &exit);
    while(1){
        cout<<1<<endl;
        chat.update(&desk, &cur_desk);
        fd_set readfds;
        int max_d = ls.main_socket;
        FD_ZERO(&readfds);
        FD_SET(ls.main_socket, &readfds);
        if(desk){
            for(i = 0; i < cur_desk; i++){
                FD_SET(desk[i].socket, &readfds);
                if(desk[i].socket > max_d) max_d = desk[i].socket;
            }
        }
        int res = select(max_d+1, &readfds, NULL, NULL, NULL);
        if(res < 1) {
            cout<<"error: select is bad"<<endl;
            return 0;
        }
        if(FD_ISSET(ls.main_socket, &readfds)) {
            //пришел запрос на подключение
            cout<<"Connection request received"<<endl;
            chat.add_client(ls.get_socket());
            chat.update(&desk,&cur_desk);
        }
        if(time(NULL) - times >= 10){
            cout<<"TIME"<<endl;
            int max = 0, sum = 0, j = -1;
            for(i = 0; i < cur_desk; i++){
                sum = desk[i].get_messages();
                cout<<desk[i].name<<" "<<sum<<endl;
                if(max < sum){
                    max = sum;
                    j = i;
                }
            }
            char str[] = {"User sent the most messages: "};
            if(max != 0){ 
                for(i = 0; i < cur_desk; i++){
                    write(desk[i].socket, str, strlen(str));
                    write(desk[i].socket, desk[j].name, strlen(desk[j].name));
                    write(desk[i].socket, "\n", 1);
                }
            } else cout<<"there are no more users who sent a message"<<endl;
            times = time(NULL);
        }
        for(i = 0; i < cur_desk; i++){
            if(FD_ISSET(desk[i].socket, &readfds)) {
                lex(&mass, buf, &sizelist, &curlist, &sizebuf, &curbuf, desk[i].socket);
                if(curlist == 0){
                    clearlist(mass, &sizelist, &curlist);
                    shutdown(desk[i].socket, 2);
                    chat.delit_client(&desk[i]);
                    chat.update(&desk, &cur_desk);
                    continue;
                }
                if(chat.check_ban(&desk[i], mass[0])){
                    write(desk[i].socket, "This nickname is banned\n", strlen("This nickname is banned\n"));
                    write(desk[i].socket, "Enter Nickname: \n", strlen("Enter Nickname: \n"));
                    clearlist(mass, &sizelist, &curlist);
                    continue;
                } else if(chat.check_nick(&desk[i], mass[0])){
                    write(desk[i].socket, "This nickname is busy\n", strlen("This nickname is busy\n"));
                    write(desk[i].socket, "Enter Nickname: \n", strlen("Enter Nickname: \n"));
                    clearlist(mass, &sizelist, &curlist);
                    continue;
                }
                k = desk[i].read_client(&mass, buf, &sizelist, &curlist, &sizebuf, &curbuf);
                switch (k){
                case 0:
                    desk[i].send_message++;
                    for(int j = 0; j < cur_desk; j++){
                        desk[j].write_client(&mass, &sizelist, &curlist, desk[i].name);
                    }
                    clearlist(mass, &sizelist, &curlist);
                    break;
                case 1:
                    write(desk[i].socket,"Users: \0", strlen("Users: \0"));
                    for(int j = 0; j < cur_desk; j++){
                        write(desk[i].socket, desk[j].name, strlen(desk[j].name));
                        write(desk[i].socket," ", 1);
                    }
                    write(desk[i].socket,"\n", 1);
                    clearlist(mass, &sizelist, &curlist);
                    break;
                case 2:
                    desk[i].send_message++;
                    for(int j = 0; j < cur_desk; j++) desk[j].qiut_client(mass, sizelist, curlist, desk[i].name);
                    clearlist(mass, &sizelist, &curlist);
                    shutdown(desk[i].socket, 2);
                    chat.delit_client(&desk[i]);
                    chat.update(&desk, &cur_desk);
                    break;
                case 3:
                    if(curlist < 3) write(desk[i].socket, "Server: few arguments\n", strlen("Server: few arguments\n"));
                    else {
                        for(j = 0; j < cur_desk; j++){
                            if(strcmp(mass[2],desk[j].name) == 0) flag = desk[i].private_message(mass, sizelist, curlist, desk[i].name, &desk[j]);
                        }
                        if(flag){
                            char str[] = {"Server: No user found with this nickname! Use the \\users command to see active users\n"};
                            write(desk[i].socket, str, strlen(str));
                        }
                        desk[i].send_message++;
                    }
                    clearlist(mass, &sizelist, &curlist);
                    break;
                case 4:
                    write(desk[i].socket, help, strlen(help));
                    clearlist(mass, &sizelist, &curlist);
                    break;
                case 5:
                    desk[i].print_privates();
                    clearlist(mass, &sizelist, &curlist);
                    break;
                case 6:
                    for(j = 0; j < cur_desk; j++) desk[j].entry_client(desk[i].name);
                    break;
                case 7:
                    desk[i].set_admin();
                    clearlist(mass, &sizelist, &curlist);
                    break;
                case 8:
                case 9:
                    if(curlist < 3) write(desk[i].socket, "Server: few arguments\n", strlen("Server: few arguments\n"));
                    else {
                        if(curlist == 3) mass[2][strlen(mass[2])] = '\0';
                        for(j = 0; j < cur_desk; j++){
                            if(strcmp(mass[2],desk[j].name) == 0){
                                flag = desk[i].ban_client(mass, sizelist, curlist, &desk[j], (k==9));
                                if(flag == -1) break;
                                shutdown(desk[j].socket, 2);
                                if(k != 9) chat.add_ban_nick(desk[j].name);
                                chat.delit_client(&desk[j]);
                                chat.update(&desk, &cur_desk);
                                break;
                            }
                        }
                        if(flag == 1){
                            char str[] = {"Server: No user found with this nickname! Use the \\users command to see active users\n"};
                            write(desk[i].socket, str, strlen(str));
                        }
                    }
                    clearlist(mass, &sizelist, &curlist);
                    break;
                case 10:
                    if(curlist < 2) write(desk[i].socket, "Server: few arguments\n", strlen("Server: few arguments\n"));
                    else if(curlist > 3){
                        for(j = 0; j < cur_desk; j++){
                            if(strcmp(mass[2], desk[j].name) == 0){
                                desk[i].change_nickname(&mass, &sizelist, &curlist, &desk[j]);
                                break;
                            }
                        }
                    } else desk[i].change_nickname(&mass, &sizelist, &curlist);
                    break;
                case 11:
                    mass2 = mass + 2;
                    sizelist2 = sizelist - 2;
                    if(!desk[i].admin){
                        char str[] = {"You do not have sufficient rights to execute this command\n"};
                        write(desk[i].socket, str, strlen(str));
                        clearlist(mass, &sizelist, &curlist);
                        break;
                    }
                    for(j = 0; j < cur_desk; j++) desk[j].write_client(&mass2, &sizelist2, &curlist, desk[i].name);
                    clearlist(mass, &sizelist, &curlist);
                    for(j = 0; j < cur_desk; j++){
                        shutdown(desk[j].socket,2);
                        close(desk[j].socket);
                    }
                    break;
                default: break;
                }
            }
        }
    }
    return 0;
}