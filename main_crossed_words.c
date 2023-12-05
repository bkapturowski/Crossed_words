#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Program, który pobiera od użytkownika cztery wyrazy, a następnie tworzy z nich dwa krzyże pojedyncze i jeden krzyż podwójny.
//
//W tym celu stworzone są funkcje, które będą: tworzyły pojedynczy krzyż z dwóch słów przekazanych w parametrach, będą tworzyły podwójny krzyż ze słów przekazanych w parametrach oraz funkcję do zwalniania krzyża.

// Funkcja create_leading_word_cross tworzy krzyż ze słów first oraz second w taki sposób, że pierwszy wyraz ma być ustawiony pionowo, a drugi poziomo.
// Mają się one krzyżować na pierwszej wspólnej literze wyrazu poziomego.
// Wyrazy mogą składać się tylko i wyłącznie z dużych liter.

// Funkcja create_double_leading_word_cross tworzy krzyże ze słów first i second oraz third i fourth i zapisuje je odpowiednio do zmiennych first_cross i second_cross w taki sam sposób jak w przypadku funkcji create_leading_word_cross, ale wyrazy muszą być wykorzystane w odwrotny sposób.
// Wyrazy first i third muszą być ułożone w poziomie natomiast wyrazy second oraz fourth w pionie.
// Dodatkowo funkcja tworzy podwójny krzyż z przekazanych do niej słów, łącząc dwa pojedyncze krzyże, w taki sposób, że wyrazy poziome znajdują się w tym samym wierszu.

//Funkcje alokują tylko i wyłącznie tyle pamięci ile będzie potrzebne do przechowywania utworzonych krzyży.
//
//Przykład1:MASK, CAT, WELFARE oraz DUFFY (podaj słowa po spacji)
//Przyklad2: IECE, WEOE

void destroy(char **words){
    if (words == NULL) {
        return;
    }
    for (int i = 0; *(words+i) != NULL; i++) {
        free(*(words+i));
    }

    free(words);
}


int create_leading_word_cross(const char *first, const char *second, char ***result){

    if(first == NULL || second == NULL || result == NULL){
        return -1;
    }

    unsigned int i;
    for(i=0;*(second+i)!='\0';i++){
        if(!isupper(*(second+i))){
            *result = NULL;
            return -1;
        }
    }
    for(i=0;*(first+i)!='\0';i++){
        if(!isupper(*(first+i))){
            *result = NULL;
            return -1;
        }
    }

    unsigned int height,width;
    int flag = 0;

    height = 0;
    while (height < strlen(second) && flag == 0){
        width = 0;
        while (width< strlen(first) && flag == 0){
            if(*(second+height) == *(first+width))
                flag = 1;
            width++;
        }
        height++;
    }


    if(flag == 0) {
        *result = NULL;
        return -2;
    }
    height--,width--;

    *result = calloc(strlen(first)+1, sizeof(char*));
    if(*result == NULL){
        return -3;
    }

    for(i = 0; i < strlen(first);i++){
        unsigned int size;
        if(i == width){
            size = strlen(second)+1;
        }
        else{
            size = height+2;
        }

        *(*result+i) = calloc(size, sizeof(char));
        if(*(*result+i) == NULL){
            destroy(*result);
            *result = NULL;
            return -3;
        }
    }

    unsigned int j;
    for(i=0; i < strlen(first); i++){
        if(i == width)
        {
            strcpy(*(*result+i), second);
        }
        else
        {
            for(j=0;j<height;j++){
                *(*(*result+i)+j) = ' ';
            }
            *(*(*result+i)+j)= *(first+i);
            j++;
            *(*(*result+i)+j) = '\0';
        }
    }

    return (int)width;
}

