package main

import (
	"bufio"
	"fmt"
	"os"
	"os/exec"
	"strconv"
	"strings"
)

type Cliente struct {
	nome         string
	idade        int
	qtd          int
	dias         int
	auxLimpeza   int
	valorTotal   float32
	adiantamento float32
}

func main() {
	menuOpcoes()
}

func clearScreen() {
	cmd := exec.Command("clear")
	cmd.Stdout = os.Stdout
	cmd.Run()
}

func exibirTitulo() {
	clearScreen()

	fmt.Println(" Casa de Campo Tranquilidade\n")
}

func menuOpcoes() {
	var opcao int

	for {
		exibirTitulo()

		fmt.Println("\n Opções: \n")
		fmt.Println("\t1. Cadastrar Cliente")
		fmt.Println("\t2. Listagem Geral")
		fmt.Println("\t3. Relatório Estatístico")
		fmt.Println("\t0. Sair\n")

		for {
			fmt.Print(" Informe a opção desejada: ")
			fmt.Scan(&opcao)

			if opcao < 0 || opcao > 3 {
				fmt.Println(" Opção inválida. Digite novamente.")
			} else {
				break
			}
		}

		switch opcao {
		case 1:
			telaCadastro()
		case 2:
			telaListagemGeral()
		case 3:
			telaRelatorioEstatistico()
		case 0:
			os.Exit(0)
		}
	}
}

func telaCadastro() {
	var opcao int
	arquivo, err := os.OpenFile("db_cadastro_clientes.txt", os.O_WRONLY|os.O_APPEND|os.O_CREATE, 0666)
	if err != nil {
		fmt.Println(" Erro ao abrir o arquivo de dados de clientes.")
		return
	}
	defer arquivo.Close()

	for {
		gravarDadosClientes(arquivo)

		fmt.Print(" Cadastrar outro (1-sim/0-não)?: ")
		fmt.Scan(&opcao)

		if opcao != 1 {
			break
		}
	}
}

func telaListagemGeral() {
	exibirTitulo()

	lerDadosClientes()

	fmt.Println("\n\n------------------------------------------------------------------------------------------------------------------------")
	fmt.Print(" Tecle enter para voltar ao menu...")
	bufio.NewReader(os.Stdin).ReadBytes('\n')
}

func telaRelatorioEstatistico() {
	exibirTitulo()

	arquivo, err := os.Open("db_cadastro_clientes.txt")
	if err != nil {
		fmt.Println("O arquivo de dados de clientes não existe. Nenhum relatório pode ser gerado.")
		fmt.Println("\n\n------------------------------------------------------------------------------------------------------------------------")
		fmt.Print(" Tecle enter para voltar ao menu...")
		bufio.NewReader(os.Stdin).ReadBytes('\n')
		return
	}
	defer arquivo.Close()

	var cliMaisDezDias, totalClientes, totalHospedes, totalDias, totalAuxLimpeza, locMaisNovo int
	var mediaHospedes, percFuncAuxiliar float32

	locMaisNovo = 120 // Valor inicial alto para ajudar a encontrar o menor (dificilmente alguém com mais de 120 anos irá alugar)

	scanner := bufio.NewScanner(arquivo)
	for scanner.Scan() {
		linha := scanner.Text()
		campos := strings.Split(linha, "|")
		if len(campos) != 7 {
			continue
		}

		idade, _ := strconv.Atoi(campos[1])
		qtd, _ := strconv.Atoi(campos[2])
		dias, _ := strconv.Atoi(campos[3])
		auxLimpeza, _ := strconv.Atoi(campos[4])

		totalClientes++
		totalHospedes += qtd
		totalDias += dias
		totalAuxLimpeza += auxLimpeza

		if dias > 10 {
			cliMaisDezDias++
		}

		if idade < locMaisNovo {
			locMaisNovo = idade
		}
	}

	if totalClientes > 0 {
		mediaHospedes = float32(totalHospedes) / float32(totalClientes)
		percFuncAuxiliar = (float32(totalAuxLimpeza) / float32(totalClientes)) * 100
	}

	exibirTitulo()

	fmt.Println("\n\n Relatório Estatístico\n")
	fmt.Println("------------------------------------------------------------------------------------------------------------------------")
	fmt.Printf(" Quantidade de clientes que ficarão mais de 10 dias.....: %d\n", cliMaisDezDias)
	fmt.Printf(" Média de quantidade de hóspedes........................: %.2f\n", mediaHospedes)
	fmt.Printf(" Percentual de clientes que desejam funcionário auxiliar: %.2f%%\n", percFuncAuxiliar)
	fmt.Printf(" Idade do locatário mais novo...........................: %d\n", locMaisNovo)
	fmt.Println("\n\n------------------------------------------------------------------------------------------------------------------------")
	fmt.Print(" Tecle enter para voltar ao menu...")
	bufio.NewReader(os.Stdin).ReadBytes('\n')
}

