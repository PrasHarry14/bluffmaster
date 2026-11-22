#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define DECK_SIZE 52
#define PLAYERS 4
#define CARDS_PER_PLAYERS 13
#define MAX_HANDS 52
typedef struct{ //we're talking abt one playing card here
    int rank; //ACE=1,NORMAL=2-10,JACK=11,QUEEN=12,KING=13
    int suit; //HEARTS=0,DIAMONDS=1,CLUBS=2,SPADES=3
} Card;
typedef struct{
    const char *name;
    Card hand[MAX_HANDS];
    int cardCount;
}Player;

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
void shuffleDeck(Card deck[]){
    for(int i=DECK_SIZE-1;i>0;i--){
        int j=rand()%(i+1);
        Card Temp=deck[i];
        deck[i]=deck[j];
        deck[j]=Temp;

    }
}
int addCard(Player *Player, Card card){
    if(Player->cardCount>=MAX_HANDS){
        return 0;
    }
    Player->hand[Player->cardCount] = card;
    Player->cardCount++;

    return 1;
}
void dealCards(Card deck[], Player players[]){
    int index = 0;

    for (int player = 0; player < PLAYERS; player++) {
        for (int card = 0; card < CARDS_PER_PLAYERS; card++) {
            addCard(&players[player], deck[index]);
            index++;




        }


    }

}
void printCard(Card card){
    char *suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    char *ranks[] = {"Ace", "2", "3", "4", "5", "6", "7","8", "9", "10", "Jack", "Queen", "King" };
    printf("%s of %s\n", ranks[card.rank-1],suits[card.suit]);
}


int main(void){
    Card deck[DECK_SIZE];
    Player players[PLAYERS]={
    {"YOU",{{0}},0},
    {"HARVEY",{{0}},0},
    {"MIKE",{{0}},0},
    {"JOE",{{0}},0}
};

    srand((unsigned int)time(NULL));
    printf("Welcome to BLUFF MASTER!\n");
    initializeDeck(deck);
    printf("\n52 cards successfully created!\n");
    shuffleDeck(deck);
    printf("Deck shuffled successfully!\n");
    dealCards(deck, players);
    printf("Cards dealt successfully!\n");
     for (int player = 0; player < PLAYERS; player++) {

        printf("\n-----------------------------\n");
        printf("%s's cards(%d):\n",players[player].name,players[player].cardCount);
        printf("-----------------------------\n");

        for (int card = 0; card < players[player].cardCount; card++) {
            printCard(players[player].hand[card]);
        }
    }

    return 0;
}

