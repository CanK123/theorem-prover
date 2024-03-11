#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BUFFER_SIZE  256

char** R3(char**, char*, int*, int*);
int checkAxiom(char*);
char* formatString(char*, int);
void R5(char*, char*, int*, int*);
void R2(char*, char*, int*, int*);
void freeArr(char**, int);
int main(void){

    //Initializing a buffer to get input string
    char *string = (char *)malloc(sizeof(char)*BUFFER_SIZE); 
    
    //Getting string from stdin
    scanf("%s", string);
    
    //Evaluating string length and managing memory according to it
    int length = strlen(string);
    string = (char *)realloc(string, sizeof(char)*(length+1));

    
    //Formating the string according to R1 and R5
    char *fString = formatString(string, length);
    if(checkAxiom(fString)){
        free(fString);
        free(string);
        printf("\n%c\n",'t');
        return 0;
    }


    int arrSize = 2;
    int inUse = 0;

    //Allocating memory for the branching sequents array
    char **SeqArr = calloc(arrSize,sizeof(char *)); 

    

    SeqArr = R3(SeqArr, fString, &arrSize, &inUse);
    free(fString);
    free(string);

    while (SeqArr[0] != NULL && inUse != 0){
        string = SeqArr[inUse-1];
        int ptrNumBefore = inUse;
        SeqArr = R3(SeqArr, string, &arrSize, &inUse);
        if(inUse == ptrNumBefore){
            string = SeqArr[inUse-1];
            fString = formatString(string, strlen(string));
            if(strcmp(fString, string) == 1){
                freeArr(SeqArr, inUse);
                free(fString);
                free(string);
                printf("\n%c\n", 'f');
                return 0;
            }
            else{
                free(string);
                if(checkAxiom(fString)){
                    free(fString);
                    inUse--;
                }
                else{
                    SeqArr[inUse-1] = fString;
                }
            }
        }
    }
    
    printf("\n%c\n",'t');
    return 0;
}


char** R3(char **SeqArr, char *substring, int *arrSize, int *inUse){
    int i = 0;
    char* sequent = (char *)malloc(sizeof(char)*BUFFER_SIZE);
    char* sequent2 = (char *)malloc(sizeof(char)*BUFFER_SIZE);

    if(*inUse == *arrSize){
        (*arrSize) = (*arrSize) * 2;
        SeqArr = (char **)realloc(SeqArr, sizeof(char *)*(*arrSize));
    }
    while (substring[i] != '#'){
        if(substring[i] == '|'){
            
            //Making the first brunch
            if(substring[i-2] == '-'){
            strncpy(sequent, substring, i-3);
            sequent[i-3] = '-';
            sequent[i-2] = substring[i-1];
            strcpy(sequent+i-1, substring+i+3);
            }
            else{
                strncpy(sequent, substring, i-3);
                sequent[i-3] = substring[i-1];
                strcpy(sequent+i-2, substring+i+3);
            }
            //Making the second brunch
            if(substring[i+1] == '-'){
            strncpy(sequent2, substring, i-3);
            sequent2[i-3] = '-';
            sequent2[i-2] = substring[i-1];
            strcpy(sequent2+i-1, substring+i+3);
            }
            else{
                strncpy(sequent2, substring, i-3);
                sequent2[i-3] = substring[i+1];
                strcpy(sequent2+i-2, substring+i+3);
            }
            break;
        }
        i++;
    }

    //Freeing the memory if they are not in use
    if(strlen(sequent) == 0){
        free(sequent);
        free(sequent2);
        return SeqArr;
    }

    //Freeing the memory from the stack in case it is from stack
    if(*inUse != 0){
        free(SeqArr[--(*inUse)]);
        SeqArr[*inUse] = NULL;
    }
    //Reallocating the memory for the sequents
    sequent = (char *)realloc(sequent, sizeof(char)*(strlen(sequent)+1));
    sequent2 = (char *)realloc(sequent2, sizeof(char)*(strlen(sequent2)+1));


    //Checking if the sequents are axiom
    if(checkAxiom(sequent) == 1){
        free(sequent);
    }
    else{
        SeqArr[(*inUse)++] = sequent;
    }
    if(checkAxiom(sequent2) == 1){
        free(sequent2);
    }
    else{
        SeqArr[(*inUse)++] = sequent2;
    }



    return SeqArr;
}

