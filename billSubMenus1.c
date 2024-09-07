#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void displayVegMenu();
void displayNonVegMenu();
void generateBillHeader(char name[50], char date[30]);
void generateBillBody(char item[20], int qty, float price);
void generateBillFooter(float total);
void displayBookings();

typedef struct items {
    char item[20];
    float price;
    int qty;
} items;

typedef struct orders {
    char customer[50];
    char date[50];
    int numOfItems;
    items itm[50]; // array of structures
} orders;

typedef struct tableBooking {
    char customer[50];
    char date[50];
    int tableNumber;
    int numberOfPeople;
    char timeSlot[20];
} tableBooking;

// Initializes an array of items structures representing the Veg menu. Each item has a name and a price.
items vegMenu[] = {
    {"Salad", 49},
    {"Pizza", 199},
    {"Pasta", 99},
    {"Burger", 149}
};

// Initializes an array of items structures representing the Non-Veg menu. Each item has a name and a price.
items nonVegMenu[] = {
    {"Chicken", 99},
    {"Kawab", 149},
    {"Mutton", 149},
    {"Fishes", 199}
    
};

int vegMenuSize = sizeof(vegMenu) / sizeof(vegMenu[0]);
int nonVegMenuSize = sizeof(nonVegMenu) / sizeof(nonVegMenu[0]);

