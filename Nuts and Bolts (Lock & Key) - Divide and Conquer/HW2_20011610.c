#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int partition(int arr[], int low, int high, int pivot);
void swap(int *a, int *b);

/* 
@brief Bu fonksiyon, anahtarlar ve kilitler aras�nda e�le�meyi bulmak i�in bir b�l ve y�net yakla��m�n� kullanarak verilen dizileri s�ralar ve e�le�tirir.

@param int keys[]: Anahtarlar� temsil eden bir tamsay� dizisi
@param int locks[]: Kilitleri temsil eden bir tamsay� dizisi
@param int low: ��lem yap�lacak dizinin alt s�n�r�
@param int high: ��lem yap�lacak dizinin �st s�n�r�
*/

void matchKeysAndLocks(int keys[], int locks[], int low, int high) {
    if (low < high) {
        // Pivot eleman� se�mek i�in partition i�levini kullan�r�z.
        int pivotIndex = partition(keys, low, high, locks[high]);

        // Pivot eleman�n� kullanarak locks dizisini b�l.
        partition(locks, low, high, keys[pivotIndex]);

        // Sol ve sa� alt dizileri i�lemek i�in rek�rsif �a�r�lar.
        matchKeysAndLocks(keys, locks, low, pivotIndex - 1);
        matchKeysAndLocks(keys, locks, pivotIndex + 1, high);
    }
}

/* 
@brief Bu fonksiyon, bir diziyi belirli bir pivot eleman�na g�re b�l�p s�ralar, k���k elemanlar� sola b�y�k elemanlar� ise sa�a yerle�tirerek pivot eleman�n�n do�ru konumunu belirler. 
@brief B�l ve Y�net gibi yakla��mlarda s�ralama algoritmalar�nda kullan�l�r.

@param int arr[]: Partition fonksiyonu, bu diziyi pivot eleman�na g�re b�lecek ve s�ralayacak.
@param int low: ��lem yap�lacak dizinin alt s�n�r
@param int high: ��lem yap�lacak dizinin �st s�n�r
@param int pivot: Diziyi b�lecek olan pivot eleman

@return Pivot eleman�n yeni indeksini hesaplar ve bu indeksi d�nd�r�r
*/

int partition(int arr[], int low, int high, int pivot) {
    int i = low - 1;

    for (int j = low; j < high; j++) {
        // Eleman� pivot ile kar��la�t�r ve s�rala.
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        } else if (arr[j] == pivot) {
            // E�le�en eleman� pivot ile ta��.
            swap(&arr[j], &arr[high]);
            j--;
        }
    }

    // Pivot eleman� sa�daki uygun konuma ta�� ve bu konumu d�nd�r.
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

/* 
@brief iki tamsay� de�eri aras�nda de�i� toku� i�lemi ger�ekle�tirir

@param int *a: Bir i�aret�i (pointer) parametresidir ve a adl� bir tamsay� de�i�kenini temsil eder. Bu i�aret�i, swap i�lemi s�ras�nda a de�erini kullan�r ve de�i�tirilir.
@param int *b: Bir ba�ka i�aret�i parametresidir ve b adl� bir tamsay� de�i�kenini temsil eder. Bu i�aret�i, swap i�lemi s�ras�nda b de�erini kullan�r ve de�i�tirilir.
*/

void swap(int *a, int *b) {
    int temp = *a;  // �lk tamsay� de�eri 'a', 'temp' adl� ge�ici bir de�i�kene kopyalan�r.
    *a = *b;        // �lk tamsay� de�eri, ikinci tamsay� de�eriyle de�i�tirilir.
    *b = temp;      // �kinci tamsay� de�eri, ge�ici de�i�ken 'temp' ile de�i�tirilir.
}

/* 
@brief Bu shuffleArray fonksiyonu, bir tamsay� dizisini rastgele kar��t�rmak i�in kullan�l�r.

@param int arr[]: shuffleArray fonksiyonu, bu diziyi kar��t�rmak i�in kullan�r.
@param int n: Dizide ka� eleman�n bulundu�unu (oda say�s�) belirtir.
*/

void shuffleArray(int arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        // Rastgele bir indis 'j' se�er, 0 ile 'i' aras�nda bir de�er.
        int j = rand() % (i + 1);

        // Dizideki arr[i] ve arr[j] elemanlar�n� de�i�tir.
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

int main() {
    int N;
    srand(time(NULL));
    do {
    	printf("Oda sayisi N'i girin: ");
    	scanf("%d", &N);
	    if (N <= 0) {
	        printf("N pozitif bir tamsayi olmali. Lutfen gecerli bir deger girin.\n");
	    }
	} while (N <= 0);

    // N kadarl�k bellek tahsis edilir ve keys ve locks dizileri dinamik olarak olu�turulur.
    int *keys = (int *)malloc(N * sizeof(int));
    int *locks = (int *)malloc(N * sizeof(int));

    // Rastgele de�erlerle keys ve locks dizileri doldurulur.
    for (int i = 0; i < N; i++) {
        int randomValue = rand();
        keys[i] = randomValue;
        locks[i] = randomValue;
    }

    // Diziler rastgele kar��t�r�l�r. �lk olu�tu�u hali belli olmas� i�in.
    shuffleArray(keys, N);
    shuffleArray(locks, N);
    
    printf("Olusturulan Anahtarlar ve Kilitler:\n");
    for (int i = 0; i < N; i++) {
        // Olu�turulan anahtarlar ve kilitler ekrana yazd�r�l�r.
        printf("%d. Anahtar ve Kilit: Anahtar %d, Kilit %d\n", i, keys[i], locks[i]);
    }

    // Anahtarlar ve kilitler e�le�tirilir.
    matchKeysAndLocks(keys, locks, 0, N - 1);
	
    printf("Eslesen Anahtarlar:\n");
    for (int i = 0; i < N; i++) {
        // E�le�en anahtarlar ve kilitler ekrana yazd�r�l�r.
        printf("Anahtar %d, Kilidi %d ile eslestirildi.\n", keys[i], locks[i]);
    }

    // Bellek temizlenir (free i�lemi).
    free(keys);
    free(locks);

    return 0;
}
