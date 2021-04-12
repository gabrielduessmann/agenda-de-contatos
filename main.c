/* 
   Desenvolvido por Cristian Eduardo e Gabriel Duessmann  
*/



#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define CAMINHO_ARQUIVO "database.txt"


typedef struct {
    int id;
    char nome[20];
    char sobrenome[30];
    char email[100];
    signed long telefone;
} Contato;

int opcaoEscolhidaPeloUsuario();
void executaComando(int opcaoSerExecutada);
void incluiContato();
void excluiContato();
void editaContato();
void listaTodosContatos();
Contato obtemNovoContato();
int obtemPosicaoIdEscolhido(Contato *contatos, int id, int quantidadeContatos);
void salvaContatoNoArquivo(FILE *arquivo, Contato contato);
void criaArquivoInicial();
void mostraMensagemDeErroAoTentarAbrirArquivo();
void mostraMensagemQueNaoExisteNenhumContato();
Contato *obtemTodosContatos(int qtdeContatos);



int main(int argc, char *argv[]) {

    setlocale(LC_ALL, "Portuguese");

    criaArquivoInicial();

    int opcaoSerExecutada = 0;
    do {
        opcaoSerExecutada = opcaoEscolhidaPeloUsuario();
        if (opcaoSerExecutada != 5) {
            executaComando(opcaoSerExecutada);
        }
    } while (opcaoSerExecutada!=5);
    

    printf("O programa foi finalizado.\n\n");

    return 0;
}

int opcaoEscolhidaPeloUsuario() {
    int opcaoEscolhida = 0;

    printf("\n");
    printf("[1] Incluir um novo contato.\n");
    printf("[2] Excluir um contato existente.\n");
    printf("[3] Alterar um contato existente.\n");
    printf("[4] Listar todos os contatos cadastrados.\n");
    printf("[5] Finalizar o programa.\n");
    printf("Digite o número da execução que você deseja executar: ");

    scanf("%i", &opcaoEscolhida);
    printf("\n");

    return opcaoEscolhida;
}

void executaComando(int opcaoSerExecutada) {
    switch(opcaoSerExecutada) {
        case 1:
            incluiContato();
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
            printf("Um número inválido foi digitado! Por favor digite um número válido.\n");
    }
}

void incluiContato() {
      
    FILE *arquivo = fopen(CAMINHO_ARQUIVO,"r");

    if (arquivo != NULL) {

        int qtdeContatos;      

        fscanf(arquivo, "%i", &qtdeContatos);
        fclose(arquivo);
                       
        Contato *contatos;

        if (qtdeContatos > 0) {
            contatos =  obtemTodosContatos(qtdeContatos);
        }
       
        printf("Observação! Ao digitar os dados, não utilize espaços.\n\n");
        printf("** Novo contato ** \n");
        Contato novoContato = obtemNovoContato();

        FILE *novoArquivo = fopen(CAMINHO_ARQUIVO, "w");

        if (novoArquivo != NULL) {
               
            fprintf(novoArquivo, "%i", qtdeContatos + 1);

            if (qtdeContatos > 0) {

                int ultimoContatoIndex = qtdeContatos - 1;
                novoContato.id = contatos[ultimoContatoIndex].id + 1;

                int i;
                
                for (i = 0; i < qtdeContatos; i++) {

                    salvaContatoNoArquivo(novoArquivo, contatos[i]);
                }

                free(contatos); 
                
            } else {
                novoContato.id = 1;
            }
                            
            salvaContatoNoArquivo(novoArquivo, novoContato);

            printf("\nContato incluido com sucesso!\n");
           
            fclose(novoArquivo);

        } else {
            mostraMensagemDeErroAoTentarAbrirArquivo();
        }    
        
           
 
    } else {
        mostraMensagemDeErroAoTentarAbrirArquivo();
    }
}

