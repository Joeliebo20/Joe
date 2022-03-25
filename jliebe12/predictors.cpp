#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <map>
#include <array>
#include <bits/stdc++.h>
#include <bitset>
#include <array>

using namespace std; 

string alwaysTaken(string inFile) {
    unsigned long long addr;
    string behavior;
    unsigned long long target;

  // Open file for reading
    ifstream f;
    f.open(inFile);
    int total_lines = 0;
    int taken = 0;

  // The following loop will read a hexadecimal number and
  // a string each time and then output them
    while(f >> std::hex >> addr >> behavior >> std::hex >> target) {

        if(behavior == "T") {
            taken++;
            total_lines++;
        }
        else {
            total_lines++;
        }
    }
    stringstream ss;
    ss <<  taken << "," << total_lines << ";";
    return ss.str();
}

string alwaysNotTaken(string inFile) {
    unsigned long long addr;
    string behavior;
    unsigned long long target;


  // Open file for reading
    ifstream f;
    f.open(inFile);
    int total_lines = 0;
    int not_taken = 0;

  // The following loop will read a hexadecimal number and
  // a string each time and then output them
    while(f >> std::hex >> addr >> behavior >> std::hex >> target) {
        if(behavior == "T") {
        total_lines++;
        }
        else {
            not_taken++;
            total_lines++;
        }
    }
    stringstream ss;
    ss << not_taken << "," << total_lines << ";";
    return ss.str();
}

string bimodalPredictor(string inFile, int table_size) {
    unsigned long long addr;
    string behavior;
    unsigned long long target;
    int total_lines = 0;
    ifstream f;
    f.open(inFile);
    int count = 0;
    int table[table_size];
    //0 is not_taken, 1 is taken
    for(int i = 0; i < table_size; i++) {
        table[i] = 1;
    }
    while(f >> std::hex >> addr >> behavior >> std::hex >> target) {
        int address = addr & addr & (table_size - 1);
        if(table[address]){
            //cout << address << endl;
            if(behavior == "T") {
                count++;
                total_lines++;
            }
            else {
                table[address] = 0;  
                total_lines++;
            }
        }
        else {
            if(behavior == "NT") {
                count++;
                total_lines++;
            }
            else {
                table[address] = 1;
                total_lines++;
            }
        }
    }
    stringstream ss;
    ss << count << "," << total_lines << "; ";
    return ss.str();
}

string twoBitBimodalPredictor(string inFile, int table_size){
    unsigned long long addr;
    string behavior;
    unsigned long long target;
    int total_lines = 0;
    ifstream f;
    f.open(inFile);
    int count = 0;
    int table[table_size];
    for(int i = 0; i < table_size; i++) {
        table[i] = 3;
    }
    //3 = strongly taken(11), 2 = taken -> not_taken(10), 1 = not_taken -> taken(01), 0 == not_taken both times(00)
    while(f >> std::hex >> addr >> behavior >> std::hex >> target) {
        int address = addr & (table_size - 1);
            //cout << address << endl;
            if(behavior == "T") {
                total_lines++;
                if(table[address] == 3){
                    count++;
                }
                else if(table[address] == 2) {
                    table[address] = 3;
                    count++;
                }
                else if(table[address] == 1) {
                    table[address] = 2;
                }
                else {
                    table[address] = 1;
                }
            }
            if(behavior == "NT") {
                total_lines++;
                if(table[address] == 3){
                    table[address] = 2;
                }
                else if(table[address] == 2) {
                    table[address] = 1;
                }
                else if(table[address] == 1) {
                    count++;
                    table[address] = 0;
                }
                else {
                    count++;
                }
            }
        }
    stringstream ss;
    ss << count << "," << total_lines << "; ";
    return ss.str();
}

