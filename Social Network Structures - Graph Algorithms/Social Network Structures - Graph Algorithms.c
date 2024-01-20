#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct {
    char start;
    char end;
    int count;
    double betweenness;
    char *paths[1000];
} Edge;

typedef struct {
    char *nodes;
    int size;
} Community;

typedef struct {
    Edge edge;
    int betweenness;
} BetweennessInfo;

void printGraph(char *nodes, int nodeCount, Edge *edges, int edgeCount);
void removeDuplicateEdges(Edge *edges, int *edgeCount);
int readGraph(const char *filename, char **nodes, int *nodeCount, Edge **edges, int *edgeCount);
int areEdgesEqual(Edge edge1, Edge edge2);

void bfs(char *nodes, int nodeCount, Edge *edges, int edgeCount);
int indexOfNode(char *nodes, int nodeCount, char node);
void printShortestPaths(char *nodes, int nodeCount, char start, int *distances, int *predecessors);
void printEdgeOccurrences(Edge *edges, int edgeCount);

void removeEdge(Edge *edges, int *edgeCount, int index);
void detectCommunities(Edge *edges, int edgeCount, char *nodes, int nodeCount, int minCommunitySize, int iterations);


/* 
@brief Verilen dugumun indisini dondurur

@param char *nodes: node listesi
@param int nodeCount: node sayisi
@param char node: aranan node

@return int: node'un indisini dondurur, bulunamazsa -1 dondurur
*/

int indexOfNode(char *nodes, int nodeCount, char node) {
	int i;
    for (i = 0; i < nodeCount; ++i) {
        if (nodes[i] == node) {
            return i;
        }
    }
    return -1;
}

/*
@brief Verilen dugumler arasindaki en kisa yolu bulur

@param char *nodes: node listesi
@param char startNode: baslangic dugumu
@param int *distances: dugumlerin uzakliklarini tutan dizi
@param int *predecessors: dugumlerin onceki dugumlerini tutan dizi
@param int endIndex: son dugumun indisi
@param char path[]: en kisa yolu tutan dizi

@return void
*/

void getShortestPath(char *nodes, char startNode, int *distances, int *predecessors, int endIndex, char path[]) {
    int currentIndex = endIndex;
    int pathIndex = 0;
	int i, j;
    while (currentIndex != -1) {
        path[pathIndex++] = nodes[currentIndex];
        currentIndex = predecessors[currentIndex];
    }

    for (i = 0, j = pathIndex - 1; i < j; ++i, --j) {
        char temp = path[i];
        path[i] = path[j];
        path[j] = temp;
    }

    path[pathIndex] = '\0';
}

/*
@brief Verilen dugumler arasinda bfs ile en kisa yolu bulur ve kenarlarin betweenness degerlerini hesaplar 

@param char *nodes: node listesi
@param int nodeCount: node sayisi
@param Edge *edges: kenar listesi
@param int edgeCount: kenar sayisi

@return void
*/

