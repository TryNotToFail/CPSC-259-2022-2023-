#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine.h"

#define BUFSIZE 256
#define text "web.txt"

int matrixdimension(FILE*);
char** read(FILE*, int);

int main(void) {
    /* Variables */
    int dimension = 0;
    int err = 0;
    char** connectivitymatrix = NULL;
    char buffer[BUFSIZE + 1];
    double temp[BUFSIZE][BUFSIZE]; 
    double one_matrix[BUFSIZE];
    int index = 0;

    FILE* file = NULL;
    Engine* ep = NULL; // A pointer to a MATLAB engine object
    mxArray* MATLABmatrix = NULL, * result = NULL; // mxArray is the fundamental type underlying MATLAB data

    //open file 
    if (err = fopen_s(&file, text, "r")) {
        printf("Error opening the file\n");
        system("pause");
        return 1;
    }
    else printf("The file is open\n");

    dimension = matrixdimension(file);

    if (dimension > 0) {
        connectivitymatrix = read(file, dimension);
        for (int row = 0; row < dimension; row++) {
            for (int column = 0; column < dimension; column++) {
                temp[row][column] = (double)connectivitymatrix[row][column];
            }
        }
    }
    else printf("Error getting the dimension and copying connectivity matrix\n");

    if (file != NULL) {
        printf("File closed \n");
        fclose(file);
    }

    /* Starts a MATLAB process */
    if (!(ep = engOpen(NULL))) {
        fprintf(stderr, "\nCan't start MATLAB engine\n");
        system("pause");
        return 1;
    }
    printf("Start MATLAB engine\n");

    //Put connectivitymatrix into MATLAB
    MATLABmatrix = mxCreateDoubleMatrix(dimension, dimension, mxREAL);

    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            one_matrix[index] = temp[j][i];
            index++;
        }
    }

    //copy array into MATLAB
    memcpy((void*)mxGetPr(MATLABmatrix), (void*)one_matrix, (dimension * dimension) * sizeof(double));
    
    //implement the 14 MATLAB code

    //declare a matrix
    if (engPutVariable(ep, "MATLABmatrix", MATLABmatrix)) {
        fprintf(stderr, "\nCannot write test array to MATLAB \n");
        system("pause");
        exit(1);
    }

    //find rows and columns
    if (engEvalString(ep, "[rows, columns] = size(MATLABmatrix)")) {
        fprintf(stderr, "\nCannot calculate the rows and columns of the MATLABmatrix\n");
        system("pause");
        exit(1);
    }

    //find dimension 
    if (engEvalString(ep, "dimension = size(MATLABmatrix, 1)")) {
        fprintf(stderr, "\nCannot calculate the dimension of the MATLABmatrix\n");
        system("pause");
        exit(1);
    }

    //find the columnsum
    if (engEvalString(ep, "columnsums = sum(MATLABmatrix, 1)")) {
        fprintf(stderr, "\nCannot calculate the columnsum of MATLABmatrix\n");
        system("pause");
        exit(1);
    }

    //input the p value 
    if (engEvalString(ep, "p = 0.85")) {
        fprintf(stderr, "\nCannot input the p value\n");
        system("pause");
        exit(1);
    }

    //find zerocolumn
    if (engEvalString(ep, "zerocolumns = find(columnsums~=0)")) {
        fprintf(stderr, "\nCannot find the zerocolumns\n");
        system("pause");
        exit(1);
    }

    //calculate D sparse of the MATLABmatrix
    if (engEvalString(ep, "D = sparse( zerocolumns, zerocolumns, 1./columnsums(zerocolumns), dimension, dimension)")) {
        fprintf(stderr, "\nCannot calculate D sparse of MATLABmatrix\n");
        system("pause");
        exit(1);
    }

    //calculate the stochastic matrix
    if (engEvalString(ep, "StochasticMatrix = MATLABmatrix * D")) {
        fprintf(stderr, "\nCannot calculate the Stochasitc matrix\n");
        system("pause");
        exit(1);
    }

    //find the zero columns 
    if (engEvalString(ep, "[row, column] = find(columnsums==0)")) {
        fprintf(stderr, "\nCannot find the zero columns of MATLAB matrix\n");
        system("pause");
        exit(1);
    }

    //generate the Stochastic matrix
    if (engEvalString(ep, "StochasticMatrix(:, column) = 1./dimension")) {
        fprintf(stderr, "\nCannot generate the Stochastic matrix\n");
        system("pause");
        exit(1);
    }

    //calculate the Q value
    if (engEvalString(ep, "Q = ones(dimension, dimension)")) {
    fprintf(stderr, "\nCannot calculate the value Q\n");
    system("pause");
    exit(1);
    }

    //generate the transition matrix
    if (engEvalString(ep, "TransitionMatrix = p * StochasticMatrix + (1 - p) * (Q/dimension)")) {
        fprintf(stderr, "\nCannot generate the transition matrix\n");
        system("pause");
        exit(1);
    }

    //calculate pagerank value
    if (engEvalString(ep, "PageRank = ones(dimension, 1)")) {
        fprintf(stderr, "\n Cannot calculate the pagerank\n");
        system("pause");
        exit(1);
    }

    //the for loop
    if (engEvalString(ep, "for i = 1:100 PageRank = TransitionMatrix * PageRank; end")) {
        fprintf(stderr, "\n Error the for loop\n");
        system("pause");
        exit(1);
    }

    //calculate the final pagerank 
    if (engEvalString(ep, "PageRank = PageRank/sum(PageRank)")) {
        fprintf(stderr, "\n Cannot calculate the final value for PageRank\n");
        system("pause");
        exit(1);
    }

    printf("\nRetrieving Page Ranks \n");

    if ((result = engGetVariable(ep, "PageRank")) == NULL) {
        fprintf(stderr, "\n Cannnot retrieve the result \n");
        system("pause");
        exit(1);
    }
    else {
        size_t sizeOfResult = mxGetNumberOfElements(result);
        size_t i = 0;
        printf("\nNODE     RANK\n");
        printf("---      ----\n");
        for (i = 0; i < sizeOfResult; ++i) {
            printf("%d        ", i+1);
            printf("%f\n", *(mxGetPr(result) + i));
        }
        printf("\n");
    }

    if (engOutputBuffer(ep, buffer, BUFSIZE)) {
        fprintf(stderr, "\n Cannnot create buffer for MATLAB output \n");
        system("pause");
        return 1;
    }
    buffer[BUFSIZE] = '\0';

    mxDestroyArray(MATLABmatrix);
    mxDestroyArray(result);
    MATLABmatrix = NULL;
    result = NULL;
    if (engClose(ep)) {
        fprintf(stderr, "\nFailed to close MATLAB engine\n");
    }
    system("pause");
    return 0;
}

