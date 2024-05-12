#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define SIZE_MENU 160
#define SIZE_TASK (SIZE_MENU/2 - 2)

#define RB "\e(0\x6a\e(B" // 188 Right Bottom corner
#define RT "\e(0\x6b\e(B" // 187 Right Top corner
#define LT "\e(0\x6c\e(B" // 201 Left Top cornet
#define LB "\e(0\x6d\e(B" // 200 Left Bottom corner
#define MC "\e(0\x6e\e(B" // 206 Midle Cross
#define HL "\e(0\x71\e(B" // 205 Horizontal Line
#define LC "\e(0\x74\e(B" // 204 Left Cross
#define RC "\e(0\x75\e(B" // 185 Right Cross
#define BC "\e(0\x76\e(B" // 202 Bottom Cross
#define TC "\e(0\x77\e(B" // 203 Top Cross
#define VL "\e(0\x78\e(B" // 186 Vertical Line
#define SP " "             // space string

void draw_menu_header(char* title) {
    int title_length = strlen(title);
    int space_length = SIZE_MENU - title_length - 4; // 2 spaces on each side
    int left_spaces = space_length / 2;
    int right_spaces = space_length - left_spaces;

    printf(LT);
    for (int i = 0; i < left_spaces; i++) {
        printf(HL);
    }
    printf("  %s  ", title);
    for (int i = 0; i < right_spaces; i++) {
        printf(HL);
    }
    printf(RT);
    printf("\n");
}

void draw_horizontal_line(int length) {
    printf(LC);
    for (int i = 0; i < length; i++) {
        printf(HL);
    }
    printf(RC);
    printf("\n");
}

void draw_top_line(int length) {
    printf(LT);
    for (int i = 0; i < length; i++) {
        printf(HL);
    }
    printf(RT);
    printf("\n");
}

void draw_bottom_line(int length) {
    printf(LB);
    for (int i = 0; i < length; i++) {
        printf(HL);
    }
    printf(RB);
    printf("\n");
}

void draw_line_cross(int length) {
    printf(LC);
    for (int i = 0; i < length; i++) {
        printf(HL);
    }
    printf(RC);
    printf("\n");
}

void center_text(int length, char* text, int end_line) {
    int text_length = strlen(text);
    int space_length = length - text_length - 2; // 1 space on each side
    int left_spaces = space_length / 2;
    int right_spaces = space_length - left_spaces;

    printf(VL);
    for (int i = 0; i < left_spaces; i++) {
        printf(SP);
    }
    printf(" %s ", text);
    for (int i = 0; i < right_spaces; i++) {
        printf(SP);
    }
    printf(VL);
    if (end_line)
        printf("\n");
}

void draw_vertical_line(int length) {
    printf(VL);
    for (int i = 0; i < length; i++) {
        printf(SP);
    }
    printf(VL);
    printf("\n");
}

void draw_blank_line(int length) {
    printf(VL);
    for (int i = 0; i < length; i++) {
        printf(SP);
    }
    printf(VL);
    printf("\n");
}

void draw_initial_menu_options() {
    draw_blank_line(SIZE_MENU);
    center_text(SIZE_MENU, "1 - Adicionar palavra ao dicionário", 1);
    draw_line_cross(SIZE_MENU);
    center_text(SIZE_MENU, "2 - Consultar definição de uma palavra", 1);
    draw_line_cross(SIZE_MENU);
    center_text(SIZE_MENU, "3 - Jogar o Jogo da Forca", 1);
    draw_line_cross(SIZE_MENU);
    center_text(SIZE_MENU, "4 - Sair", 1);
    draw_blank_line(SIZE_MENU);
}

// Estrutura para representar a forca
typedef struct {
    char palavra[50];
    char definicao[300];
} PalavraDefinicao;

const char *imagensForca[] = {
    "     +---------+ \n"
    "     |         | \n"
    "               | \n"
    "               | \n"
    "               | \n"
    "               | \n"
    "               | \n"
    "===============", // 0 erros
    "     +---------+ \n"
    "     |         | \n"
    "     O         | \n"
    "               | \n"
    "               | \n"
    "               | \n"
    "               | \n"
    "===============", // 1 erro
    "     +---------+ \n"
    "     |         | \n"
    "     O         | \n"
    "     |         | \n"
    "               | \n"
    "               | \n"
    "               | \n"
    "===============", // 2 erros
    "     +---------+ \n"
    "     |         | \n"
    "     O         | \n"
    "    /|         | \n"
    "               | \n"
    "               | \n"
    "               | \n"
    "===============", // 3 erros
    "     +---------+ \n"
    "     |         | \n"
    "     O         | \n"
    "    /|\\        | \n"
    "               | \n"
    "               | \n"
    "               | \n"
    "===============", // 4 erros
    "     +---------+ \n"
    "     |         | \n"
    "     O         | \n"
    "    /|\\        | \n"
    "    /          | \n"
    "               | \n"
    "               | \n"
    "===============", // 5 erros
    "     +---------+ \n"
    "     |         | \n"
    "     O         | \n"
    "    /|\\        | \n"
    "    / \\        | \n"
    "               | \n"
    "               | \n"
    "===============" // 6 erros
};

