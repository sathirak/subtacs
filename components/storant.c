#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include "../lib/cJSON.h"

#include "../main.h"

int get_filenumber(const char* folder_path) {
    DIR *dir;
    struct dirent *entry;
    int file_count = 0;

    // Open the directory
    dir = opendir(folder_path);
    if (dir == NULL) {
        perror("[ error ] unable to open directory \n");
        return -1; // Return -1 to indicate an error
    }

    // Count the files in the directory
    while ((entry = readdir(dir)) != NULL) {
        // Check if the entry is a regular file
        struct stat statbuf;
        char full_path[FILE_PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", folder_path, entry->d_name);
        if (stat(full_path, &statbuf) == 0 && S_ISREG(statbuf.st_mode)) {
            file_count++;
        }
    }

    // Close the directory
    closedir(dir);

    return file_count;
}

void convert_to_json(struct clipboard_container *cargo) {

    cJSON *root = cJSON_CreateObject();
    
    cJSON_AddNumberToObject(root, "id", cargo->id);
    cJSON_AddStringToObject(root, "title", cargo->title);
    cJSON_AddStringToObject(root, "type", cargo->type);
    cJSON_AddStringToObject(root, "source", cargo->source);
    cJSON_AddStringToObject(root, "date-time", cargo->date_time);
    cJSON_AddStringToObject(root, "content", cargo->content);
    cJSON_AddStringToObject(root, "owner-class", cargo->owner_class);

    cJSON *urlsArray = cJSON_AddArrayToObject(root, "urls");
    for (int i = 0; i < cargo->num_urls; ++i) {
        cJSON_AddItemToArray(urlsArray, cJSON_CreateString(cargo->urls[i]));
    }

    cJSON *emailsArray = cJSON_AddArrayToObject(root, "emails");
    for (int i = 0; i < cargo->num_emails; ++i) {
        cJSON_AddItemToArray(emailsArray, cJSON_CreateString(cargo->emails[i]));
    }

    char *jsonString = cJSON_Print(root);

    char filename[FILE_PATH_MAX];

    int file_count = get_filenumber("./data");

    if (file_count == -1) {
        fprintf(stderr, "[ error ] error calculating filename.\n");
        cJSON_Delete(root);
        free(jsonString);
        return;
    }

    sprintf(filename, "./data/%d.json", file_count);

    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        fprintf(file, "%s\n", jsonString);
        fclose(file);
    } else {
        fprintf(stderr, "[ error ] error opening %s for writing.\n", filename);
    }

    cJSON_Delete(root);
    free(jsonString);
}
