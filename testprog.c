#include <stdio.h>
#include <string.h>



int main() {
    char* input[100]; 
    /*input[0] = "Hi";
    input[1] = "Hi2";
    input[2] = "Hi3";
    char* inputcpy[100];
    char** ptr = input;
    ptr++;
    int i = 0;
    while(*ptr != 0) {
        //printf("%s\n", *ptr);
        inputcpy[i++] = *ptr;
        ptr++;
    }
    for(int j = 0; j < 100; j++) {
    printf("%s\n", inputcpy[j]);
    }

    char* arr[2]; 
    arr[0] = "Elem 1";
    arr[1] = "Elem 2";
    printf("Vorher: %s\n", arr[0]);

    shiftArrayPointer(&arr[0]);

    printf("Nachher: %s\n", arr[0]);*/

    
    
    fgets(input, sizeof(input), stdin);

    printf("Gepiped: %s", input);
}