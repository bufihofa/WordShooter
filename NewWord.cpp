#ifndef NEWWORD__CPP
#define NEWWORD__CPP
#include "NewWord.h"
#include <bits/stdc++.h>
#include <SDL.h>
using namespace std;

string raw1 = "bcdghklmnprstvxfj";
string raw2 = "aeiou";
int i = 0;
int getRandomNumber(int _min, int _max){
    return rand() % ( _max - _min +1) + _min;
}
string getNewWord(int difficulty){
    srand(SDL_GetTicks());
    string s = "";
    if(difficulty == 1){
        i++;
        s += raw1[i % raw1.size()];
        s += raw2[rand() % raw2.size()];
        int temp = getRandomNumber(0,2);
        if(temp == 0){
            switch(s[1]){
                case 'a': s+='e'; break;
                case 'e': s+='a'; break;
                case 'i': s+='e'; break;
                case 'o': s+='u'; break;
                case 'u': s+='o'; break;
            }
        }
        else if(temp == 1) {
            switch(s[1]){
                case 'e': s+='e'; break;
                case 'o': s+='o'; break;
                case 'u': s+='a'; break;
            }
        }
        temp = getRandomNumber(0,12);
        switch(temp){
            case 0: s+="ng"; break;
            case 1: s+='y'; break;
            case 2: s+='t'; break;
            case 3: s+='c'; break;
            case 4: s+='r'; break;
            case 5: s+='n'; break;
            case 6: s+='k'; break;
            case 7: s+='f'; break;
            case 8: s+='m'; break;
            case 9: s+='l'; break;
            case 10: s+='g'; break;
            case 11: s+='x'; break;
            case 12: s+='d'; break;
        }
    }
    return s;

}



#endif