int main() {
    int choice, no_of_item, menuType, menuChoice;
    char saveBill;
    char temp = 'y'; // Use a single character for the loop condition
    char name[30]; // Declare the name variable here
    FILE *ptr; // Used to save bill
    orders ord;
    orders ord1;
    tableBooking booking;
    tableBooking existingBooking;

    while (temp == 'y' || temp == 'Y') {
        float total = 0.0;
        int invoiceFound = 0;
        printf("============ The Only Place ============");
        printf("\n\nPlease select your preferred operation");
        printf("\n\n1. Choose Menu");
        printf("\n2. Show all Invoices");
        printf("\n3. Book a table");
        printf("\n4. Search for Invoices");
        printf("\n5. Display all table bookings");        
        printf("\n6. Exit");

        printf("\n\nYour Choice:\t");
        scanf("%d", &choice);
        fgetc(stdin); // Consume newline character left by scanf

        switch (choice) {
            case 1:
                system("cls");
                printf("\nPlease enter name of customer:\t");
                fgets(ord.customer, 50, stdin);
                ord.customer[strlen(ord.customer) - 1] = 0; // Remove newline character
                strcpy(ord.date, __DATE__); // (destination, source)
                printf("\nPlease enter number of items:\t");
                scanf("%d", &no_of_item);
                ord.numOfItems = no_of_item;

                for (int i = 0; i < no_of_item; i++) {
                    printf("\nSelect Menu Type:");
                    printf("\n1. Veg Menu");
                    printf("\n2. Non-Veg Menu");
                    printf("\nYour Choice:\t");
                    scanf("%d", &menuType);

                    if (menuType == 1) {
                        displayVegMenu();
                        printf("\nSelect item %d:\t", i + 1);
                        scanf("%d", &menuChoice);
                        menuChoice--; // Adjusting for 0-based index

                        if (menuChoice < 0 || menuChoice >= vegMenuSize) {
                            printf("\nInvalid selection. Try again.\n");
                            i--;
                            continue;
                        }
                        printf("Please enter the quantity:\t");
                        scanf("%d", &ord.itm[i].qty);
                        strcpy(ord.itm[i].item, vegMenu[menuChoice].item);
                        ord.itm[i].price = vegMenu[menuChoice].price;
                        total += ord.itm[i].qty * ord.itm[i].price;
                    } 
                    else if (menuType == 2) {
                        displayNonVegMenu();
                        printf("\nSelect item %d:\t", i + 1);
                        scanf("%d", &menuChoice);
                        menuChoice--; // Adjusting for 0-based index

                        if (menuChoice < 0 || menuChoice >= nonVegMenuSize) {
                            printf("\nInvalid selection. Try again.\n");
                            i--;
                            continue;
                        }
                        printf("Please enter the quantity:\t");
                        scanf("%d", &ord.itm[i].qty);
                        strcpy(ord.itm[i].item, nonVegMenu[menuChoice].item);
                        ord.itm[i].price = nonVegMenu[menuChoice].price;
                        total += ord.itm[i].qty * ord.itm[i].price;
                    }
                     else {
                        printf("\nInvalid Menu Type. Please select again.\n");
                        i--;
                        continue;
                    }
                }

                // Generate bill
                generateBillHeader(ord.customer, ord.date);
                for (int i = 0; i < ord.numOfItems; i++) {
                    generateBillBody(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
                }
                generateBillFooter(total);

                printf("\nDo you want to save the invoice (y/n):\t");
                scanf(" %c", &saveBill); // Note the space before %c to consume any leftover newline

                if (saveBill == 'y' || saveBill == 'Y') {
                    ptr = fopen("RestaurantBill.txt", "ab+");
                    fwrite(&ord, sizeof(orders), 1, ptr);
                    if (fwrite != 0) {
                        printf("\nInvoice is saved successfully.");
                    } else {
                        printf("\nError in saving bill....");
                    }
                    fclose(ptr);
                } else {
                    printf("\nInvoice was not saved.");
                }
                break;

            case 2:
                system("cls");
                ptr = fopen("RestaurantBill.txt", "rb"); // Open file in read mode
                if (ptr == NULL) {
                    printf("\nError: Unable to open file or no invoices found.\n");
                    break;
                }
                printf("\n ========== Your Previous Invoices ==========\n");
                while (fread(&ord1, sizeof(orders), 1, ptr)) {
                    float tot = 0;
                    generateBillHeader(ord1.customer, ord1.date);
                    for (int i = 0; i < ord1.numOfItems; i++) {
                        generateBillBody(ord1.itm[i].item, ord1.itm[i].qty, ord1.itm[i].price);
                        tot += ord1.itm[i].qty * ord1.itm[i].price;
                    }
                    generateBillFooter(tot);
                }
                fclose(ptr);
                break;

             case 3:                                                             // Book a Table
                printf("Enter name of customer: ");
                fgets(booking.customer, 50, stdin);
                booking.customer[strlen(booking.customer) - 1] = 0;             // Remove newline character

                printf("Enter table number (1-10): ");
                scanf("%d", &booking.tableNumber);
                fgetc(stdin);                                                    // Consume newline character

                printf("Enter time slot (e.g., 6:00 PM - 8:00 PM): ");
                fgets(booking.timeSlot, 20, stdin);
                booking.timeSlot[strlen(booking.timeSlot) - 1] = 0;             // Remove newline character

                // Check if the table is already booked for the entered time slot
                int isBooked = 0;
                ptr = fopen("TableBooking.txt", "rb");
                if (ptr != NULL) {
                    while (fread(&existingBooking, sizeof(tableBooking), 1, ptr)) {
                        if (existingBooking.tableNumber == booking.tableNumber &&
                            strcmp(existingBooking.timeSlot, booking.timeSlot) == 0) {
                            isBooked = 1;
                            break;
                        }
                    }
                    fclose(ptr);
                }

                if (isBooked) {
                    printf("\nSorry, Table %d is already booked for the time slot %s.\n", booking.tableNumber, booking.timeSlot);
                } else {
                    printf("Enter number of people: ");
                    scanf("%d", &booking.numberOfPeople);
                    fgetc(stdin); // Consume newline character

                    ptr = fopen("TableBooking.txt", "ab+");
                    if (ptr == NULL) {
                        printf("Error: Unable to open file for writing.\n");
                        break;
                    }
                    fwrite(&booking, sizeof(tableBooking), 1, ptr);
                    fclose(ptr);
                    printf("Table booked successfully.\n");
                }
                break;


            case 4:
                printf("Enter the name of the customer:\t");
                fgets(name, 30, stdin);
                name[strlen(name) - 1] = 0; // Remove newline character
                system("cls");
                ptr = fopen("RestaurantBill.txt", "rb"); // Open file in read mode
                if (ptr == NULL) {
                    printf("\nError: Unable to open file or no invoices found.\n");
                    break;
                }
                printf("\t ========== Invoice of %s ==========\n", name);
                invoiceFound = 0; // Initialize invoiceFound = 0 (false)
                while (fread(&ord1, sizeof(orders), 1, ptr)) {
                    float tot = 0;
                    if (strcmp(ord1.customer, name) == 0) {
                        generateBillHeader(ord1.customer, ord1.date);
                        for (int i = 0; i < ord1.numOfItems; i++) {
                            generateBillBody(ord1.itm[i].item, ord1.itm[i].qty, ord1.itm[i].price);
                            tot += ord1.itm[i].qty * ord1.itm[i].price;
                        }
                        generateBillFooter(tot);
                        invoiceFound = 1; // Set flag to true if invoice is found
                    }
                }
                if (!invoiceFound) {
                    printf("Sorry the invoice for %s does not exist", name);
                }
                fclose(ptr);
                break;

            case 5: // Display All Bookings
                system("cls");
                displayBookings();
                break;

            case 6:
                printf("\n\t\t Bye Bye :)\n\n");
                exit(0);
                break;

            default:
                printf("Sorry invalid option.\n");
                break;
        }

        printf("\nDo you want to perform another operation (y/n):\t");
        scanf(" %c", &temp); // Note the space before %c to consume any leftover newline
    }
    printf("\n\t\t Bye Bye :)\n\n");
    return 0;
}


// Function to display the Veg menu
void displayVegMenu() {
    printf("\n============== Veg Menu ==============\n");
    for (int i = 0; i < vegMenuSize; i++) {
        printf("%d. %s\t\tRs.%.2f\n", i + 1, vegMenu[i].item, vegMenu[i].price);
    }
    printf("===================================\n");
}

// Function to display the Non-Veg menu
void displayNonVegMenu() {
    printf("\n============== Non-Veg Menu ==============\n");
    for (int i = 0; i < nonVegMenuSize; i++) {
        printf("%d. %s\t\tRs.%.2f\n", i + 1, nonVegMenu[i].item, nonVegMenu[i].price);
    }
    printf("===================================\n");
}

// Function to generate bill header
void generateBillHeader(char name[50], char date[30]) {
    printf("\n\n");
    printf("\t\t The Only Place");
    printf("\n---------------------------------------------------------");
    printf("\nDate: %s", date);
    printf("\nInvoice To: %s", name);
    printf("\n");
    printf("---------------------------------------------------------\n");
    printf("Items\t\t");
    printf("Qty\t\t");
    printf("Price\t\t");
    printf("Total\t\t");
    printf("\n---------------------------------------------------------");
    printf("\n\n");
}

// Function to generate bill body
void generateBillBody(char item[20], int qty, float price) {
    printf("%s\t\t", item);
    printf("%d\t\t", qty);
    printf("%.2f\t\t", price);
    printf("%.2f\t\t", qty * price);
    printf("\n");
}

// Function to generate bill footer
void generateBillFooter(float total) {
    printf("\n");
    float dis = 0.1 * total;
    float netTotal = total - dis;
    float cgst = 0.09 * netTotal, grandTotal = netTotal + 2 * cgst;
    printf("---------------------------------------------------------\n");
    printf("Sub Total\t\t\t\t\t%.2f", total);
    printf("\nDiscount @10%%\t\t\t\t\t%.2f", dis);
    printf("\n---------------------------------------------------------");
    printf("\nNet Total\t\t\t\t\t%.2f", netTotal);
    printf("\nCGST @9%%\t\t\t\t\t%.2f", cgst);
    printf("\nSGST @9%%\t\t\t\t\t%.2f", cgst);
    printf("\n---------------------------------------------------------");
    printf("\nGrand Total\t\t\t\t\t%.2f", grandTotal);
    printf("\n---------------------------------------------------------\n");
}

void displayBookings() {
    FILE *ptr;
    tableBooking booking;
    ptr = fopen("TableBooking.txt", "rb");
    if (ptr == NULL) {
        printf("Error: Unable to open file.\n");
        return;
    }
    printf("\nExisting Table Bookings:\n");
    while (fread(&booking, sizeof(tableBooking), 1, ptr)) {
        printf("Customer: %s, Table Number: %d, Time Slot: %s, Number of People: %d\n",
               booking.customer, booking.tableNumber, booking.timeSlot, booking.numberOfPeople);
    }
    fclose(ptr);
}