// Função para adicionar uma palavra e sua definição ao dicionário
void adicionarPalavra() {
    char palavra[50];
    char definicao[300];
    FILE *arquivo;

    arquivo = fopen("dicionario.txt", "a+");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo dicionario.txt\n");
        return;
    }

    printf("Digite a palavra que deseja adicionar: ");
    scanf("%s", palavra);

    // Verifica se a palavra já existe no dicionário
    char palavraExistente[50];
    rewind(arquivo);
    while (fscanf(arquivo, "%s", palavraExistente) != EOF) {
        if (strcmp(palavra, palavraExistente) == 0) {
            printf("Essa palavra já existe no dicionário. Tente novamente.\n");
            fclose(arquivo);
            return;
        }
    }

    printf("Digite a definição da palavra que deseja adicionar: ");
    scanf(" %[^\n]", definicao);

    fprintf(arquivo, "%s | %s\n", palavra, definicao);
    printf("Palavra adicionada com sucesso!\n");

    fclose(arquivo);
}

// Função para consultar a definição de uma palavra no dicionário
void consultarDefinicao() {
    char palavra[50];
    char linha[500];
    FILE *arquivo;

    arquivo = fopen("dicionario.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo dicionario.txt\n");
        return;
    }

    printf("Digite a palavra que deseja consultar: ");
    scanf("%s", palavra);

    int encontrada = 0;
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char *palavraDef = strtok(linha, " |");
        if (strcmp(palavra, palavraDef) == 0) {
            encontrada = 1;
            printf("Definição: %s\n", strtok(NULL, "\n"));
            break;
        }
    }

    if (!encontrada) {
        printf("Essa palavra não está no dicionário.\n");
    }

    fclose(arquivo);
}

// Função para jogar o jogo da forca
void jogarForca() {
    FILE *arquivo;
    PalavraDefinicao palavras[100];
    int numPalavras = 0;

    arquivo = fopen("dicionario.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo dicionario.txt\n");
        return;
    }

    // Ler as palavras do arquivo
    while (fscanf(arquivo, "%s | %[^\n]", palavras[numPalavras].palavra, palavras[numPalavras].definicao) != EOF) {
        numPalavras++;
    }

    fclose(arquivo);

    // Selecionar uma palavra aleatoriamente
    srand(time(NULL));
    int indicePalavra = rand() % numPalavras;
    char *palavra = palavras[indicePalavra].palavra;
    char *definicao = palavras[indicePalavra].definicao;
    int tamanhoPalavra = strlen(palavra);

    // Inicializar o jogo
    char letrasUsadas[26] = {0}; // Todas as letras do alfabeto não usadas
    int erros = 0;
    char palavraDescoberta[tamanhoPalavra + 1];
    memset(palavraDescoberta, '_', tamanhoPalavra);
    palavraDescoberta[tamanhoPalavra] = '\0';

    while (erros < 6 && strcmp(palavra, palavraDescoberta) != 0) {
        system("clear"); // Limpa a tela do console (Windows: "cls")

        // Exibir imagem da forca
        printf("%s\n", imagensForca[erros]);

        // Exibir a palavra descoberta até agora
        printf("Palavra: %s\n", palavraDescoberta);

        // Exibir letras usadas
        printf("Letras usadas: ");
        for (char letra = 'A'; letra <= 'Z'; letra++) {
            if (letrasUsadas[letra - 'A']) {
                printf("%c ", letra);
            }
        }
        printf("\n");

        // Pedir ao usuário para adivinhar uma letra
        char letra;
        printf("Digite uma letra: ");
        scanf(" %c", &letra);
        letra = toupper(letra);

        // Verificar se a letra já foi usada
        if (letrasUsadas[letra - 'A']) {
            printf("Você já usou essa letra. Tente outra.\n");
            continue;
        }

        // Marcar a letra como usada
        letrasUsadas[letra - 'A'] = 1;

        // Verificar se a letra está na palavra
        int encontrou = 0;
        for (int i = 0; i < tamanhoPalavra; i++) {
            if (tolower(palavra[i]) == tolower(letra)) { // Convertendo para minúsculas
                palavraDescoberta[i] = palavra[i]; // Usando a letra da palavra secreta
                encontrou = 1;
            }
        }

        // Se a letra não estiver na palavra, incrementar os erros
        if (!encontrou) {
            erros++;
        }
    }

    system("clear"); // Limpa a tela do console (Windows: "cls")

    // Exibir imagem final da forca
    printf("%s\n", imagensForca[erros]);

    // Exibir resultado do jogo
    if (erros == 6) {
        printf("Você perdeu! A palavra era: %s\n", palavra);
    } else {
        printf("Parabéns! Você acertou a palavra: %s (%s)\n", palavra, definicao);
    }

    // Esperar o usuário pressionar Enter antes de voltar ao menu
    printf("\nPressione Enter para voltar ao menu...");
    while (getchar() != '\n');
    getchar(); // Aguarda o Enter ser pressionado
}

// Função principal
int main() {
    int opcao;

    while (1) {
        system("clear"); // Limpa a tela do console (Windows: "cls")
        draw_menu_header("Menu Principal");
        draw_initial_menu_options();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionarPalavra();
                break;
            case 2:
                consultarDefinicao();
                break;
            case 3:
                jogarForca();
                break;
            case 4:
                printf("Saindo...\n");
                return 0;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}
