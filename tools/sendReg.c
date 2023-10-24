/* sendReg.c
 * programa cliente modbus TCP/IP para enviar um valor para Holding Register de um servidor
 * Autor: Djan Rosario
 * Copyright (c) Indústrias Allca
 * Data: 24/10/2023
 * Versão: 0.1
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <modbus.h>

uint16_t tab_reg;

volatile sig_atomic_t flag = 1;

void sig_handler(int signum)
{
    if (signum == SIGINT)
    {
        fprintf(stdout, "Encerrando o programa...\n");
        flag = 0;
    }
}

int main(int argc, char *argv[])
{
    modbus_t *mb;
    int i = 0, j = 0;
    unsigned short porta = 502, initAddr = 0;
    uint16_t value;
    char *ip_servidor = "127.0.0.1";

    signal(SIGINT, sig_handler);

    if ((argc < 4) || (argc > 5))
    {
        printf("Uso: %s <Modbus Server IP Address> <Modbus Server Port> <Address HReg> <Value>\n", argv[0]);
        exit(1);
    }

    ip_servidor = argv[1];
    porta = atoi(argv[2]);
    initAddr = atoi(argv[3]);
    value = atoi(argv[4]);

    mb = modbus_new_tcp(ip_servidor, porta);
    if (modbus_connect(mb) == -1)
    {
        fprintf(stderr, "Falha em conectar\n");
        modbus_free(mb);
        return -1;
    }

    while (flag)
    {
        j = modbus_write_register(mb, initAddr, value);
        i = modbus_read_registers(mb, initAddr, 1, &tab_reg);
        if (i > 0)
            printf("Lidos %d holding registers de %s:%d\n", i, ip_servidor, porta);
        else
            printf("Falha na leitura dos holding registers do servidor especificado\n");

        fprintf(stdout, "Holding register %2d = %d\n", initAddr, tab_reg);

        sleep(1);
    }

    modbus_close(mb);
    modbus_free(mb);
}