#include <thread>
#include <mutex>
#include <iomanip>
#include <sys/time.h>
#include <vector>
#include <iostream>
#include <cmath>
using namespace std;

vector<vector<float>> result_matrix;
vector<float> sum_list;
mutex result_lock;

void rows_x_columns(vector<vector<float>> rows, vector<vector<float>> columns, int start, int interval){
    int column_num = columns.size(); 
    int row_num = columns[0].size();
    vector<float> results;
    float sum = 0.0;
    for (int i = start; i < (start + interval); i++){
        for(int j = 0; j < column_num; j++){
            for (int k = 0; k < row_num; k++){
                sum += rows[i][k]*columns[k][j];
            }
            results.push_back(sum);
            sum = 0.0;
        }
        result_lock.lock();
        result_matrix.push_back(results);
        result_lock.unlock();
        results = vector<float>();
    }
}

vector<vector<float>> matrix_multipication(vector<vector<float>> matrix1, vector<vector<float>> matrix2, int thread_amount){
    int rows, columns, interval, start, num_result_lists, num_interval_tuples, column_index;
    rows = matrix1[0].size();
    columns = matrix2.size();
    start = 0;
    interval = rows/thread_amount + ceil(rows%thread_amount);
    vector<thread> threads; 
    vector<vector<float>> result_matrix(rows, vector<float>(columns));
    vector<float> results;
    vector<int> interval_tuple;
    while (start < rows){
        if ((start + interval)> rows){
            threads.push_back(thread(rows_x_columns, matrix1, matrix2, start, rows-start));
        }else{
            threads.push_back(thread(rows_x_columns, matrix1, matrix2, start, interval));
        }
        start += interval;
    }
    for (thread &thrd : threads){
        if (thrd.joinable()){
            thrd.join();
        }
    }
    return result_matrix;
}

void sum_rows(vector<vector<float>> matrix, int start, int interval){
    int columns = matrix.size();
    float sum = 0.0;
    for (int i = start; i < (start + interval); i++){
        for (int j = 0; j < columns; j++){
            sum += matrix[j][i];
        }
        result_lock.lock();
        sum_list.push_back(sum);
        result_lock.unlock();
        sum = 0.0;
    }
}

float sum_matrix(vector<vector<float>> matrix, int thread_amount){
    int rows, interval, start;
    float total_sum = 0.0;
    rows = matrix[0].size();
    interval = rows/thread_amount + ceil(rows%thread_amount);
    start = 0;
    vector<thread> thread_list;
    while (start < rows){
        if ((start + interval)> rows){
            thread_list.push_back(thread(sum_rows, matrix, start, rows-start));
        }else{
            thread_list.push_back(thread(sum_rows, matrix, start, interval));
        }
        start += interval;
    }
    for (thread &thrd : thread_list){
        if (thrd.joinable()){
            thrd.join();
        }
    }
    for (float value : sum_list){
        total_sum += value;
    }
    return total_sum;
}

string print_matrix(vector<vector<float>> matrix){
    int rows, columns;
    rows = matrix[0].size();
    columns = matrix.size();
    string out_string;
    out_string = "["+to_string(matrix[0][0])+"]["+to_string(matrix[0][columns-1])+"]\n"+"["+to_string(matrix[rows-1][0])+"]["+to_string(matrix[rows-1][columns-1])+"]";
    return out_string;
}

int main(){
    int thread_amount, rows = 3000, columns = 3000;
    vector<vector<float>> matrix1(columns, vector<float>(rows)), matrix2(columns, vector<float>(rows));
    vector<float> sum_vector;
    timeval time1, time2, p_time1, p_time2;
    double linear_time, parallel_time;
    float total_sum, parallel_total_sum;
    for (int i = 0; i < columns; i++){
        for (int j = 0; j < rows; j++){
            matrix1[i][j] = 0.1;
            matrix2[i][j] = 0.2;
        }
    }
    cout << "Ingrese la cantidad de hilos a usar en la ejecución paralela: " << endl;
    cin >> thread_amount;
    cout << "Realizando el cálculo de forma linear..." << endl;
    gettimeofday(&time1,NULL);
    matrix_multipication(matrix1, matrix2, 1);
    total_sum = sum_matrix(result_matrix, 1);
    gettimeofday(&time2,NULL);
    cout << "La matriz resultado es result_matrix: " << endl;
    cout << print_matrix(result_matrix) << endl;
    cout << "La suma total de los valores en la matriz es: " << total_sum << endl;
    cout << endl;
    linear_time = double(time2.tv_sec - time1.tv_sec) + double(time2.tv_usec - time1.tv_usec)/1000000;
    cout << "El tiempo que tomó realizar el cálculo de forma linear es de " << linear_time << " segundos." << endl;
    cout << endl;
    cout << "Realizando el cálculo utilizando " << thread_amount << " hilos..." << endl;
    result_matrix = vector<vector<float>>();
    sum_list = vector<float>();
    gettimeofday(&p_time1,NULL);
    matrix_multipication(matrix1, matrix2, thread_amount);
    parallel_total_sum = sum_matrix(result_matrix, thread_amount);
    gettimeofday(&p_time2,NULL);
    cout << "La matriz resultado es result_matrix: " << endl;
    cout << print_matrix(result_matrix) << endl;
    cout << "La suma total de los valores en la matriz es: " << parallel_total_sum << endl;
    cout << endl;
    parallel_time = double(p_time2.tv_sec - p_time1.tv_sec) + double(p_time2.tv_usec - p_time2.tv_usec)/1000000;
    cout << "El tiempo que tomó realizar el cálculo de forma paralela es de " << parallel_time << " segundos." << endl;
    cout << endl;
    // Calculo del speedup
    double speedup = linear_time/parallel_time; // Calculo del speedup.
    cout << "El speedup fue de: " << setprecision(5) << speedup << endl;
    cout << endl;
}