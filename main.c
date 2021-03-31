#include <stdio.h>
#include <locale.h>

int opcaoEscolhidaPeloUsuario();
void executaComando(int opcaoSerExecutada);
void incluiContanto();
void excluiContato();
void editaContato();
void listaTodosContatos();

int main(int argc, char *argv[]) {

    setlocale(LC_ALL, "Portuguese");

    int opcaoSerExecutada;
    do {
        opcaoSerExecutada = 0;
        opcaoSerExecutada = opcaoEscolhidaPeloUsuario();
        if (opcaoSerExecutada != 5) {
            executaComando(opcaoSerExecutada);
        }
    } while (opcaoSerExecutada!=5);
    
    printf("Programa foi finalizado.");

    return 0;
}

int opcaoEscolhidaPeloUsuario() {
    int opcaoEscolhida = 0;

    printf("[1] Incluir um novo contato.\n");
    printf("[2] Excluir um contanto existente.\n");
    printf("[3] Alterar um contato existente.\n");
    printf("[4] Listar todos os contantos cadastrados\n");
    printf("[5] Finalizar o programa\n");
    printf("Digite o número da execução que você deseja executar: \n");

    scanf("%d", &opcaoEscolhida);

    return opcaoEscolhida;
}

void executaComando(int opcaoSerExecutada) {
    switch(opcaoSerExecutada) {
        case 1:
            incluiContanto();
            break;
        case 2:
            excluiContato();
            break;
        case 3:
            editaContato();
            break;
        case 4:
            listaTodosContatos();
            break;
        default:
            printf("Erro! Um número inválido foi digitado!\n");
    }
}

void incluiContanto() {
    printf("Novo contato\n");
}

void excluiContato() {
    printf("Contato excluido\n");
}

void editaContato() {
    printf("Contato editado\n");
}

void listaTodosContatos() {
    printf("Todos contatos listados\n");
}