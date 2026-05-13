

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "credit.dat"
#define MAX_RECORDS 100

// structure definition
struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// function prototypes
unsigned int enterChoice(void);

void initializeFile(FILE *fPtr);

void textFile(FILE *readPtr);

void updateRecord(FILE *fPtr);

void newRecord(FILE *fPtr);

void deleteRecord(FILE *fPtr);

void listAccounts(FILE *fPtr);

void searchAccount(FILE *fPtr);

void clearInputBuffer(void);

// ==========================================================
// MAIN FUNCTION
// ==========================================================
int main()
{
    FILE *cfPtr;

    // open file
    cfPtr = fopen(FILE_NAME, "rb+");

    // if file does not exist, create it
    if (cfPtr == NULL)
    {
        cfPtr = fopen(FILE_NAME, "wb+");

        if (cfPtr == NULL)
        {
            printf("File could not be created.\n");
            return 1;
        }

        initializeFile(cfPtr);
    }

    unsigned int choice;

    while ((choice = enterChoice()) != 7)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;

        case 2:
            updateRecord(cfPtr);
            break;

        case 3:
            newRecord(cfPtr);
            break;

        case 4:
            deleteRecord(cfPtr);
            break;

        case 5:
            listAccounts(cfPtr);
            break;

        case 6:
            searchAccount(cfPtr);
            break;

        default:
            printf("Invalid choice.\n");
        }
    }

    fclose(cfPtr);

    printf("\nProgram terminated successfully.\n");

    return 0;
}

// ==========================================================
// INITIALIZE FILE WITH EMPTY RECORDS
// ==========================================================
void initializeFile(FILE *fPtr)
{
    struct clientData blankClient = {0, "", "", 0.0};

    rewind(fPtr);

    for (int i = 0; i < MAX_RECORDS; i++)
    {
        fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);
    }

    fflush(fPtr);
}

// ==========================================================
// MENU
// ==========================================================
unsigned int enterChoice(void)
{
    unsigned int menuChoice;

    printf("\n====================================\n");
    printf(" BANK ACCOUNT MANAGEMENT SYSTEM\n");
    printf("====================================\n");

    printf("1 - Generate accounts.txt\n");
    printf("2 - Update an account\n");
    printf("3 - Add a new account\n");
    printf("4 - Delete an account\n");
    printf("5 - List all accounts\n");
    printf("6 - Search account\n");
    printf("7 - Exit\n");

    printf("Enter your choice: ");
    scanf("%u", &menuChoice);

    return menuChoice;
}

// ==========================================================
// GENERATE TEXT FILE
// ==========================================================
void textFile(FILE *readPtr)
{
    FILE *writePtr;

    struct clientData client = {0, "", "", 0.0};

    writePtr = fopen("accounts.txt", "w");

    if (writePtr == NULL)
    {
        printf("accounts.txt could not be created.\n");
        return;
    }

    rewind(readPtr);

    fprintf(writePtr,
            "%-10s%-20s%-20s%-10s\n",
            "Acct No",
            "Last Name",
            "First Name",
            "Balance");

    while (fread(&client,
                 sizeof(struct clientData),
                 1,
                 readPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr,
                    "%-10u%-20s%-20s%-10.2lf\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
        }
    }

    fclose(writePtr);

    printf("\naccounts.txt generated successfully.\n");
}

// ==========================================================
// ADD NEW ACCOUNT
// ==========================================================
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter new account number (1-100): ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists.\n");
        return;
    }

    printf("Enter last name: ");
    scanf("%14s", client.lastName);

    printf("Enter first name: ");
    scanf("%9s", client.firstName);

    printf("Enter balance: ");
    scanf("%lf", &client.balance);

    client.acctNum = accountNum;

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    fflush(fPtr);

    printf("\nAccount added successfully.\n");
}

// ==========================================================
// UPDATE ACCOUNT
// ==========================================================
void updateRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int account;

    double transaction;

    printf("Enter account number to update: ");
    scanf("%u", &account);

    if (account < 1 || account > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
        return;
    }

    printf("\nCurrent Details:\n");

    printf("Account No : %u\n", client.acctNum);
    printf("Name       : %s %s\n",
           client.firstName,
           client.lastName);

    printf("Balance    : %.2lf\n",
           client.balance);

    printf("\nEnter transaction amount (+deposit / -withdraw): ");
    scanf("%lf", &transaction);

    client.balance += transaction;

    fseek(fPtr,
          -sizeof(struct clientData),
          SEEK_CUR);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    fflush(fPtr);

    printf("\nUpdated balance = %.2lf\n",
           client.balance);
}

// ==========================================================
// DELETE ACCOUNT
// ==========================================================
void deleteRecord(FILE *fPtr)
{
    struct clientData client;

    struct clientData blankClient = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter account number to delete: ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&blankClient,
           sizeof(struct clientData),
           1,
           fPtr);

    fflush(fPtr);

    printf("\nAccount deleted successfully.\n");
}

// ==========================================================
// LIST ALL ACCOUNTS
// ==========================================================
void listAccounts(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\n=============================================================\n");

    printf("%-10s%-20s%-20s%-10s\n",
           "Acct No",
           "Last Name",
           "First Name",
           "Balance");

    printf("=============================================================\n");

    while (fread(&client,
                 sizeof(struct clientData),
                 1,
                 fPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            printf("%-10u%-20s%-20s%-10.2lf\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}

// ==========================================================
// SEARCH ACCOUNT
// ==========================================================
void searchAccount(FILE *fPtr)
{
    struct clientData client;

    unsigned int accountNum;

    printf("Enter account number to search: ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("\n====================================\n");

    printf("Account Number : %u\n",
           client.acctNum);

    printf("Last Name      : %s\n",
           client.lastName);

    printf("First Name     : %s\n",
           client.firstName);

    printf("Balance        : %.2lf\n",
           client.balance);

    printf("====================================\n");
}

// ==========================================================
// CLEAR BUFFER
// ==========================================================
void clearInputBuffer(void)
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}