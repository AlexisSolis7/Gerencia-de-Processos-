#include <iostream>
#include <unistd.h>     
#include <sys/mman.h>   
#include <sys/wait.h>   
#include <semaphore.h>  
#include <fcntl.h>    
#include <cstdlib>    
#include <ctime>       

using namespace std;

int main() {
    int nprocs;
    cout << "Digite a quantidade de processos (nprocs): ";
    cin >> nprocs;

    if (nprocs <= 0) return 0;


    int* id_compartilhado = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, 
                                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *id_compartilhado = 0; 

    const char* NOME_SEMAFORO = "/semaforo_trabalho";
   

    sem_t* semaforo = sem_open(NOME_SEMAFORO, O_CREAT | O_EXCL, 0644, 1);
    

    if (semaforo == SEM_FAILED) {
        sem_unlink(NOME_SEMAFORO);
        semaforo = sem_open(NOME_SEMAFORO, O_CREAT | O_EXCL, 0644, 1);
    }


    for (int i = 0; i < nprocs; i++) {
        pid_t pid = fork();

        if (pid == 0) { 

            srand(time(NULL) ^ (getpid() << 16));
            
            int tempo_sono = rand() % 10 + 1;
            sleep(tempo_sono);

            sem_wait(semaforo); 
            
            (*id_compartilhado)++; 
            cout << "Processo " << *id_compartilhado << " criado (PID: " << getpid() << ")" << endl;
            
            sem_post(semaforo); // Libera o cadeado

            sem_close(semaforo);
            exit(0); 
        }
    }


    for (int i = 0; i < nprocs; i++) {
        wait(NULL);
    }

    cout << "Todos os processos finalizaram. Limpando recursos..." << endl;
    sem_close(semaforo);
    sem_unlink(NOME_SEMAFORO);
    munmap(id_compartilhado, sizeof(int));

    return 0;
}
