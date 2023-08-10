# Use a imagem oficial do Golang como base
FROM golang:latest

# Defina o diretório de trabalho dentro do contêiner
WORKDIR /app

# Copie o código fonte para o diretório de trabalho no contêiner
COPY . .

# Compile o código do Go
RUN go build -o main .

# Comando padrão a ser executado quando o contêiner for iniciado
CMD ["./main"]
