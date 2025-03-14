#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define FILE_NAME "inventory.dat"

// Product structure
typedef struct {
    char name[MAX_NAME_LENGTH];
    int quantity;
    float price;
} Product;

// Function to clear the input buffer
void clearBuffer() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Function to open the inventory file (creates the file if it doesn't exist)
FILE* openFile(const char* mode) {
    FILE *file = fopen(FILE_NAME, mode);
    if (!file) {
        printf("Error opening the inventory file.\n");
        exit(1);
    }
    return file;
}

// Function to add a product to the inventory
void addProduct() {
    Product product;
    FILE *file = openFile("a+b"); // Open in append mode to add products at the end

    printf("Enter product name: ");
    clearBuffer();
    fgets(product.name, MAX_NAME_LENGTH, stdin);
    product.name[strcspn(product.name, "\n")] = '\0';  // Remove newline character

    printf("Enter quantity: ");
    scanf("%d", &product.quantity);

    printf("Enter price: ");
    scanf("%f", &product.price);

    fwrite(&product, sizeof(Product), 1, file);
    fclose(file);
    printf("Product added successfully.\n");
}

// Function to display all products in the inventory
void showInventory() {
    Product product;
    FILE *file = openFile("r+b"); // Open in read-write mode

    printf("\nInventory:\n");
    while (fread(&product, sizeof(Product), 1, file)) {
        printf("Name: %s | Quantity: %d | Price: %.2f\n", product.name, product.quantity, product.price);
    }
    fclose(file);
}

// Function to search for a product by name
void searchProduct() {
    char searchName[MAX_NAME_LENGTH];
    Product product;
    int found = 0;
    FILE *file = openFile("r+b");

    printf("Enter the product name to search: ");
    clearBuffer();
    fgets(searchName, MAX_NAME_LENGTH, stdin);
    searchName[strcspn(searchName, "\n")] = '\0';

    while (fread(&product, sizeof(Product), 1, file)) {
        if (strcmp(product.name, searchName) == 0) {
            printf("Product found: %s | Quantity: %d | Price: %.2f\n", product.name, product.quantity, product.price);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Product not found.\n");
    }
    fclose(file);
}

// Function to delete a product from the inventory
void deleteProduct() {
    char deleteName[MAX_NAME_LENGTH];
    FILE *file, *tempFile;
    Product product;
    int found = 0;

    printf("Enter the product name to delete: ");
    clearBuffer();
    fgets(deleteName, MAX_NAME_LENGTH, stdin);
    deleteName[strcspn(deleteName, "\n")] = '\0';

    file = openFile("r+b");
    tempFile = fopen("temp.dat", "wb");

    while (fread(&product, sizeof(Product), 1, file)) {
        if (strcmp(product.name, deleteName) != 0) {
            fwrite(&product, sizeof(Product), 1, tempFile);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove(FILE_NAME);
        rename("temp.dat", FILE_NAME);
        printf("Product deleted successfully.\n");
    } else {
        printf("Product not found for deletion.\n");
    }
}

// Function to modify a product's quantity
void modifyProduct() {
    char modifyName[MAX_NAME_LENGTH];
    Product product;
    int found = 0;
    FILE *file, *tempFile;

    printf("Enter the product name to modify: ");
    clearBuffer();
    fgets(modifyName, MAX_NAME_LENGTH, stdin);
    modifyName[strcspn(modifyName, "\n")] = '\0';

    file = openFile("r+b");
    tempFile = fopen("temp.dat", "wb");

    while (fread(&product, sizeof(Product), 1, file)) {
        if (strcmp(product.name, modifyName) == 0) {
            printf("Product found: %s | Current Quantity: %d | Current Price: %.2f\n", product.name, product.quantity, product.price);
            printf("Enter new quantity: ");
            scanf("%d", &product.quantity);
            fwrite(&product, sizeof(Product), 1, tempFile);
            found = 1;
        } else {
            fwrite(&product, sizeof(Product), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove(FILE_NAME);
        rename("temp.dat", FILE_NAME);
        printf("Product modified successfully.\n");
    } else {
        printf("Product not found for modification.\n");
    }
}

// Main menu function to handle user options
void menu() {
    int option;

    do {
        printf("\nInventory Menu:\n");
        printf("1. Add product\n");
        printf("2. Show inventory\n");
        printf("3. Search for product\n");
        printf("4. Delete product\n");
        printf("5. Modify product\n");
        printf("6. Exit\n");
        printf("Select an option: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                addProduct();
                break;
            case 2:
                showInventory();
                break;
            case 3:
                searchProduct();
                break;
            case 4:
                deleteProduct();
                break;
            case 5:
                modifyProduct();
                break;
            case 6:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid option. Please try again.\n");
                break;
        }
    } while (option != 6);
}

int main() {
    menu();
    return 0;
}
