#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <unistd.h>
using namespace std;

// To display the suits of the cards in the output, represented in unicode
const string SPADE = "\u2660";
const string CLUB = "\u2663";
const string HEART = "\u2665";
const string DIAMOND = "\u2666";

// Function Prototypes
void runBlackjack();
int numInDeck(vector<string> &, int);
bool validateAmount(float);

// Inside Hand - holds the numerical value of each card
// Outside Hand - simply for output purposes (stores symbols and face of each card)
float userAmount = 500;
vector<string> cpuOutsideHand;
vector<int> cpuInsideHand;
int cpuTotal = 0;
float userBet = 0;
bool doubleDownFlag = false;

int generateRandomIndex(int limitNum)
{
    int randomIndex;
    randomIndex = (rand() % limitNum);
    return randomIndex;
}
void determineWinner(int userTotal)
{
    if (userTotal > 21)
    {
        userTotal = 0;
    }
    else if (cpuTotal > 21)
    {
        cpuTotal = 0;
    }

    if (userTotal == cpuTotal)
    {
        cout << "It's a tie!" << endl;
    }
    else if (userTotal > cpuTotal)
    {
        cout << "You beat the dealer! You won $" << userBet << "." << endl;
        userAmount += userBet;
        cpuTotal = 0;
        cpuOutsideHand.clear();
        cpuInsideHand.clear();
        runBlackjack();
    }
    else
    {
        userAmount -= userBet;
        if (userAmount == 0)
        {
            cout << "The Dealer beat you. You have lost all of your money. Please run the program again to restart." << endl;
        }
        else
        {
            cout << "The Dealer beat you. Sorry, you lost $" << userBet << "." << endl;
            cpuTotal = 0;
            cpuOutsideHand.clear();
            cpuInsideHand.clear();
            runBlackjack();
        }
    }
}
void dealerMoves(vector<string> &cardDeck, int userTotal)
{
    int convertedToInt;
    if (cpuTotal <= 21 && cpuTotal >= 17)
    {
        cout << "The dealer stays" << endl;
        determineWinner(userTotal);
    }
    else
    {
        bool aceFound = false;
        int randomIndex = generateRandomIndex(cardDeck.size());
        cpuOutsideHand.push_back(cardDeck[randomIndex]);
        int aceIndex;
        cout << "The dealer hits and is dealt: " << cardDeck[randomIndex] << endl;
        cout << "The dealer now has: ";
        for (int i = 0; i < cpuOutsideHand.size(); i++)
        {
            if (i == 0)
            {
                cout << cpuOutsideHand[i];
            }
            else
            {
                cout << ", " << cpuOutsideHand[i];
            }
        }
        cout << endl;
        string valueString = cardDeck[randomIndex].substr(0, 1);
        if (valueString == "A")
        {
            convertedToInt = 11;
            cpuInsideHand.push_back(convertedToInt);
            cpuTotal = cpuTotal + convertedToInt;
        }
        else if (valueString == "T" || valueString == "J" || valueString == "Q" || valueString == "K")
        {
            convertedToInt = 10;
            cpuInsideHand.push_back(convertedToInt);
            cpuTotal = cpuTotal + convertedToInt;
        }
        else
        {
            convertedToInt = stoi(valueString);
            cpuInsideHand.push_back(convertedToInt);
            cpuTotal = cpuTotal + convertedToInt;
        }

        for (int i = 0; i < cpuInsideHand.size(); i++)
        {
            if (cpuInsideHand[i] == 11)
            {
                aceIndex = i;
                aceFound = true;
            }
        }

        if (cpuTotal <= 21 && cpuTotal >= 17)
        {
            cout << "The dealer stays." << endl;
            determineWinner(userTotal);
        }
        else if (cpuTotal > 21)
        {
            if (aceFound)
            {
                cpuInsideHand[aceIndex] = 1;
                cpuTotal = cpuTotal - 10;
                cardDeck.erase(cardDeck.begin() + randomIndex);
                dealerMoves(cardDeck, userTotal);
                return;
            }
            else
            {
                cout << "The Dealer busted!" << endl;
                determineWinner(userTotal);
            }
        }
        else
        {
            cardDeck.erase(cardDeck.begin() + randomIndex);
            sleep(1);
            dealerMoves(cardDeck, userTotal);
            return;
        }
    }
}
int numInDeck(vector<string> &cardDeck, int prob)
{
    int num = 0;
    string first;
    if (prob == 10)
    {
        for (string str : cardDeck)
        {
            first = str.substr(0, 1);
            if (first == "T" || first == "J" || first == "Q" || first == "K")
            {
                num++;
            }
        }
        return num;
    }

    if (prob < 10 && prob > 1)
    {
        for (string str : cardDeck)
        {
            first = str.substr(0, 1);
            if (first != "T" && first != "J" && first != "Q" && first != "K" && first != "A")
            {
                int card = stoi(first);
                if (card == prob)
                {
                    num++;
                }
            }
        }
        return num;
    }

    if (prob == 11 || prob == 1)
    {
        for (string str : cardDeck)
        {
            first = str.substr(0, 1);
            if (first == "A")
            {
                num++;
            }
        }
    }
    return num;
}