int matrixdimension(FILE* file_pointer) {

    int  dimension = 0;
    char line_buffer[BUFSIZE];

    dimension = strlen(fgets(line_buffer, BUFSIZE, file_pointer));

    /* You don't need to know this. It 'resets' the file's internal pointer to the
       beginning of the file. */
    fseek(file_pointer, 0, SEEK_SET);

    /* Checks if text file was created in Windows and contains '\r'
       IF TRUE reduce strlen by 2 in order to omit '\r' and '\n' from each line
       ELSE    reduce strlen by 1 in order to omit '\n' from each line */
    if (strchr(line_buffer, '\r') != NULL) {
        dimension -= 2;
        // return ...
        return (dimension / 2 + 1);		//
    }
    else {
        dimension--;
        // return ...
        return (dimension / 2 + 1);			//
    }
}

char** read(FILE* file_pointer, int dimension){
    /* Variables */
    char         line_buffer[BUFSIZE];
    int          row = 0;
    int 	   column = 0;

    /* Allocates memory for correctly-sized maze */
    char **connectivity_matrix = (char**)calloc(dimension, sizeof(char));


    for (row = 0; row < dimension; ++row) {
        connectivity_matrix[row] = (char*)calloc(dimension, sizeof(char));
    }

    row = 0;
    while (fgets(line_buffer, BUFSIZE, file_pointer)) {
        for (column = 0; column < dimension; ++column) {
            //allows for spaces in txt file
            connectivity_matrix[row][column] = line_buffer[column * 2] - '0';
        }
        row++;
    }
    return connectivity_matrix;
}
