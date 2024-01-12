#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

/* 
@brief Parametreler arasindaki büyük olani belirlemek icin kullanilan fonksiyon

@param int a: Herhangi bir tam sayi
@param int b: Herhangi bir tam sayi

@return int: Eðer a büyükse, o zaman fonksiyon a'yý döndürür. Deðilse (yani, a küçük veya eþitse), fonksiyon b'yi döndürür.
*/

int max(int a, int b) {
    return (a > b) ? a : b;
}

/* 
@brief Matris'in bastirilmasi icin kullanilan fonksiyon

@param int **matrix: Matris
@param int rows: Satirlar
@param int int cols: Sutunlar
*/

void printMatrix(int **matrix, int rows, int cols) {
	int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

/* 
@brief Matris'in bastirilmasi icin kullanilan fonksiyon

@param int **matrix: Matris
@param int rows: Satirlar
@param int int cols: Sutunlar
*/

void printCharMatrix(int **matrix, int rows, int cols) {
	int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%c ", (matrix[i][j] == 1) ? 'X' : '.');
        }
        printf("\n");
    }
}

/* 
@brief Matris'in olusturulmasi icin bellekte yer acan fonksiyon

@param int **dp: Dinamik Programlama Matrisi
@param int ***isChosen: Harf Secim Matrisi
@param int m: Satir Uzunlugu
@param int n: Sutun Uzunlugu
*/

void initializeMatrices(int ***dp, int ***isChosen, int m, int n) {
	int i, j;
    *dp = (int **)malloc((m + 1) * sizeof(int *));
    *isChosen = (int **)malloc((m + 1) * sizeof(int *));

    for (i = 0; i <= m; i++) {
        (*dp)[i] = (int *)malloc((n + 1) * sizeof(int));
        (*isChosen)[i] = (int *)malloc((n + 1) * sizeof(int));
    }

    for (i = 0; i <= m; i++) {
        for (j = 0; j <= n; j++) {
            (*dp)[i][j] = 0;
            (*isChosen)[i][j] = 0;
        }
    }
}

/* 
@brief Matris'lerin bellekte atik olarak kalmamasi icin bosaltilmasini saglayan fonksiyon

@param int **dp: Dinamik Programlama Matrisi
@param int ***isChosen: Harf Secim Matrisi
@param int m: Satir Uzunlugu
*/

void freeMatrices(int ***dp, int ***isChosen, int m) {
	int i, j;
    for (i = 0; i <= m; i++) {
        free((*dp)[i]);
        free((*isChosen)[i]);
    }

    free(*dp);
    free(*isChosen);
}

/* 
@brief Maksimum uzunlugu bulan ve matrisleri dolduran fonksiyon

@param int **dp: Dinamik Programlama Matrisi
@param int ***isChosen: Harf Secim Matrisi
@param char *string1: Ilk Girilen String
@param char *string2: Ikinci Girilen String
@param int m: Satir Uzunlugu
@param int n: Sutun Uzunlugu
@param int *maxLen: maxLen'i kullanmak icin yolladigimiz parametre baslangic = 0
@param int *lastIndex: son indeks'i saklayip kullanmak icin yolladigimiz parametre baslangic = 0
*/

void findMaxLen(int **dp, int **isChosen, char *string1, char *string2, int m, int n, int *maxLen, int *lastIndex) {
	int i, j;
    for (i = 1; i <= m; i++) {
        for (j = 1; j <= n; j++) {
            if (string1[i - 1] == string2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                isChosen[i][j] = 1;  // Harf seï¿½ildi
                if (dp[i][j] > *maxLen) { 
                    *maxLen = dp[i][j]; // 4
                    *lastIndex = i; // 7 -> BACEDAB
                }
            } else {
                if (dp[i - 1][j] >= dp[i][j - 1]) {
                    dp[i][j] = dp[i - 1][j];
                    isChosen[i][j] = 0;  // Harf seï¿½ilmedi
                } else {
                    dp[i][j] = dp[i][j - 1];
                    isChosen[i][j] = 0;  // Harf seï¿½ilmedi
                }
            }
        }
        
        printf("\nMatrisin Durumu (Satir %d):\n", i);
        printMatrix(dp, m + 1, n + 1);

        printf("\nHarf Secme Durumu (Satir %d):\n", i);
        printMatrix(isChosen, m + 1, n + 1);
    }
}