void userHitOrStayFunction(vector<string> &userOutsideHand, vector<int> &userInsideHand, vector<string> &cardDeck, int &userTotal)
{
    string input;
    int convertedToInt, prob;

    // Probability of getting to 21
    cout << "Probability: ";
    prob = 21 - userTotal;
    if (prob > 10 && prob != 11)
    {
        cout << 100 * (static_cast<float>(numInDeck(cardDeck, 10)) / cardDeck.size()) * (static_cast<float>(numInDeck(cardDeck, prob - 10)) / (cardDeck.size() - 1)) << "%" << endl;
    }
    else
    {
        cout << 100 * (static_cast<float>(numInDeck(cardDeck, prob)) / cardDeck.size()) << "%" << endl;
    }

    cout << "Would you like to hit or stay? ";
    cin >> input;
    if (input == "hit")
    {
        // Option to Double-Down
        cout << "Would you like to double your bet? ";
        cin >> input;
        if (input == "yes" && validateAmount(userBet * 2))
        {
            userBet *= 2;
            doubleDownFlag = true;
            input = "stay";
            cout << "New Bet: " << userBet << endl;
        }
        else if (input == "no")
        {
            doubleDownFlag = false;
        }
        else
        {
            cout << "Invalid input, defaulting to No." << endl;
        }

        bool aceFound = false;
        int randomIndex = generateRandomIndex(cardDeck.size());
        userOutsideHand.push_back(cardDeck[randomIndex]);
        int aceIndex;
        cout << "You are dealt: " << cardDeck[randomIndex] << endl;
        cout << "You now have: ";
        for (int i = 0; i < userOutsideHand.size(); i++)
        {
            if (i == 0)
            {
                cout << userOutsideHand[i];
            }
            else
            {
                cout << ", " << userOutsideHand[i];
            }
        }
        cout << endl;
        string valueString = cardDeck[randomIndex].substr(0, 1);
        if (valueString == "A")
        {
            convertedToInt = 11;
            userInsideHand.push_back(convertedToInt);
            userTotal += convertedToInt;
        }
        else if (valueString == "T" || valueString == "J" || valueString == "Q" || valueString == "K")
        {
            convertedToInt = 10;
            userInsideHand.push_back(convertedToInt);
            userTotal += convertedToInt;
        }
        else
        {
            convertedToInt = stoi(valueString);
            userInsideHand.push_back(convertedToInt);
            userTotal += convertedToInt;
        }

        for (int i = 0; i < userInsideHand.size(); i++)
        {
            if (userInsideHand[i] == 11)
            {
                aceIndex = i;
                aceFound = true;
            }
        }

        if (userTotal == 21)
        {
            cout << "Blackjack you win" << endl;
            determineWinner(userTotal);
        }
        else if (userTotal > 21)
        {
            if (aceFound && !doubleDownFlag)
            {
                userInsideHand[aceIndex] = 1;
                userTotal = userTotal - 10;
                cardDeck.erase(cardDeck.begin() + randomIndex);
                userHitOrStayFunction(userOutsideHand, userInsideHand, cardDeck, userTotal);
                return;
            }
            else
            {
                cout << "Boom! You Busted." << endl;
                determineWinner(userTotal);
            }
        }
        else
        {
            cardDeck.erase(cardDeck.begin() + randomIndex);
            if (!doubleDownFlag)
            {
                userHitOrStayFunction(userOutsideHand, userInsideHand, cardDeck, userTotal);
                return;
            }
        }
    }
    if (input == "stay")
    {
        cout << "The dealer has: ";
        if (userTotal == 21)
        {
            determineWinner(userTotal);
        }
        else if (userTotal < 21)
        {
            for (int i = 0; i < cpuOutsideHand.size(); i++)
            {

                cout << cpuOutsideHand[i] << ", ";
            }
            cout << "\b\b\n";

            dealerMoves(cardDeck, userTotal);
        }
        else
        {
            determineWinner(userTotal);
            return;
        }
    }
    else
    {
        cout << "That is an invalid option. Please try again. " << endl;
        userHitOrStayFunction(userOutsideHand, userInsideHand, cardDeck, userTotal);
        return;
    }
}

