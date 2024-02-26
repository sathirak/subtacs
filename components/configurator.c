#include <stdio.h>

// Function to check if the file exists
int check_file_existence(const char *filename) {
    FILE *filePtr;
    filePtr = fopen(filename, "r"); // Attempt to open the file in read mode

    // Check if the file exists
    if (filePtr == NULL) {
        fclose(filePtr); // Close the file pointer if it was opened
        return 0; // File doesn't exist
    } else {
        fclose(filePtr); // Close the file pointer
        return 1; // File exists
    }
}

// Function to create the file if it doesn't exist
int create_file(const char *filename) {
    FILE *filePtr;
    filePtr = fopen(filename, "w"); // Create the file

    // Check if file creation was successful
    if (filePtr != NULL) {
        printf("[ Initializing ] %s created successfully.\n", filename);
        fclose(filePtr); // Close the file pointer
        return 1; // File created successfully
    } else {
        printf("[ Initializing ] Error creating %s.\n", filename);
        return 0; // Error creating file
    }
}

// Function to handle configuration file existence
void handle_configuration() {
    const char *config_file = "subtacs.json";

    // Check if the file exists
    if (!check_file_existence(config_file)) {
        printf("[ Initializing ] File not found. Creating %s...\n", config_file);
        if (!create_file(config_file)) {
            printf("[ Initializing ] Failed to create %s.\n", config_file);
        }
    } else {
        printf("[ Initializing ] %s already exists.\n", config_file);
    }
}