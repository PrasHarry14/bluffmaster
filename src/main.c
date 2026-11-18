#include<stdio.h>
#define DECK_SIZE 52
typedef struct{ //we're talking abt one playing card here
    int rank; //ACE=1,NORMAL=2-10,JACK=11,QUEEN=12,KING=13
    int suit; //HEARTS=0,DIAMONDS=1,CLUBS=2,SPADES=3
} Card;
void initializeDeck(Card deck[]) {
    int index=0;
    for(int suit=0;suit<4;suit++){
        for(int rank=1;rank<=13;rank++){
            deck[index].rank=rank;
            deck[index].suit=suit;
            index++;
        }
    }
}
void printCard(Card card){
    char *suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    char *ranks[] = {"Ace", "2", "3", "4", "5", "6", "7","8", "9", "10", "Jack", "Queen", "King" };
    printf("%s of %s\n", ranks[card.rank-1],suits[card.suit]);
}
void printDeck(Card deck[]) {
    for (int i = 0; i < DECK_SIZE; i++) {
        printCard(deck[i]);
    }
}
int main(void){
    Card deck[DECK_SIZE];
    printf("Welcome to Bluff Master!\n");
    printf("Initializing deck...\n\n");
    initializeDeck(deck);
    printf("Deck successfully created!\n");
    printf("Total cards: %d\n\n", DECK_SIZE);
    printDeck(deck); 
    return 0;
}