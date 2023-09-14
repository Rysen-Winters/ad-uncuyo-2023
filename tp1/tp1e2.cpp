#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <thread>
#include <sys/time.h>
using namespace std;


vector<int> create_LPS(string pattern){
    int pattern_ln = pattern.length();
    vector<int> table;
    table.push_back(0);
    for i
    return table;
}

int pattern_matching_KMP(string pattern, string text){
    vector<int> table_LPS = create_LPS(pattern);
    int pattern_ln = pattern.length();
    int pattern_index = 0;
    int matches = 0;
    int times_found = 0;
    for (int i = 0; (i < text.length()); i++){
        if (pattern[pattern_index] == text[i]){
            pattern_index += 1;
            matches += 1;
            if (pattern_ln == matches){
                times_found += 1;
                matches = 0;
                pattern_index = 0;
            }
        }else{
            pattern_index = table_LPS[matches];
            matches = table_LPS[matches];
        }
    }
    cout << "Times found = " << times_found << endl;
    return times_found;
}

int main(){
    ifstream pattern_file("archivos/patrones.txt"), text_file("archivos/texto.txt");
    string text, current_pattern;
    cout << (pattern_matching_KMP("leoneleon", "ababacaaleoneleonsdasadaleoneleonbabacaasdasdababacaasdasableoneleonleoneleonabacaasdasdababacleoneleonaasdasdababacaababaca")) << endl;
    getline(text_file, text);
    int times_found = 0;
    timeval time1,time2;
    gettimeofday(&time1,NULL);

    getline(pattern_file, current_pattern);
    times_found = pattern_matching_KMP(current_pattern, text);
    cout << "Patron:" << current_pattern << "Patron:" << endl;
    vector<int> vector_lps = create_LPS(current_pattern);
    for (int i = 0; i < current_pattern.length(); i++){
        cout << "vector_lps["<< i <<"]: " << vector_lps[i] << endl;
    }
    cout << "Encontrado " << times_found << " veces." << endl; 
    /*
    while (pattern_file.eof() == false){

        
        getline(pattern_file, current_pattern);
        times_found = pattern_matching_KMP(current_pattern, text);
        cout << "Patron: " << current_pattern << " encontrado " << times_found << " veces." << endl;
        
    }
    */
    gettimeofday(&time2,NULL);
    cout << "El tiempo de ejecución fue: " << double(time2.tv_sec - time1.tv_sec) + double(time2.tv_usec-time1.tv_usec)/1000000 << endl;
}