#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Kullanıcı bilgileri struct'ı
typedef struct user {
    char *username; // kullanıcı adı
    int deleted;  // silinme durumu
} USER;

// Hash tablosu struct'ı
typedef struct hashTable {
    USER *user; // kullanıcı
} HASHTABLE;

int h1(int key, int m, int modSecimi);
int h2(int key, int m, int modSecimi);
int h(int key, int i, int m, int modSecimi);
int calculateKey(char* word, int R, int modSecimi);
void insert(HASHTABLE *hashTable, char *userName, int m, int modSecimi);
int search(HASHTABLE *hashTable, char *userName, int m, int modSecimi);
void removeUser(HASHTABLE *hashTable, char *userName, int m, int modSecimi);
HASHTABLE* editTable(HASHTABLE *hashTable, int m, int modSecimi);
void printTable(HASHTABLE *hashTable, int m);
HASHTABLE* resizeTable(HASHTABLE *hashTable, int *M, int modSecimi);
int isPrime(int num);
int findNextPrime(int num);

/* 
@brief Horner's method ile bir kelimenin sayisal degerini hesaplayan fonksiyon

@param char* word: Hash tablosuna eklenecek kelime
@param int R: Formuldeki taban (asal sayi secilir)
@param int modSecimi: Secilen moda gore islem yapilir 1: NORMAL, 2: DEBUG 

@return int: Kelimenin sayisal degeri
*/

int calculateKey(char* word, int R, int modSecimi) {
    int result = 0;
    int length = strlen(word);

    for (int i = 0; i < length; ++i) {
        result = (result * R + (word[i] - 'A' + 1));
        if (modSecimi == 2){
            printf("word[%d] = %c, result = %d\n", i, word[i], result);
        }
    }

    return result;
}

/* 
@brief Hash fonksiyonlarından ilki: h1(key) = key % m

@param int key: calculateKey fonksiyonu ile horner's method kullanılarak hesaplanan key degeri
@param int m: Tablo uzunluğu
@param int modSecimi: Secilen moda gore islem yapilir 1: NORMAL, 2: DEBUG

@return int: Hash tablosunda kullanılacak index degeri
*/

int h1(int key, int m, int modSecimi) {
    if (modSecimi == 2){
        printf("h1(%d) = %d\n", key, key % m);
        return key % m;
    }
    return key % m;
}

/* 
@brief Hash fonksiyonlarından ikincisi: h2(key) = 1 + (key % m2)

@param int key: calculateKey fonksiyonu ile horner's method kullanılarak hesaplanan key degeri
@param int m: Tablo uzunluğu
@param int modSecimi: Secilen moda gore islem yapilir 1: NORMAL, 2: DEBUG 

@return int: Hash tablosunda kullanılacak index degeri
*/

int h2(int key, int m, int modSecimi) {
    int m2 = m - 2;
    if (modSecimi == 2){
        printf("h2(%d) = %d\n", key, 1 + (key % m2));
        return 1 + (key % m2);
    }
    return 1 + (key % m2);
}

/* 
@brief Ana Hash fonksiyonu: h(key, i) = (h1(key) + i * h2(key)) % m

@param int key: calculateKey fonksiyonu ile horner's method kullanılarak hesaplanan key degeri
@param int m: Tablo uzunluğu
@param int i: Formüldeki i degeri
@param int modSecimi: Secilen moda gore islem yapilir 1: NORMAL, 2: DEBUG

@return int: Hash tablosunda kullanılacak index degeri
*/

int h(int key, int i, int m, int modSecimi) {
    if (modSecimi == 2){
        printf("h(%d, %d) = (%d + %d * %d) %% %d = %d\n", key, i, h1(key, m, modSecimi), i, h2(key, m, modSecimi), m, (h1(key, m, modSecimi) + i * h2(key, m, modSecimi)) % m);
    }
    return (h1(key, m, modSecimi) + i * h2(key, m, modSecimi)) % m;
}

/* 
@brief Hash tablosuna kullanıcı ekleyen fonksiyon

@param HASHTABLE *hashTable: Hash tablosu
@param char *userName: Hash tablosuna eklenecek kullanıcı adı
@param int m: Tablo uzunluğu
@param int modSecimi: Secilen moda gore islem yapilir 1: NORMAL, 2: DEBUG 
*/

