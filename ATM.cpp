// NikosPin = 2667
// KwstasPin = 1234
// ManosPin = 3248
// MariaPin = 1923
// XrhstosPin = 9920
// AggelosPin = 3482

#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <random>
#include <stdexcept>
#include <iomanip>
#include <sstream>
using namespace std;

struct Account
{
    string name;
    int pin;
    double balance;
};

const int MAX_ATTEMPTS=5;
const int GO_BACK_PIN = 9;
const int USER_COUNT=6;


Account accounts[USER_COUNT] = {
    {"Nikos", 2667, 0},
    {"Kwstas", 1234, 0},
    {"Manos", 3248, 0},
    {"Maria", 1923, 0},
    {"Xrhstos", 9920, 0},
    {"Aggelos", 3482, 0}
};

void giveRandomMoney()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> cents(100,1000000);
    for(int i=0;i<USER_COUNT;i++)
        accounts[i].balance = cents(gen) / 100.0;
}

string money(double value)
{
    ostringstream out;
    out << fixed << setprecision(2) << value << " EUR";
    return out.str();
}

string readLine()
{
    string line;
    if(!getline(cin, line))
        exit(0);
    return line;
}

int readInt()
{
    string line=readLine();
    size_t used=0;
    int value;
    try
    {
        value = stoi(line, &used);
    }
    catch(const exception&) {
        throw invalid_argument("Please enter a whole number");
    }
    if (line.find_first_not_of(" \t", used) != string::npos)
        throw invalid_argument("Please enter a whole number");
    return value;
}

double readAmount()
{
    string line = readLine();
    size_t used = 0;
    double value;
    try
    {
        value = stod(line, &used);
    }
    catch(const exception&)
    {
        throw invalid_argument("Please enter a valid amount");
    }
    if(line.find_first_not_of(" \t", used) != string::npos || !isfinite(value))
        throw invalid_argument("Please enter a valid amount");
    if(value < 0)
        throw invalid_argument("Amount can't be negative");
    return value;
}

void printUsers()
{
    for(int i = 0; i < USER_COUNT; i++)
        cout << ", " << i + 1 << ": " << accounts[i].name;
}

int chooseUser()
{
    while(true)
    {
        cout << "\nWhich User [1-6] are you? (0: Exit";
        printUsers();
        cout << "): ";
        try
        {
            int choice = readInt();
            if(choice == 0)
            {
                cout << "\nEXITING...\n";
                exit(0);
            }
            if(choice < 1 || choice > USER_COUNT)
                throw out_of_range("There is no such user");
            return choice - 1;
        }
        catch(const exception& e)
        {
            cout << "\nInvalid User: " << e.what() << ", try again..\n";
        }
    }
}

bool login(const Account &acc)
{
    int attempts=MAX_ATTEMPTS;
    cout << "\nGreetings " << acc.name << ", please enter your PIN code to confirm it's really you.. (" << GO_BACK_PIN << ". To go Back)\n";
    while(attempts > 0)
    {
        cout << "(" << attempts << " attempts remaining): ";
        try
        {
            int pin = readInt();
            if(pin == acc.pin)
            {
                cout << "\nSuccessful Login!\n\nWelcome to your account ";
                return true;
            }
            if(pin == GO_BACK_PIN)
                return false;
            attempts--;
            if (attempts>0)
                cout << "\nInvalid PIN Code, try again..\n";
        }
        catch(const exception &e)
        {
            attempts--;
            cout << "\n" << e.what() << " (the PIN is numbers only)\n";
        }
    }
    cout << "\nYou have run out of attempts, run the program again if you wish to login!\n\n";
    exit(0);
}

void deposit(Account& acc)
{
    while(true)
    {
        cout << "\n(You have " << money(acc.balance) << ") Enter an amount to deposit or type 0 if you wish to go back: ";
        try
        {
            double amount=readAmount();
            if(amount == 0)
                return;
            acc.balance+=amount;
            cout << "\nDeposited " << money(amount) << ". Your new balance is " << money(acc.balance) << ".\n\n";
            return;
        }
        catch(const exception &e)
        {
            cout << "\nError: " << e.what() << ", try again..\n";
        }
    }
}

