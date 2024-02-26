#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../main.h"

void output_container_in()
{
    printf("\n  ---------------------");

    printf("\n  [ i ] title   > %-20s", container_in.title);
    printf("\n  [ i ] source  > %-20s", container_in.source);

    convert_to_json(&container_in);
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
