#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Song{
    char *genre;
    char *artist;
    char *title;
    char *album;
    char *trackid;
    short popularity;
    bool shown;
} Song;

typedef struct SongArrayList {
    Song *songs;         
    size_t size;         
    size_t capacity;     
} SongArrayList;

typedef struct {
    char *genre;         
    int count;
} GenreInfo;

typedef struct LinkListNode {
    GenreInfo *data;
    struct LinkListNode *next;
} LinkListNode;


//bool readInSongs(SongArrayList *songList){};
//void printSongList(SongArrayList * songList, int starting_index);
//void generateUniqueGenres(LinkListNode ** ptrUniqueGenresLL, SongArrayList songList){}

// *************************************************************************************
// CODE: Copy in functions from prior assignments for Array Lists and Link Lists.
//       Modify those functions to handle the structures used in this assignment
// *************************************************************************************

//ArrayList

SongArrayList* createArrayList(int initialCapacity) {
    SongArrayList *list = (SongArrayList*)malloc(sizeof(Song));

    list->songs = (Song*)malloc(initialCapacity * sizeof(Song));
    
    list->size = 0;
    list->capacity = initialCapacity;

    return list;

}

void addElement(SongArrayList* list, Song value) {
    if (list->size == list->capacity) {
        int newCapacity = list->capacity +=10;
        Song *newData = list->songs = (Song *)realloc(list->songs, list->capacity * sizeof(Song));
    

        if (newData == NULL){
            return;
        }
        list->songs = newData;
        list->capacity = newCapacity;
    }
    list->songs[list->size] = value;
    list->size++;
}


void removeLastElement(SongArrayList* list) {
    if (list->size > 0) {
        int newCapacity;
        Song *newData;

        list->size--;

        if (list->size < list->capacity / 4 && list->capacity / 2 >= 2) {
            newCapacity = list->capacity / 2;
            newData = (Song *)realloc(list->songs, newCapacity * sizeof(Song));

        }
        if (newData != NULL) {
            list->songs = newData;
            list->capacity = newCapacity;
        }
    }

}

Song getElement(SongArrayList* list, int index){
    if (index < 0 || index >= list->size) {
        
        printf("Error: Index out of bounds\n");
        return list->songs[0];
    }

    return list->songs[index];
}

void freeArrayList(SongArrayList* list){
    free(list->songs);
    free(list);
}

//LinkedList

LinkListNode* head;

void addLinkElement(GenreInfo *data) {
    LinkListNode* newNode = (LinkListNode*)malloc(sizeof(LinkListNode));\
    
    newNode->data = data;
    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
        return;
    }

    LinkListNode* current = head;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = newNode;

}

void addElementInTheNthPosition(GenreInfo *data, int position) {
    
    LinkListNode* newNode = (LinkListNode*)malloc(sizeof(LinkListNode));

    newNode->data = data;
    newNode->next = NULL;

    if (head == NULL && position == 1) {
        head = newNode;
        return;
    }

    if (position == 1) {
        newNode->next = head;
        head = newNode;
        return;
    }

    LinkListNode* current = head;
    for (int i = 1; i < position - 1 && current != NULL; i++) {
        current = current->next;
    }

    if (current == NULL) {
        printf("Invalid Operation");
        return;
    }

    newNode->next = current->next;
    current->next = newNode;

}

void removeLastLinkElement() {
    
    if (head == NULL) {
        printf("List empty");
        return;
    }


    LinkListNode* current = head;
    while (current->next->next != NULL) {
        current = current->next;
    }

    free(current->next);
    current->next = NULL;

}

void removeNthElement(int position) {

    if (head == NULL) {
        printf("List empty");
        return;
    }

    if (position == 1) {
        LinkListNode* var = head;
        head = head->next;
        free(var);
        return;
    }
    
    LinkListNode* current = head;
    for (int i = 1; i < position -1 && current != NULL; i++) {
        current = current->next;
    }

    if (current == NULL || current->next == NULL) {
        printf("Invalid operation");
        return;
    }

    LinkListNode* var = current->next;
    current->next = var->next;
    free(var);

}

int searchElementByValue(char *genre) {

    LinkListNode* current = head;
    while (current != NULL) {
        if (strcmp(current->data->genre, genre) == 0) {

            return 1;
        }

        current = current->next;
    }
    return 0;
}

