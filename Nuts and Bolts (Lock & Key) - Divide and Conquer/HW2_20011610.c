#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int partition(int arr[], int low, int high, int pivot);
void swap(int *a, int *b);

/* 
@brief Bu fonksiyon, anahtarlar ve kilitler arasýnda eþleþmeyi bulmak için bir böl ve yönet yaklaþýmýný kullanarak verilen dizileri sýralar ve eþleþtirir.

@param int keys[]: Anahtarlarý temsil eden bir tamsayý dizisi
@param int locks[]: Kilitleri temsil eden bir tamsayý dizisi
@param int low: Ýþlem yapýlacak dizinin alt sýnýrý
@param int high: Ýþlem yapýlacak dizinin üst sýnýrý
*/

void matchKeysAndLocks(int keys[], int locks[], int low, int high) {
    if (low < high) {
        // Pivot elemaný seçmek için partition iþlevini kullanýrýz.
        int pivotIndex = partition(keys, low, high, locks[high]);

        // Pivot elemanýný kullanarak locks dizisini böl.
        partition(locks, low, high, keys[pivotIndex]);

        // Sol ve sað alt dizileri iþlemek için rekürsif çaðrýlar.
        matchKeysAndLocks(keys, locks, low, pivotIndex - 1);
        matchKeysAndLocks(keys, locks, pivotIndex + 1, high);
    }
}

/* 
@brief Bu fonksiyon, bir diziyi belirli bir pivot elemanýna göre bölüp sýralar, küçük elemanlarý sola büyük elemanlarý ise saða yerleþtirerek pivot elemanýnýn doðru konumunu belirler. 
@brief Böl ve Yönet gibi yaklaþýmlarda sýralama algoritmalarýnda kullanýlýr.

@param int arr[]: Partition fonksiyonu, bu diziyi pivot elemanýna göre bölecek ve sýralayacak.
@param int low: Ýþlem yapýlacak dizinin alt sýnýr
@param int high: Ýþlem yapýlacak dizinin üst sýnýr
@param int pivot: Diziyi bölecek olan pivot eleman

@return Pivot elemanýn yeni indeksini hesaplar ve bu indeksi döndürür
*/

int partition(int arr[], int low, int high, int pivot) {
    int i = low - 1;

    for (int j = low; j < high; j++) {
        // Elemaný pivot ile karþýlaþtýr ve sýrala.
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        } else if (arr[j] == pivot) {
            // Eþleþen elemaný pivot ile taþý.
            swap(&arr[j], &arr[high]);
            j--;
        }
    }

    // Pivot elemaný saðdaki uygun konuma taþý ve bu konumu döndür.
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

/* 
@brief iki tamsayý deðeri arasýnda deðiþ tokuþ iþlemi gerçekleþtirir

@param int *a: Bir iþaretçi (pointer) parametresidir ve a adlý bir tamsayý deðiþkenini temsil eder. Bu iþaretçi, swap iþlemi sýrasýnda a deðerini kullanýr ve deðiþtirilir.
@param int *b: Bir baþka iþaretçi parametresidir ve b adlý bir tamsayý deðiþkenini temsil eder. Bu iþaretçi, swap iþlemi sýrasýnda b deðerini kullanýr ve deðiþtirilir.
*/

void swap(int *a, int *b) {
    int temp = *a;  // Ýlk tamsayý deðeri 'a', 'temp' adlý geçici bir deðiþkene kopyalanýr.
    *a = *b;        // Ýlk tamsayý deðeri, ikinci tamsayý deðeriyle deðiþtirilir.
    *b = temp;      // Ýkinci tamsayý deðeri, geçici deðiþken 'temp' ile deðiþtirilir.
}

/* 
@brief Bu shuffleArray fonksiyonu, bir tamsayý dizisini rastgele karýþtýrmak için kullanýlýr.

@param int arr[]: shuffleArray fonksiyonu, bu diziyi karýþtýrmak için kullanýr.
@param int n: Dizide kaç elemanýn bulunduðunu (oda sayýsý) belirtir.
*/

void shuffleArray(int arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        // Rastgele bir indis 'j' seçer, 0 ile 'i' arasýnda bir deðer.
        int j = rand() % (i + 1);

        // Dizideki arr[i] ve arr[j] elemanlarýný deðiþtir.
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

    // N kadarlýk bellek tahsis edilir ve keys ve locks dizileri dinamik olarak oluþturulur.
    int *keys = (int *)malloc(N * sizeof(int));
    int *locks = (int *)malloc(N * sizeof(int));

    // Rastgele deðerlerle keys ve locks dizileri doldurulur.
    for (int i = 0; i < N; i++) {
        int randomValue = rand();
        keys[i] = randomValue;
        locks[i] = randomValue;
    }

    // Diziler rastgele karýþtýrýlýr. Ýlk oluþtuðu hali belli olmasý için.
    shuffleArray(keys, N);
    shuffleArray(locks, N);
    
    printf("Olusturulan Anahtarlar ve Kilitler:\n");
    for (int i = 0; i < N; i++) {
        // Oluþturulan anahtarlar ve kilitler ekrana yazdýrýlýr.
        printf("%d. Anahtar ve Kilit: Anahtar %d, Kilit %d\n", i, keys[i], locks[i]);
    }

    // Anahtarlar ve kilitler eþleþtirilir.
    matchKeysAndLocks(keys, locks, 0, N - 1);
	
    printf("Eslesen Anahtarlar:\n");
    for (int i = 0; i < N; i++) {
        // Eþleþen anahtarlar ve kilitler ekrana yazdýrýlýr.
        printf("Anahtar %d, Kilidi %d ile eslestirildi.\n", keys[i], locks[i]);
    }

    // Bellek temizlenir (free iþlemi).
    free(keys);
    free(locks);

    return 0;
}