void insert(HASHTABLE *hashTable, char *userName, int m, int modSecimi) {
    if (modSecimi == 2){
        printf("----- %s KULLANICISI EKLENIYOR -----\n", userName);
        printf("Kullanici adi: %s\n", userName);
    }

    int key = calculateKey(userName, 31, modSecimi);
    if (modSecimi == 2){
        printf("Key: %d\n", key);
    }

    int i = 0;
    int index = h(key, i, m, modSecimi);
    if (modSecimi == 2){
        printf("Index: %d\n", index);
    }

    while (hashTable[index].user != NULL) {
        i++;
        index = h(key, i, m, modSecimi);
        if (modSecimi == 2){
            printf("Index: %d\n", index);
        }
    }

    // Yeni kullanıcı adını ekleyerek bellek alanını ayır
    hashTable[index].user = (USER*)malloc(sizeof(USER));
    hashTable[index].user->username = strdup(userName);
    hashTable[index].user->deleted = 0;
}

/* 
@brief Hash tablosunda kullanıcı arayan fonksiyon

@param HASHTABLE *hashTable: Hash tablosu
@param char *userName: Hash tablosuna eklenecek kullanıcı adı
@param int m: Tablo uzunluğu
@param int modSecimi: Secilen moda gore islem yapilir 1: NORMAL, 2: DEBUG

@return int: Kullanıcı adının bulunduğu indexi döndürür, bulunamazsa -1 döndürür
*/

int search(HASHTABLE *hashTable, char *userName, int m, int modSecimi) {
    int key = calculateKey(userName, 31, modSecimi);
    int i = 0;
    int index = h(key, i, m, modSecimi);

    while (hashTable[index].user != NULL) {
        if (modSecimi == 2){
            printf("Adim %d - Index %d: %s\n", i + 1, index, hashTable[index].user->username);
        }
        if (strcmp(hashTable[index].user->username, userName) == 0 && hashTable[index].user->deleted != 1) {
            return index;
        }

        i++;
        index = h(key, i, m, modSecimi);
    }

    return -1;
}

/* 
@brief Hash tablosunda kullanıcı silen fonksiyon

@param HASHTABLE *hashTable: Hash tablosu
@param char *userName: Hash tablosuna eklenecek kullanıcı adı
@param int m: Tablo uzunluğu
@param int modSecimi: Secilen moda gore islem yapilir 1: NORMAL, 2: DEBUG 
*/

void removeUser(HASHTABLE *hashTable, char *userName, int m, int modSecimi) {
    int index = search(hashTable, userName, m, modSecimi);

    if (index != -1) {
        hashTable[index].user->deleted = 1;
    } else {
        printf("%s elemani tabloda bulunmuyor.\n", userName);
    }
}

/* 
@brief Hash tablosunda silinmiş kullanıcıları silen fonksiyon bir nevi düzenleme ve boşaltma işlemleri yapar

@param HASHTABLE *hashTable: Hash tablosu
@param int m: Tablo uzunluğu
@param int modSecimi: Secilen moda gore islem yapilir 1: NORMAL, 2: DEBUG 
*/

HASHTABLE* editTable(HASHTABLE *hashTable, int m, int modSecimi) {
    HASHTABLE *newHashTable = (HASHTABLE *)malloc(sizeof(HASHTABLE) * (m));

    // Yeni hash tablosunu başlangıçta boş olarak ayarla
    
    for (int i = 0; i < m; ++i) {
        newHashTable[i].user = NULL;
    }
    
    // Eski hash tablosundaki kullanıcıları yeni tabloya ekle
    for (int i = 0; i < m; ++i) {
        if (hashTable[i].user != NULL && hashTable[i].user->deleted != 1) {
            insert(newHashTable, hashTable[i].user->username, m, modSecimi);
        }
    }
    
    // Eski hash tablosunu serbest bırak
    for (int i = 0; i < m; ++i) {
        if(hashTable[i].user != NULL){
            free(hashTable[i].user->username);
            free(hashTable[i].user); 
        }
    }
    free(hashTable);
    
    return newHashTable;
}