int create_double_leading_word_cross(const char *first, const char *second, const char *third, const char *fourth,
                                     char ***result, char ***first_cross, char ***second_cross){

    if(first == NULL || second == NULL || third == NULL || fourth == NULL || result == NULL || first_cross == NULL || second_cross == NULL){
        return 1;
    }

    *result = *first_cross = *second_cross = NULL;

    int cross_first = create_leading_word_cross(second,first,first_cross);
    if(cross_first == -1){
        return 1;
    }
    if(cross_first == -2){
        return 2;
    }
    if(cross_first == -3){
        return 3;
    }

    int cross_second = create_leading_word_cross(fourth,third,second_cross);
    if(cross_second == -1){
        destroy(*first_cross);
        *result = NULL;
        *first_cross = NULL;
        *second_cross = NULL;
        return 1;
    }
    if(cross_second == -2){
        destroy(*first_cross);
        *result = NULL;
        *first_cross = NULL;
        *second_cross = NULL;
        return 2;
    }
    if(cross_second == -3){
        destroy(*first_cross);
        *result = NULL;
        *first_cross = NULL;
        *second_cross = NULL;
        return 3;
    }

    unsigned int size,temp;
    if((strlen(second) - cross_first) > (strlen(fourth) - cross_second))
    {
        temp = strlen(second) - cross_first;
    }
    else
    {
        temp = strlen(fourth) - cross_second;
    }

    if (cross_first > cross_second)
    {
        size = cross_first + 1 + temp;
    }
    else
    {
        size = cross_second + 1 + temp;
    }


    *result = calloc(size, sizeof(char*));
    if(*result == NULL){
        destroy(*first_cross);
        destroy(*second_cross);
        *result = NULL;
        *first_cross = NULL;
        *second_cross = NULL;
        return 3;
    }

    unsigned int begin1;
    if(cross_second > cross_first){
        begin1 = cross_second-cross_first;
    }
    else{
        begin1 = 0;
    }
    unsigned int begin2;
    if(cross_first > cross_second){
        begin2 = cross_first-cross_second;
    }
    else{
        begin2 = 0;
    }


    unsigned int end1 = strlen(second) + begin1 - 1;
    unsigned int end2 = strlen(fourth) + begin2 - 1;


    unsigned int cross;
    if(cross_second > cross_first){
        cross = cross_second;
    }
    else{
        cross = cross_first;
    }


    unsigned int i;
    for(i=0; i < size - 1;i++){

        unsigned int row_size;

        if(i > cross) {
            if (end2 < end1) {
                if(i<end2+1){
                    row_size = strlen(first) + 3 + strlen(*(*second_cross + i - begin2)) + 1;
                }
                else{
                    row_size = strlen( *(*first_cross + i - begin1)) +1;
                }
            }
            else {
                row_size = strlen(first) + 3 + strlen(*(*second_cross + i - begin2)) + 1;
            }
        }

        if(i < cross)
        {
            if(begin1 == 0)
            {
                if(i < begin2){
                    row_size = strlen(*(*first_cross+i)) + 1;
                }
                else{
                    row_size = strlen(first) + 3 + strlen(*(*second_cross +i - begin2)) +1;
                }
            }
            else
            {
                row_size = strlen(first) + 3 + strlen(*(*second_cross+i)) + 1;
            }
        }

        if(i == cross)
        {
            row_size = strlen(first) + 3 + strlen(third) + 1;
        }

        *(*result+i) = calloc(row_size, sizeof(char));

        if(*(*result+i) == NULL)
        {
            destroy(*first_cross);
            destroy(*second_cross);
            destroy(*result);
            *result = NULL;
            *first_cross = NULL;
            *second_cross = NULL;
            return 3;
        }


        if(i > cross){
            if(end2 < end1){
                if(i < end2+1){
                    strcpy(*(*result+i),*(*first_cross+i-begin1));
                    unsigned int j = 0;
                    while (j < strlen(first) - strlen(*(*first_cross + i - begin1))) {
                        strcat(*(*result + i), " ");
                        j++;
                    }
                    strcat(*(*result+i),"   ");
                    strcat(*(*result+i), *(*second_cross + i - begin2));
                }
                else{
                    strcpy(*(*result+i),*(*first_cross + i - begin1));
                }
            }
            else{
                if(i < end1+1){
                    strcpy(*(*result+i),*(*first_cross+i-begin1));
                    unsigned int j = 0;
                    while (j < strlen(first) - strlen(*(*first_cross + i - begin1))) {
                        strcat(*(*result + i), " ");
                        j++;
                    }
                    strcat(*(*result+i),"   ");
                    strcat(*(*result+i), *(*second_cross + i - begin2));
                }
                else{
                    unsigned int j = 0;
                    while (j < strlen(first)) {
                        strcat(*(*result + i), " ");
                        j++;
                    }
                    strcat(*(*result+i), "   ");
                    strcat(*(*result+i), *(*second_cross + i - begin2));
                }
            }
        }

        if(i < cross){
            if(begin1 == 0){
                if(i < begin2){
                    strcpy(*(*result+i), *(*first_cross+i));
                }
                else{
                    strcpy(*(*result+i),*(*first_cross+i));
                    unsigned int j = 0;
                    while (j < strlen(first) - strlen(*(*first_cross + i))) {
                        strcat(*(*result + i), " ");
                        j++;
                    }
                    strcat(*(*result+i), "   ");
                    strcat(*(*result+i), *(*second_cross+i-begin2));
                }
            }
            else{
                if(i < begin1){
                    unsigned int j = 0;
                    while (j < strlen(first)) {
                        strcat(*(*result + i), " ");
                        j++;
                    }
                }
                else {
                    strcpy(*(*result + i), *(*first_cross + i - begin1));
                    unsigned int j = 0;
                    while (j < strlen(first) - strlen(*(*first_cross + i - begin1))) {
                        strcat(*(*result + i), " ");
                        j++;
                    }
                }
                strcat(*(*result+i),"   ");
                strcat(*(*result+i), *(*second_cross+i));
            }
        }

        if(i==cross){
            strcpy(*(*result+i),first);
            strcat(*(*result+i), "   ");
            strcat(*(*result+i),third);
        }
    }

    return 0;
}


