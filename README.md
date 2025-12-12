# C Generic List

Uma implementação de uma lista encadeada genérica e de tipagem dinâmica em C, utilizando um estilo de programação orientado a objetos com ponteiros de função.

Esta biblioteca fornece uma estrutura de lista flexível que pode armazenar diferentes tipos de dados, incluindo `int`, `float`, `double`, `char*` (strings) e ponteiros genéricos (`void*`). A biblioteca copia automaticamente valores para tipos primitivos e strings; para ponteiros genéricos (tipo `T`/`void*`) a lista armazena o ponteiro por valor — veja a seção "Gerenciamento de memória" abaixo.

Status do projeto
- Estado: este repositório está marcado como "deprecated". Considere o projeto como arquivado/obsoleto; use com cautela ou procure uma alternativa mantida ativamente.

Índice
- Funcionalidades
- Requisitos
- Compilação
- Uso rápido
- Gerenciamento de memória
- Exemplos
- Documentação (Doxygen)
- Testes e CI
- Licença
- Contato

## ✨ Funcionalidades

- Tipagem dinâmica: listas para `int`, `float`, `double`, `char*` (strings) e ponteiros genéricos (`void*`/`T`).
- Interface orientada a objetos via ponteiros de função (ex.: `lista->push(lista, valor)`).
- Gerenciamento de memória para tipos primitivos e strings (valores copiados).
- Operações suportadas:
  - `push`: adiciona ao final.
  - `pop`: remove e retorna o primeiro elemento.
  - `insert`: insere em índice específico.
  - `remove`: remove elemento em índice específico.
  - `pick`: remove e retorna elemento em índice específico.
  - `get`: obtém ponteiro para elemento em índice.
  - `set`: atualiza valor em índice.
  - `len`: retorna número de elementos.
  - `foreach`: aplica callback a cada elemento.
  - `print`: imprime conteúdo (para tipos básicos).
  - `free`: libera memória alocada pelos nós da lista.

## Requisitos
- CMake
- Compilador C (GCC, Clang, MSVC)
- (Opcional) Doxygen para gerar documentação da API

## Compilação

1. Clone o repositório (ajuste a URL/nome conforme o repo ativo):
```bash
git clone https://github.com/Slot148/C-generic-list-deprecated.git
cd C-generic-list-deprecated
```

2. Crie diretório de build e compile:
```bash
mkdir build
cd build
cmake ..
cmake --build .
# ou, em sistemas Unix:
# cmake ..
# make
```

3. Após compilar, o binário e/ou biblioteca deve ser gerado em `build/` ou em `lib/` conforme CMakeLists. Para compilar um programa que usa a biblioteca:
```bash
# Exemplo de compilação linkando com a biblioteca gerada
gcc meu_arquivo.c -Iinclude -Llib -ltlist -o meu_programa
```
Observações:
- Se a biblioteca for gerada como arquivo estático (`libtlist.a`) ou dinâmica (`libtlist.so`/`tlist.dll`), ajuste `-ltlist` e `-L` conforme necessário.
- Adicione `-Wl,-rpath,<path>` ou configure PATH/LD_LIBRARY_PATH no caso de bibliotecas dinâmicas.

## Uso rápido

Inclua o header:
```c
#include "Tlist.h"
```

Crie a lista (exemplo para inteiros):
```c
List intList = newList(INT);
intList->push(intList, 10);
intList->push(intList, 20);
intList->push(intList, 30);
intList->print(intList); // Saída: [ 10, 20, 30 ]
intList->free(intList);
free(intList);
```

## Gerenciamento de memória (importante)
- Tipos primitivos (`int`, `float`, `double`) e strings (`char*`) são copiados pela lista (ou duplicados, para strings). A lista faz a alocação necessária para armazenar esses valores.
- Para o tipo genérico `T` / `void*`: a lista armazena apenas o ponteiro fornecido; ela NÃO faz cópia profunda automática. Isso significa:
  - O chamador é responsável por garantir que o apontado tenha vida útil compatível com o uso pela lista.
  - O chamador é responsável por alocar e liberar a memória apontada (a menos que a implementação especifique o contrário).
- Ao chamar `list->free(list)`, a implementação libera os nós e as cópias internas que a lista gerou (por exemplo, para strings e tipos primitivos). Se você armazenou `void*` para objetos alocados externamente, libere esses objetos antes ou depois de `list->free`, conforme apropriado.
- Exemplo inseguro (não faça isto, pois `p1` é local e ficará inválido após a função):
```c
Pessoa p1 = {"Alice", 30};
pessoaList->push(pessoaList, &p1); // perigoso: ponteiro para stack
```
- Exemplo seguro (alocando Pessoa dinamicamente):
```c
Pessoa *p1 = malloc(sizeof(Pessoa));
p1->nome = strdup("Alice");
p1->idade = 30;
pessoaList->push(pessoaList, p1);
/* ... mais tarde ... */
free(p1->nome);
free(p1);
```

## Exemplos

### Lista de inteiros
```c
#include "Tlist.h"

int main(void) {
    List intList = newList(INT);
    intList->push(intList, 10);
    intList->push(intList, 20);
    intList->push(intList, 30);
    intList->print(intList); // [ 10, 20, 30 ]
    intList->free(intList);
    free(intList);
    return 0;
}
```

### Lista de structs (ponteiros genéricos) — versão segura
```c
#include "Tlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* nome;
    int idade;
} Pessoa;

void printPessoa(void* data) {
    Pessoa* p = (Pessoa*)data;
    printf("Pessoa{ nome=%s, idade=%d }\n", p->nome, p->idade);
}

int main() {
    List pessoaList = newList(T);

    Pessoa *p1 = malloc(sizeof(Pessoa));
    p1->nome = strdup("Alice");
    p1->idade = 30;

    Pessoa *p2 = malloc(sizeof(Pessoa));
    p2->nome = strdup("Bob");
    p2->idade = 25;

    pessoaList->push(pessoaList, p1);
    pessoaList->push(pessoaList, p2);

    pessoaList->foreach(pessoaList, printPessoa);

    // Se a lista não libera automaticamente os dados apontados, libere manualmente:
    for (size_t i = 0; i < pessoaList->len(pessoaList); ++i) {
        Pessoa *p = pessoaList->get(pessoaList, i);
        free(p->nome);
        free(p);
    }

    pessoaList->free(pessoaList);
    free(pessoaList);

    return 0;
}
```

## Documentação (Doxygen)
1. Instale o Doxygen.
2. Na pasta `documentation` (ou onde o Doxyfile estiver), execute:
```bash
doxygen Doxyfile
```
3. Abra `documentation/html/index.html` (ou `doxygenDocs/html/index.html` conforme sua configuração) no navegador.


## Licença
Este projeto usa a Licença MIT. Veja o arquivo `LICENSE` para detalhes.

---

Feito por **Slot148**.

[LinkedIn](https://www.linkedin.com/in/nicolas-anderson-34b082302/) | [GitHub](https://github.com/Slot148)
```
