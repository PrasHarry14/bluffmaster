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

typedef struct {
    Card pile[DECK_SIZE];
    int pileCount;
    int currentPlayer;
    int currentRank;
    int lastPlayer;
    int lastPlayCount;
} Game;


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
int removeCard(Player *player, int index){
    if(index<0 || index>=player->cardCount){
        return 0;
    }
    for (int i = index; i < player->cardCount - 1; i++) {
        player->hand[i] = player->hand[i + 1];
    }
    player->cardCount--;
    return 1;

}
int nextPlayer(int currentPlayer){
    return (currentPlayer + 1) % PLAYERS;
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

void showHand(Player *player) {
    printf("\n=== YOUR HAND ===\n");

    for (int i = 0; i < player->cardCount; i++) {
        printf("%d. ", i + 1);
        printCard(player->hand[i]);
    }
}

const char *rankName(int rank) {
    static const char *names[] = {
        "Ace", "2", "3", "4", "5", "6", "7",
        "8", "9", "10", "Jack", "Queen", "King"
    };
    if (rank < 1 || rank > 13) {
        return "Invalid";
    }
    return names[rank - 1];
}
void startRound(Game*game,Player players[]){
    int chosenRank;
    printf("\n=== NEW ROUND ===\n");
    printf("%s is starting this round!\n",
           players[game->currentPlayer].name);

    if (game->currentPlayer == 0) {
        do {
            printf("Choose a rank (1-13): ");

            if (scanf("%d", &chosenRank) != 1) {
                printf("Please enter a number.\n");

                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF) {
                }

                chosenRank = 0;
                continue;
            }

            if (chosenRank < 1 || chosenRank > 13) {
                printf("Invalid rank! Choose between 1 and 13.\n");
            }

        } while (chosenRank < 1 || chosenRank > 13);
    } else {
        chosenRank = rand() % 13 + 1;
    }

    game->currentRank = chosenRank;
    game->lastPlayer = -1;
    game->lastPlayCount = 0;

    printf("%s chose %s!\n",
           players[game->currentPlayer].name,
           rankName(game->currentRank));
}

void playCards(Game *game, Player players[]) {
    Player *player = &players[game->currentPlayer];

    int count;
    int selected[4];

    // Step 1: Display the player's hand
    showHand(player);

    // Step 2: Ask how many cards to play
    while (1) {
        printf("\nHow many cards do you want to play (1-4)? ");

        if (scanf("%d", &count) != 1) {
            printf("Invalid input! Enter a number.\n");

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
            continue;
        }

        if (count >= 1 && count <= 4 &&
            count <= player->cardCount &&
            count <= DECK_SIZE - game->pileCount) {
            break;
        }

        printf("Invalid number of cards! Try again.\n");
    }

    // Step 3: Ask which cards to play
    for (int i = 0; i < count; i++) {
        int choice;

        printf("Choose card %d (enter its number): ", i + 1);

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Enter a number.\n");

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }

            i--;
            continue;
        }

        // Check whether the card number exists
        if (choice < 1 || choice > player->cardCount) {
            printf("Invalid card number. Try again.\n");
            i--;
            continue;
        }

        // Check whether this card was already selected
        int duplicate = 0;

        for (int j = 0; j < i; j++) {
            if (selected[j] == choice - 1) {
                duplicate = 1;
                break;
            }
        }

        if (duplicate) {
            printf("You already selected that card. Try again.\n");
            i--;
            continue;
        }

        selected[i] = choice - 1;
    }

    // Step 4: Copy selected cards into the pile
    for (int i = 0; i < count; i++) {
        game->pile[game->pileCount] = player->hand[selected[i]];
        game->pileCount++;
    }

    // Step 5: Sort selected indices from highest to lowest
    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            if (selected[i] < selected[j]) {
                int temp = selected[i];
                selected[i] = selected[j];
                selected[j] = temp;
            }
        }
    }

    // Step 6: Remove selected cards from the player's hand
    for (int i = 0; i < count; i++) {
        removeCard(player, selected[i]);
    }

    // Step 7: Record the last play
    game->lastPlayer = game->currentPlayer;
    game->lastPlayCount = count;

    // Step 8: Announce the play without revealing the cards
    printf("\n%s placed %d card(s) face down!\n",
           player->name, count);

    printf("%s claims to have played %d %s(s).\n",
           player->name, count, rankName(game->currentRank));

    printf("Cards remaining: %d\n", player->cardCount);
    printf("Cards in pile: %d\n", game->pileCount);

    // Step 9: Move to the next player
    game->currentPlayer = nextPlayer(game->currentPlayer);
}
int findCard(Player*player, int rank,int wantMatch){
    for(int i=0;i<player->cardCount;i++){
        if(wantMatch&&player->hand[i].rank==rank){
            return i;
        }
        if (!wantMatch && player->hand[i].rank != rank) {
            return i;
    }
}
return -1;
}