void bfs(char *nodes, int nodeCount, Edge *edges, int edgeCount) {
	int i, j, k;
	
    for (i = 0; i < edgeCount; ++i) {
        edges[i].count = 0;  // Her kenara ait yollarÄ± temizle
    }

    for (i = 0; i < nodeCount; ++i) {
        char startNode = nodes[i];
        int *distances = (int *)malloc(nodeCount * sizeof(int));
        int *predecessors = (int *)malloc(nodeCount * sizeof(int));
        int *sigma = (int *)malloc(nodeCount * sizeof(int));
        double *delta = (double *)malloc(nodeCount * sizeof(double));

        for (j = 0; j < nodeCount; ++j) {
            distances[j] = -1;
            predecessors[j] = -1;
            sigma[j] = 0;
            delta[j] = 0.0;
        }

        distances[i] = 0;
        sigma[i] = 1;

        int queue[nodeCount];
        int front = 0, rear = 0;
        queue[rear++] = i;

        while (front < rear) {
            int current = queue[front++];
            for (k = 0; k < edgeCount; ++k) {
                if (edges[k].start == nodes[current]) {
                    int nextIndex = indexOfNode(nodes, nodeCount, edges[k].end);
                    if (distances[nextIndex] == -1) {
                        distances[nextIndex] = distances[current] + 1;
                        predecessors[nextIndex] = current;
                        queue[rear++] = nextIndex;

                        char path[1000];
                        getShortestPath(nodes, startNode, distances, predecessors, nextIndex, path);

                        edges[k].paths[edges[k].count] = (char *)malloc(strlen(path) + 1);
                        strcpy(edges[k].paths[edges[k].count], path);
                        edges[k].count++;

                        sigma[nextIndex] += sigma[current];
                        delta[nextIndex] += ((double)sigma[current] / sigma[nextIndex]) * (1 + delta[current]);
                    }
                } else if (edges[k].end == nodes[current]) {
                    int nextIndex = indexOfNode(nodes, nodeCount, edges[k].start);
                    if (distances[nextIndex] == -1) {
                        distances[nextIndex] = distances[current] + 1;
                        predecessors[nextIndex] = current;
                        queue[rear++] = nextIndex;

                        char path[1000];
                        getShortestPath(nodes, startNode, distances, predecessors, nextIndex, path);

                        edges[k].paths[edges[k].count] = (char *)malloc(strlen(path) + 1);
                        strcpy(edges[k].paths[edges[k].count], path);
                        edges[k].count++;

                        sigma[nextIndex] += sigma[current];
                        delta[nextIndex] += ((double)sigma[current] / sigma[nextIndex]) * (1 + delta[current]);
                    }
                }
            }
        }

        // Accumulate edge betweenness values
        for (j = 0; j < nodeCount; ++j) {
            if (j != i) {
                int pred = predecessors[j];
                double numPaths = (double)sigma[j] / sigma[i];
                double edgeContrib = numPaths * (1 + delta[j]);
                while (pred != -1) {
                    for (k = 0; k < edgeCount; ++k) {
                        if ((nodes[pred] == edges[k].start && nodes[j] == edges[k].end) ||
                            (nodes[pred] == edges[k].end && nodes[j] == edges[k].start)) {
                            edges[k].betweenness += edgeContrib;
                            break;
                        }
                    }
                    pred = predecessors[pred];
                }
            }
        }

        printShortestPaths(nodes, nodeCount, startNode, distances, predecessors);

        free(distances);
        free(predecessors);
        free(sigma);
        free(delta);
    }

    // Print edge occurrences with betweenness values
    printEdgeOccurrences(edges, edgeCount);

    int maxOccurrence = -1;
    int maxIndex = 0;

    for (i = 0; i < edgeCount; ++i) {
        int occurrenceCount = 0;
        for (j = 0; j < edgeCount; ++j) {
            for (k = 0; k < edges[j].count; ++k) {
                char *path = edges[j].paths[k];
                char *edgeStart = strchr(path, edges[i].start);
                char *edgeEnd = strchr(path, edges[i].end);

                if (edgeStart != NULL && edgeEnd != NULL && edgeStart < edgeEnd) {
                    occurrenceCount++;
                }
            }
        }

        if (occurrenceCount > maxOccurrence) {
            maxOccurrence = occurrenceCount;
            maxIndex = i;
        }
    }

    printf("The edge with the highest betweenness is %c-%c.\n", edges[maxIndex].start, edges[maxIndex].end);
    
    // Removed edge
    printf("%c-%c deleted\n", edges[maxIndex].start, edges[maxIndex].end);
}

/*
@brief Verilen dugumler arasindaki en kisa yolu ekrana yazdirir

@param char *nodes: node listesi
@param int nodeCount: node sayisi
@param char start: baslangic dugumu
@param int *distances: dugumlerin uzakliklarini tutan dizi
@param int *predecessors: dugumlerin onceki dugumlerini tutan dizi

@return void
*/