void withdraw(Account &acc)
{
    while(true)
    {
        cout << "\n(You have " << money(acc.balance) << ") Enter an amount to withdraw or type 0 if you wish to go back: ";
        try
        {
            double amount = readAmount();
            if(amount == 0)
                return;
            if(amount > acc.balance)
                throw runtime_error("Insufficient funds");
            acc.balance-=amount;
            cout << "\nWithdrew " << money(amount) << ". Your new balance is " << money(acc.balance) << ".\n\n";
            return;
        }
        catch(const exception &e)
        {
            cout << "\nError: " << e.what() << ", try again..\n";
        }
    }
}

void checkBalance(const Account &acc)
{
    cout << "\nYour current balance is " << money(acc.balance) << ". Press 0 to go back: ";
    while(true)
    {
        try
        {
            if(readInt() == 0)
                break;
            cout << "\nInvalid input. Press 0 when you are ready to go back: ";
        }
        catch(const exception&)
        {
            cout << "\nInvalid input. Press 0 when you are ready to go back: ";
        }
    }
    cout<<endl;
}


void transfer(Account &sender)
{
    while(true)
    {
        cout << "\nWhich User [1-6] do you wish to transfer funds to? (0: Go back";
        printUsers();
        cout << "): ";
        int choice;
        try 
        {
            choice = readInt();
            if(choice == 0)
            {
                cout << endl;
                return;
            }
            if(choice < 1 || choice > USER_COUNT)
                throw out_of_range("Select a valid user or option");
            if(&accounts[choice - 1] == &sender)
                throw invalid_argument("You cannot send money to yourself");
        }
        catch(const exception &e)
        {
            cout << "\nError: " << e.what() << ", try again..\n";
            continue;
        }

        Account& receiver = accounts[choice-1];
        while(true)
        {
            cout << "\n" << sender.name << ", select the amount of funds (" << money(sender.balance) << ") you want " << receiver.name << " to receive or press 0 to go back: ";
            try
            {
                double amount = readAmount();
                if(amount == 0)
                    break;
                if(amount > sender.balance)
                    throw runtime_error("Insufficient funds");
                sender.balance-=amount;
                receiver.balance+=amount;
                cout << "\nTransaction Successful! " << receiver.name << " has received " << money(amount) << ".\n\n";
                return;
            }
            catch(const exception &e)
            {
                cout << "\nError: " << e.what() << ", try again..\n";
            }
        }
    }
}

void mainMenu(Account &acc)
{
    while(true)
    {
        cout << acc.name << ", what would you like to do?\n\n1. Deposit\n2. Withdraw\n3. Check Balance\n4. Transfer Balance to Another User\n5. Logout & Exit\n\n";
        int choice=0;
        try
        {
            choice = readInt();
        }
        catch(const exception &e)
        {
            cout << "\nError: " << e.what() << "\n\n";
            continue;
        }
        switch(choice)
        {
            case 1: deposit(acc); break;
            case 2: withdraw(acc); break;
            case 3: checkBalance(acc); break;
            case 4: transfer(acc); break;
            case 5:
                cout << "\nSuccessfully logged out! This session has expired..\n";
                return;
            default:
                cout << "\nPlease try again and select a valid option..\n\n";
        }
    }
}

int main()
{
    giveRandomMoney();

    cout << "\nARE YOU A MEMBER OF OUR BANKING SYSTEM? (y/n)\n";
    while(true)
    {
        string answer = readLine();
        if(answer == "n")
        {
            cout << "\nEXITING...\n";
            return 0;
        }
        if(answer == "y")
            break;
        cout << "\nTRY AGAIN AND USE A VALID INPUT THIS TIME\n.\n\nARE YOU A MEMBER OF OUR BANKING SYSTEM? (y/n)\n";
    }

    while(true)
    {
        Account& acc = accounts[chooseUser()];
        if(login(acc))
        {
            mainMenu(acc);
            return 0;
        }
    }
}