void computerPlay(Game *game, Player players[]) {
    int playerIndex = game->currentPlayer;

    // This function is only for computer players
    if (playerIndex == 0) {
        return;
    }

    Player *player = &players[playerIndex];

    if (player->cardCount == 0 ||
        game->currentRank < 1 ||
        game->currentRank > 13 ||
        game->pileCount >= DECK_SIZE) {
        return;
    }

    int wantMatch;

    // HARVEY: 80% preference for truthful play
    if (playerIndex == 1) {
        wantMatch = (rand() % 100 < 80);
    }

    // MIKE: 70% preference for bluffing
    else if (playerIndex == 2) {
        wantMatch = (rand() % 100 < 30);
    }

    // JOE: 50/50
    else {
        wantMatch = (rand() % 100 < 50);
    }

    // Search for a suitable card
    int cardIndex = findCard(
        player,
        game->currentRank,
        wantMatch
    );

    // If no suitable card exists, try the opposite
    if (cardIndex == -1) {
        cardIndex = findCard(
            player,
            game->currentRank,
            !wantMatch
        );
    }

    if (cardIndex == -1) {
        return;
    }

    // Move the card into the face-down pile
    game->pile[game->pileCount] = player->hand[cardIndex];
    game->pileCount++;

    // Remove the card from the computer's hand
    removeCard(player, cardIndex);

    // Record the play for future BLUFF challenges
    game->lastPlayer = playerIndex;
    game->lastPlayCount = 1;

    // Announce only the claim, not the actual card
    printf("\n%s placed 1 card face down.\n", player->name);
    printf("%s claims to have played one %s.\n",
           player->name,
           rankName(game->currentRank));

    printf("%s has %d cards remaining.\n",
           player->name,
           player->cardCount);

    printf("Cards in pile: %d\n", game->pileCount);

    // Move to the next player
    game->currentPlayer = nextPlayer(game->currentPlayer);
}
int checkBluff(Game*game){
    int start=game->pileCount - game->lastPlayCount;

    // Check only the cards from the last play
    for (int i = start; i < game->pileCount; i++) {

        if (game->pile[i].rank != game->currentRank) {
            return 1; // Bluff detected
        }
    }

    return 0; // All cards match the claimed rank
}

void givePile(Game *game, Player *player) {

    for (int i = 0; i < game->pileCount; i++) {
        addCard(player, game->pile[i]);
    }

    game->pileCount = 0;
}

void callBluff(Game *game, Player players[], int challenger) {

    // Nobody can challenge before the first play
    if (game->lastPlayer == -1 || game->lastPlayCount == 0) {
        printf("There is no play to challenge!\n");
        return;
    }

    // The player cannot challenge their own play
    if (challenger == game->lastPlayer) {
        printf("You cannot challenge yourself!\n");
        return;
    }

    int accused = game->lastPlayer;

    printf("\n=== BLUFF CALLED! ===\n");
    printf("%s challenges %s!\n",
           players[challenger].name,
           players[accused].name);

    // Reveal only the last played cards
    int start = game->pileCount - game->lastPlayCount;

    printf("\nRevealed cards:\n");

    for (int i = start; i < game->pileCount; i++) {
        printCard(game->pile[i]);
    }

    int bluffDetected = checkBluff(game);

    int winner;
    int loser;

    if (bluffDetected) {
        printf("\nBLUFF DETECTED!\n");

        winner = challenger;
        loser = accused;
    } else {
        printf("\nThe cards were truthful!\n");

        winner = accused;
        loser = challenger;
    }

    printf("%s loses and picks up the pile!\n",
           players[loser].name);

    givePile(game, &players[loser]);

    // The winner starts the next round
    game->currentPlayer = winner;

    // Reset the round information
    game->currentRank = 0;
    game->lastPlayer = -1;
    game->lastPlayCount = 0;

    printf("\n%s wins the challenge!\n",
           players[winner].name);

    printf("%s will choose the next rank.\n",
           players[winner].name);
}





int main(void){
    Card deck[DECK_SIZE];
    Player players[PLAYERS]={
    {"YOU",{{0}},0},
    {"HARVEY",{{0}},0},
    {"MIKE",{{0}},0},
    {"JOE",{{0}},0}
};
    Game game = {
        .pileCount = 0,
        .currentPlayer = 0,
        .currentRank = 0,
        .lastPlayer = -1,
        .lastPlayCount = 0
    };

    srand((unsigned int)time(NULL));
    printf("Welcome to BLUFF MASTER!\n");
    initializeDeck(deck);
    printf("\n52 cards successfully created!\n");
    shuffleDeck(deck);
    printf("Deck shuffled successfully!\n");
    dealCards(deck, players);
    printf("Cards dealt successfully!\n");
    showHand(&players[0]);

printf("\n=== GAME STATE ===\n");
printf("Current player: %s\n", players[game.currentPlayer].name);
printf("Current rank: %d\n", game.currentRank);
printf("Cards in pile: %d\n", game.pileCount);
printf("Last player: %d\n", game.lastPlayer);
printf("Last play count: %d\n", game.lastPlayCount);
startRound(&game, players);
printf("\n=== UPDATED GAME STATE ===\n");
printf("Current player: %s\n", players[game.currentPlayer].name);
printf("Current rank: %s\n", rankName(game.currentRank));
printf("Cards in pile: %d\n", game.pileCount);


playCards(&game, players);

printf("\n=== COMPUTER TURNS ===\n");

for (int i = 0; i < 3; i++) {
    computerPlay(&game, players);
}

printf("\n=== AFTER ALL FOUR PLAYERS ===\n");
printf("Next player: %s\n", players[game.currentPlayer].name);
printf("Cards in pile: %d\n", game.pileCount);


printf("\n=== TESTING BLUFF CHALLENGE ===\n");
callBluff(&game, players, 0);

printf("\n=== AFTER CHALLENGE ===\n");
printf("Next round starter: %s\n",
       players[game.currentPlayer].name);
printf("Cards in pile: %d\n", game.pileCount);
printf("Current rank: %d\n", game.currentRank);

return 0;
}

