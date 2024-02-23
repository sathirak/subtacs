#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../main.h"

void cargo_out()
{
    printf("\n\033[36m_____________\033[0m\n");

    printf("\n\033[33mtitle   >\033[0m %s ...", CargoClip.title);

    printf("\n\033[33msource  >\033[0m %s", CargoClip.source);

    printf("\n\033[33murls    >\033[0m %d", CargoClip.num_urls);

    printf("\n\033[33memails  >\033[0m %d", CargoClip.num_emails);

    cargo_to_json(&CargoClip);
    reset_cargo();
}

void reset_cargo()
{
    memset(CargoClip.title, 0, sizeof(CargoClip.title));
    memset(CargoClip.type, 0, sizeof(CargoClip.type));
    memset(CargoClip.source, 0, sizeof(CargoClip.source));
    memset(CargoClip.content, 0, strlen(CargoClip.content));

    for (int i = 0; i < MAX_URLS; ++i)
    {
        memset(CargoClip.urls[i], 0, sizeof(CargoClip.urls[i]));
        memset(CargoClip.emails[i], 0, sizeof(CargoClip.emails[i]));
    }

    CargoClip.num_urls = 0;
    CargoClip.num_emails = 0;

    free(CargoClip.content);
}
