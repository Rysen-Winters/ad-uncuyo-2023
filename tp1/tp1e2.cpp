#include <string>
#include <vector>
#include <iostream>
using namespace std;


vector<int> create_LPS(string pattern){
    int pattern_ln = pattern.length();
    vector<int> table;
    table.push_back(0);
    int matches;
    string substring;
    int substr_index;
    for (int i = 2; i<(pattern_ln+1);i++){
        substring = pattern.substr(0,i);
        matches = 0;
        substr_index = 0;
        for (int j = 1; (j < i); j++){
            if (substring[j]==substring[substr_index]){
                matches += 1;
                substr_index += 1;
            }else{
                matches = 0;
                substr_index = 0;
            }
        }
        table.push_back(matches);
    }
    return table;
}

vector<int> pattern_matching_KMP(string pattern, string text){
    vector<int> table_LSP = create_LPS(pattern);
    int pattern_ln = pattern.length();
    int pattern_index = 0;
    int matches = 0;
    int times_found;
    for (int i = 0; (i < text.length()); i++){
        if (pattern[pattern_index] == text[i]){
            pattern_index += 1;
            matches += 1;
            if (pattern_ln == matches){
                times_found += 1;
            }
        }else{
            pattern_index = table_LSP[matches];
            matches = table_LSP[matches];
        }
    }
}

int main(){
    string pattern = "ABABACA";
    vector<int> table = create_LPS(pattern);
    for (int i = 0; i < table.size(); i++){
        cout << "Tabla: " << table.at(i) << endl; 
    }
}