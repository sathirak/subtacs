#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include "../lib/cJSON.h"

#include "../main.h"


char* calc_filename(const char* folder_path) {
    DIR *dir;
    struct dirent *entry;
    int file_count = 0;
    
    // Open the directory
    dir = opendir(folder_path);
    if (dir == NULL) {
        perror("Unable to open directory");
        return NULL;
    }

    // Count the files in the directory
    while ((entry = readdir(dir)) != NULL) {
        // Check if the entry is a regular file
        struct stat statbuf;
        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", folder_path, entry->d_name);
        if (stat(full_path, &statbuf) == 0 && S_ISREG(statbuf.st_mode)) {
            file_count++;
        }
    }

    // Close the directory
    closedir(dir);

    // Convert the count to hexadecimal string
    char* hex_count = (char*)malloc(sizeof(char) * 9); // Assuming 32-bit integer
    if (hex_count == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }
    sprintf(hex_count, "%X", file_count);

    return hex_count;
}

void cargo_to_json(struct Cargo *cargo) {
    cJSON *root = cJSON_CreateObject(); // Create a JSON object

    // Add string fields
    cJSON_AddStringToObject(root, "title", cargo->title);
    cJSON_AddStringToObject(root, "type", cargo->type);
    cJSON_AddStringToObject(root, "source", cargo->source);
    cJSON_AddStringToObject(root, "content", cargo->content);

    // Add arrays
    cJSON *urlsArray = cJSON_AddArrayToObject(root, "urls");
    for (int i = 0; i < cargo->num_urls; ++i) {
        cJSON_AddItemToArray(urlsArray, cJSON_CreateString(cargo->urls[i]));
    }

    cJSON *emailsArray = cJSON_AddArrayToObject(root, "emails");
    for (int i = 0; i < cargo->num_emails; ++i) {
        cJSON_AddItemToArray(emailsArray, cJSON_CreateString(cargo->emails[i]));
    }

    // Print JSON to string
    char *jsonString = cJSON_Print(root);

    // Calculate filename
    char filename[PATH_MAX];
    char* filehex = calc_filename("./data");
    if (filehex == NULL) {
        fprintf(stderr, "Error calculating filename.\n");
        cJSON_Delete(root);
        free(jsonString);
        return;
    }
    sprintf(filename, "./data/%s.json", filehex);
    free(filehex);

    // Save JSON to file
    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        fprintf(file, "%s\n", jsonString);
        fclose(file);
    } else {
        fprintf(stderr, "Error opening file for writing.\n");
    }

    // Free cJSON objects and JSON string
    cJSON_Delete(root);
    free(jsonString);
}