void printShortestPaths(char *nodes, int nodeCount, char start, int *distances, int *predecessors) {
	int i, j;
    printf("Shortest paths from %c:\n", start);
    for (i = 0; i < nodeCount; ++i) {
        char currentNode = nodes[i];
        if (currentNode != start) {
            printf("to %c: [", currentNode);

            int current = i;
            int path[nodeCount];
            int pathIndex = 0;

            while (current != -1) {
                path[pathIndex++] = current;
                current = predecessors[current];
            }

            if (distances[i] != -1) {
                printf("'%c'", nodes[path[pathIndex - 1]]);
                for (j = pathIndex - 2; j >= 0; --j) {
                    printf(", '%c'", nodes[path[j]]);
                }
            }

            printf("], Distance: %d\n", (distances[i] == -1) ? -1 : abs(distances[i]));
        }
    }
}

/*
@brief Verilen kenarlarin kac kez gectigini hesaplar ve ekrana yazdirir

@param Edge *edges: kenar listesi
@param int edgeCount: kenar sayisi

@return void
*/

void printEdgeOccurrences(Edge *edges, int edgeCount) {
    printf("Edge betweenness:\n");
	int i, j, k;
    for (i = 0; i < edgeCount; ++i) {
        printf("Edge ('%c', '%c'): ", edges[i].start, edges[i].end);

        // Count occurrences
        int occurrenceCount = 0;
        for (j = 0; j < edgeCount; ++j) {
            for (k = 0; k < edges[j].count; ++k) {
                // Check if the current edge is in the current path
                char *path = edges[j].paths[k];
                char *edgeStart = strchr(path, edges[i].start);
                char *edgeEnd = strchr(path, edges[i].end);

                if (edgeStart != NULL && edgeEnd != NULL && edgeStart < edgeEnd) {
                    occurrenceCount++;
                }
            }
        }

        printf("%d times\n", occurrenceCount);
    }
}

/*
@brief Verilen kenarlarin esit olup olmadigini kontrol eder

@param Edge edge1: kenar 1
@param Edge edge2: kenar 2

@return int: kenarlar esitse 1, degilse 0 dondurur
*/

int areEdgesEqual(Edge edge1, Edge edge2) {
    return (edge1.start == edge2.start && edge1.end == edge2.end) || (edge1.start == edge2.end && edge1.end == edge2.start);
}

/*
@brief Verilen kenarlarin tekrar edenlerini siler

@param Edge *edges: kenar listesi
@param int *edgeCount: kenar sayisi

@return void
*/

void removeDuplicateEdges(Edge *edges, int *edgeCount) {
	int i, j, k;
    for (i = 0; i < *edgeCount; ++i) {
        for (j = i + 1; j < *edgeCount; ++j) {
            if (areEdgesEqual(edges[i], edges[j])) {
                for (k = j; k < *edgeCount - 1; ++k) {
                    edges[k] = edges[k + 1];
                }
                (*edgeCount)--;
                j--;
            }
        }
    }
}

/*
@brief Verilen dosyadan dugum ve kenar listesini okur

@param const char *filename: dosya adi
@param char **nodes: dugum listesi
@param int *nodeCount: dugum sayisi
@param Edge **edges: kenar listesi
@param int *edgeCount: kenar sayisi

@return int: dosya okunursa 1, okunamazsa 0 dondurur
*/

int readGraph(const char *filename, char **nodes, int *nodeCount, Edge **edges, int *edgeCount) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return 0;
    }

    char line[256];
    *nodeCount = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] != '\n') {
            (*nodeCount)++;
        }
    }

    *nodes = (char *)malloc(*nodeCount * sizeof(char));

    fseek(file, 0, SEEK_SET);
    int nodeIndex = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] != '\n') {
            (*nodes)[nodeIndex++] = line[0];
        }
    }

    fseek(file, 0, SEEK_SET);
    *edgeCount = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] != '\n') {
            char *token = strtok(line + 2, ",;");
            while (token != NULL) {
                if (*token != '\n' && *token != ' ') {
                    (*edgeCount)++;
                }
                token = strtok(NULL, ",;");
            }
        }
    }

    *edges = (Edge *)malloc(*edgeCount * sizeof(Edge));

    fseek(file, 0, SEEK_SET);
    int edgeIndex = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] != '\n') {
            char *token = strtok(line + 2, ",;");
            while (token != NULL) {
                if (*token != '\n' && *token != ' ') {
                    Edge edge;
                    edge.start = line[0];
                    edge.end = *token;
                    edge.count = 0;  // Initialize count to 0
                    (*edges)[edgeIndex++] = edge;
                }
                token = strtok(NULL, ",;");
            }
        }
    }

    fclose(file);

    return 1;
}

