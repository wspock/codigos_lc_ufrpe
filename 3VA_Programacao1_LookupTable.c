/*
  Título: Sistema de Cadastro de Hóspedes - Casa de Campo Tranquilidade;
  Autor: Ervelany Lins de França;
  Data de Criação: 11/07/2023;
  Descrição: Programa que cadastra os hóspedes e gera relatórios dos cadastros;
*/

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#ifdef _WIN32 // Não precisa por _WIN64 pois não existe Windows 64 bit puro!!!
    #define LIMPA_TELA system("cls")
#else
    #define LIMPA_TELA system("clear")
#endif

struct Cliente{
    char nome[26];
    int idade;
    int qtd;
    int dias;
    int auxLimpeza;
    float valorTotal;
    float adiantamento;
};

struct EntradaDados{
    int qtdHospedes;
    float taxaAluguel;
    float taxaFuncAuxiliar;
};

struct EntradaDados tabelaReferenciaDados[] = {
    {5, 450.0, 90.0},
    {8, 680.0, 110.0},
    {12, 950.0, 140.0}
};

// Prototipagem das funções. É necessário porque o GCC exige;
void exibirTitulo();
void menuOpcoes();
void telaCadastro();
void telaListagemGeral();
void telaRelatorioEstatistico();
void gravarDadosClientes(FILE* arquivo);
void lerDadosClientes();
float calcularValorAPagar(struct EntradaDados, int, int); //

int main(){
    // Configurando o locale para português
    setlocale(LC_ALL, "portuguese");

    // Chama o menu principal do programa
    menuOpcoes();

    return 0;
}

void exibirTitulo(){
    LIMPA_TELA;

    printf(" Casa de Campo Tranquilidade\n\n");
}

void menuOpcoes(){
    int opcao;

    do{
        exibirTitulo();

        printf("\n Opções: \n\n");
        printf("\t1. Cadastrar Cliente\n");
        printf("\t2. Listagem Geral\n");
        printf("\t3. Relatório Estatístico\n");
        printf("\t0. Sair\n\n");

        do{
            printf(" Informe a opção desejada: ");
            scanf("%d", &opcao);

            if(opcao < 0 || opcao > 3){
                printf(" Opção inválida. Digite novamente.\n");
                getchar();
            }
        }while(opcao < 0 || opcao > 3);

        switch(opcao){
        case 1:
            telaCadastro();
            break;
        case 2:
            telaListagemGeral();
            break;
        case 3:
            telaRelatorioEstatistico();
            break;
        case 0:
            exit(0); // Fecha o programa;
            break;
        }
    }while(opcao != 0);
}

void telaCadastro(){
    int opcao;
    FILE* arquivo = fopen("db_cadastro_clientes.txt", "a");

    if(arquivo == NULL){
        printf(" Erro ao abrir o arquivo de dados de clientes.\n");
        return;
    }

    do{
        gravarDadosClientes(arquivo);

        printf(" Cadastrar outro (1-sim/0-não)?: ");
        scanf("%d", &opcao);

    }while(opcao == 1);

    fclose(arquivo);
}

void telaListagemGeral(){
    exibirTitulo();

    lerDadosClientes();

    printf("\n\n------------------------------------------------------------------------------------------------------------------------\n");

    printf(" Tecle enter para voltar ao menu...");
    getchar();
    while(getchar() != '\n');
}

void telaRelatorioEstatistico() {
    exibirTitulo();

    FILE* arquivo = fopen("db_cadastro_clientes.txt", "r");
    if (arquivo == NULL) {
        printf("O arquivo de dados de clientes não existe. Nenhum relatório pode ser gerado.\n");
        printf("\n\n------------------------------------------------------------------------------------------------------------------------\n");
        printf(" Tecle enter para voltar ao menu...");
        getchar();
        while (getchar() != '\n');
        return;
    }

    int cliMaisDezDias = 0;
    float mediaHospedes = 0.0;
    float percFuncAuxiliar = 0.0;
    int locMaisNovo = 120; // Valor inicial alto para encontrar o menor (difícilmente alguém com mais de 120 anos irá alugar);

    int totalClientes = 0;
    int totalHospedes = 0;
    int totalDias = 0;
    int totalAuxLimpeza = 0;

    struct Cliente Cliente;

    while (fscanf(arquivo, "%25[^|]|%d|%d|%d|%d|%f|%f\n", Cliente.nome, &Cliente.idade, &Cliente.qtd, &Cliente.dias, &Cliente.auxLimpeza, &Cliente.valorTotal, &Cliente.adiantamento) == 7) {
        totalClientes++;
        totalHospedes += Cliente.qtd;
        totalDias += Cliente.dias;
        totalAuxLimpeza += Cliente.auxLimpeza;

        if (Cliente.dias > 10) {
            cliMaisDezDias++;
        }

        if (Cliente.idade < locMaisNovo) {
            locMaisNovo = Cliente.idade;
        }
    }

    if (totalClientes > 0) {
        mediaHospedes = (float)totalHospedes / totalClientes;
        percFuncAuxiliar = (float)totalAuxLimpeza / totalClientes * 100;
    }

    fclose(arquivo);

    exibirTitulo();

    printf("\n\n Relatório Estatístico\n\n");
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf(" Quantidade de clientes que ficarão mais de 10 dias.....: %d\n", cliMaisDezDias);
    printf(" Média de quantidade de hóspedes........................: %.2f\n", mediaHospedes);
    printf(" Percentual de clientes que desejam funcionário auxiliar: %.2f%%\n", percFuncAuxiliar);
    printf(" Idade do locatário mais novo...........................: %d\n", locMaisNovo);

    printf("\n\n------------------------------------------------------------------------------------------------------------------------\n");

    printf(" Tecle enter para voltar ao menu...");
    getchar();
    while (getchar() != '\n');
}