/* 
@brief Hash tablosunun yazdırıldığı fonksiyon

@param HASHTABLE *hashTable: Hash tablosu
@param int m: Tablo uzunluğu
*/

void printTable(HASHTABLE *hashTable, int m) {
    for (int i = 0; i < m; ++i) {
        if (hashTable[i].user != NULL) {
            printf("%d. index: %s (Silinme Durumu: %d)\n", i, hashTable[i].user->username, hashTable[i].user->deleted);
        } else {
            printf("%d. index: NULL\n", i);
        }
    }
}

/* 
@brief Asal sayı mı değil mi kontrol eden fonksiyon

@param int num: Kontrol edilecek sayı

@return int: Asal ise 1, değilse 0 döndürür
*/

int isPrime(int num) {
    if (num <= 1) return 0;
    for (int i = 2; i <= sqrt(num); ++i) {
        if (num % i == 0) return 0;
    }
    return 1;
}

/* 
@brief Bir sonraki asal sayıyı bulan fonksiyon

@param int num: Kontrol edilecek sayı

@return int: Bir sonraki asal sayıyı döndürür
*/

int findNextPrime(int num) {
    while (!isPrime(num)) {
        num++;
    }
    return num;
}

/* 
@brief Hash tablosunda yer kalmadığında tabloyu genişleten fonksiyon

@param HASHTABLE *hashTable: Hash tablosu
@param int m: Tablo uzunluğu
@param int modSecimi: Secilen moda gore islem yapilir 1: NORMAL, 2: DEBUG

@return HASHTABLE*: Yeni hash tablosunu döndürür
*/

HASHTABLE* resizeTable(HASHTABLE *hashTable, int *M, int modSecimi) {
    int oldM = *M;
    *M = findNextPrime(2 * oldM);
    HASHTABLE *newHashTable = (HASHTABLE *)malloc(sizeof(HASHTABLE) * (*M));

    // Yeni hash tablosunu başlangıçta boş olarak ayarla
    for (int i = 0; i < *M; ++i) {
        newHashTable[i].user = NULL;
    }

    // Eski hash tablosundaki kullanıcıları yeni tabloya ekle
    for (int i = 0; i < oldM; ++i) {
        if (hashTable[i].user != NULL && hashTable[i].user->deleted != 1) {
            insert(newHashTable, hashTable[i].user->username, *M, modSecimi);
        }
    }

    // Eski hash tablosunu serbest bırak
    for (int i = 0; i < oldM; ++i) {
        free(hashTable[i].user->username);
        free(hashTable[i].user);
    }
    free(hashTable);

    return newHashTable;
}