/*
@brief Verilen dugum ve kenar listesini ekrana yazdirir

@param char *nodes: dugum listesi
@param int nodeCount: dugum sayisi
@param Edge *edges: kenar listesi
@param int edgeCount: kenar sayisi

@return void
*/

void printGraph(char *nodes, int nodeCount, Edge *edges, int edgeCount) {
	int i, j, k;
    printf("Nodes: ");
    for (i = 0; i < nodeCount; ++i) {
        printf("%c", nodes[i]);
        if (i < nodeCount - 1) {
            printf(", ");
        }
    }
    printf("\n");

    printf("Edges: \n");
    for (i = 0; i < edgeCount; ++i) {
        printf("%c-%c\n", edges[i].start, edges[i].end);
    }
}

/*
@brief Verilen kenari kenar listesinden siler

@param Edge *edges: kenar listesi
@param int *edgeCount: kenar sayisi
@param int index: silinecek kenarin indisi

@return void
*/

void removeEdge(Edge *edges, int *edgeCount, int index) {
	int i;
    if (index >= *edgeCount) {
        // Invalid index
        return;
    }

    // Shift elements to fill the gap
    for (i = index; i < *edgeCount - 1; ++i) {
        edges[i] = edges[i + 1];
    }

    // Decrease the edge count
    (*edgeCount)--;
}

/*
@brief Verilen kenar listesinden topluluklari bulur

@param Edge *edges: kenar listesi
@param int edgeCount: kenar sayisi
@param char *nodes: dugum listesi
@param int nodeCount: dugum sayisi
@param int minCommunitySize: minimum topluluk boyutu
@param int iterations: olasi topluluk bulma sayisi

@return void
*/

