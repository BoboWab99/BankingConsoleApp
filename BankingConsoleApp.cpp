// BankingConsoleApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

// types of users
const string USER_ADMIN = "admin";
const string USER_NORMAL = "normal";

// admin
bool registerClient();
void displayAllClients();
bool deposit(string clientName, double amount);

// normal user
void withdraw(double amount);
void checkBalance();

// admin & normal user
bool login(string userType);
bool changePassword();

// temporal data of active user
vector<string> activeUserData;
bool adminFileChanged = false;
bool usersFileChanged = false;

// utils
void _saveChanges(string fileName);
void saveChanges();
int goBack();
int EXIT_KEY = 2;
int GO_BACK_KEY = 1;


int main()
{
    int option;

    do
    {
        system("cls");
        cout << "WELCOME TO E-CASH SERVICES" << endl;
        cout << "1. Normal User Login" << endl;
        cout << "2. Administrator Login" << endl;
        cout << "3. Close application" << endl;
        cout << "Choose an optin to proceed: ";
        cin >> option;

        switch (option)
        {
        case 1:
            if (!login(USER_NORMAL))
            {
                int trialsRemaining = 2;
                do
                {
                    login(USER_NORMAL);
                    trialsRemaining--;
                } while (trialsRemaining > 0);
                break;
            }      
            // if normal user logged in
            do
            {                
                system("cls"); // clear console
                cout << "USER MENU" << endl;
                cout << "----------------------------" << endl;
                cout << "1. Withdraw cash" << endl;
                cout << "2. Check balance" << endl;
                cout << "3. Reset your password" << endl;
                cout << "4. Save & Exit" << endl;
                cout << "Choose an optin to proceed: ";
                cin >> option;

                switch (option)
                {
                case 1:
                {
                    system("cls");
                    cout << "WITHDRAW CASH" << endl;
                    cout << "----------------------------" << endl;
                    double amount;
                    cout << "Amount to withdraw: ";
                    cin >> amount;
                    withdraw(amount);
                    usersFileChanged = true;
                    int key = goBack();
                    if (key == EXIT_KEY)
                    {
                        return 1;
                    }
                    if (key == GO_BACK_KEY)
                    {
                        saveChanges();
                        break;
                    }
                }
                    break;

                case 2:
                {
                    checkBalance();
                    int key = goBack();
                    if (key == EXIT_KEY)
                    {
                        return 1;
                    }
                }
                    break;

                case 3:
                {
                    changePassword();
                    usersFileChanged = true;
                    int key = goBack();
                    if (key == EXIT_KEY)
                    {
                        return 1;
                    }
                    if (key == GO_BACK_KEY)
                    {
                        saveChanges();
                    }
                }
                    break;

                default:
                    break;
                }

            } while (option != 4);
            
            break;  // end normal user logged in

        case 2:
            if (!login(USER_ADMIN))
            {
                int trialsRemaining = 2;
                do
                {
                    login(USER_ADMIN);
                    trialsRemaining--;
                } while (trialsRemaining > 0);
                break;
            }
            // if admin logged in            
            do
            {
                system("cls"); // clear console
                cout << "ADMIN MENU" << endl;
                cout << "----------------------------" << endl;
                cout << "1. Deposit money for client" << endl;
                cout << "2. Register new client" << endl;
                cout << "3. Display all clients" << endl;
                cout << "4. Reset your password" << endl;
                cout << "5. Save & Exit" << endl;
                cout << "Choose an optin to proceed: ";
                cin >> option;

                switch (option)
                {
                case 1:
                {
                    string clientName;
                    double amount;
                    system("cls");
                    cout << "DEPOSIT" << endl;
                    cout << "----------------------------" << endl;
                    cout << "Client's Name: ";
                    cin >> clientName;
                    cout << "Amount to deposit: ";
                    cin >> amount;
                    deposit(clientName, amount);
                    int key = goBack();
                    if (key == EXIT_KEY)
                    {
                        return 1;
                    }
                }
                    break;

                case 2:
                {
                    registerClient();
                    int key = goBack();
                    if (key == EXIT_KEY)
                    {
                        return 1;
                    }
                }
                    break;

                case 3:
                {
                    displayAllClients();
                    int key = goBack();
                    if (key == EXIT_KEY)
                    {
                        return 1;
                    }
                }
                    break;

                case 4:
                {
                    changePassword();
                    adminFileChanged = true;
                    int key = goBack();
                    if (key == EXIT_KEY)
                    {
                        return 1;
                    }
                    if (key == GO_BACK_KEY)
                    {
                        saveChanges();
                    }
                }
                    break;

                default:
                    break;
                }

            } while (option != 5);

            break;  // end admin logged in

        default:
            break;
        }

    } while (option != 3);
}

