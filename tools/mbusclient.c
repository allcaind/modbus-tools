/* mbusClient.c
* programa cliente modbus TCP/IP para teste do estado dos Coils de um servidor
* Autor: Djan Rosario
* Copyright (c) Indústrias Allca
* Data: 18/01/2023
* Versão: 0.1
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <modbus.h>

#define MBUS_SRV_IP "192.168.21.200"
#define MBUS_PORT_IP 503

uint8_t tab_coils[100];

volatile sig_atomic_t flag = 1;

void sig_handler(int signum) {
    if (signum == SIGINT) {
        fprintf(stdout, "Encerrando o programa...\n");
        flag = 0;
    }
}

int main(int argc, char *argv[]) {
    modbus_t *mb;
    int i = 0, j = 0;
    unsigned short porta = 503, limiteCoils = 100;
    char *ip_servidor = "192.168.21.200";

    signal(SIGINT, sig_handler);

    if ((argc < 3) || (argc > 4)) {
        printf("Uso: %s <IP do Servidor Modbus> <Porta> <Quantidade de Coils lida>", argv[0]);
        exit(1);
    }

    ip_servidor = argv[1];
    porta = atoi(argv[2]);
    limiteCoils = atoi(argv[3]);

    mb = modbus_new_tcp(ip_servidor, porta);
    if (modbus_connect(mb) == -1) {
        fprintf(stderr, "Falha em conectar\n");
        modbus_free(mb);
        return -1;
    }

    while(flag) {
        memset(tab_coils, 0, limiteCoils);
        i = modbus_read_bits(mb, 0, limiteCoils, tab_coils);
        if (i > 0) printf("Read %d coils\n", i);
        else printf("Unable to read coils from CLP\n");
        for (j = 0; j < i; j++) {
            if (tab_coils[j] == 1) {
                fprintf(stdout,"Coil %d ativada\n", j);
            }

        }
        
        sleep(1);
    }

    modbus_close(mb);
    modbus_free(mb);
}