/* ### Created by Vecnik88 - POKER
 *
 *      Программа представляет собой покер, с возможностью замены двух карт.
 *      Игроки могут делать ставки от 1 до 5, им выдается по 5 карт, после чего
 *      игрок решает какие карты оставить, какие заменить.
 *      После этого происходит оценка карт игрока, изменяется его счет, показываетяся его счет
 *      и игра продолжается.
 *
 *
 *      The program is a poker replaceably two cards.
 *      Players can bet from 1 to 5, they are given 5 cards, and then
 *      Player chooses which cards to hold, which replaced.
 *      This is followed by evaluation of the player's cards, changed his account, his account pokazyvaetyasya
 *      And the game continues.
 *
 *      Цели для версии 2.0 ### Написать графический интерфейс в будущем ###
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define FALSE 0                 // <---. две константы, для определения имеется на руках у игрока
#define TRUE 1                  //       флеш или стрит

void printGreeting();
int getBet();
char getSuit(int suit);
char getRank(int rank);
void getFirstHand(int cardRank[], int cardSuit[]);
void getFinalHand(int cardRank[], int cardSuit[], int finalRank[],
                  int finalSuit[], int ranksinHand[], int suitsinHand[]);
int analyzeHand(int ranksinHand[], int suitsinHand[]);

int main()
{
   int bet;
   int bank = 100;
   int i;
   int cardRank[5];             // <---. одно из 13 значений(туз - король)
   int cardSuit[5];             // <---. одно из 4-х значений(пик, бубен, треф, червей)

   int finalRank[5];
   int finalSuit[5];
   int ranksinHand[13];         // <---. используем для последней раздачи
   int suitsinHand[4];          // <---. используется для последней раздачи

   int winnings;
   time_t t;                    // <---. время прошедшее с появления UNIX, 1.01.1970

   char suit, rank, stillPlay;

   printGreeting();             // <---. приветствие игрока

   // ### цикл do while запускается каждый раз когда игрок начинает новую игру ##
   do
   {
       bet = getBet();
       srand(time(&t));
       getFirstHand(cardRank, cardSuit);
       printf("Your five cards\n");

       for(i = 0; i < 5; ++i)
       {
           suit = getSuit(cardSuit[i]);
           rank = getRank(cardRank[i]);
           printf("Card nomer %d: %c %c\n", i+1, rank, suit);
       }

       for(i = 0; i < 4; ++i)               // <---. обнуляем массивы, которые оценивают карты игрока
       {                                    //       если игрок потребует несколько раздач
           suitsinHand[i] = 0;
       }
       for(i = 0; i < 13; ++i)
       {
           ranksinHand[i] = 0;
       }

       getFinalHand(cardRank, cardSuit, finalRank, finalSuit, ranksinHand, suitsinHand);

       printf("Your last series of cards\n");
       for(i = 0; i < 5; ++i)
       {
           suit = getSuit(cardSuit[i]);
           rank = getRank(cardRank[i]);
           printf("Card nomer %d: %c %c\n", i+1, rank, suit);
       }
       winnings = analyzeHand(ranksinHand, suitsinHand);
       printf("You win %d!", bet*winnings);
       bank = bank - bet + (bet*winnings);
       printf("In your bank now %d.\n", bank);
       printf("You want to play again?(y/n)\n");
       scanf(" %c", &stillPlay);
   }while(toupper(stillPlay) == 'Y');

return 0;
}

//###############################################################################################################

// Приветствие, обзор правил

void printGreeting()
{
    printf("********************************************************************************\n");
    printf("\t\t\tWELCOME TO CASINO\n\n");
    printf("********************************************************************************\n");
    printf("Specification:\n");
    printf("Your balance 100 credits, ");
    printf("you have to bet from 1 to 5 credits.\n");
    printf("You given 5 cards, you must choose which cards to hold and which to lose\n");
    printf("You need to make the best combination\n");
    printf("\n\t\tGood Luck bro!\n\n");
}

// ф-ция для выдачи первых 5-ти карт
void getFirstHand(int cardRank[], int cardSuit[])
{
    int i, j;
    int cardDup;
    for(i = 0; i < 5; ++i)
    {
        cardDup = 0;
        do
        {
            cardRank[i] = (rand()%13);          // <---. одна из 13 карт (2-10, В, Д, К, Т)
            cardSuit[i] = (rand()&4);           // <---. пики, бубны, трефы, червы

            for(j = 0; j < i; ++j)              // <---. гарантирует чтобы не было одинаковых карт
                if(cardRank[i]==cardRank[j] && cardSuit[i]==cardSuit[j]) cardDup = 1;

        }while(cardDup == 1);
    }
}

// ф-ция заменяющая цифру на символ масти карты
char getSuit(int suit)
{
    switch(suit)
    {

    case 0:
        return ('p');                             // <---. пики
    case 1:
        return ('b');                             // <---. бубны
    case 2:
        return ('c');                             // <---. червы
    case 3:
        return ('t');                             // <---. трефы(крести)
    }
}

// ф-ция заменяющая цифру на номинал карты
char getRank(int rank)
{
    switch(rank)
    {
    case 0:
        return ('T');                              // <---. заменяет на туз
    case 1:
        return ('2');                              // <---. заменяет на 2
    case 2:
        return ('3');                              // <---. заменяет на 3
    case 3:
        return ('4');                              // <---. заменяет на 4
    case 4:
        return ('5');                              // <---. заменяет на 5
    case 5:
        return ('6');                              // <---. заменяет на 6
    case 6:
        return ('7');                              // <---. заменяет на 7
    case 7:
        return ('8');                              // <---. заменяет на 8
    case 8:
        return ('9');                              // <---. заменяет на 9
    case 9:
        return ('10');                             // <---. заменяет на 10
    case 10:
        return ('V');                              // <---. заменяет на вальта
    case 11:
        return ('D');                              // <---. заменяет на даму
    case 12:
        return ('K');                              // <---. заменяет на короля
    }
}

// получает ставку от пользователя

int getBet()
{
    int bet;
    // пока пользователь не введет от 0 до 5 будет повторяться
    do
    {
        printf("How much you want to bet?(Enter value for 1 to 5 or 0 about exit on game) ");
        scanf(" %d", &bet);
        if(bet>0 && bet<6) return bet;
        else if(bet == 0) exit(1);
        else
        printf("How much you want to bet?(Enter value for 1 to 5 or 0 about exit on game) ");
    }while(bet<0 || 5<bet);
}

// производит оценку последней выданной игроку партии карт

int analyzeHand(int ranksinHand[], int suitsinHand[])
{
    int num_consec = 0;
    int i, rank, suit;
    int straight = FALSE;
    int flush = FALSE;
    int four = FALSE;
    int three = FALSE;
    int pairs = 0;

    for(suit = 0; suit < 4; ++suit)
        if(suitsinHand[suit]==5)
            flush = TRUE;
    rank = 0;
    while(ranksinHand[rank]==0)
        ++rank;
    for(; rank<13 && ranksinHand[rank]; ++rank)
        ++num_consec;
    if(num_consec==5) straight = TRUE;

    for(rank = 0; rank < 13; ++rank)
    {
        if(ranksinHand[rank]==4) four = TRUE;
        if(ranksinHand[rank]==3) three = TRUE;
        if(ranksinHand[rank]==2) ++pairs;
    }
    if(straight && flush)
    {
        printf("Flash-roal\n\n");
        return 20;
    }
    else if(four)
    {
        printf("Four on one type\n\n");
        return 10;
    }
    else if(three && pairs)
    {
        printf("Full-house\n\n");
        return 8;
    }
    else if (flush)
    {
        printf("Flash\n\n");
        return 5;
    }
    else if(straight)
    {
        printf("Street\n\n");
        return 4;
    }
    else if(three)
    {
        printf("Three of one type\n\n");
        return 3;
    }
    else if(pairs==2)
    {
        printf("Two pairs\n\n");
        return 2;
    }
    else if(pairs == 1)
    {
        printf("Pairs\n\n");
        return 1;
    }
    else
    {
        printf("High card");
        return 0;
    }
}

// ф-ция просматривает все 5 карт и спрашивает пользователя хочет ли он оставить карту

void getFinalHand(int cardRank[], int cardSuit[], int finalRank[], int finalSuit[], int ranksinHand[], int suitsinHand[])
{
    int i, j, cardUp;
    char suit, rank, ans;
    for(i = 0; i < 5; ++i)
    {
        suit = getSuit(cardSuit[i]);
        rank = getRank(cardRank[i]);
        printf("You want to keep the cards nomer %d: %c%c?\n", i+1, rank, suit);
        printf("Please reply(Y/N):");
        scanf(" %c", &ans);
        if(toupper(ans) == 'Y')
        {
            finalRank[i] = cardRank[i];
            finalSuit[i] = cardSuit[i];
            ranksinHand[finalRank[i]]++;
            suitsinHand[finalSuit[i]]++;
            continue;
        }
        else if(toupper(ans) == 'N')
        {
            cardUp = 0;
            do
            {
                cardUp = 0;
                finalRank[i] = (rand()%13);
                finalSuit[i] = (rand()%4);
                for(j = 0; j < 5; ++j)              // <---. проверяем, чтобы не было повторяющихся карт
                {
                    if((finalRank[i] == cardRank[j]) && (finalSuit[i] == cardSuit[j]))
                        cardUp = 1;
                }
                for(j = 0; j < 5; ++j)
                    if((finalRank[i]==finalRank[j]) && (finalSuit[i]==finalSuit[j]))
                        cardUp = 1;
            }while(cardUp == 1);
            ranksinHand[finalRank[i]]++;
            suitsinHand[finalSuit[i]]++;
        }
    }
}


