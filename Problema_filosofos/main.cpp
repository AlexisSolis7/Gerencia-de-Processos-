#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <semaphore.h> 
#include <cstdlib>     
#include <chrono>

using namespace std; 
const int TOTAL_FILOSOFOS = 5;
char estados[TOTAL_FILOSOFOS]; // aqui estara os vetores de estado dos filosofos


mutex trava_vetor;              
sem_t semaforos[TOTAL_FILOSOFOS];


void imprimir_estados() {
    for (int i = 0; i < TOTAL_FILOSOFOS; i++) {
        cout << estados[i];
    }
    cout << endl;
}


void testar_se_pode_comer(int id) {
    int esquerda = (id + 4) % TOTAL_FILOSOFOS; 
    int direita = (id + 1) % TOTAL_FILOSOFOS;

    if (estados[id] == 'F' && estados[esquerda] != 'C' && estados[direita] != 'C') {
        estados[id] = 'C';
        imprimir_estados();
        
        sem_post(&semaforos[id]);
    }
}


void rotina_do_filosofo(int id) {
    while (true) {
        this_thread::sleep_for(chrono::milliseconds(rand() % 2000 + 1000));

        trava_vetor.lock();
        estados[id] = 'F';
        imprimir_estados();
        testar_se_pode_comer(id);
        trava_vetor.unlock();

        sem_wait(&semaforos[id]);

        this_thread::sleep_for(chrono::milliseconds(rand() % 2000 + 1000));

        trava_vetor.lock();
        estados[id] = 'P';
        imprimir_estados();

        testar_se_pode_comer((id + 4) % TOTAL_FILOSOFOS); 
        testar_se_pode_comer((id + 1) % TOTAL_FILOSOFOS); 
        trava_vetor.unlock();
    }
}


int main() {
    srand(time(NULL));

    for (int i = 0; i < TOTAL_FILOSOFOS; i++) {
        estados[i] = 'P';
        sem_init(&semaforos[i], 0, 0);
    }

    cout << "Estado Inicial: ";
    imprimir_estados();

    vector<thread> filosofos;
    for (int i = 0; i < TOTAL_FILOSOFOS; i++) {
        filosofos.push_back(thread(rotina_do_filosofo, i));
    }

    for (int i = 0; i < TOTAL_FILOSOFOS; i++) {
        filosofos[i].join();
    }

    return 0;
}