int main() {

    char *word_1;
    word_1 = malloc(sizeof(char) * 11);
    if(word_1 == NULL){
        printf("Failed to allocate memory");
        return 8;
    }
    char *word_2;
    word_2 = malloc(sizeof(char) * 11);
    if(word_2 == NULL){
        free(word_1);
        printf("Failed to allocate memory");
        return 8;
    }
    char *word_3;
    word_3 = malloc(sizeof(char) * 11);
    if(word_3 == NULL){
        free(word_1);
        free(word_2);
        printf("Failed to allocate memory");
        return 8;
    }
    char *word_4;
    word_4 = malloc(sizeof(char) * 11);
    if(word_4 == NULL){
        free(word_1);
        free(word_2);
        free(word_3);
        printf("Failed to allocate memory");
        return 8;
    }


    printf("Enter words:\n");
    if (scanf("%10s %10s %10s %10s", word_1, word_2, word_3, word_4) != 4) {
        printf("Incorrect input\n");
        free(word_1);
        free(word_2);
        free(word_3);
        free(word_4);
        return 1;
    }

    char **first_cross = NULL;
    char **second_cross = NULL;
    char **double_cross = NULL;
    int result = create_double_leading_word_cross( word_1, word_2, word_3, word_4, &double_cross, &first_cross, &second_cross);

    if (result == 1) {
        printf("Incorrect input\n");
        free(word_1);
        free(word_2);
        free(word_3);
        free(word_4);
        destroy(first_cross);
        destroy(second_cross);
        return 1;
    }
    if (result == 2) {
        printf("Unable to make two crosses\n");
        free(word_1);
        free(word_2);
        free(word_3);
        free(word_4);
        destroy(first_cross);
        destroy(second_cross);
        return 0;
    }
    if (result == 3) {
        printf("Failed to allocate memory\n");
        destroy(first_cross);
        destroy(second_cross);
        free(word_1);
        free(word_2);
        free(word_3);
        free(word_4);
        return 8;
    }

    // Wyświetlanie krzyży
    for (unsigned int i = 0; *(first_cross+i) != NULL ; i++) {
        printf("%s\n", *(first_cross+i));
    }
    printf("\n");
    for (unsigned int i = 0; *(second_cross+i) != NULL ; i++) {
        printf("%s\n", *(second_cross+i));
    }
    printf("\n");
    for (unsigned int i = 0; *(double_cross +i) != NULL; i++) {
        printf("%s\n", *(double_cross+i));
    }

    // Zwolnienie pamięci
    free(word_1);
    free(word_2);
    free(word_3);
    free(word_4);
    destroy(first_cross);
    destroy(second_cross);
    destroy(double_cross);

    return 0;
}