int checkAxiom(char *substring){
    int arrSize = 4;
    char* lArr = (char *)calloc(arrSize, sizeof(char));
    char* rArr = (char *)calloc(arrSize, sizeof(char));
    
    //Going through left side and storing free variables
    int i = 0;
    while(substring[i] != '#'){
        if(substring[i] == ',' && substring[i-1] != ')'){
            if(i-2 >= 0 && substring[i-2] == '-'){
                if(strlen(lArr)+1 == arrSize){
                    arrSize *= 2;
                    lArr = (char *)realloc(lArr, sizeof(char)*arrSize);
                }
            if(strlen(lArr)+1 == arrSize){
                arrSize *= 2;
                lArr = (char *)realloc(lArr, sizeof(char)*arrSize);
            }
                lArr[strlen(lArr)] = '-';
            }
            lArr[strlen(lArr)] = substring[i-1];
        }
        else if((substring[i+1] == '#' && substring[i-1] != ')')){
            if(i-2 >= 0 && substring[i-2] == '-'){
                if(strlen(lArr)+1 == arrSize){
                    arrSize *= 2;
                    lArr = (char *)realloc(lArr, sizeof(char)*arrSize);
                }
            if(strlen(lArr)+1 == arrSize){
                arrSize *= 2;
                lArr = (char *)realloc(lArr, sizeof(char)*arrSize);
            }
                lArr[strlen(lArr)] = '-';
            }
            lArr[strlen(lArr)] = substring[i];
        }
        i++;
    }
    //Going through right side and storing free variables
    while(substring[i] != '\0'){
        if(substring[i] == ',' && substring[i-1] != ')'){
            if(i-2 >= 0 && substring[i-2] == '-'){
                 if(strlen(rArr)+1 == arrSize){
                    arrSize *= 2;
                    rArr = (char *)realloc(rArr, sizeof(char)*arrSize);
                }
            if(strlen(rArr)+1 == arrSize){
                arrSize *= 2;
                rArr = (char *)realloc(rArr, sizeof(char)*arrSize);
            }
                rArr[strlen(rArr)] = '-';
            }
            rArr[strlen(rArr)] = substring[i-1];
        }
        i++;
    }
    rArr[strlen(rArr)] = substring[i-1];
    
    //Checking if the axiom is valid
    for(int i = 0; i < strlen(lArr); i++){
        int j = 0;
        if(lArr[i] == '-'){
            i++;
        }
            while(j < strlen(rArr)){
                if(lArr[i] == rArr[j]){
                    if(i != 0 && lArr[i-1] == '-'){
                        if(j == 0){
                            j++;
                        }
                        else if (rArr[j-1] == '-'){
                            free(lArr);
                            free(rArr);
                            return 1;
                        }
                        else{
                            j++;
                        }
                    }
                    else if (i == 0 && rArr[j-1] == '-'){
                        j++;
                    }
                    else{
                        free(lArr);
                        free(rArr);
                        return 1;
                    }
                    
                }
            j++;
            }
        
    }
    free(lArr);
    free(rArr);
    return 0;
}

char* formatString(char* string, int length){
    //Allocatin a memory for the formated string
    char *fString = (char *)malloc(sizeof(char)*(length+8));
    //Reading through left premises
    int i = 0;
    int j = 0;
    int arrSize = 2;
    char *R1Arr = (char *)calloc(arrSize, sizeof(char));
    while(string[i] != '#'){
        if (string[i] == '>'){
                R5(string, fString, &i, &j);
                i++;
            }
        else if (string[i] == '-' && (i == 0 || string[i-1] == ',')){
            fString[--j] = '\0';
            if(strlen(R1Arr)+1 == arrSize){
                arrSize *= 2;
                R1Arr = (char *)realloc(R1Arr, sizeof(char)*arrSize);
            }
            char len = strlen(R1Arr);
            R1Arr[len] = string[++i];
            R1Arr[len+2] = '\0';
            i++;
        }
        else if(string[i] == '&'){
            R2(string, fString, &i, &j);
            i++;
        }
        else{
            fString[j] = string[i];
            i++;
            j++;
        }
    }

    //Reading through right premises
    while(i < length){
        if (string[i] == '>'){
            R5(string, fString, &i, &j);
            i++;
        }
        else if(string[i] == '|'){
            R2(string, fString, &i, &j);
            i++;
        }
        else{
            fString[j] = string[i];
            i++;
            j++;
        }
    }
    //Adding R1 products to the formated string;
    for(int i = 0; i < strlen(R1Arr); i++){
        fString[j++] = ',';
        fString[j++] = R1Arr[i];
    }
    fString[j] = '\0';    
    
    //Freeing the memory for the R1Arr
    free(R1Arr);
    return fString;
}

void R5(char* string, char *fString, int *i, int *j){
            char temp;
            temp = fString[*j-1];
            if(string[*i-2] == '-'){
                fString[--*j-1] = temp;
            }
            else{
            fString[(*j)-1] = '-';
            fString[(*j)++] = temp;
            }
            fString[(*j)++] = '|';
}

void R2(char* string, char *fString, int *i, int *j){
    if(string[(*i)-2] == '('){
        fString[(*j)-2] = fString[(*j)-1];
        (*j)--;
        fString[(*j)++] = ',';
        if(string[(*i)+2] != ')'){
            fString[(*j)++] = '(';
            fString[(*j)++] = string[++(*i)];
            return;
        }
        fString[(*j)++] = string[++(*i)];
        (*i)++;
        return;
    } 
    else{
        fString[(*j)++] = ')';
        fString[(*j)++] = ',';
        fString[(*j)++] = string[++(*i)];
        (*i)++;
        return; 
    }
}

void freeArr(char** seqArr, int inUse){
    for(int i = 0; i < inUse; i++){
        free(seqArr[i]);
    }
    free(seqArr);
}