bool registerClient()
{
    string username;
    string password;
    double balance;
    fstream fs;

    system("cls"); // clear console
    cout << "REGISTER CLIENT" << endl;
    cout << "----------------------------" << endl;
    cout << "Enter client username: ";
    cin >> username;
    cout << "Enter client password: ";
    cin >> password;
    cout << "Enter client initial deposit: ";
    cin >> balance;

    fs.open("users.txt", ios::app);
    if (fs.is_open())
    {
        fs << username << "," << password << "," << balance << endl;
        fs.close();
        return true;
    }
    else
    {
        cout << "Unable to open database file!" << endl;
        return false;
    }
}

void displayAllClients()
{
    string line;
    fstream fs;

    fs.open("users.txt", ios::in);
    if (fs.is_open())
    {
        system("cls");
        cout << "ALL CLIENTS" << endl;
        cout << "-----------------------------------------------" << endl;
        cout << "USERNAME\tPASSWORD\tBALANCE" << endl;
        cout << "-----------------------------------------------" << endl;

        while (getline(fs, line))
        {
            stringstream row(line);
            vector<string> data; // user data
            while (row.good())
            {
                string col;
                getline(row, col, ',');
                data.push_back(col);
            }
            cout << data[0] << "\t\t" << data[1] << "\t\t" << data[2] << endl;
        }
        fs.close();
    }
}

bool deposit(string clientName, double amount)
{
    fstream fs;
    fstream temp;
    bool userFound = false;

    fs.open("users.txt", ios::in);
    temp.open("temp.txt", ios::app);

    if (fs.is_open() && temp.is_open())
    {
        string line;
        while (getline(fs, line))
        {
            stringstream row(line);
            vector<string> data; // user data

            while (row.good())
            {
                string col;
                getline(row, col, ',');
                data.push_back(col);
            }
            if (data[0] == clientName)
            {
                userFound = true;
                double balance = stod(data[2]);
                balance += amount;
                stringstream ssBalance;
                ssBalance << balance;
                temp << data[0] << "," << data[1] << "," << ssBalance.str() << endl;
            }
            else
            {
                temp << line << endl;
            }
        }
        fs.close();
        temp.close();

        if (!userFound)
        {
            cout << "No such user found!" << endl;
            remove("temp.txt");
            return false;
        }
        else
        {
            remove("users.txt");    // delete old copy
            fs.clear();
            fs.close();

            fs.open("users.txt", ios::app);   // create new copy
            temp.open("temp.txt", ios::in);

            if (fs.is_open() && temp.is_open())
            {
                string output;
                while (getline(temp, output))
                {
                    fs << output << endl;
                }
                fs.close();
                temp.close();
                remove("temp.txt");
                return true;
            }
            return false;
        }
    }
    else
    {
        return false;
    }
}

void withdraw(double amount)
{
    double balance = stod(activeUserData[2]);
    if (amount < balance) {
        balance -= amount;
        stringstream ssBalance;
        ssBalance << balance;
        activeUserData[2] = ssBalance.str();
    }
    else
        cout << "Insufficient funds!" << endl;
}

