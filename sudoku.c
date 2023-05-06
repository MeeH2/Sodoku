/**
 * date: 9/19/2020
 * Authors: Alexander Lyubomirov, Thu Thach
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef int bool;
#define TRUE 1
#define FALSE 0

bool validColumns[9] = {FALSE};
bool validRows[9] = {FALSE};
bool validSubgrids[9] = {FALSE};

struct range{
    int topRow; //index (0, 1, …, or 8) of top row to be checked by a worker thread
    int bottomRow; //index (0, 1, …, or 8) of bottom row to be checked by a worker thread
    int leftColumn; //index (0, 1, …, or 8) of left column to be checked by a worker thread
    int rightColumn; //index (0, 1, …, or 8) of right column to be checked by a worker thread
};

//Sudoku puzzle
int sudokuPuzzle[9][9];

//compare for quick sort
int compare( const void* a, const void* b){
     int int_a = * ( (int*) a );
     int int_b = * ( (int*) b );

     if ( int_a == int_b ) return 0;
     else if ( int_a < int_b ) return -1;
     else return 1;
}

// evaluate range for repetition
void *isRangeValid(int tRow, int bRow, int lCol, int rCol) {
    int arr[9];
    int i,j,k = 0;
    for(i=lCol; i<=rCol; i++) {
        for(j=tRow; j<=bRow; j++) {
            arr[k++] = sudokuPuzzle[j][i];
        }
    }
    qsort( arr, 9, sizeof(int), compare );
    
    char threadName [sizeof(unsigned long)*8+1];
    sprintf(threadName, "%lX", pthread_self());
    
    for(i=0; i<9; i++) {
        if (arr[i] != i+1){
            printf("%s TRow: %d, BRow: %d, LCol:%d, RCol: %d invalid!\n",
            threadName, tRow, bRow, lCol, rCol);
            pthread_exit(NULL);
        }
    }

    printf("%s TRow: %d, BRow: %d, LCol:%d, RCol: %d valid!\n",
    threadName, tRow, bRow, lCol, rCol);
}

void *isColValid(void* input) {
    struct range *rng = (struct range *)input;
    isRangeValid(rng->topRow, rng->bottomRow, rng->leftColumn, rng->rightColumn);
    validColumns[rng->leftColumn] = TRUE;
}

void *isRowValid(void* input) {
    struct range *rng = (struct range *)input;
    isRangeValid(rng->topRow, rng->bottomRow, rng->leftColumn, rng->rightColumn);
    validRows[rng->topRow] = TRUE;
}

void *isSubgridValid(void* input) {
    struct range *rng = (struct range *)input;
    isRangeValid(rng->topRow, rng->bottomRow, rng->leftColumn, rng->rightColumn);
    validSubgrids[rng->topRow+rng->leftColumn/3] = TRUE;
}

int main() {
    
    //read file "SudokuPuzzle.txt"
    FILE * fp;
    fp = fopen("SudokuPuzzle.txt","r");
    
    //save file in sudokuPuzzle[][]
    int i,j;
    for(i=0; i<9; i++) {
        printf("\n");
        for(j=0; j<9; j++) {
            fscanf(fp,"\t\n%d",&sudokuPuzzle[i][j]);
            printf("%d\t",sudokuPuzzle[i][j]);
        }
    }
    printf("\n");
    
    fclose(fp);
    
    //start Columns worker threads
    struct range columns[9];
    pthread_t tid_column[9];
    
    for(i=0; i<9; i++) {
        columns[i].topRow = 0;
        columns[i].bottomRow = 8;
        columns[i].leftColumn = i;
        columns[i].rightColumn = i;
        pthread_create(&tid_column[i], NULL, isColValid, (void *)&columns[i]);
    }
    
    //start Rows worker threads
    struct range rows[9];
    pthread_t tid_row[9];
    
    for(i=0; i<9; i++) {
        rows[i].topRow = i;
        rows[i].bottomRow = i;
        rows[i].leftColumn = 0;
        rows[i].rightColumn = 8;
        pthread_create(&tid_row[i], NULL, isRowValid, (void *)&rows[i]);
    }
	
	//start Subgrids worker threads
    struct range subgrids[9];
    pthread_t tid_subgrid[9];
    
    for(i=0; i<9; i++) {
        subgrids[i].topRow = i/3*3;
        subgrids[i].bottomRow = i/3*3+2;
        subgrids[i].leftColumn = i%3*3;
        subgrids[i].rightColumn = i%3*3+2;
        pthread_create(&tid_subgrid[i], NULL, isSubgridValid, (void *)&subgrids[i]);
    }
    
    //join all the threads
	for (i = 0; i < 9; i++) {
	    pthread_join(tid_column[i], NULL);
	    pthread_join(tid_row[i], NULL);
		pthread_join(tid_subgrid[i], NULL);
	}
	
	//print boolean arrays
	char threadName [sizeof(unsigned long)*8+1];
	bool sudokuCorrect = TRUE;
	
	for (i = 0; i < 3; i++) {
	    for (j = 0; j < 9; j++) {
	        if(i == 0){
	            sprintf(threadName, "%lX", tid_column[j]);
	            printf("Column: %s %s\n",
                threadName, (validColumns[j] == FALSE)?"invalid":"valid");
                if (validColumns[j] == FALSE)
                    sudokuCorrect = FALSE;
	        } else if(i == 1){
	            sprintf(threadName, "%lX", tid_row[j]);
	            printf("Row: %s %s\n",
                threadName, (validRows[j] == FALSE)?"invalid":"valid");
                if (validRows[j] == FALSE)
                    sudokuCorrect = FALSE;
	        } else{
	            sprintf(threadName, "%lX", tid_subgrid[j]);
	            printf("Subgrid: %s %s\n",
                threadName, (validSubgrids[j] == FALSE)?"invalid":"valid");
                if (validSubgrids[j] == FALSE)
                    sudokuCorrect = FALSE;
	        }
	    }
	}
	
	printf("Sudoku Puzzle: %s\n",(sudokuCorrect == FALSE)?"invalid":"valid");
	return EXIT_SUCCESS;
}