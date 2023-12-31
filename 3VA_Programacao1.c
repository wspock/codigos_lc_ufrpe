#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#ifdef _WIN32
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

int main(){
    setlocale(LC_ALL, "portuguese");

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

        printf("\n Op��es: \n\n");
        printf("\t1. Cadastrar Cliente\n");
        printf("\t2. Listagem Geral\n");
        printf("\t3. Relat�rio Estat�stico\n");
        printf("\t0. Sair\n\n");

        do{
            printf(" Informe a op��o desejada: ");
            scanf("%d", &opcao);

            if(opcao < 0 || opcao > 3){
                printf(" Op��o inv�lida. Digite novamente.\n");
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

        printf(" Cadastrar outro (1-sim/0-n�o)?: ");
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
        printf("O arquivo de dados de clientes n�o existe. Nenhum relat�rio pode ser gerado.\n");
        printf("\n\n------------------------------------------------------------------------------------------------------------------------\n");
        printf(" Tecle enter para voltar ao menu...");
        getchar();
        while (getchar() != '\n');
        return;
    }

    int cliMaisDezDias = 0;
    float mediaHospedes = 0.0;
    float percFuncAuxiliar = 0.0;
    int locMaisNovo = 120; // Valor inicial alto para ajudar a encontrar o menor (dif�cilmente algu�m com mais de 120 anos ir� alugar);

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

    printf("\n\n Relat�rio Estat�stico\n\n");
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf(" Quantidade de clientes que ficar�o mais de 10 dias.....: %d\n", cliMaisDezDias);
    printf(" M�dia de quantidade de h�spedes........................: %.2f\n", mediaHospedes);
    printf(" Percentual de clientes que desejam funcion�rio auxiliar: %.2f%%\n", percFuncAuxiliar);
    printf(" Idade do locat�rio mais novo...........................: %d\n", locMaisNovo);

    printf("\n\n------------------------------------------------------------------------------------------------------------------------\n");

    printf(" Tecle enter para voltar ao menu...");
    getchar();
    while (getchar() != '\n');
}

float calcularValorAPagar(int qtdHospedes, int dias, int auxLimpeza) {
    float valorAluguel;
    float valorFuncAuxiliar;

    if (qtdHospedes <= 5) {
        valorAluguel = 450.0;
        valorFuncAuxiliar = 90.0;
    } else if (qtdHospedes <= 8) {
        valorAluguel = 680.0;
        valorFuncAuxiliar = 110.0;
    } else if (qtdHospedes <= 12) {
        valorAluguel = 950.0;
        valorFuncAuxiliar = 140.0;
    } else {
        // Valor inv�lido de quantidade de h�spedes
        return 0.0;
    }

    float valorTotal = valorAluguel * dias;

    if (auxLimpeza)
        valorTotal += valorFuncAuxiliar * dias;

    return valorTotal;
}

void gravarDadosClientes(FILE* arquivo) {
    struct Cliente Cliente;

    exibirTitulo();

    printf("\n Cadastrar Cliente\n\n");

    // Valida��o do nome
    printf("\n\t Nome...............................................: ");
    getchar(); // Limpa o buffer de entrada antes de usar fgets
    fgets(Cliente.nome, sizeof(Cliente.nome), stdin);
    Cliente.nome[strcspn(Cliente.nome, "\n")] = '\0'; // Remover o caractere de nova linha (\n) lido pelo fgets

    // Valida��o da idade
    do {
        printf("\n\t Idade..............................................: ");
        scanf("%d", &Cliente.idade);
        getchar();

        if (Cliente.idade <= 0) {
            printf(" Valor inv�lido. Digite novamente.\n");
        }
    } while (Cliente.idade <= 0);

    // Valida��o da quantidade de pessoas na casa
    do {
        printf("\n\t Quantidade de pessoas na casa......................: ");
        scanf("%d", &Cliente.qtd);
        getchar();

        if (Cliente.qtd <= 0 || Cliente.qtd > 12) {
            printf(" Valor inv�lido. Digite novamente.\n");
        }
    } while (Cliente.qtd <= 0 || Cliente.qtd > 12);

    // Valida��o da quantidade de dias
    do {
        printf("\n\t Quantidade de dias.................................: ");
        scanf("%d", &Cliente.dias);
        getchar();

        if (Cliente.dias <= 0) {
            printf(" Valor inv�lido. Digite novamente.\n");
        }
    } while (Cliente.dias <= 0);


    // Valida��o da necessidade de funcion�rio para auxiliar na limpeza;
    do {
        printf("\n\t Funcion�rio para auxiliar na limpeza? (1-sim/0-n�o): ");
        scanf("%d", &Cliente.auxLimpeza);
        getchar();

        if (Cliente.auxLimpeza != 1 && Cliente.auxLimpeza != 0) {
            printf(" Valor inv�lido. Digite novamente.\n");
        }
    } while (Cliente.auxLimpeza != 1 && Cliente.auxLimpeza != 0);

    Cliente.valorTotal = calcularValorAPagar(Cliente.qtd, Cliente.dias, Cliente.auxLimpeza);
    Cliente.adiantamento = Cliente.valorTotal * 0.30; // 30% de adiantamento;

    if (Cliente.valorTotal == 0.0) {
        printf(" N�o h� taxas dispon�veis para a quantidade de h�spedes especificada.\n");
        return;
    }

    fprintf(arquivo, "%s|%d|%d|%d|%d|%.2f|%.2f\n", Cliente.nome, Cliente.idade, Cliente.qtd, Cliente.dias, Cliente.auxLimpeza, Cliente.valorTotal, Cliente.adiantamento);

    printf("\n\n Cliente cadastrado com sucesso!\n\n");
}

void lerDadosClientes(){
    FILE* arquivo = fopen("db_cadastro_clientes.txt", "r");

    if(arquivo == NULL){
        printf(" O arquivo de dados de clientes n�o existe. Ser� criado um novo arquivo.\n");
        return;
    }

    exibirTitulo();

    printf("\n\n Listagem Geral\n\n");
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf(" Nome                         Idade    H�spedes    Dias    Funcion�rio    Valor Total    Adiantamento\n");
    printf("------------------------------------------------------------------------------------------------------------------------\n");

    struct Cliente Cliente;

    while (fscanf(arquivo, "%25[^|]|%d|%d|%d|%d|%f|%f\n", Cliente.nome, &Cliente.idade, &Cliente.qtd, &Cliente.dias, &Cliente.auxLimpeza, &Cliente.valorTotal, &Cliente.adiantamento) == 7) {
        // Da linha 310 � 314 � uma "gambiarra" para fazer exibir os valores em float com o espa�amento adequado;
        char stringValorTotal[10];
        char stringAdiantamento[10];

        snprintf(stringValorTotal, sizeof(stringValorTotal), "%.2f", Cliente.valorTotal);
        snprintf(stringAdiantamento, sizeof(stringAdiantamento), "%.2f", Cliente.adiantamento);

        printf(" %-27s  %-7d  %-10d  %-6d  %-14d R$ %-11s R$ %s\n", Cliente.nome, Cliente.idade, Cliente.qtd, Cliente.dias, Cliente.auxLimpeza, stringValorTotal, stringAdiantamento);
    }

    fclose(arquivo);
}
