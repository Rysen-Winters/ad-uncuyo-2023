#include <iostream>
#include <thread>
#include <sys/time.h>
#include <mutex>
#include <cmath>
#include <vector>
#include <iomanip>
using namespace std;

vector<int> lower_primes;
mutex result_lock;

bool is_prime(int number){
    int end = int(sqrt(number)) + 1;
    for (int i = 2; i < end; i++){
        if ((number%i) == 0){
            return false;
        }
    }
    return true;
}

vector<int> find_lower_primes(int top, int start, int numb_to_test){
    vector<int> prime_list;
    for (int i = start; i <= (start + numb_to_test); i++){
        if (is_prime(i)){
            prime_list.push_back(i);
            result_lock.lock();
            lower_primes.push_back(i);
            result_lock.unlock();
        }
    }
    return prime_list;
}

int main(){
    int numb_threads, top, interval, start;
    timeval time1, time2, p_time1, p_time2;
    long double linear_time, parallel_time;
    cout << "Ingrese el valor máximo a partir del cual se van a buscar los número primos más pequeños: ";
    cin >> top;
    cout << "Ingrese el número de hilos a usar en la ejecución paralela: ";
    cin >> numb_threads;
    start = 2;
    cout << "Realizando el cálculo de forma linear..." << endl;
    gettimeofday(&time1,NULL);
    find_lower_primes(top, start, top - start);
    gettimeofday(&time2,NULL);
    cout << "Los 5 números primos más grandes encontrados son: " << endl;
    for (int i = lower_primes.size() - 5; i < lower_primes.size(); i++){
        cout << "("<< lower_primes[i] << "), ";
    }
    cout << endl;
    cout << "La cantidad de números primos encontrados es: " << lower_primes.size() << endl;
    linear_time = double(time2.tv_sec - time1.tv_sec) + double(time2.tv_usec - time1.tv_usec)/1000000;
    cout << endl;
    cout << "El tiempo que tomó realizar el cálculo de forma linear es de " << linear_time << " segundos." << endl;
    cout << endl;
    interval = top/numb_threads + ceil(top%numb_threads);
    vector<thread> thread_list;
    lower_primes = vector<int>();
    gettimeofday(&p_time1,NULL);
    while (start < top){
        if ((start + interval)> top){
            thread_list.push_back(thread(find_lower_primes, top, start, top - start));
        }else{
            thread_list.push_back(thread(find_lower_primes, top, start, interval));
        }
        start += interval;
    }
    for (thread &thrd : thread_list){
        if (thrd.joinable()){
            thrd.join();
        }
    }
    gettimeofday(&p_time2,NULL);
    cout << "Los 5 números primos más grandes encontrados son: " << endl;
    for (int i = lower_primes.size() - 5; i < lower_primes.size(); i++){
        cout << "("<< lower_primes[i] << "), ";
    }
    cout << endl;
    cout << "La cantidad de números primos encontrados es: " << lower_primes.size() << endl;
    cout << endl;
    parallel_time = double(p_time2.tv_sec - p_time1.tv_sec) + double(p_time2.tv_usec - p_time2.tv_usec)/1000000;
    cout << "El tiempo que tomó realizar el cálculo de forma paralela es de " << parallel_time << " segundos." << endl;
    cout << endl;
    // Calculo del speedup
    long double speedup = linear_time/parallel_time; // Calculo del speedup.
    cout << "El speedup fue de: " << speedup << endl;
    cout << endl;
}