float calcularValorAPagar(struct EntradaDados dados, int dias, int auxLimpeza) {
    float valorAluguel = dados.taxaAluguel * dias;
    float valorFuncAuxiliar = dados.taxaFuncAuxiliar * dias;

    if (auxLimpeza)
        return valorAluguel + valorFuncAuxiliar;
    else
        return valorAluguel;
}

void gravarDadosClientes(FILE* arquivo) {
    struct Cliente Cliente;

    exibirTitulo();

    printf("\n Cadastrar Cliente\n\n");

    // Validação do nome
    printf("\n\t Nome...............................................: ");
    getchar(); // Limpa o buffer de entrada antes de usar fgets
    fgets(Cliente.nome, sizeof(Cliente.nome), stdin);
    Cliente.nome[strcspn(Cliente.nome, "\n")] = '\0'; // Remover o caractere de nova linha (\n) lido pelo fgets

    // Validação da idade
    do {
        printf("\n\t Idade..............................................: ");
        scanf("%d", &Cliente.idade);
        getchar();

        if (Cliente.idade <= 0) {
            printf(" Valor inválido. Digite novamente.\n");
        }
    } while (Cliente.idade <= 0);

    // Validação da quantidade de pessoas na casa
    do {
        printf("\n\t Quantidade de pessoas na casa......................: ");
        scanf("%d", &Cliente.qtd);
        getchar();

        if (Cliente.qtd <= 0 || Cliente.qtd > 12) {
            printf(" Valor inválido. Digite novamente.\n");
        }
    } while (Cliente.qtd <= 0 || Cliente.qtd > 12);

    // Validação da quantidade de dias
    do {
        printf("\n\t Quantidade de dias.................................: ");
        scanf("%d", &Cliente.dias);
        getchar();

        if (Cliente.dias <= 0) {
            printf(" Valor inválido. Digite novamente.\n");
        }
    } while (Cliente.dias <= 0);


    // Validação da necessidade de funcionário para auxiliar na limpeza;
    do {
        printf("\n\t Funcionário para auxiliar na limpeza? (1-sim/0-não): ");
        scanf("%d", &Cliente.auxLimpeza);
        getchar();

        if (Cliente.auxLimpeza != 1 && Cliente.auxLimpeza != 0) {
            printf(" Valor inválido. Digite novamente.\n");
        }
    } while (Cliente.auxLimpeza != 1 && Cliente.auxLimpeza != 0);

    struct EntradaDados dados;
    int encontrado = 0;
    for (int i = 0; i < sizeof(tabelaReferenciaDados) / sizeof(tabelaReferenciaDados[0]); i++) {
        if (Cliente.qtd <= tabelaReferenciaDados[i].qtdHospedes) {
            dados = tabelaReferenciaDados[i];
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("Não há taxas disponíveis para a quantidade de hóspedes especificada.\n");
        return;
    }

    Cliente.valorTotal = calcularValorAPagar(dados, Cliente.dias, Cliente.auxLimpeza);
    Cliente.adiantamento = Cliente.valorTotal*0.30;

    fprintf(arquivo, "%s|%d|%d|%d|%d|%.2f|%.2f\n", Cliente.nome, Cliente.idade, Cliente.qtd, Cliente.dias, Cliente.auxLimpeza, Cliente.valorTotal, Cliente.adiantamento);

    printf("\n\n Cliente cadastrado com sucesso!\n\n");
}

void lerDadosClientes(){
    FILE* arquivo = fopen("db_cadastro_clientes.txt", "r");

    if(arquivo == NULL){
        printf(" O arquivo de dados de Clientes não existe. Será criado um novo arquivo.\n");
        return;
    }

    exibirTitulo();

    printf("\n\n Listagem Geral\n\n");
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf(" Nome                         Idade    Hóspedes    Dias    Funcionário    Valor Total    Adiantamento\n");
    printf("------------------------------------------------------------------------------------------------------------------------\n");

    struct Cliente Cliente;

    while (fscanf(arquivo, "%25[^|]|%d|%d|%d|%d|%f|%f\n", Cliente.nome, &Cliente.idade, &Cliente.qtd, &Cliente.dias, &Cliente.auxLimpeza, &Cliente.valorTotal, &Cliente.adiantamento) == 7) {
        // Da linha 310 à 314 é um "RTI" para fazer exibir os valores em float com o espaçamento adequado;
        char stringValorTotal[10];
        char stringAdiantamento[10];

        snprintf(stringValorTotal, sizeof(stringValorTotal), "%.2f", Cliente.valorTotal);
        snprintf(stringAdiantamento, sizeof(stringAdiantamento), "%.2f", Cliente.adiantamento);

        printf(" %-27s  %-7d  %-10d  %-6d  %-14d R$ %-11s R$ %s\n", Cliente.nome, Cliente.idade, Cliente.qtd, Cliente.dias, Cliente.auxLimpeza, stringValorTotal, stringAdiantamento);
    }

    fclose(arquivo);
}