void editaContato() {
   
        
    FILE *arquivo = fopen(CAMINHO_ARQUIVO, "r");

    if (arquivo != NULL) {

        int qtdeContatos;
        fscanf(arquivo, "%i", &qtdeContatos);
        fclose(arquivo);
        
        if (qtdeContatos > 0) {

            int id;

            printf("\nDigite o Id do contato a ser editado (0 para cancelar): ");
            scanf("%i", &id);
    
            if (id == 0) {
                printf("\n");
            } else {          

                Contato *contatos = obtemTodosContatos(qtdeContatos);
                
                int indexIdContatoQueSeraEditado = obtemPosicaoIdEscolhido(contatos, id, qtdeContatos);
                if (indexIdContatoQueSeraEditado >= 0) {

                    
                    printf("\nObservação! Ao digitar os dados, não digite mais que uma palavra, pois não pode conter espaços.\n\n");

                    printf("** Editar contato ** \n");
                    Contato novoContato = obtemNovoContato();

                                    
                    FILE *novoArquivo = fopen(CAMINHO_ARQUIVO, "w");
            

                    if (novoArquivo != NULL) {
                        
                        fprintf(novoArquivo, "%i", qtdeContatos);
                            
                        int i;
                        
                        for (i = 0; i < qtdeContatos; i++) {
                            if (i == indexIdContatoQueSeraEditado) {
                                novoContato.id = contatos[i].id;
                                salvaContatoNoArquivo(novoArquivo, novoContato);
                            }
                            else {
                                salvaContatoNoArquivo(novoArquivo, contatos[i]);
                            }
                        }
                        fclose(novoArquivo);
                    } else {
                        mostraMensagemDeErroAoTentarAbrirArquivo();
                    }

                    printf("\nContato editado com sucesso!\n");
                
                } else {
                    printf("\nFalha! O Id informado não existe.\n");
                } 

                free(contatos); 

            }                

        } else {
            mostraMensagemQueNaoExisteNenhumContato();
        }

    } else {
       mostraMensagemDeErroAoTentarAbrirArquivo();
    }

}


void excluiContato() {
        
    FILE *arquivo = fopen(CAMINHO_ARQUIVO, "r");
    
    if (arquivo != NULL) {
      
        int qtdeContatos;

        fscanf(arquivo, "%i", &qtdeContatos);
        fclose(arquivo); 
        
        if (qtdeContatos > 0) {

            int id;
    
            printf("\nDigite o Id do contato a ser excluído (0 para cancelar): ");
            scanf("%i", &id);
            
            if (id == 0) {
                printf("\n");

            } else {

                Contato *contatos = obtemTodosContatos(qtdeContatos);

                int indexIdContatoQueSeraExcluido = obtemPosicaoIdEscolhido(contatos, id, qtdeContatos);
                
                if (indexIdContatoQueSeraExcluido >= 0) {
                
                    FILE *novoArquivo = fopen(CAMINHO_ARQUIVO, "w");
            
                    if (novoArquivo != NULL) {

                        int novaQtdeContatos = qtdeContatos - 1;
                        fprintf(novoArquivo, "%i", novaQtdeContatos);

                        if (novaQtdeContatos > 0) {
                                            
                            int i;

                            for (i = 0; i < qtdeContatos; i++) {

                                if (i != indexIdContatoQueSeraExcluido) {
                                    salvaContatoNoArquivo(novoArquivo, contatos[i]);
                                }
                            }
                                                   
                        }

                        printf("\nContato excluído com sucesso!\n");

                        fclose(novoArquivo);
                    } else {
                        mostraMensagemDeErroAoTentarAbrirArquivo();            
                    }
                    
                    
                } else {
                    printf("\nFalha! O Id informado não existe.\n");
                }

                free(contatos);

            }
        } else {
            mostraMensagemQueNaoExisteNenhumContato();
        }

    } else {
        mostraMensagemDeErroAoTentarAbrirArquivo();
    }
    
}

