/* pingcoil.c
* programa cliente modbus TCP/IP para pingar o estado dos Coils de um servidor
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
    unsigned short porta = 502, limiteCoils = 100, initAddr = 0;
    char *ip_servidor = "127.0.0.1";

    signal(SIGINT, sig_handler);

    if ((argc < 4) || (argc > 5)) {
        printf("Uso: %s <IP do Servidor Modbus> <Porta Servidor Modbus> <Offset> <Quantidade de Coils lida>\n", argv[0]);
        exit(1);
    }

    ip_servidor = argv[1];
    porta = atoi(argv[2]);
    initAddr = atoi(argv[3]);
    limiteCoils = atoi(argv[4]);

    mb = modbus_new_tcp(ip_servidor, porta);
    if (modbus_connect(mb) == -1) {
        fprintf(stderr, "Falha em conectar\n");
        modbus_free(mb);
        return -1;
    }

    while(flag) {
        memset(tab_coils, 0, limiteCoils);
        i = modbus_read_bits(mb, initAddr, limiteCoils, tab_coils);
        if (i > 0) printf("Lidos %d coils de %s:%d\n", i, ip_servidor, porta);
        else printf("Falha na leitura dos coils do servidor especificado\n");
        for (j = 0; j < i; j++) {
            fprintf(stdout,"Coil %2d \t(%2o) = %d\n", initAddr+j, initAddr+j, tab_coils[j]);
        }
        
        sleep(1);
    }

    modbus_close(mb);
    modbus_free(mb);
}