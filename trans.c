/*
    ============================================================
      ADVANCED BANK MANAGEMENT SYSTEM (Random Access File)
    ============================================================

    FEATURES:
    ------------------------------------------------------------
    ✅ Add New Account
    ✅ View All Accounts
    ✅ Search Account
    ✅ Update Account Balance
    ✅ Deposit Money
    ✅ Withdraw Money
    ✅ Delete Account
    ✅ Generate Text Report
    ✅ Input Validation
    ✅ Better UI
    ✅ Error Handling
    ✅ Random Access File Handling
    ✅ Fully Modular
    ✅ Professional Code Structure

    FILES USED:
    ------------------------------------------------------------
    accounts.dat   -> Binary database file
    accounts.txt   -> Generated report file

    AUTHOR:
    ------------------------------------------------------------
    Professional Modified Version
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "accounts.dat"
#define MAX_ACCOUNTS 100

// ===================== STRUCTURE ======================

typedef struct
{
    int accountNumber;
    char firstName[30];
    char lastName[30];
    double balance;

} Account;

// ================= FUNCTION PROTOTYPES =================

void initializeFile();
void menu();

void addAccount(FILE *fp);
void viewAccounts(FILE *fp);
void searchAccount(FILE *fp);
void updateAccount(FILE *fp);
void depositMoney(FILE *fp);
void withdrawMoney(FILE *fp);
void deleteAccount(FILE *fp);
void generateReport(FILE *fp);

int accountExists(FILE *fp, int accNo);
void clearInputBuffer();

// ======================== MAIN =========================

int main()
{
    FILE *fp;

    initializeFile();

    fp = fopen(FILE_NAME, "rb+");

    if (fp == NULL)
    {
        printf("ERROR: Cannot open file.\n");
        return 1;
    }

    int choice;

    while (1)
    {
        menu();

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addAccount(fp);
            break;

        case 2:
            viewAccounts(fp);
            break;

        case 3:
            searchAccount(fp);
            break;

        case 4:
            updateAccount(fp);
            break;

        case 5:
            depositMoney(fp);
            break;

        case 6:
            withdrawMoney(fp);
            break;

        case 7:
            deleteAccount(fp);
            break;

        case 8:
            generateReport(fp);
            break;

        case 9:
            printf("\nThank you for using Bank System.\n");
            fclose(fp);
            exit(0);

        default:
            printf("\nInvalid Choice.\n");
        }
    }

    return 0;
}

// ================= MENU =================

void menu()
{
    printf("\n");
    printf("========================================\n");
    printf("        BANK MANAGEMENT SYSTEM\n");
    printf("========================================\n");

    printf("1. Add New Account\n");
    printf("2. View All Accounts\n");
    printf("3. Search Account\n");
    printf("4. Update Account Details\n");
    printf("5. Deposit Money\n");
    printf("6. Withdraw Money\n");
    printf("7. Delete Account\n");
    printf("8. Generate Report File\n");
    printf("9. Exit\n");

    printf("========================================\n");
}

// ============ INITIALIZE FILE =============

void initializeFile()
{
    FILE *fp = fopen(FILE_NAME, "rb");

    if (fp == NULL)
    {
        fp = fopen(FILE_NAME, "wb");

        if (fp == NULL)
        {
            printf("Unable to create file.\n");
            exit(1);
        }

        Account blank = {0, "", "", 0.0};

        for (int i = 0; i < MAX_ACCOUNTS; i++)
        {
            fwrite(&blank, sizeof(Account), 1, fp);
        }

        printf("Database initialized successfully.\n");
    }

    fclose(fp);
}

// ================= ADD ACCOUNT =================

void addAccount(FILE *fp)
{
    Account acc;

    printf("\nEnter Account Number (1-100): ");
    scanf("%d", &acc.accountNumber);

    if (acc.accountNumber < 1 || acc.accountNumber > MAX_ACCOUNTS)
    {
        printf("Invalid account number.\n");
        return;
    }

    if (accountExists(fp, acc.accountNumber))
    {
        printf("Account already exists.\n");
        return;
    }

    printf("Enter First Name: ");
    scanf("%s", acc.firstName);

    printf("Enter Last Name: ");
    scanf("%s", acc.lastName);

    printf("Enter Initial Balance: ");
    scanf("%lf", &acc.balance);

    fseek(fp,
          (acc.accountNumber - 1) * sizeof(Account),
          SEEK_SET);

    fwrite(&acc, sizeof(Account), 1, fp);

    fflush(fp);

    printf("\nAccount created successfully.\n");
}

// ================= VIEW ACCOUNTS =================

void viewAccounts(FILE *fp)
{
    Account acc;

    rewind(fp);

    printf("\n");
    printf("==============================================================\n");
    printf("%-10s %-15s %-15s %-10s\n",
           "Acc No",
           "First Name",
           "Last Name",
           "Balance");

    printf("==============================================================\n");

    while (fread(&acc, sizeof(Account), 1, fp))
    {
        if (acc.accountNumber != 0)
        {
            printf("%-10d %-15s %-15s %.2lf\n",
                   acc.accountNumber,
                   acc.firstName,
                   acc.lastName,
                   acc.balance);
        }
    }
}

// ================= SEARCH ACCOUNT =================

void searchAccount(FILE *fp)
{
    int accNo;
    Account acc;

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    fseek(fp,
          (accNo - 1) * sizeof(Account),
          SEEK_SET);

    fread(&acc, sizeof(Account), 1, fp);

    if (acc.accountNumber == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("\nAccount Found:\n");

        printf("Account Number : %d\n", acc.accountNumber);
        printf("First Name     : %s\n", acc.firstName);
        printf("Last Name      : %s\n", acc.lastName);
        printf("Balance        : %.2lf\n", acc.balance);
    }
}

// ================= UPDATE ACCOUNT =================

void updateAccount(FILE *fp)
{
    int accNo;
    Account acc;

    printf("\nEnter Account Number to Update: ");
    scanf("%d", &accNo);

    fseek(fp,
          (accNo - 1) * sizeof(Account),
          SEEK_SET);

    fread(&acc, sizeof(Account), 1, fp);

    if (acc.accountNumber == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Enter New First Name: ");
    scanf("%s", acc.firstName);

    printf("Enter New Last Name: ");
    scanf("%s", acc.lastName);

    fseek(fp,
          (accNo - 1) * sizeof(Account),
          SEEK_SET);

    fwrite(&acc, sizeof(Account), 1, fp);

    fflush(fp);

    printf("Account updated successfully.\n");
}

// ================= DEPOSIT =================

void depositMoney(FILE *fp)
{
    int accNo;
    double amount;
    Account acc;

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    fseek(fp,
          (accNo - 1) * sizeof(Account),
          SEEK_SET);

    fread(&acc, sizeof(Account), 1, fp);

    if (acc.accountNumber == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Enter Amount to Deposit: ");
    scanf("%lf", &amount);

    if (amount <= 0)
    {
        printf("Invalid amount.\n");
        return;
    }

    acc.balance += amount;

    fseek(fp,
          (accNo - 1) * sizeof(Account),
          SEEK_SET);

    fwrite(&acc, sizeof(Account), 1, fp);

    fflush(fp);

    printf("Deposit successful.\n");
    printf("Updated Balance: %.2lf\n", acc.balance);
}

// ================= WITHDRAW =================

void withdrawMoney(FILE *fp)
{
    int accNo;
    double amount;
    Account acc;

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    fseek(fp,
          (accNo - 1) * sizeof(Account),
          SEEK_SET);

    fread(&acc, sizeof(Account), 1, fp);

    if (acc.accountNumber == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Enter Amount to Withdraw: ");
    scanf("%lf", &amount);

    if (amount <= 0)
    {
        printf("Invalid amount.\n");
        return;
    }

    if (amount > acc.balance)
    {
        printf("Insufficient balance.\n");
        return;
    }

    acc.balance -= amount;

    fseek(fp,
          (accNo - 1) * sizeof(Account),
          SEEK_SET);

    fwrite(&acc, sizeof(Account), 1, fp);

    fflush(fp);

    printf("Withdrawal successful.\n");
    printf("Remaining Balance: %.2lf\n", acc.balance);
}

// ================= DELETE ACCOUNT =================

void deleteAccount(FILE *fp)
{
    int accNo;
    Account blank = {0, "", "", 0.0};
    Account acc;

    printf("\nEnter Account Number to Delete: ");
    scanf("%d", &accNo);

    fseek(fp,
          (accNo - 1) * sizeof(Account),
          SEEK_SET);

    fread(&acc, sizeof(Account), 1, fp);

    if (acc.accountNumber == 0)
    {
        printf("Account does not exist.\n");
        return;
    }

    fseek(fp,
          (accNo - 1) * sizeof(Account),
          SEEK_SET);

    fwrite(&blank, sizeof(Account), 1, fp);

    fflush(fp);

    printf("Account deleted successfully.\n");
}

// ================= GENERATE REPORT =================

void generateReport(FILE *fp)
{
    FILE *report = fopen("accounts.txt", "w");

    if (report == NULL)
    {
        printf("Unable to create report file.\n");
        return;
    }

    Account acc;

    rewind(fp);

    fprintf(report,
            "============================================================\n");

    fprintf(report,
            "%-10s %-15s %-15s %-10s\n",
            "Acc No",
            "First Name",
            "Last Name",
            "Balance");

    fprintf(report,
            "============================================================\n");

    while (fread(&acc, sizeof(Account), 1, fp))
    {
        if (acc.accountNumber != 0)
        {
            fprintf(report,
                    "%-10d %-15s %-15s %.2lf\n",
                    acc.accountNumber,
                    acc.firstName,
                    acc.lastName,
                    acc.balance);
        }
    }

    fclose(report);

    printf("\nReport generated successfully -> accounts.txt\n");
}

// ================= ACCOUNT EXISTS =================

int accountExists(FILE *fp, int accNo)
{
    Account acc;

    fseek(fp,
          (accNo - 1) * sizeof(Account),
          SEEK_SET);

    fread(&acc, sizeof(Account), 1, fp);

    if (acc.accountNumber != 0)
        return 1;

    return 0;
}

// ================= CLEAR BUFFER =================

void clearInputBuffer()
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}