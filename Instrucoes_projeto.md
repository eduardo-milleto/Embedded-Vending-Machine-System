### README.md

# Projeto Vending Machine

Este projeto implementa o controle de uma máquina de vendas automática utilizando um microcontrolador AVR, um display LCD, e um teclado matricial. A máquina permite ao usuário selecionar um produto, escolher o método de pagamento (cartão ou dinheiro), e efetuar a compra.

## Estrutura do Projeto

O código está organizado em várias bibliotecas, cada uma responsável por uma funcionalidade específica. Abaixo, descrevemos a função de cada biblioteca e os arquivos principais do projeto.

### Bibliotecas

#### 1. `lcd`

A biblioteca `lcd` é responsável por controlar o display LCD. Ela permite a inicialização do display, o envio de comandos e dados, e a exibição de mensagens.

- **lcd.h**: Declara as funções para inicializar o LCD, limpar a tela, definir a posição do cursor, enviar caracteres e exibir mensagens.
- **lcd.cpp**: Implementa as funções declaradas em `lcd.h`. Inclui a lógica para enviar comandos e dados ao LCD, bem como a inicialização do display.

#### 2. `teclado`

A biblioteca `teclado` é responsável por gerenciar a leitura do teclado matricial. Ela inclui funções para debouncing e leitura das teclas pressionadas.

- **teclado.h**: Declara as funções para debouncing, leitura das teclas e atraso de tempo.
- **teclado.cpp**: Implementa as funções declaradas em `teclado.h`. Inclui a lógica para debouncing e leitura das teclas do teclado matricial.

#### 3. `opcao`

A biblioteca `opcao` gerencia a seleção do método de pagamento (cartão ou dinheiro) e exibe as informações do produto.

- **opcao.h**: Declara as funções para solicitar o código do produto, exibir as informações do produto, e selecionar o método de pagamento.
- **opcao.cpp**: Implementa as funções declaradas em `opcao.h`. Inclui a lógica para solicitar o código do produto, exibir as informações do produto (nome e preço), e permitir que o usuário escolha entre pagamento com cartão ou dinheiro.

#### 4. `produtos`

A biblioteca `produtos` armazena informações sobre os produtos disponíveis na máquina de vendas.

- **produtos.h**: Define a estrutura de dados `Product` e declara um array de produtos e seu tamanho.
- **produtos.cpp**: Inicializa o array de produtos com informações como código, nome e preço.

#### 5. `cartao`

A biblioteca `cartao` gerencia o processo de pagamento com cartão.

- **cartao.h**: Declara as funções para lidar com o pagamento com cartão e exibir mensagens de erro ou sucesso.
- **cartao.cpp**: Implementa as funções declaradas em `cartao.h`. Inclui a lógica para verificar o saldo do cartão, debitar o valor da compra, e exibir mensagens de erro ou sucesso no display.

#### 6. `dinheiro`

A biblioteca `dinheiro` gerencia o processo de pagamento em dinheiro.

- **dinheiro.h**: Declara as funções para lidar com o pagamento em dinheiro e exibir mensagens de erro ou sucesso.
- **dinheiro.cpp**: Implementa as funções declaradas em `dinheiro.h`. Inclui a lógica para calcular o total inserido, verificar se o valor é suficiente para a compra, e exibir mensagens de erro ou sucesso no display.

### Arquivo Principal

#### `main.cpp`

O arquivo principal do projeto, `main.cpp`, configura os pinos de entrada e saída, inicializa o display LCD, e chama a função para selecionar o método de pagamento.

- Configura os pinos das linhas e colunas do teclado como entrada e saída.
- Configura os pinos do LCD como saída.
- Inicializa o LCD e limpa a tela.
- Chama a função `selectPaymentMethod` da biblioteca `opcao` para iniciar o processo de seleção do método de pagamento.

### Instruções de Uso

1. **Configuração do Hardware**:
   - Conecte o display LCD aos pinos definidos em `lcd.cpp`.
   - Conecte o teclado matricial aos pinos definidos em `teclado.cpp`.
   - Conecte os LEDs e outros componentes conforme necessário.

2. **Compilação e Upload**:
   - Use o Atmel Studio ou outro ambiente de desenvolvimento AVR para compilar o código.
   - Faça o upload do código compilado para o microcontrolador AVR.

3. **Operação**:
   - Ao ligar a máquina de vendas, o display LCD solicitará o código do produto.
   - Após inserir o código do produto, o nome e o preço do produto serão exibidos no LCD.
   - O usuário poderá então selecionar o método de pagamento (cartão ou dinheiro).
   - O processo de pagamento será gerenciado pelas funções das bibliotecas `cartao` ou `dinheiro`.

4. **Personalização**:
   - Para adicionar ou modificar produtos, edite o array `products` em `produtos.cpp`.
   - Para alterar as mensagens exibidas no LCD, edite as funções em `lcd.cpp`.

Este projeto fornece uma base sólida para o desenvolvimento de uma máquina de vendas automática utilizando um microcontrolador AVR, um display LCD, e um teclado matricial. Sinta-se à vontade para expandir e personalizar conforme necessário.