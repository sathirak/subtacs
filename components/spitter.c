#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../main.h"

void remove_escape_chars(char *str)
{
    int i, j;
    for (i = 0, j = 0; str[i] != '\0'; i++)
    {
        if (str[i] != '\\' && str[i] != '\n' && str[i] != '\t' && str[i] != '\r' && str[i] != '\f' && str[i] != '\b')
        {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

void output_container_in()
{
    remove_escape_chars(container_in.title);

    printf("\n\033[1;36m   > %-30.30s... \033[0m",container_in.title);

    printf("\n");

    insert_to_db(&container_in);
    reset_container();
}

void reset_container()
{
    memset(container_in.title, 0, sizeof(container_in.title));
    memset(container_in.type, 0, sizeof(container_in.type));
    memset(container_in.source, 0, sizeof(container_in.source));
    memset(container_in.date_time, 0, sizeof(container_in.date_time));
    memset(container_in.content, 0, strlen(container_in.content));

    for (int i = 0; i < MAX_URLS; ++i)
    {
        memset(container_in.urls[i], 0, sizeof(container_in.urls[i]));
        memset(container_in.emails[i], 0, sizeof(container_in.emails[i]));
    }

    container_in.num_urls = 0;
    container_in.num_emails = 0;

    free(container_in.content);
}