void startGame()
{
    srand((unsigned int)time(NULL));
    vector<string> cardNum = {"2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A"};
    vector<string> cardSuits = {SPADE, HEART, CLUB, DIAMOND};
    vector<string> cardDeck;
    vector<string> userOutsideHand;
    vector<int> userInsideHand;
    string valueString;
    int convertedToInt;
    int userTotal = 0;

    for (int i = 0; i < cardNum.size(); i++)
    {
        for (int x = 0; x < cardSuits.size(); x++)
        {
            cardDeck.push_back(cardNum[i] + cardSuits[x]);
        }
    }

    // Generates a random card for the user and erases it from cardDeck
    for (int i = 0; i < 2; i++)
    {
        int randomIndex = generateRandomIndex(cardDeck.size());
        userOutsideHand.push_back(cardDeck[randomIndex]);
        cardDeck.erase(cardDeck.begin() + randomIndex);
    }

    // Generates a random card for the dealer (cpu) and erases it from cardDeck
    for (int i = 0; i < 2; i++)
    {
        int randomIndex = generateRandomIndex(cardDeck.size());
        cpuOutsideHand.push_back(cardDeck[randomIndex]);
        cardDeck.erase(cardDeck.begin() + randomIndex);
    }

    // Calculates User Total
    for (int i = 0; i < userOutsideHand.size(); i++)
    {
        valueString = userOutsideHand[i].substr(0, 1);
        if (userTotal <= 21)
        {
            if (valueString == "A")
            {
                if (userInsideHand.size() && userInsideHand[0] == 11)
                {
                    convertedToInt = 1;
                    userInsideHand.push_back(convertedToInt);
                    userTotal += convertedToInt;
                }
                else
                {
                    convertedToInt = 11;
                    userInsideHand.push_back(convertedToInt);
                    userTotal += convertedToInt;
                }
            }
            else if (valueString == "T" || valueString == "J" || valueString == "Q" || valueString == "K")
            {
                convertedToInt = 10;
                userInsideHand.push_back(convertedToInt);
                userTotal += convertedToInt;
            }
            else
            {
                convertedToInt = stoi(valueString);
                userInsideHand.push_back(convertedToInt);
                userTotal += convertedToInt;
            }
        }
        else
        {
            return;
        }
    }

    cout << "You are dealt: ";
    for (int i = 0; i < userOutsideHand.size(); i++)
    {
        if (i == 0)
        {
            cout << userOutsideHand[i];
        }
        else
        {
            cout << ", " << userOutsideHand[i];
        }
    }
    cout << endl;

    for (int i = 0; i < cpuOutsideHand.size(); i++)
    {
        valueString = cpuOutsideHand[i].substr(0, 1);
        if (userTotal <= 21)
        {
            if (valueString == "A")
            {
                if (cpuInsideHand.size() && cpuInsideHand[0] == 11)
                {
                    convertedToInt = 1;
                    cpuInsideHand.push_back(convertedToInt);
                    cpuTotal += convertedToInt;
                }
                else
                {
                    convertedToInt = 11;
                    userInsideHand.push_back(convertedToInt);
                    cpuTotal += convertedToInt;
                }
            }
            else if (valueString == "T" || valueString == "J" || valueString == "Q" || valueString == "K")
            {
                convertedToInt = 10;
                cpuInsideHand.push_back(convertedToInt);
                cpuTotal += convertedToInt;
            }
            else
            {
                convertedToInt = stoi(valueString);
                cpuInsideHand.push_back(convertedToInt);
                cpuTotal += convertedToInt;
            }
        }
        else
        {
            return;
        }
    }

    cout << "The dealer is dealt with: ";
    for (int i = 0; i < cpuOutsideHand.size(); i++)
    {
        if (i == 0)
        {
            cout << cpuOutsideHand[i];
        }
        else if (i == 1)
        {
            cout << ", Unknown";
        }
        else
        {
            cout << ", " << cpuOutsideHand[i];
        }
    }
    cout << endl;
    userHitOrStayFunction(userOutsideHand, userInsideHand, cardDeck, userTotal);
}

bool validateAmount(float bet)
{
    if (bet > userAmount)
    {
        cout << "Sorry, you can only bet an amount under your current value right now." << endl;
        return false;
    }
    else if (userBet < 1)
    {
        cout << "The minimum bet is $1." << endl;
        return false;
    }
    else if (userBet >= 1 && userBet <= userAmount)
    {
        return true;
    }
}

void runBlackjack()
{
    string userPlayChoice;

    cout << "You are starting with $" << userAmount << ". Would you like to play? ";
    cin >> userPlayChoice;
    if (userPlayChoice == "yes")
    {
        cout << "Place your bet: ";
        cin >> userBet;

        if (!(validateAmount(userBet)))
        {
            runBlackjack();
            return;
        }
        else
        {
            startGame();
        }
    }

    else if (userPlayChoice == "no")
    {
        cout << "You have exited the game with $" << userAmount << "." << endl;
        userAmount = 500;
        return;
    }
    else
    {
        cout << "That is an invalid input. Please try again." << endl;
        runBlackjack();
        return;
    }
}
int main()
{
    // Check cout the total or the amount to the user within 2 decimal places
    cout << setprecision(2) << fixed << showpoint;
    cout << "Welcome to Blackjack." << endl;
    runBlackjack();
    return 0;
}