/* 
@brief LCS'lerin essiz olup olmamasini ayirt eden fonksiyon


@param char *lcs: 
@param char *lcsSet: 
@param int lcsLenght: 
@param int lcsCount: 

@return 1 || 0: 1'se essiz, 0'sa degil
*/

int isLCSUnique(char* lcs, int lcsLength, char* lcsSet, int lcsCount) {
	int i, j;
    for (i = 0; i < lcsCount; i++) {
        int currentLength = 0;
        while (currentLength < 50 && lcs[currentLength] != '\0' && lcsSet[i * 50 + currentLength] != '\0') {
            if (lcs[currentLength] != lcsSet[i * 50 + currentLength]) {
                break; 
            }
            currentLength++;
        }

        if (currentLength == lcsLength && (lcsSet[i * 50 + currentLength] == '\0' || lcs[currentLength] == '\0')) {
            return 0; // degil
        }
    }
    return 1; // essiz
}

/* 
@brief En uzun ortak sekansi bastiran fonksiyon

@param char *string1: Ilk girilen string
@param char *string2: Ikinci girilen string
@param int** dp: Dinamik Programlama Matrisi
@param int m: Satir Uzunlugu
@param int n: Sutun uzunlugu
@param char* lcs: LCS'i tutacak dizi
@param: int lcsLength: LCS'in uzunlugu
@param char* lcsSet: LCS kumesi
@param int* lcsCount: LCS sayisi
@param int index: LCS uzunlugu
*/

void printLongestSequences(char* string1, char* string2, int** dp, int m, int n, char* lcs, int lcsLength, char* lcsSet, int* lcsCount, int index) {
    if (m == 0 || n == 0) {
        if (isLCSUnique(lcs, lcsLength, lcsSet, *lcsCount)) {
            strcpy(lcsSet + (*lcsCount)*50, lcs); 
            (*lcsCount)++;
            printf("%s\n", lcs); 
        }
        return;
    }

    if (string1[m - 1] == string2[n - 1]) {
        lcs[index - 1] = string1[m - 1];
        printLongestSequences(string1, string2, dp, m - 1, n - 1, lcs, lcsLength, lcsSet, lcsCount, index - 1);
    } else {
        if (dp[m - 1][n] >= dp[m][n - 1]) {
            printLongestSequences(string1, string2, dp, m - 1, n, lcs, lcsLength, lcsSet, lcsCount, index);
        }
        if (dp[m][n - 1] >= dp[m - 1][n]) {
            printLongestSequences(string1, string2, dp, m, n - 1, lcs, lcsLength, lcsSet, lcsCount, index);
        }
    }
    
}

/* 
@brief LCS'i hesaplayan ana fonksiyon

@param char *string1: Ilk girilen string
@param char *string2: Ikinci girilen string
*/

void calculateLCS(char* string1, char* string2) {
    int m = strlen(string1);
    int n = strlen(string2);

    int **dp;
    int **isChosen;
    initializeMatrices(&dp, &isChosen, m, n);

    
    printf("Matrisin Ilk Hali:\n");
    printMatrix(dp, m + 1, n + 1);

    printf("Harf Secme Durumu:\n");
    printMatrix(isChosen, m + 1, n + 1);

    
    int maxLen = 0;
    int lastIndex = 0;
    findMaxLen(dp, isChosen, string1, string2, m, n, &maxLen, &lastIndex);

    int lcsLength = dp[m][n];
    char lcs[50];
    char lcsSet[50*50]; // Set to store unique LCS strings
    int lcsCount = 0;
	printf("En Uzun Sekans Boyutu: %d\n", lcsLength);
	printf("En Uzun Sekanslar:\n");
    printLongestSequences(string1, string2, dp, m, n, lcs, lcsLength, lcsSet, &lcsCount, lcsLength);
}

int main() {
    char string1[100], string2[100];

    printf("String 1: ");
    scanf("%s", string1);
    printf("String 2: ");
    scanf("%s", string2);

    calculateLCS(string1, string2);

    return 0;
}
