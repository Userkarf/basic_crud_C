#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INVENTORY "Inventory.txt"
#define OPERATIONSLOG "Operations_log.txt"

// DEFINITION OF ITEM STRUCT
struct Item
{
    int ID;
    float price;
    int quantityInInventory;
};

// LIST OPERATIONS LOGS FILE IN THE TERMINAL
int displayLogs()
{
    system("cls");

    char logContent;

    FILE *file;
    file = fopen(OPERATIONSLOG, "r");

    if (file == NULL)
    {
        printf("There are still no logs of operations carried out on the system\n");
        fclose(file);
        return 1;
    }

    while (!feof(file))
    {
        logContent = fgetc(file);
        printf("%c", logContent);
    }

    fclose(file);
    printf("%c", logContent);

    return 0;
}

// GENERATES LOG OF ALL EDITING OPERATIONS MADE ON THE FILE
int generateLog(char *option)
{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    FILE *logFile;
    logFile = fopen(OPERATIONSLOG, "a");

    fprintf(logFile, "%s -> %s <-\n\n", asctime(timeinfo), option);

    fclose(logFile);

    return 0;
}

// LIST ITEMS IN STOCK
int listItems()
{
    system("cls");

    char fileContent;

    // fILE OPENING
    FILE *file;
    file = fopen(INVENTORY, "r");

    if (file == NULL)
    {
        printf("There are no items registered in stock yet =(\n");
        fclose(file);
        return 1;
    }

    while (!feof(file))
    {
        fileContent = fgetc(file);
        printf("%c", fileContent);
    }

    fclose(file);
    printf("%c \n", fileContent);

    return 0;
}

// REGISTER A NEW ITEM IN STOCK
int createItem()
{
    system("cls");
    FILE *file;
    file = fopen(INVENTORY, "a");

    // DEFINITION OF THE STRUCT ITEM TYPE VARIABLE
    struct Item newItem;

    printf("Enter the product ID, value per unit and quantity separated by a space \n");
    scanf("%i %f %i", &newItem.ID, &newItem.price, &newItem.quantityInInventory);

    fprintf(file, "\nItem ID %i -> Price: %.2f, Quantity in stock: %i", newItem.ID, newItem.price, newItem.quantityInInventory);

    fclose(file);

    printf("\n** Done! Item %i into the system successfully =) **\n", newItem.ID);

    char itemAdded[1024];

    sprintf(itemAdded, "Item %i added", newItem.ID);

    generateLog(itemAdded);

    return 0;
}

// UPDATE AN EXISTING ITEM IN STOCK
int updateItem()
{
    char edited[1024];
    int validID = 0, counter = 0;
    char matrix[1024][1024], ItemId[1024];
    do
    {
        system("cls");

        char buffer[2048], *target, searchConcatItem[1024] = "Item ID ";
        int exception;

        exception = listItems();

        if (exception == 0)
        {
            FILE *file;
            file = fopen(INVENTORY, "r+");

            printf("\n\n** Enter the ID of the item you want to update **\n");
            scanf("%s", &ItemId);
            
            strcat(searchConcatItem, ItemId);
            printf("%s", &searchConcatItem);
            
            while (fgets(buffer, 2048, file))
            {
                target = strstr(buffer, searchConcatItem);
                if (target)
                {
                    validID = 1;
                }
                else
                {
                    for (int i = 0; i < sizeof(buffer); i++)
                    {
                        matrix[counter][i] = buffer[i];
                    }
                    counter++;
                }
            }

            if (validID == 0)
            {
                counter = 0;
                printf("\n** Item not found. Press enter to insert a new ID **\n");
                fflush(stdin);
                getchar();
            }

            fclose(file);
        }

    } while (validID == 0);

    FILE *fileEdit;
    fileEdit = fopen(INVENTORY, "w");
    for (int i = 0; i < counter; i++)
    {
        fprintf(fileEdit, "%s", matrix[i]);
    }

    fclose(fileEdit);

    sprintf(edited, "Item %s successfully edited", ItemId);

    generateLog(edited);

    createItem();

    return 0;
}

// DELETES AN ITEM FROM STOCK
int deleteItem()
{
    int validID = 0, counter = 0;
    char matrix[1024][1024], deleted[1024], itemId[1024];
    do
    {
        system("cls");

        char buffer[2048], *target, searchConcatItem[1024] = "Item ";
        int exception;

        exception = listItems();

        if (exception == 0)
        {
            FILE *file;
            file = fopen(INVENTORY, "r+");

            printf("\n\n** Enter the ID of the item to be deleted **\n");
            scanf("%s", &itemId);

            strcat(searchConcatItem, itemId);

            while (fgets(buffer, 2048, file))
            {
                target = strstr(buffer, searchConcatItem);
                if (target)
                {
                    validID = 1;
                }
                else
                {
                    for (int i = 0; i < sizeof(buffer); i++)
                    {
                        matrix[counter][i] = buffer[i];
                    }
                    counter++;
                }
            }

            if (validID == 0)
            {
                counter = 0;
                printf("\n** Item not found. Press enter to insert a new ID **\n");
                fflush(stdin);
                getchar();
            }

            fclose(file);
        }

    } while (validID == 0);

    FILE *fileEdit;
    fileEdit = fopen(INVENTORY, "w");
    for (int i = 0; i < counter; i++)
    {
        fprintf(fileEdit, "%s", matrix[i]);
    }
    fclose(fileEdit);

    sprintf(deleted, "Item %s was deleted", itemId);

    generateLog(deleted);

    system("cls");
    printf("** Success! Item duly deleted from stock **\n");

    return 0;
}

// DIRECT USER TO THE SELECTED OPERATION
void chooseOption()
{
    int option = 0;

    // OPTIONS MENU
    printf("1 - See all registered items \n");
    printf("2 - Add a new item \n");
    printf("3 - Update the information of a registered item \n");
    printf("4 - Delete an item from inventory \n");
    printf("5 - View logs \n");
    scanf("%d", &option);

    // FLOW DEFINITION
    switch (option)
    {
    case 1:
        listItems();
        break;

    case 2:
        createItem();
        break;

    case 3:
        updateItem();
        break;

    case 4:
        deleteItem();
        break;

    case 5:
        displayLogs();
        break;

    default:
        system("cls");
        printf("- Enter a valid command - \n\n");
        chooseOption();
        break;
    }
}

int main()
{
    int showMenu = 1;

    // CLEAR TERMINAL AND DISPLAY WELCOME MESSAGE
    system("cls");

    printf("** Welcome to the inventory system v1.0 **\n");
    printf("\nWhat would you like to do today? \n\n");

    do
    {
        chooseOption();

        printf("\nWould you like to carry out a new operation? \n");
        printf("1: Yes \n2: No \n");
        scanf("%i", &showMenu);
        system("cls");
    } while (showMenu == 1);

    printf("** All done! Have a great day :) **");
}
