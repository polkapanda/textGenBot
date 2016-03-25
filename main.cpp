#include <string>
#include <map>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

using std::cout;
using std::cin;
using std::map;
using std::vector;
using std::string;
using std::array;
using std::ifstream;
using std::ofstream;

map< array<string, 3>, vector<string>> markovChain;
vector<string> splitSentence;
void parseSentence(string);
void split(string);
void print_map(map<array<string, 3>, vector<string>>);
string generateText();

int main(int argc, char** argv){
    srand(time(NULL));
    ifstream ifs("seedText.txt", std::ifstream::in);
    string line = "";
    char c = ' ';
    bool interact = true;
    while(!ifs.eof()){
        c = ifs.get();
        if (c == '\n'){
            parseSentence(line);
            line = "";
        }else{
            line += c;
        }
    }
    ifs.close();
    ofstream ofs("seedText.txt", std::ofstream::app);
    while(interact){
        cout << "You: ";
        std::getline(cin, line);
        if (line == "say something" || line == "Say Something"
          ||line == "Say something" || line == "say Something"){
            cout << std::endl << "Robot: " << generateText() << std::endl;
        }else if (line == "quit"){
            ofs.close();
            interact = false;
        }else{
            ofs << line << std::endl;
            parseSentence(line);
        }
        line = "";
    }

//    parseSentence("Hello I'm Robert");
//    parseSentence("Hi I'm a duck");
//    parseSentence("a duck goes quack");
//    print_map(markovChain);
//    cout << generateText();
    return 0;
}
void split(string s){
    int index1 = 0;
    int index2 = 0;
    string word = "";
    for(index1; index1 < s.size(); index1++){
        if (s.at(index1) != ' '){
            word += s.at(index1);
        }else{
            splitSentence.push_back(word);
            word = "";
        }
        if (index1 == s.size() -1){
            splitSentence.push_back(word);
        }
    }
}

void parseSentence(string s){
    split(s);
    array<string, 3> sArray = {" ", " ", " "};
    for(int i = 0; i < splitSentence.size(); i++){
        if(i == 0){
            markovChain[sArray].push_back(splitSentence.at(i));
            sArray[2] = splitSentence.at(i);
        }
        if (i != splitSentence.size() - 1){
        markovChain[sArray].push_back(splitSentence.at(i+1));
        sArray[0] = sArray[1];
        sArray[1] = sArray[2];
        sArray[2] = splitSentence.at(i+1);
        }else{
            markovChain[sArray].push_back("\n");
        }
    }
    splitSentence.clear();
}

void print_map(map< array<string, 3>, vector<string>> m){
    cout << "{";
    for(auto& p: m){
        if (p.first[2] == " "){
            cout << " [ " << '"' << '"' << " " << '"' << '"' << " " << '"' << '"' << " | ";
        }else if (p.first[1] == " "){
            cout << " [ " << '"' << '"' << " " << '"' << '"' << " " << p.first[2] << " | ";
        }else if (p.first[0] == " "){
            cout << " [ " << '"' << '"' << " " << p.first[1] << " " << p.first[2] << " | ";
        }else{
            cout << " [ " << p.first[0] << " " << p.first[1] << " " << p.first[2] << " | ";
        }
        for (int i = 0; i < p.second.size(); i++){
            if (p.second.at(i) != "\n"){
            cout << p.second.at(i) << " ";
            }else{
            cout << "\\n";
            }
        }
        cout << "]";
    }
    cout << "}" << std::endl;
}

string generateText(){
    vector<string> getter;
    int range;
    string previousS = " ", currentS = " ";
    array<string, 3> sArray = {previousS, currentS, " "};
    string fullGen = "";

    auto it = markovChain.find(sArray);
    if (it == markovChain.end()){
        cout << "oopsie";
    }

    while(it != markovChain.end()){
        sArray[0] = sArray[1];
        sArray[1] = sArray[2];
        getter = it->second;
        range = rand() % getter.size();
        fullGen += getter.at(range) + " ";
        sArray[2] = getter.at(range);
        it = markovChain.find(sArray);
    }
    return fullGen;
}
