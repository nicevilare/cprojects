#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "forca.h" //include the header file for the game of hangman, which contains the function prototypes and global variables

// global variables
char palavrasecreta[TAMANHO_PALAVRA]; //array to store the secret word chosen for the game
char chutes[26]; //array to store the letters guessed by the user
int chutesdados = 0; 


void abertura(){
    printf("*********************************\n");
    printf("*       Jogo da Forca           *\n");
    printf("*********************************\n");
}

void chuta(){
    char chute;
    printf("Digite uma letra: ");       
    scanf(" %c", &chute); //read a character from the user, blank space before %c to consume any leftover newline character

    chutes[chutesdados] = chute; //store the guessed letter in the chutes array
    chutesdados++; //

    // for(int i = 0; i < strlen(palavrasecreta); i++){
    //     if(palavrasecreta[i] == chute){
    //         printf("A posição %d tem essa letra!\n", i);
    //         break;
    //     }
    // }
}

int jachutou(char letra){
    int achou = 0;

    for(int j = 0; j < chutesdados; j++){
    //printf("Tentativa %d: %c\n", j, chutes[j]); //print the guessed letters for debugging purposes
        if(chutes[j] == letra){
            achou = 1;
            break;
        }
    }

    return achou;
}

void desenhaforca(){
    for(int i = 0; i < strlen(palavrasecreta); i++){

        //printf("Letra secreta %d = %c\n", i, palavrasecreta[i]); //print the secret word letters for debugging purposes
        int achou = jachutou(palavrasecreta[i]); //call the function to check if the letter has been guessed         

        if(achou){
            printf("%c ", palavrasecreta[i]);
        } else {
            printf("_ ");
        }
    }

    printf("\n");
}

void adicionarpalavra(){
    char quer;
    printf("Você deseja adicionar uma nova palavra ao jogo? (S/N) ");
    scanf(" %c", &quer); //read a character from the user, blank

    if(quer == 'S'){
        char novapalavra[TAMANHO_PALAVRA];

        printf("Qual a nova palavra: ");
        scanf("%s", novapalavra); //read a string from the user and store it in the array palavrasecreta
 
        FILE* f; //declare a file pointer to write the new word to the file

        f = fopen("palavras.txt", "r+"); //open the file with the list of words in read and write mode
        if(f == 0){
            printf("Desculpe, banco de palavras não disponível\n");
            exit(1); //if the file cannot be opened, print an error message and exit the program with a non-zero status
        }

        int qtd;
        fscanf(f, "%d", &qtd); //read the first line of the file, which contains the number of words in the list
        qtd++; //increment the number of words by 1

        fseek(f, 0, SEEK_SET); //move the file pointer to the beginning of the file to overwrite the number of words
        fprintf(f, "%d\n", qtd); //write the new number of words to the file

        fseek(f, 0, SEEK_END); //move the file pointer to the end of the file to append the new word
        fprintf(f, "%d\n%s", qtd, novapalavra); //write the new word to the file, along with the number of words (1 in this case)

        fclose(f); //close the file after writing    
    
    }
}

void escolhepalavra(){
    //sprintf(palavrasecreta, "MELANCIA"); //copy the string "banana" into the array palavrasecreta
    FILE* f; //declare a file pointer to read the list of words from a file
    int qntdepalavras;

    f = fopen("palavras.txt", "r"); //open the file with the list of words in read mode
    if(f == 0){
        printf("Desculpe, banco de palavras não disponível\n");
        exit(1); //if the file cannot be opened, print an error message and exit the program with a non-zero status
    }

    fscanf(f, "%d", &qntdepalavras); //read the first line of the file, which contains the number of words in the list
    
    srand(time(0)); //seed the random number generator with the current time
    int randomico = rand() % qntdepalavras; //generate a random number between 0
    
    for(int i = 0; i <= randomico; i++){
        fscanf(f, "%s", palavrasecreta); //read a word from the file and store it in the array palavrasecreta
    }

    fclose(f); //close the file after reading

}

int ganhou(){
    for(int i = 0; i < strlen(palavrasecreta); i++){
        if(!jachutou(palavrasecreta[i])){
            return 0; //if any letter in the secret word has not been guessed, return 0 (not won)
        }
    }
    return 1; //if all letters in the secret word have been guessed, return 1 (won)
}

int enforcou(){
    int erros = 0;

    for(int i= 0; i < chutesdados; i++){
        int existe = 0;

        for(int j = 0; j < strlen(palavrasecreta); j++){
            if(chutes[i] == palavrasecreta[j]){
                existe = 1; //if any guessed letter matches a letter in the secret word, return 0 (not hanged)
                break;
            }
        }

        if(!existe) erros++; //if the guessed letter does not match any letter in the secret word, increment the errors counter
    }
    return erros >= 5; //if the number of errors is greater than or equal to 5, return 1 (hanged), otherwise return 0 (not hanged)
}

int main(){
    //int notas[10]; //array with 10 positions
    escolhepalavra(); //call the function to choose the secret word and store it in the array palavrasecreta

    abertura();   

    do { 

        desenhaforca(); //call the function to draw the hangman and show the guessed letters

        chuta(); //call the function to get a letter from the user and store it in the chutes array      

    } while(!ganhou() && !enforcou());

    if(ganhou()){
        printf("Parabéns, você ganhou!\n");
    } else {
        printf("Puxa, você foi enforcado!\n");
        printf("A palavra era **%s**\n", palavrasecreta);
    }
    
    adicionarpalavra(); //call the function to ask the user if they want to add a new word to the list and write it to the file if they do
    
}

//example of a function to calculate the power of a number using pointers
// void potencia(int* resultado, int a, int b) {
//     *resultado = 1;
//     for(int i = 0; i < b; i++) {
//         *resultado = *resultado * a;
//     }
// }