void detectCommunities(Edge *edges, int edgeCount, char *nodes, int nodeCount, int minCommunitySize, int iterations) {
	int i, j, k, l, iter;
    for (iter = 0; iter < iterations; ++iter) {
        bfs(nodes, nodeCount, edges, edgeCount);

        int maxOccurrence = -1;
        int maxIndex = 0;

        for (i = 0; i < edgeCount; ++i) {
            int occurrenceCount = 0;
            for (j = 0; j < edgeCount; ++j) {
                for (k = 0; k < edges[j].count; ++k) {
                    char *path = edges[j].paths[k];
                    char *edgeStart = strchr(path, edges[i].start);
                    char *edgeEnd = strchr(path, edges[i].end);

                    if (edgeStart != NULL && edgeEnd != NULL && edgeStart < edgeEnd) {
                        occurrenceCount++;
                    }
                }
            }

            if (occurrenceCount > maxOccurrence) {
                maxOccurrence = occurrenceCount;
                maxIndex = i;
            }
        }
        removeEdge(edges, &edgeCount, maxIndex);
        printf("\n");
        printGraph(nodes, nodeCount, edges, edgeCount);

        // Detect communities
        Community communities[nodeCount];
        for (i = 0; i < nodeCount; ++i) {
            communities[i].nodes = (char *)malloc(nodeCount * sizeof(char));
            communities[i].size = 0;
        }

        for (i = 0; i < edgeCount; ++i) {
            for (j = 0; j < edges[i].count; ++j) {
                char *path = edges[i].paths[j];
                char *edgeStart = strchr(path, edges[i].start);
                char *edgeEnd = strchr(path, edges[i].end);

                if (edgeStart != NULL && edgeEnd != NULL && edgeStart < edgeEnd) {
                    // Edge is in the path
                    int startIndex = indexOfNode(nodes, nodeCount, edges[i].start);
                    int endIndex = indexOfNode(nodes, nodeCount, edges[i].end);

                    // Add nodes to their respective communities only if they are not already added
                    if (indexOfNode(communities[startIndex].nodes, communities[startIndex].size, edges[i].start) == -1) {
                        communities[startIndex].nodes[communities[startIndex].size++] = edges[i].start;
                    }
                    if (indexOfNode(communities[startIndex].nodes, communities[startIndex].size, edges[i].end) == -1) {
                        communities[startIndex].nodes[communities[startIndex].size++] = edges[i].end;
                    }

                    if (indexOfNode(communities[endIndex].nodes, communities[endIndex].size, edges[i].start) == -1) {
                        communities[endIndex].nodes[communities[endIndex].size++] = edges[i].start;
                    }
                    if (indexOfNode(communities[endIndex].nodes, communities[endIndex].size, edges[i].end) == -1) {
                        communities[endIndex].nodes[communities[endIndex].size++] = edges[i].end;
                    }
                }
            }
        }

        // Remove duplicate nodes in communities
        for (i = 0; i < nodeCount; ++i) {
            for (j = 0; j < communities[i].size; ++j) {
                for (k = j + 1; k < communities[i].size; ++k) {
                    if (communities[i].nodes[j] == communities[i].nodes[k]) {
                        // Remove duplicate node
                        for (l = k; l < communities[i].size - 1; ++l) {
                            communities[i].nodes[l] = communities[i].nodes[l + 1];
                        }
                        communities[i].size--;
                        k--;
                    }
                }
            }
        }

        // Print communities
        printf("Possible Communities:\n");
        int communityCount = 0;
        for (i = 0; i < nodeCount; ++i) {
            if (communities[i].size > 0) {
                // Check if the community is already printed
                int isPrinted = 0;
                for (j = 0; j < i; ++j) {
                    if (communities[i].size == communities[j].size) {
                        int match = 1;
                        for (k = 0; k < communities[i].size; ++k) {
                            if (indexOfNode(communities[j].nodes, communities[j].size, communities[i].nodes[k]) == -1) {
                                match = 0;
                                break;
                            }
                        }
                        if (match) {
                            isPrinted = 1;
                            break;
                        }
                    }
                }

                if (!isPrinted) {
                    printf("Community %zu: ", ++communityCount);
                    for (j = 0; j < communities[i].size; ++j) {
                        printf("%c", communities[i].nodes[j]);
                        if (j < communities[i].size - 1) {
                            printf(", ");
                        }
                    }
                    printf("\n");
                }
            }
        }
        printf("\n");
        // Free allocated memory for communities
        for (i = 0; i < nodeCount; ++i) {
            free(communities[i].nodes);
        }
    }
}

/*
@brief Programin baslangic noktasi

@return int: program basarili bir sekilde sonlanirsa 0, hata olusursa 1 dondurur
*/

int main() {
    char *nodes;
    int nodeCount;
    Edge *edges;
    int edgeCount;
	int i, j;

    // Komsuluk listesini verilen giris dosyasindan okuyoruz
    if (!readGraph("input.txt", &nodes, &nodeCount, &edges, &edgeCount)) {
        fprintf(stderr, "Dosya okuma hatasi\n");
        return 1;
    }

    // Ayni kenarlarin tekrar edilmesini engelliyoruz
    removeDuplicateEdges(edges, &edgeCount);

    // Dugum ve kenar listesini ekrana yazdiriyoruz
    printGraph(nodes, nodeCount, edges, edgeCount);

    // T ve K degerlerini kullanicidan aliyoruz

    printf("Enter the minimum number of community members (T): \n");
    int T;
    scanf("%d", &T);

    printf("Enter the number of iterations (K): \n");
    int K;
    scanf("%d", &K);

    printf("Number of edges: %zu, Number of nodes: %zu\n", edgeCount, nodeCount);
    printf("\n");
    // TopluluklarÄ± buluyoruz
    detectCommunities(edges, edgeCount, nodes, nodeCount, T, K);

    // Tahsis edilen bellekleri serbest birakiyoruz
    free(nodes);
    free(edges);
    return 0;
}