func calcularValorAPagar(qtdHospedes, dias, auxLimpeza int) float32 {
	var valorAluguel, valorFuncAuxiliar float32

	if qtdHospedes <= 5 {
		valorAluguel = 450.0
		valorFuncAuxiliar = 90.0
	} else if qtdHospedes <= 8 {
		valorAluguel = 680.0
		valorFuncAuxiliar = 110.0
	} else if qtdHospedes <= 12 {
		valorAluguel = 950.0
		valorFuncAuxiliar = 140.0
	} else {
		return 0.0
	}

	valorTotal := valorAluguel * float32(dias)

	if auxLimpeza != 0 {
		valorTotal += valorFuncAuxiliar * float32(dias)
	}

	return valorTotal
}

func gravarDadosClientes(arquivo *os.File) {
	var Cliente Cliente

	exibirTitulo()

	fmt.Println("\n Cadastrar Cliente\n")

	scanner := bufio.NewScanner(os.Stdin)

	fmt.Print("\t Nome...............................................: ")
	scanner.Scan()
	Cliente.nome = scanner.Text()

	fmt.Print("\n\t Idade..............................................: ")
	scanner.Scan()
	Cliente.idade, _ = strconv.Atoi(scanner.Text())

	fmt.Print("\n\t Quantidade de pessoas na casa......................: ")
	scanner.Scan()
	Cliente.qtd, _ = strconv.Atoi(scanner.Text())

	fmt.Print("\n\t Quantidade de dias.................................: ")
	scanner.Scan()
	Cliente.dias, _ = strconv.Atoi(scanner.Text())

	fmt.Print("\n\t Funcionário para auxiliar na limpeza? (1-sim/0-não): ")
	scanner.Scan()
	Cliente.auxLimpeza, _ = strconv.Atoi(scanner.Text())

	Cliente.valorTotal = calcularValorAPagar(Cliente.qtd, Cliente.dias, Cliente.auxLimpeza)
	Cliente.adiantamento = Cliente.valorTotal * 0.30

	if Cliente.valorTotal == 0.0 {
		fmt.Println(" Não há taxas disponíveis para a quantidade de hóspedes especificada.")
		return
	}

	linha := fmt.Sprintf("%s|%d|%d|%d|%d|%.2f|%.2f\n", Cliente.nome, Cliente.idade, Cliente.qtd, Cliente.dias, Cliente.auxLimpeza, Cliente.valorTotal, Cliente.adiantamento)
	arquivo.WriteString(linha)

	fmt.Println("\n\n Cliente cadastrado com sucesso!\n\n")
}

func lerDadosClientes() {
	arquivo, err := os.Open("db_cadastro_clientes.txt")
	if err != nil {
		fmt.Println(" O arquivo de dados de clientes não existe. Será criado um novo arquivo.")
		return
	}
	defer arquivo.Close()

	exibirTitulo()

	fmt.Println("\n\n Listagem Geral\n")
	fmt.Println("------------------------------------------------------------------------------------------------------------------------")
	fmt.Println(" Nome                         Idade    Hóspedes    Dias    Funcionário    Valor Total    Adiantamento")
	fmt.Println("------------------------------------------------------------------------------------------------------------------------")

	scanner := bufio.NewScanner(arquivo)
	for scanner.Scan() {
		linha := scanner.Text()
		campos := strings.Split(linha, "|")
		if len(campos) != 7 {
			continue
		}

		nome := campos[0]
		idade, _ := strconv.Atoi(campos[1])
		qtd, _ := strconv.Atoi(campos[2])
		dias, _ := strconv.Atoi(campos[3])
		auxLimpeza, _ := strconv.Atoi(campos[4])
		valorTotal, _ := strconv.ParseFloat(campos[5], 32)
		adiantamento, _ := strconv.ParseFloat(campos[6], 32)

		fmt.Printf(" %-27s  %-7d  %-10d  %-6d  %-14d R$ %-11.2f R$ %.2f\n", nome, idade, qtd, dias, auxLimpeza, valorTotal, adiantamento)
	}
}