string gSharePredictor(string inFile, int size){
    unsigned long long addr;
    string behavior;
    unsigned long long target;
    int total_lines = 0;
    ifstream f;
    f.open(inFile);
    int count = 0;
    int table_size = 2048;
    int table[table_size];
    unsigned int global = 0;
    for(int k = 0; k < table_size; k++) {
        table[k] = 3;
    }
    //3 = strongly taken(11), 2 = taken -> not_taken(10), 1 = not_taken -> taken(01), 0 == not_taken both times(00)
    while(f >> std::hex >> addr >> behavior >> std::hex >> target) {
        total_lines++;
        int address = ((addr & (table_size - 1)) ^ (global & ((1 << size) - 1)));
        if(((table[address] >> 1) & 1) == 1) {
            if(behavior == "T") {
                count++;
                if(table[address] == 2){
                    table[address] += 1;
                }
                else if(table[address] == 1){
                    table[address] += 1;
                }
                else if(table[address] == 0) {
                    table[address] += 1;
                }
                global = (global << 1) + 1;
            }
            else {
                if(table[address] == 3){
                    table[address] -= 1;
                }
                else if(table[address] == 2){
                    table[address] -= 1;
                }
                else if(table[address] == 1) {
                    table[address] -= 1;
                }
                global = global << 1;
            }
        }
        else {      
            if(behavior == "NT") {
                count++;
                if(table[address] == 3){
                    table[address] -= 1;
                }
                else if(table[address] == 2){
                    table[address] -= 1;
                }
                else if(table[address] == 1) {
                    table[address] -= 1;
                }
                global = global << 1;
            }
            else {
                if(table[address] == 2){
                    table[address] += 1;
                }
                else if(table[address] == 1){
                    table[address] += 1;
                }
                else if(table[address] == 0) {
                    table[address] += 1;
                }
                global = (global << 1) + 1;
            }

        }
    }
    stringstream ss;
    ss << count << "," << total_lines << "; ";
    return ss.str();
}

string tournamentPredictor(string inFile) {
    unsigned long long addr;
    string behavior;
    unsigned long long target;

  // Open file for reading
    ifstream f;
    f.open(inFile);
    int total_lines = 0;
    int count = 0;
    int global = 0;
    int table_size = 2048;
    int gshareTable[table_size];
    int bimodalTable[table_size];
    int selector[table_size];
    int bits = 11;
    for(int i = 0; i < table_size; i++) {
        gshareTable[i] = 3;
        bimodalTable[i] = 3;
        selector[i]= 0;
    }

  // The following loop will read a hexadecimal number and
  // a string each time and then output them
    while(f >> std::hex >> addr >> behavior >> std::hex >> target) {
        total_lines++;
        bool gShare = false;
        bool bimodal = false;
        int bimodal_addr = addr & (table_size - 1);
        int address = ((addr & (table_size - 1)) ^ (global & ((1 << bits) - 1)));
        //gshare code
        if(((gshareTable[address] >> 1) & 1) == 1) {
            if(behavior == "T") {
                gShare = true;
                if(gshareTable[address] == 2){
                    gshareTable[address] += 1;
                }
                else if(gshareTable[address] == 1){
                    gshareTable[address] += 1;
                }
                else if(gshareTable[address] == 0) {
                    gshareTable[address] += 1;
                }
                else {
                }
                global = (global << 1) + 1;
            }
            else {
                if(gshareTable[address] == 3){
                    gshareTable[address] -= 1;
                }
                else if(gshareTable[address] == 2){
                    gshareTable[address] -= 1;
                }
                else if(gshareTable[address] == 1) {
                    gshareTable[address] -= 1;
                }
                else {
                }
                global = global << 1;
            }
        }
        else {      
            if(behavior == "NT") {
                gShare = true;
                if(gshareTable[address] == 3){
                    gshareTable[address] -= 1;
                }
                else if(gshareTable[address] == 2){
                    gshareTable[address] -= 1;
                }
                else if(gshareTable[address] == 1) {
                    gshareTable[address] -= 1;
                }
                global = global << 1;
            }
            else {
                if(gshareTable[address] == 2){
                    gshareTable[address] += 1;
                }
                else if(gshareTable[address] == 1){
                    gshareTable[address] += 1;
                }
                else if(gshareTable[address] == 0) {
                    gshareTable[address] += 1;
                }
                global = (global << 1) + 1;
            }
        }
        //bimodal code
        if(behavior == "T") {
                if(bimodalTable[bimodal_addr] == 3){
                    bimodal = true;
                }
                else if(bimodalTable[bimodal_addr] == 2) {
                    bimodalTable[bimodal_addr] = 3;
                    bimodal = true;
                }
                else if(bimodalTable[bimodal_addr] == 1) {
                    bimodalTable[bimodal_addr] = 2;
                }
                else {
                    bimodalTable[address] = 1;
                }
            }
        else {
            if(behavior == "NT") {
                if(bimodalTable[bimodal_addr] == 3){
                    bimodalTable[bimodal_addr] = 2;
                }
                else if(bimodalTable[bimodal_addr] == 2) {
                    bimodalTable[bimodal_addr] = 1;
                }
                else if(bimodalTable[bimodal_addr] == 1) {
                    bimodalTable[bimodal_addr] = 0;
                    bimodal = true;
                }
                else {
                    bimodal = true;
                }
            }
        }
        //if selector is 2 or 3, favor bimodal
        if(selector[bimodal_addr] > 1) {
            if(bimodal) {
                count++; 
            }
        }
        //if selector is 0 or 1, favor gshare
        else if(selector[bimodal_addr] < 2) {
            if(gShare) {
               count++;
            }
        }
        if(gShare && !bimodal && selector[bimodal_addr] > 0){
            selector[bimodal_addr] -= 1;
        }
        else if(!gShare && bimodal && selector[bimodal_addr] < 3) {
            selector[bimodal_addr] += 1;
        }
    }
    stringstream ss;
    ss << count << "," << total_lines << ";";
    return ss.str();
}

