#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#define SIZE 16
#define N 10
using namespace std;
void null_list(char ***mass, int *sizelist, int *curlist){
    *mass = NULL;
    *sizelist = 0;
    *curlist = 0;
}

//очищение списка
void clearlist(char **mass, int *sizelist, int *curlist){
    int i;
    if (mass != NULL){
        for(i=0; i<*sizelist-1; i++){ 
            free(mass[i]);
        }
        free(mass[i]);
        free(mass);
        mass = NULL;
    }
    *curlist = 0;
    *sizelist = 0;
}
//корректировка размеров списка
void termlist(char ***mass, int *sizelist, int *curlist){
    if (*mass != NULL){
        if (*curlist>(*sizelist)-1) *mass = (char**)realloc(*mass, ((*sizelist)+1)*sizeof(**mass));
        (*mass)[*curlist] = NULL;
        *sizelist = (*curlist)+1;
        *mass = (char**)realloc(*mass, ((*sizelist)*sizeof(**mass)));
    }
}
//печать списка
void printlist(char ***mass, int *sizelist, int socket, int f){
    int i;
    if (*mass != NULL){
        write(socket,(*mass)[0], strlen((*mass)[0]));
        write(f,(*mass)[0], strlen((*mass)[0]));
        for(i = 1; i<(*sizelist)-1; i++){
            write(f," ", 1);
            write(socket," ", 1);
            write(f,(*mass)[i] , strlen((*mass)[i]));
            write(socket,(*mass)[i] , strlen((*mass)[i]));
        }
    }
    write(f,"\n", 1);
    write(socket,"\n", 1);
}
//очищаем буфер
void null_buf(char *buf, int *sizebuf, int *curbuf){
    buf = NULL;
    *sizebuf = 0;
    *curbuf = 0;
}
//добавляем символ в буфер
void addsym(int c, char **buf, int *sizebuf, int *curbuf){
    if (*curbuf > (*sizebuf)-1){
        *buf = (char*)realloc(*buf, (*sizebuf)+=SIZE);
    }
    (*buf)[(*curbuf)++] = c;
}
//проверка не печатающиеся символы
int symset(int c){
    return (c!='\0') && (c!=' ') && (c!='\t') && (c!=EOF) && (c!='\n') ;
}
//добавление слова в список
void addword(char **buf, int *sizebuf, int *curbuf, char ***mass, int *sizelist, int *curlist){
    char **temp;
    if (*curbuf>(*sizebuf)-1){
        *buf = (char*)realloc(*buf, (*sizebuf)+=1);
    }
    (*buf)[(*curbuf)++] = '\0';
    *buf =  (char*)realloc(*buf, *sizebuf=*curbuf);
    if(*curlist > (*sizelist)-1){
        temp =  (char**)realloc(*mass, ((*sizelist)+=SIZE)*sizeof(*mass));
        if (temp != NULL) *mass = temp;
        else printf("Произошла ошибка\n");
    }
    (*mass)[(*curlist)++] = *buf;
}

int lex(char ***mass, char *buf, int *sizelist, int *curlist, int *sizebuf, int *curbuf, int socket) {
    typedef enum {Start, Word, SpecWord, NewLine, Stop} vertex;
    vertex state = Start;
    char *str;
    int c=0, dop, curelem = 0, flag=1;
    read(socket, &c, 1);
    null_list(mass, sizelist, curlist);
    *mass = (char**)malloc((*sizelist+=SIZE)*sizeof(*mass));
    while(flag){
        switch (state) {
        case Start:
            if (c == ' ' || c == '\t') read(socket, &c, 1);
            else if (c == '\0' || c == '\n') {
                state = NewLine;                
                termlist(mass, sizelist, curlist);
            }
            else if (c == EOF) {
                state = Stop;
                termlist(mass, sizelist, curlist);
                flag = 0;
            }
            else {
                null_buf(buf, sizebuf, curbuf);
                buf = (char*)malloc((*sizebuf+=SIZE));
                addsym(c, &buf, sizebuf, curbuf);
                state = (c!='\\')?Word:SpecWord;
            }
            break;
        
        case Word:
            read(socket, &c, 1);
            if (symset(c)){
                if (c!='\\'){
                    addsym(c, &buf, sizebuf, curbuf);
                } else {
                    addword(&buf, sizebuf, curbuf, mass, sizelist, curlist);
                    state = Start;
                }
            } else {
                addword(&buf, sizebuf, curbuf, mass, sizelist, curlist);
                state = Start;
            }
            break;

        case SpecWord:
            addword(&buf, sizebuf, curbuf, mass, sizelist, curlist);
            read(socket, &c, 1);
            state = Start;
            break;
        case NewLine:
            flag = 0;
            break;
        case Stop:
            exit(0);
            break;
        }
    }
    return 0;
}