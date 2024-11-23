#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define TAMANHO_TOTAL_MBR 512
#define TAMANHO_PARTICAO 16
#define TAMANHO_SETOR 512
#define QNT_PARTICOES 4
#define OFFSET_BOOTSTRAP 446
#define OFFSET_PARTICOES 510

typedef struct {
    uint8_t status;
    uint8_t chs_first_abs_sector[3];
    uint8_t partition_type;
    uint8_t chs_last_abs_sector[3];
    uint32_t lba_first_abs_sector;
    uint32_t num_sections;
} PartitionEntry;

typedef struct {
    uint8_t part1;
    uint8_t part2;
} SignatureEntry;

void imprimir_assinatura(unsigned char* buffer);
void imprimir_informacoes_do_disco(unsigned char* buffer, uint32_t total_sectors);
void imprimir_tabela_particoes(unsigned char* buffer);

int main() {
    unsigned char buffer[TAMANHO_TOTAL_MBR];
    FILE* ptr = fopen("mbr.bin", "rb");

    if (ptr == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    fread(buffer, sizeof(buffer), 1, ptr);
    fclose(ptr);

    uint32_t total_sectors = 0;
    for (int i = 0; i < QNT_PARTICOES; i++) {
        PartitionEntry* pe = (PartitionEntry*)&buffer[OFFSET_BOOTSTRAP + (i * TAMANHO_PARTICAO)];
        total_sectors += pe->num_sections;
    }

    imprimir_assinatura(buffer);
    imprimir_informacoes_do_disco(buffer, total_sectors);
    imprimir_tabela_particoes(buffer);

    return 0;
}

void imprimir_assinatura(unsigned char* buffer) {
    SignatureEntry* se = (SignatureEntry*)&buffer[OFFSET_PARTICOES];
    printf("Assinatura MBR: %02x%02x\n", se->part1, se->part2);
}

void imprimir_informacoes_do_disco(unsigned char* buffer, uint32_t total_sectors) {
    uint64_t total_bytes = (uint64_t)total_sectors * TAMANHO_SETOR;
    double total_gib = (double)total_bytes / (1024 * 1024 * 1024);
    uint32_t* disk_id = (uint32_t*)&buffer[440];

    printf("\nInformacoes do Disco:\n");
    printf("Disk: %.2f GiB, %llu bytes, %u sectors\n", total_gib, total_bytes, total_sectors);
    printf("Units: sectors of 1 * %d = %d bytes\n", TAMANHO_SETOR, TAMANHO_SETOR);
    printf("Sector size (logical/physical): %d bytes / %d bytes\n", TAMANHO_SETOR, TAMANHO_SETOR);
    printf("I/O size (minimum/optimal): %d bytes / %d bytes\n", TAMANHO_SETOR, TAMANHO_SETOR);
    printf("Disklabel type: dos\n");
    printf("Disk identifier: 0x%8x\n\n", *disk_id);
}

void imprimir_tabela_particoes(unsigned char* buffer) {
    printf("Device      Boot  Start   End       Sectors   Size    Id Type\n");

    for (int i = 0; i < QNT_PARTICOES; i++) {
        PartitionEntry* pe = (PartitionEntry*)&buffer[OFFSET_BOOTSTRAP + (i * TAMANHO_PARTICAO)];

        if (pe->partition_type == 0x00) continue;

        char boot_flag = (pe->status == 0x80) ? '*' : ' ';
        uint32_t num_sectors = pe->num_sections;
        uint32_t start_sector = pe->lba_first_abs_sector;
        uint32_t end_sector = start_sector + num_sectors - 1;
        double size = ((double)num_sectors * TAMANHO_SETOR) / (1024 * 1024); 

        printf("/dev/vda%d   %c   %8u  %8u  %8u  %.1fM  %02x Unknown\n", 
               i + 1, 
               boot_flag, 
               start_sector, 
               end_sector, 
               num_sectors, 
               size, 
               pe->partition_type);
    }
}
