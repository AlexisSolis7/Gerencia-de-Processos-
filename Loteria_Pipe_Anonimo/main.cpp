#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <ctime>
#include <cstdio>   

using namespace std;

int main() {

    int fd[2];

    if (pipe(fd) == -1) {
        cout << "Erro ao criar pipe\n";
        return 1;
    }

    pid_t filhos[10];

    for (int i = 0; i < 10; i++) {
        pid_t pid = fork();  //aquii aprendi que o fork cria processos

        if (pid < 0) {
            cout << "Erro no fork\n";
            return 1;
        }

        if (pid == 0) {

            close(fd[1]); // filho nao escreve

            char mensagem[100];

            // ler do pipe
            read(fd[0], mensagem, sizeof(mensagem));

            pid_t meu_pid = getpid();
            pid_t pid_sorteado;

            sscanf(mensagem, "PID sorteado: %d", &pid_sorteado);

            if (meu_pid == pid_sorteado) {
                cout << meu_pid << ": fui sorteado" << endl;
            }

            close(fd[0]);
            exit(0);
        }
        else {
            filhos[i] = pid;
        }
    }

    close(fd[0]); 

    srand(time(NULL));

    int indice = rand() % 10;
    pid_t sorteado = filhos[indice];

    char mensagem[100];
    sprintf(mensagem, "PID sorteado: %d", sorteado);

    cout << "Pai sorteou: " << mensagem << endl;

    for (int i = 0; i < 10; i++) {
        write(fd[1], mensagem, sizeof(mensagem));
    }

    close(fd[1]);

    // esperar filhos
    for (int i = 0; i < 10; i++) {
        wait(NULL);
    }

    return 0;
}