#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdlib>
#include <ctime>

#define PORTA 40028
#define NUM_FILHOS 10

using namespace std;

int main() {

    int servidor = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in endereco;
    endereco.sin_family = AF_INET;
    endereco.sin_addr.s_addr = INADDR_ANY;
    endereco.sin_port = htons(PORTA);

    bind(servidor, (sockaddr*)&endereco, sizeof(endereco));
    listen(servidor, NUM_FILHOS);

    cout << "Servidor esperando conexoes...\n";

    for (int i = 0; i < NUM_FILHOS; i++) {
        if (fork() == 0) {
            // 👶 FILHO

            int cliente = socket(AF_INET, SOCK_STREAM, 0);

            sockaddr_in serv;
            serv.sin_family = AF_INET;
            serv.sin_port = htons(PORTA);
            serv.sin_addr.s_addr = inet_addr("127.0.0.1");

            connect(cliente, (sockaddr*)&serv, sizeof(serv));

            int pid = getpid();

            // envia  oo PID
            send(cliente, &pid, sizeof(pid), 0);

            // recebe PID sorteado
            int sorteado;
            recv(cliente, &sorteado, sizeof(sorteado), 0);

            if (pid == sorteado) {
                cout << pid << ": fui sorteado\n";
            }

            close(cliente);
            exit(0);
        }
    }

    int sockets[NUM_FILHOS];
    int pids[NUM_FILHOS];

    for (int i = 0; i < NUM_FILHOS; i++) {
        sockets[i] = accept(servidor, NULL, NULL);

        recv(sockets[i], &pids[i], sizeof(int), 0);

        cout << "Recebi PID: " << pids[i] << endl;
    }

    srand(time(NULL));
    int escolhido = rand() % NUM_FILHOS;
    int pid_sorteado = pids[escolhido];

    cout << "PID sorteado: " << pid_sorteado << endl;

    for (int i = 0; i < NUM_FILHOS; i++) {
        send(sockets[i], &pid_sorteado, sizeof(pid_sorteado), 0);
        close(sockets[i]);
    }

    close(servidor);

    return 0;
}