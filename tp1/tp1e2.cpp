#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <thread>
#include <sys/time.h>
#include <mutex>
#include <iomanip>
using namespace std;

vector<string> pattern_list;
vector<int> result_list;
mutex result_lock;

int pattern_matching_std(string pattern, string text, int pattern_num){    
    int pattern_ln = pattern.length();
    int pattern_index = 0;
    int times_found = 0;
    for (int i = 0; (i < text.length()); i++){
        if (pattern[pattern_index] == text[i]){
            pattern_index += 1;
            if (pattern_ln == pattern_index){
                times_found += 1;
                pattern_index = 0;
            }
        }else{
            pattern_index = 0;
            if (pattern[pattern_index] == text[i]){
                pattern_index += 1;
            }
        }
    }
    result_lock.lock();
    result_list[pattern_num] = times_found;
    result_lock.unlock();
    return times_found;
}

int main(){
    ifstream pattern_file("archivos/patrones.txt"), text_file("archivos/texto.txt");
    string text, current_pattern;
    int times_found;
    timeval time1,time2, p_time1, p_time2;
    text_file >> text; 
    while (pattern_file.eof() == false){
        pattern_file >> current_pattern;
        pattern_list.push_back(current_pattern);
        result_list.push_back(0);
    }
    gettimeofday(&time1,NULL);
    cout << "Buscando patrones en una ejecución linear..."<< endl;
    for (int i = 0; i < pattern_list.size(); i++){
        times_found = pattern_matching_std(pattern_list.at(i), text, i);
        cout << "Patron "<< i << " (" << pattern_list.at(i) << ") encontrado " << times_found << " veces." << endl;
    }
    gettimeofday(&time2,NULL);
    double linear_time = double(time2.tv_sec - time1.tv_sec) + double(time2.tv_usec-time1.tv_usec)/1000000;
    cout << "El tiempo de ejecución, sin paralelizar, fue: " << linear_time << " segundos."<< endl;
    vector<thread> thread_list;
    cout << endl;
    cout << "Paralelizando usando 32 hilos..." << endl;
    gettimeofday(&p_time1,NULL);
    for (int i = 0; i < 32; i++){
        thread_list.push_back(thread(pattern_matching_std,pattern_list.at(i),text, i));
    }
    for (auto &thr : thread_list){
        if (thr.joinable()){
            thr.join();
        }
    }
    gettimeofday(&p_time2,NULL);
    double parallel_time = double(p_time2.tv_sec - p_time1.tv_sec) + double(p_time2.tv_usec-p_time1.tv_usec)/1000000;
    cout << endl;
    cout << "Imprimiendo resultados de los hilos" << endl;
    for (int i = 0; i < result_list.size(); i++){
        cout << "Patron " << i << " encontrado " << result_list[i] << endl;
    }
    cout << "El tiempo de ejecución, paralelizando en 32 hilos, fue: " << parallel_time << " segundos."<< endl;
    double speedup = linear_time / parallel_time;
    cout << endl;
    cout << "El speedup fue de: " << setprecision(5) << speedup << endl;
}