#include <stdio.h>
#include <stdlib.h>
#include "../lib/cJSON.h"

const char *language_to_string(int language);
// Enum for development languages
enum DevelopmentLanguages {
    C,
    CPlusPlus,
    Java,
    Python,
    JavaScript,
    NumLanguages
};

// Function to check if the file exists
int check_file_existence(const char *filename) {
    FILE *filePtr;
    filePtr = fopen(filename, "r"); // Attempt to open the file in read mode

    // Check if the file exists
    if (filePtr == NULL) {
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
        fclose(filePtr); // Close the file pointer
        return 1; // File created successfully
    } else {
        return 0; // Error creating file
    }
}

// Function to handle configuration file existence
void handle_configuration() {
    const char *config_file = "subtacs.json";

    // Check if the file exists
    if (!check_file_existence(config_file)) {

        printf("   [---    welcome to subtacs    ---]\n");
        printf("   [-- configuring program --]\n");
        printf("   [ configuring ] file not found. creating %s...\n", config_file);

        // Prompt the user for input
        char windows_version[50];

        // Prompt user to select development languages


        printf("   [ configuring ] enter windows version >  ");
        scanf("%s  \n", windows_version);

        int selectedLanguages[NumLanguages] = {0}; // Array to store user-selected languages

        for (int i = 0; i < NumLanguages; ++i) {
            printf("       %d. %s\n", i + 1, language_to_string(i));
        }

        // Prompt user to select development languages
        printf("   [ configuring ] select languages > \n");
        int language;
        while (scanf("%d", &language) == 1) {
            if (language >= 1 && language <= NumLanguages) {
                selectedLanguages[language - 1] = 1;
            } else {
                printf("   [ configuring ] invalid input. please enter a valid language number.\n");
            }
        }

        // Create a cJSON object and add user input
        cJSON *root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "windows-v", windows_version);

        cJSON *languagesArray = cJSON_CreateArray();
        for (int i = 0; i < NumLanguages; ++i) {
            if (selectedLanguages[i]) {
                cJSON_AddItemToArray(languagesArray, cJSON_CreateString(language_to_string(i)));
            }
        }

        cJSON_AddItemToObject(root, "dev-langs", languagesArray);

        // Write cJSON object to the JSON file
        FILE *filePtr = fopen(config_file, "w");
        if (filePtr != NULL) {
            char *json_str = cJSON_Print(root);
            fprintf(filePtr, "%s", json_str);
            fclose(filePtr);
            free(json_str);
            cJSON_Delete(root);

            printf("   [ configuring ] configuration saved successfully.\n");
        } else {
            printf("   [ configuring ] error creating %s.\n", config_file);
        }
    } else {
        //printf("   [ configuring ] %s already exists.\n", config_file);
    }
}

// Function to convert language enum to string
const char *language_to_string(int language) {
    switch (language) {
        case C:
            return "C";
        case CPlusPlus:
            return "C++";
        case Java:
            return "Java";
        case Python:
            return "Python";
        case JavaScript:
            return "JavaScript";
        // Add more languages as needed
        default:
            return "Unknown";
    }
}
