# mbr-reader-simulation

# Simulador de MBR e Partições

Este projeto simula a leitura e a análise de um MBR (Master Boot Record) em um disco rígido, permitindo que você visualize as informações sobre o MBR e as partições contidas nele. É um programa simples escrito em C que lê um arquivo binário de MBR e exibe seus detalhes de forma legível, incluindo as informações sobre as partições.

## Funcionalidades

- **Leitura do MBR**: O programa lê um arquivo binário contendo um MBR e imprime seu conteúdo em formato hexadecimal.
- **Exibição da Tabela de Partições**: As partições do MBR são exibidas com informações como o tipo da partição, o número de setores e o endereço dos setores no disco.
- **Informações do Disco**: O programa também exibe dados gerais do disco, como o número total de setores e o tamanho total do disco.
- **Assinatura do MBR**: A assinatura do MBR é verificada e exibida para garantir a validade do registro.

## Como Funciona

1. O programa lê o conteúdo de um arquivo binário que representa o MBR de um disco.
2. Em seguida, ele imprime as informações do MBR e a tabela de partições.
3. As informações incluem dados como a assinatura do MBR, o endereço dos setores das partições e o tipo de cada partição.
4. O programa também exibe o tamanho do disco em GiB, o número total de setores e o identificador único do disco.



