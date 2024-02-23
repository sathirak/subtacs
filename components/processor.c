#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../main.h"

struct Cargo CargoClip;
// final function that gets processed

void cargo_out()
{
    printf("\n\n\033[34m#############################################\033[0m\n");

    printf("\n\033[33mtitle   > \n\033[0m%s", CargoClip.title);
    printf("\n\033[33msource  > \n\033[0m%s", CargoClip.source);

    printf("\n\033[33murls    > %d\033[0m \n", CargoClip.num_urls);
    for (int i = 0; i < CargoClip.num_urls; ++i)
    {
        printf("\t > %s\n", CargoClip.urls[i]);
    }

    printf("\n\033[33memails  > %d\033[0m \n", CargoClip.num_emails);
    for (int i = 0; i < CargoClip.num_emails; ++i)
    {
        printf("\t > %s\n", CargoClip.emails[i]);
    }

    printf("\n\033[33mcontent > \033[0m\n%s\n", CargoClip.content);

    free(CargoClip.content);
}

// function that searches for urls in the content
void search_urls(struct Cargo *cargo, const char *text)
{
    // Search for URLs in the text and store them in the Cargo struct
    const char *urlStart = text;
    while ((urlStart = strstr(urlStart, "http")) != NULL && cargo->num_urls < MAX_URLS)
    {
        const char *urlEnd = strpbrk(urlStart, " \r\n\t");
        if (urlEnd == NULL)
        {
            urlEnd = urlStart + strlen(urlStart);
        }

        int urlLength = urlEnd - urlStart;
        strncpy(cargo->urls[cargo->num_urls], urlStart, urlLength);
        cargo->urls[cargo->num_urls][urlLength] = '\0';

        printf("URL > %s\n", cargo->urls[cargo->num_urls]);

        cargo->num_urls++;

        // Move the pointer to search for the next URL
        urlStart = urlEnd;
    }
}

// function set that catches emails
int email_valid_char(char c)
{
    return isalnum(c) || c == '.' || c == '+' || c == '-' || c == '_' || c == '@';
}

int email_invalid_char(char c)
{
    return c == ' ' || c == ',' || c == ';' || c == ':' || c == '\n' || c == '\0';
}

// main function that catches emails
void search_emails(struct Cargo *cargo, const char *text) {
    int i = 0;
    while (text[i] != '\0') {
        if (text[i] == '@') {
            char email[MAX_EMAIL_LENGTH];
            int j = i - 1;
            int k = 0;

            // Extracting characters before '@' (local part)
            while (j >= 0 && email_valid_char(text[j]) && !email_invalid_char(text[j]) && k < MAX_EMAIL_LENGTH - 1) {
                email[k++] = text[j--];
            }
            if (k == 0) {
                i++;
                continue; // No characters found before '@', move to the next character
            }
            email[k] = '\0';
            strrev(email); // Reversing the local part

            email[k++] = '@';

            // Extracting characters after '@' (domain part)
            int l = i + 1;
            int m = 0;
            while (text[l] != '\0' && email_valid_char(text[l]) && !email_invalid_char(text[l]) && m < MAX_EMAIL_LENGTH - 1) {
                email[k + m] = text[l];
                l++;
                m++;
            }
            email[k + m] = '\0';

            // Store the email in the Cargo struct
            strncpy(cargo->emails[cargo->num_emails], email, sizeof(email));
            cargo->emails[cargo->num_emails][sizeof(email) - 1] = '\0';
            cargo->num_emails++;

            // Move to the next character
            i = l;
        }
        i++;
    }
}

// main function of the script that parses the raw content
void search_content(char *clipboardText) {

    

    // Extract title (up to 30 characters or until a newline character)
    int titleLength = strcspn(clipboardText, "\n");
    titleLength = (titleLength > 30) ? 30 : titleLength;
    snprintf(CargoClip.title, sizeof(CargoClip.title), "%.*s", titleLength, clipboardText);

    // Dynamically allocate memory for content
    CargoClip.content = strdup(clipboardText);
    if (CargoClip.content == NULL)
    {
        // Handle allocation failure
        fprintf(stderr, "Memory allocation failed for content.\n");
        exit(EXIT_FAILURE);
    }

    CargoClip.num_urls = 0; // Reset num_urls for the new array
    memset(CargoClip.urls, 0, sizeof(CargoClip.urls));     
    
    CargoClip.num_emails = 0; // Reset num_emails for the new array
    memset(CargoClip.emails, 0, sizeof(CargoClip.emails)); 

    search_urls(&CargoClip, CargoClip.content);
    search_emails(&CargoClip, CargoClip.content);

    cargo_out();    
}

// catches the source url of the copied place if the copied part is from html
void search_html(char *htmlText)
{

    char *urlStart = strstr(htmlText, "SourceURL:");
    char *urlEnd = strstr(htmlText, "<html>");

    // this is a hardcoded function that can strip out the sourceurl likely will change
    if (urlStart != NULL && urlEnd != NULL)
    {
        // Advance the pointer to skip "SourceURL:"
        urlStart += strlen("SourceURL:");

        int urlLength = urlEnd - urlStart;
        char *url = (char *)malloc(urlLength + 1);
        strncpy(url, urlStart, urlLength);
        url[urlLength] = '\0';

        snprintf(CargoClip.source, sizeof(CargoClip.source), "%.*s", 256, url);

        free(url);
    }

    // Output the HTML content
    // printf("\nHTML Content > %s", htmlText);
}
