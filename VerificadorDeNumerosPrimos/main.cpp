#include <iostream>
#include <cstdio>      
#include <pthread.h>   
#include <cmath>

using namespace std;
long long numero_n;
int resultado_eh_primo = 1;
pthread_t thread_id[2];    

struct Intervalo {
    long long inicio;
    long long fim;
    int id_da_thread;
};

void* verificar_divisor(void* arg) {
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    Intervalo* dados = (Intervalo*)arg;

    for (long long i = dados->inicio; i <= dados->fim; i++) {
        if (numero_n % i == 0) {
            resultado_eh_primo = 0; // Não é primo
            
            int outra_thread = (dados->id_da_thread == 0) ? 1 : 0;
            pthread_cancel(thread_id[outra_thread]);

            pthread_exit(NULL);
        }
    }

    pthread_exit(NULL);
}

int main() {
    if (scanf("%lld", &numero_n) == EOF) return 0;

    if (numero_n <= 1) {
        printf("0\n");
        return 0;
    }

    long long limite_total = numero_n - 1;
    long long meio = 2 + (limite_total - 2) / 2;


    Intervalo intervalo_t1 = {2, meio, 0};
    Intervalo intervalo_t2 = {meio + 1, limite_total, 1};

    pthread_create(&thread_id[0], NULL, verificar_divisor, &intervalo_t1);
    pthread_create(&thread_id[1], NULL, verificar_divisor, &intervalo_t2);

    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);

    printf("%d\n", resultado_eh_primo);

    return 0;
}