void checkBalance()
{
    system("cls");
    cout << "CHECK BALANCE" << endl;
    cout << "----------------------------" << endl;
    cout << "Your balance is " << activeUserData[2];
}

bool login(string userType)
{
    system("cls");
    string fileName;

    if (userType == USER_ADMIN)
    {
        fileName = "admin.txt";
        cout << "ADMIN LOGIN" << endl;
        cout << "----------------------------" << endl;
    }
    else if (userType == USER_NORMAL)
    {
        fileName = "users.txt";
        cout << "NORMAL USER LOGIN" << endl;
        cout << "----------------------------" << endl;
    }

    string username;
    string password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    string line;    // line in a file
    fstream file;   // file handler

    file.open(fileName, ios::in);   // try to open users file
    if (file.is_open()) // if it opens
    {
        while (getline(file, line)) // read line by line
        {
            stringstream row(line);
            vector<string> data; // user data
            while (row.good())
            {
                string col;
                getline(row, col, ',');
                data.push_back(col);
            }
            if (data[0] == username && data[1] == password) // if user is found
            {
                activeUserData = data;
                file.close();
                return true;
            }
        }
        file.close();
        cout << "No records found for that user." << endl;
        if (userType == USER_NORMAL)
        {
            cout << "Contact the admin to register a NEW account!" << endl;
        }
        return false;
    }
    else
    {
        return false;
    }
}

bool changePassword()
{
    string newPassword;
    string ConfirmPassword;

    system("cls");
    cout << "CHANGE PASSWORD" << endl;
    cout << "----------------------------" << endl;
    cout << "Enter new password: ";
    cin >> newPassword;
    cout << "Confirm new password: ";
    cin >> ConfirmPassword;

    if (newPassword == ConfirmPassword)
    {
        activeUserData[1] = newPassword;
        cout << "Password changed successfully!" << endl;
        return true;
    }
    cout << "Failed!" << endl;
    return false;
}


// utility functions
void saveChanges()
{
    if (adminFileChanged)
    {
        _saveChanges("admin.txt");
    }
    else if (usersFileChanged)
    {
        _saveChanges("users.txt");
    }
    else
    {
        _saveChanges("admin.txt");
        _saveChanges("users.txt");
    }
}

void _saveChanges(string fileName)
{
    fstream fs;
    fstream temp;
    fs.open(fileName, ios::in);
    temp.open("temp.txt", ios::app);

    if (fs.is_open() && temp.is_open())
    {         
        string line;        
        while (getline(fs, line))
        {
            stringstream row(line);
            vector<string> data;
            while (row.good())
            {
                string col;
                getline(row, col, ',');
                data.push_back(col);
            }           
            if (data[0] == activeUserData[0])
            {
                if (data.size() == 2)   // admin
                {
                    temp << activeUserData[0] << "," << activeUserData[1] << endl;
                }
                else if (data.size() == 3)  // clients
                {
                    temp << activeUserData[0] << "," << activeUserData[1] << "," << activeUserData[2] << endl;
                }
            }
            else
            {
                temp << line << endl;
            }
        }
        fs.close();
        temp.close();

        remove(fileName.c_str());    // delete old copy
        fs.clear();
        fs.close();

        fs.open(fileName, ios::app);   // create new copy
        temp.open("temp.txt", ios::in);

        if (fs.is_open() && temp.is_open())
        {
            string output;
            while (getline(temp, output))
            {
                fs << output << endl;
            }
            fs.close();
            temp.close();
            remove("temp.txt");
        }
    }
}

int goBack()
{
    cout << "\n\nOPTIONS" << endl;
    cout << "----------------------------" << endl;
    cout << "1. Go Back" << endl;
    cout << "2. Exit application" << endl;
    int key;
    do
    {
        cout << "Choose an optin to proceed: ";
        cin >> key;
        if (key == EXIT_KEY || key == GO_BACK_KEY)
        {
            return key;
        }
    } while (true);
}

                                         
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