void listaTodosContatos() {
    
    FILE *arquivo = fopen(CAMINHO_ARQUIVO, "r");
      
    if (arquivo != NULL) {

        int qtdeContatos;
        fscanf(arquivo, "%i", &qtdeContatos);

        if (qtdeContatos > 0) {

            Contato *contatos = obtemTodosContatos(qtdeContatos);

            int i;
            
            for (i = 0; i < qtdeContatos; i++) {
                printf(" ** CONTATO %i **\n\n", i + 1);
                printf(" Id: %i \n Nome completo: %s %s\n Email: %s\n Telefone: %ld\n\n", contatos[i].id, contatos[i].nome, contatos[i].sobrenome, contatos[i].email, contatos[i].telefone);
            
            }

            free(contatos);

        } else {
            mostraMensagemQueNaoExisteNenhumContato();
        }

        fclose(arquivo);
        
    } else {
        mostraMensagemDeErroAoTentarAbrirArquivo();
    }
}

Contato obtemNovoContato() {
   
    Contato contato;

    printf("Nome (máx. 20 caracteres): ");
    scanf("%s", contato.nome);
    fflush(stdin);

    printf("Sobrenome (máx. 30 caracteres): ");
    scanf("%s", contato.sobrenome);
    fflush(stdin);


    printf("Email (máx. 100 caracteres): ");
    scanf("%s", contato.email);
    fflush(stdin);
    
    bool excedeCaracteres = true;

    do {
    printf("Telefone (máx. 9 caracteres): ");
    scanf("%ld", &contato.telefone);
        if (contato.telefone <= 999999999) {
            excedeCaracteres = false;
        } else {
            printf("\nVocê excedeu o limite máximo de caracteres permitido para o campo telefone. Por favor digite novamente.\n\n");
        }
    } while (excedeCaracteres == true);

    printf("\n");

    return contato;

}

int obtemPosicaoIdEscolhido(Contato *contatos, int id, int quantidadeContatos) {
    
    int i;
    
    for (i = 0; i < quantidadeContatos; i++) {
            if (contatos[i].id == id) {
                return i;
            }
    }
    return -1;
}

void salvaContatoNoArquivo(FILE *arquivo, Contato contato) {

     fprintf(arquivo, "\n%i %s %s %s %ld", contato.id, contato.nome, contato.sobrenome, contato.email, contato.telefone);

}

void criaArquivoInicial() {

    if (access( CAMINHO_ARQUIVO, F_OK ) != 0) {
        FILE *novoArquivo = fopen(CAMINHO_ARQUIVO, "w");
        if (novoArquivo != NULL) {
            int qtdeContatos = 0;
            fprintf(novoArquivo, "%i", qtdeContatos);
            fclose(novoArquivo);
        } else {
            mostraMensagemDeErroAoTentarAbrirArquivo();
        }
    }
}

void mostraMensagemDeErroAoTentarAbrirArquivo() {
    printf("Erro! Não foi possível abrir o arquivo '%s'.\nTente executar o comando novamente ou verificar se o arquivo existe.\n", CAMINHO_ARQUIVO);
}

void mostraMensagemQueNaoExisteNenhumContato() {
    printf("Não há contatos salvos. Você deve primeiro incluir um contato em sua agenda para executar esse comando.\n");
}

Contato *obtemTodosContatos(int qtdeContatos) {
   
    FILE *arquivo = fopen(CAMINHO_ARQUIVO, "r");
    
    if (arquivo != NULL) {

        fscanf(arquivo, "%i", &qtdeContatos);
        
        Contato *contatos = malloc(sizeof(Contato) * qtdeContatos);

        int i;

        for (i = 0; i < qtdeContatos; i++) {
            fscanf(arquivo, "%i %s %s %s %ld", &contatos[i].id, contatos[i].nome, contatos[i].sobrenome, contatos[i].email, &contatos[i].telefone);
        }

        fclose(arquivo);
        return contatos;

    } else {
        mostraMensagemDeErroAoTentarAbrirArquivo();
        return NULL;
    }

    
}