void printLinkList() {
    LinkListNode* current = head;
    printf("\n");

    while (current != NULL) {
        printf("%s", current->data->genre);

        current = current->next;
    }
    printf("\n");
}
//end of data structures


Song songs[3000];

bool isHeader(const char *line) {
    return (strstr(line, "genre") != NULL);
}

bool readInSongs(SongArrayList *songList) {
    
    char line[1000] = "";
    FILE *file = fopen("songs.txt", "r");
    int counter = 0;

    while (fgets(line, sizeof(line), file)) {
        if (isHeader(line)) {
            continue;
        }
        Song s;

        char *token = strtok(line, "\t");
        s.genre = strdup(token);

        token = strtok(NULL, "\t");
        s.artist = strdup(token);
        token = strtok(NULL, "\t");
        s.title = strdup(token);
        token = strtok(NULL, "\t");
        s.trackid = strdup(token);
        token = strtok(NULL, "\t");
        s.popularity = atoi(token);
        
        addElement(songList, s);
        counter++;

    }
    printf("%d\n", counter);
    fclose(file);
    return true;
}

void printSongList(SongArrayList *songList, int starting_index) {

    for (int i = starting_index; i < (starting_index+10); i++) {
        printf("\t%d. %s - %s - %s \n", i+1, songList->songs[i].genre, songList->songs[i].artist, songList->songs[i].title);
    }
}

void generateUniqueGenres(LinkListNode **ptrUniqueGenresLL, SongArrayList songList) {
    
    for (size_t i = 0; i < songList.size; i++) {
        GenreInfo *currentGenre = (GenreInfo *)malloc(sizeof(GenreInfo));
        currentGenre->genre = strdup(songList.songs[i].genre);
        currentGenre->count = 0;

        if (searchElementByValue(currentGenre->genre) == 1) {
            LinkListNode *current = head;
            
            while (current != NULL) {
                if (strcmp(current->data->genre, songList.songs[i].genre) == 0) {
                    current->data->count++;
                    break;
                }
                current = current->next;
            }
        } else {
            GenreInfo *genreInfo = (GenreInfo *)malloc(sizeof(GenreInfo));
            genreInfo->genre = strdup(songList.songs[i].genre);
            genreInfo->count = 1;

            addLinkElement(genreInfo);
        }  
    }
    *ptrUniqueGenresLL = head;
} 

void printUniqueGenres(LinkListNode *uniqueGenresLL) {
    LinkListNode *current = uniqueGenresLL;

    printf("Unique Genres:\n");
    while (current != NULL) {
        printf("%s    %d\n", current->data->genre, current->data->count);
        current = current->next;
    }
}

int main(){
    
    // CODE: probably need to initialize songList
    SongArrayList* songList = createArrayList(50);
    if (readInSongs(songList)) {
    
        LinkListNode *headUniqueGenreLL = NULL;
        //int uniqueCount = 0;

        generateUniqueGenres(&headUniqueGenreLL, *songList);

        printUniqueGenres(headUniqueGenreLL);

        bool stop = false;
        char input;
        int currentIndex = 0;
        //bool filterEnabled = true;

        while (!stop) {


            printSongList(songList, currentIndex);
            printf("Navigation: n for next, p for previous, q to quit, s for start, e for end, r for reset filter\n");
            scanf(" %c", &input);


            switch (input) {
            case 'n':
                // Next 10
                if (currentIndex < 40) {
                    currentIndex += 10;
                } else {
                    break;
                }
                printSongList(songList, currentIndex);
                break;
            case 'p':
                // Previous 10
                if (currentIndex > 10) {
                    currentIndex -=10;
                } else {
                    break;
                }
                printSongList(songList, currentIndex);
                break;
            case 'q':
                // Quit
                stop = true;
                break;
            case 's':
                // Move to start
                currentIndex = 0;
                printSongList(songList, currentIndex);
                break;
            case 'e':
                // Move to end
                currentIndex = 2448-10;
                printSongList(songList, currentIndex);
                break;
            case 'r':
                // Reset
                //filterEnabled = false;
                break;
            case '0': 

                break;
            case '1': 

                break;
            case '2': 

                break;
            case '3': 

                break;
            case '4': 

                break;
            case '5': 

                break;
            case '6': 

                break;
            case '7': 

                break;
            case '8': 

                break;
            case '9':

                break;
            default:
                printf("Invalid choice\n");
                break;
            }

        }


    }

    
    return 0;
}
