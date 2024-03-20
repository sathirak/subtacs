#include <stdio.h>
#include <stdlib.h>
#include "../lib/sqlite/sqlite3.h"
#include "../lib/cJSON/cJSON.h"

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
    const char *cargo_db = "cargo.db";

    // Check if the file exists
    if (!check_file_existence(config_file)) {

        printf("   [---    welcome to subtacs    ---]\n");
        printf("   [-- configuring program --]\n");
        printf("   [ configuring ] file not found. creating %s...\n", config_file);

        // Prompt the user for input
        char windows_version[50];

        // Prompt user to select development languages


        printf("   [ configuring ] enter windows version >  ");
        scanf("%s", windows_version);

        // Create a cJSON object and add user input
        cJSON *root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "windows-v", windows_version);


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

    if (!check_file_existence(cargo_db)) {

        printf("   [ configuring ] database not found. creating %s...\n", cargo_db);

        sqlite3 *db;
        char *errMsg = 0;
        int rc;

        // Open the database
        rc = sqlite3_open("cargo.db", &db);
        if (rc) {
            fprintf(stderr, "   [ configuring ] cannot open database.\n", sqlite3_errmsg(db));
            sqlite3_close(db);
        } else {
            fprintf(stdout, "   [ configuring ] database opened successfully\n");
        }

        const char *sql = "CREATE TABLE IF NOT EXISTS clipboard_container ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "title TEXT,"
                            "type TEXT,"
                            "source TEXT,"
                            "content TEXT,"
                            "urls TEXT," 
                            "emails TEXT,"
                            "num_urls INTEGER,"
                            "num_emails INTEGER,"
                            "date_time TEXT NOT NULL,"
                            "owner_class TEXT"
        ");";

        rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "   [ configuring ] sql error\n\n", errMsg);
            sqlite3_free(errMsg);
        } else {
            fprintf(stdout, "   [ configuring ] table created successfully\n");
        }

        // Close the database
        sqlite3_close(db);

    }
}