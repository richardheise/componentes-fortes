# Componentes Fortemente Conectados

Este projeto encontra os componentes fortemente conectados de um grafo direcionado.

## Como executar

Para compilar o código, execute o seguinte comando:

```bash
make
```

Para executar o código, você precisa fornecer um grafo no formato `dot` para a entrada padrão. Por exemplo:

```bash
./test < input.txt
```

A saída serão os componentes fortemente conectados do grafo de entrada, também no formato `dot`.