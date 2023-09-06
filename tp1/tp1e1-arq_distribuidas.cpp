#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;

mutex result_mutex;
long double result = 0.0;

long double sum(vector<long double> values_list)
{    
    long double result = 0.0;
    long int size = values_list.size();
    for (int i = 0;i<size;i++){
        result += values_list.at(i); 
    }
    return result;
}

void calculate_ln(unsigned int start_term, unsigned int term_num, long double number)
{
    vector<long double> terms;
    long double term_result;
    long double n;
    for (int i=0;i<term_num;i++){
        n = start_term+i;
        term_result = (2/(2*n+1))*(pow((number-1)/(number+1),2*n+1));
        terms.push_back(term_result);
    }
    long double sum_total = sum(terms);
    result_mutex.lock();
    result += sum_total;
    result_mutex.unlock();
}

int main()
{
    // Ejecución sin paralelizar. ------------------------------------------------------------------------------------------------------
    cout << endl;
    int num_threads;
    long double x;
    cout << "Ingrese el número al cual quiere calcular el logaritmo natural: ";
    cin >> x;
    cout << endl;
    cout << "Ingrese la cantidad de hilos a usar: ";
    cin >> num_threads;
    cout << endl;
    unsigned int total_terms = 10000000; // cantidad de términos de la serie de Taylor para el calculo del logaritmo natural.
    cout << "Realizando el cálculo sin paralelizar..." << endl;
    auto linear_start = std::chrono::high_resolution_clock::now(); // Tomar la hora al inicio del programa sin paralelizar.
    calculate_ln(0, total_terms, x);
    long double result_total_linear = result;
    cout << "El ln(" << setprecision(20) << x << ") es: " << setprecision(15) << result_total_linear << endl;
    auto linear_end = std::chrono::high_resolution_clock::now(); // Tomar la hora al final del programa sin paralelizar.
    auto linear_duration = std::chrono::duration_cast<std::chrono::microseconds>(linear_end - linear_start); // Calcular el tiempo que tomo calcular sin paralelizar el logaritmo.
    cout << "La ejecucion lineal tomó: " << linear_duration.count() << " microsegundos" << endl;
    cout << endl;
    // Paralelizar N threads.------------------------------------------------------------------------------------------------------
    result = 0.0;   // Reinicio de la variable global result.
    vector<thread> thread_list; // Lista de los hilos a usar.
    auto n_parallel_start = std::chrono::high_resolution_clock::now(); // Tomar la hora al inicio del programa paralelizando con N hilos.
    vector<unsigned int> start_term_list; // Lista para almacenar los índices del termino en que debe iniciar un hilo.
    unsigned int interval = total_terms/num_threads + ceil(total_terms%num_threads); // El intervalo de separación entre índices
    for (int i=0;i<10000000;i += interval){
        start_term_list.push_back(i);
    }
    cout << "Realizando el cálculo paralelizando en " << num_threads << " hilos..." << endl;
    for (int i=0;i<num_threads;i++){
        thread_list.push_back(thread(calculate_ln, start_term_list.at(i), interval, x));
    }
    for (auto &thread : thread_list){
        if (thread.joinable()){
            thread.join();
        }
    }
    result_mutex.lock();
    long double result_total_nparallel = result; // Resultado de haber calculado con N hilos en paralelo.
    result_mutex.unlock();
    cout << endl;
    cout << "El ln(" << setprecision(20) << x << ") es: " << setprecision(15) << result_total_nparallel << endl; // Escribimos por pantalla el resultado del calculo de ln(x).
    auto n_parallel_end = std::chrono::high_resolution_clock::now(); // Tomar la hora al final del programa paralelizando con N hilos.
    auto n_parallel_duration = std::chrono::duration_cast<std::chrono::microseconds>(n_parallel_end - n_parallel_start); // Calcular el tiempo que tomo calcular paralelizando con N hilos el logaritmo.
    cout << "La ejecución paralela usando " << num_threads << " hilos tomó: " << n_parallel_duration.count() << " microsegundos" << endl;
    // Calculo del speedup
    double speedup = linear_duration.count()/n_parallel_duration.count(); // Calculo del speedup.
    cout << "El speedup fue de: " << setprecision(4) << speedup << endl;
    cout << endl;
}