string branchTargetBuffer(string inFile) {
    unsigned long long addr;
    string behavior;
    unsigned long long target;
    int count = 0;
    int total = 0;
    int table[512];
    int current_val;
    ifstream f;
    f.open(inFile);
    unsigned long long btb[512][2] = {0};
    for(int i = 0; i < 512; i++) {
        table[i] = 1;
    }
    while(f >> std::hex >> addr >> behavior >> std::hex >> target) {
        if(behavior == "T") {
            current_val = 1;
        }
        else {
            current_val = 0;
        }
        unsigned int address = addr & 511;
        if(table[address] == 1) {
            table[address] = current_val;
            total++;
            if(btb[address][0] == addr) {
                if(btb[address][1] == target) {
                    count++;
                }
            }
        }
        else {
            table[address] = current_val;
        }
        btb[address][0] = addr;
        btb[address][1] = target;
    }
    stringstream ss;
    ss << count << "," << total << ";";
    return ss.str();
}

int main(int argc, char * argv[]){
    string inputFile(argv[1]);
    string outputFile(argv[2]);
    ofstream of;
    of.open(outputFile);
    string ans1 = alwaysTaken(inputFile);
    string ans2 = alwaysNotTaken(inputFile);
    vector<string> ans3;
    vector<string> ans4;
    of << ans1 << endl;
    of << ans2 << endl;
    for(int i = 16; i <= 2048; i *= 2) {
        if(i == 64) {
            i *= 2;
            ans3.push_back(bimodalPredictor(inputFile, i));
            ans4.push_back(twoBitBimodalPredictor(inputFile, i));
        }
        else {
           ans3.push_back(bimodalPredictor(inputFile, i));
           ans4.push_back(twoBitBimodalPredictor(inputFile, i));
        }
    }
    for(int j = 0; j < ans3.size(); j++) {
        of << ans3.at(j);
    }
    of << endl;
    for(int j = 0; j < ans4.size(); j++) {
        of << ans4.at(j);
    }
    of << endl;
    for(int i = 3; i <= 11; i++){
        of << gSharePredictor(inputFile, i);
    }
    of << endl;
    of << tournamentPredictor(inputFile) << endl;
    of << branchTargetBuffer(inputFile) << endl;
}