int main(){
    int modSecimi;
    int menuSecimi;
    int N; // eklenebilecek eleman sayisi
    int M; // hash tablosu uzunluğu
    double loadFactor; // load factor

    // Kullanıcıdan mod seçiminin alınması
    printf("Mod secimi yapiniz (1: NORMAL, 2: DEBUG): ");
    scanf("%d", &modSecimi);
    
    // Kullanıcıdan N ve M değerlerinin alınması
    printf("Eklenebilecek eleman sayisi (N) giriniz: ");
    scanf("%d", &N);

    // Kullanıcıdan load factor değerinin alınması
    printf("Load factor (lambda) giriniz: ");
    scanf("%lf", &loadFactor);

    M = N / loadFactor;
    M = findNextPrime(M);  // M'yi bir sonraki asal sayıya güncelle
    printf("Hash tablosu uzunlugu (M): %d\n", M);

    // Hash tablosunun oluşturulması
    HASHTABLE *hashTable = (HASHTABLE*)malloc(sizeof(HASHTABLE) * M);

    for (int i = 0; i < M; ++i) {
        hashTable[i].user = NULL;
    }

    // Kullanıcıdan kullanıcı adlarının alınması
    for (int i = 0; i < N; ++i) {
        // Kullanıcı adını dinamik olarak tutacak bir dizi oluştur
        char *userName = (char*)malloc(sizeof(char) * 256);

        // Kullanıcıdan girişi al
        printf("%d. kullanici adini giriniz: ", i + 1);
        scanf("%255s", userName);

        // Kullanıcı adını hash tablosuna ekle
        insert(hashTable, userName, M, modSecimi);
        printf("%s kullanici adi hash tablosuna eklendi.\n", userName);
        // printf("Hash tablosu:\n");
        // printTable(hashTable, M);

        // Dinamik olarak ayrılan belleği serbest bırak
        free(userName);
    }

    do {
        // Menü seçenekleri
        printf("\n----- MENU -----\n");
        printf("1. Ekle\n");
        printf("2. Ara\n");
        printf("3. Sil\n");
        printf("4. Tabloyu Yazdir\n");
        printf("5. Yeni Tabloya Gec\n");
        printf("6. Cikis\n");

        printf("Seciminizi yapiniz: ");
        if (scanf("%d", &menuSecimi) != 1) {
        // Eğer beklenen türde bir değer alınamazsa
        printf("Hatali secim! Lutfen bir sayi girin.\n");
        // Giriş akışını temizle
        while (getchar() != '\n');
        // Döngünün başına geri dön
        continue;
        }

        switch (menuSecimi) {
            case 1: {
                // Ekleme işlemi
                char *userName = (char *)malloc(sizeof(char) * 256);
                printf("Eklemek istediginiz kullanici adini giriniz: ");
                scanf("%255s", userName);

                // Kullanıcı adının tabloda olup olmadığını kontrol et
                int existingIndex = search(hashTable, userName, M, modSecimi);
                if (existingIndex != -1 && hashTable[existingIndex].user->deleted != 1) {
                    // Kullanıcı adı zaten var ve silinmemiş
                    printf("Kullanici adi tabloda zaten oldugu icin eklenemedi.\n");
                } else {
                    // Doluluk kontrolü
                    double loadFactor = (double)N / (double)(M);
                    if (loadFactor > 0.95) {
                        printf("Tablo dolu! Tabloyu genisletme islemi yapiliyor...\n");
                        // Tabloyu genişlet
                        hashTable = resizeTable(hashTable, &M, modSecimi);
                        printf("Yeni hash tablosu olusturuldu. Yeni uzunluk: %d\n", M);
                    }

                    insert(hashTable, userName, M, modSecimi);
                    printf("%s kullanici adi hash tablosuna eklendi.\n", userName);
                }
                free(userName);
                break;
            }
            case 2: {
                // Arama işlemi
                char *userName = (char *)malloc(sizeof(char) * 256);
                printf("Aramak istediginiz kullanici adini giriniz: ");
                scanf("%255s", userName);
                int index = search(hashTable, userName, M, modSecimi);
                if (index != -1) {
                    printf("%s kullanici adi %d. adreste bulunmaktadir.\n", userName, index);
                } else {
                    printf("%s kullanici adi hash tablosunda bulunamadi.\n", userName);
                }
                free(userName);
                break;
            }
            case 3: {
                // Silme işlemi
                char *userName = (char *)malloc(sizeof(char) * 256);
                printf("Silmek istediginiz kullanici adini giriniz: ");
                scanf("%255s", userName);

                removeUser(hashTable, userName, M, modSecimi);
                printf("%s kullanici adi silindi.\n", userName);
                free(userName);
                break;
            }
            // Diğer case'ler eklenmeli
            case 4: {
                // Tabloyu Yazdırma işlemi
                printf("\nHash tablosu:\n");
                printTable(hashTable, M);
                break;
            }
            case 5: {
                // Yeni Tabloya Geçme işlemi
                printf("Yeni tabloya geciliyor...\n");
                hashTable = editTable(hashTable, M, modSecimi);
                break;
            }
            case 6: {
                // Çıkış
                printf("Programdan cikiliyor...\n");
                break;
            }
            default: {
                printf("Gecersiz secim! Lutfen tekrar deneyin.\n");
                break;
            }
        }
    } while (menuSecimi != 6);

    // Belleği serbest bırak
    for (int i = 0; i < M; ++i) {
        if (hashTable[i].user != NULL) {
            free(hashTable[i].user->username);
            free(hashTable[i].user);
        }
    }
    free(hashTable);

    return 0;
}
