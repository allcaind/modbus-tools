# Projeto modbus-tools

Ferramentas úteis para teste em servidores modbus IP (slave side), verificando estado dos coils e holding registers enviados por este.

Executam em qualquer distro Linux com pacotes libmodbus instalados. Para tanto, a compilação exige:

- libmodbus5
- libmodbus-dev

Ao compilar, além de linkar com a biblioteca modbus, atentar para o endereço de modbus.h incluso nas fontes (normalmente instalados em /usr/include/modbus)
Por exemplo: gcc fonte.c -o fonte -lmodbus -I/usr/include/modbus
