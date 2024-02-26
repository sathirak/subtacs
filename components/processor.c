#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "../main.h"

// function that searches for urls in the content
void check_urls(struct clipboard_container *cargo, const char *text)
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
void check_emails(struct clipboard_container *cargo, const char *text)
{
    int i = 0;
    while (text[i] != '\0')
    {
        if (text[i] == '@')
        {
            char email[MAX_EMAIL_LENGTH];
            int j = i - 1;
            int k = 0;

            // Extracting characters before '@' (local part)
            while (j >= 0 && email_valid_char(text[j]) && !email_invalid_char(text[j]) && k < MAX_EMAIL_LENGTH - 1)
            {
                email[k++] = text[j--];
            }
            if (k == 0)
            {
                i++;
                continue; // No characters found before '@', move to the next character
            }
            email[k] = '\0';
            strrev(email); // Reversing the local part

            email[k++] = '@';

            // Extracting characters after '@' (domain part)
            int l = i + 1;
            int m = 0;
            while (text[l] != '\0' && email_valid_char(text[l]) && !email_invalid_char(text[l]) && m < MAX_EMAIL_LENGTH - 1)
            {
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
void process_container_in(char *clipboard_content)
{

    time_t currentTime;
    time(&currentTime);

    container_in.date_time = ctime(&currentTime);

    // Extract title (up to 30 characters or until a newline character)
    int titleLength = strcspn(clipboard_content, "\n");
    titleLength = (titleLength > 30) ? 30 : titleLength;
    snprintf(container_in.title, sizeof(container_in.title), "%.*s", titleLength, clipboard_content);

    // Dynamically allocate memory for content
    container_in.content = strdup(clipboard_content);
    if (container_in.content == NULL)
    {
        // Handle allocation failure
        fprintf(stderr, "[ error ] memory allocation failed for content.\n");
        exit(EXIT_FAILURE);
    }

    check_urls(&container_in, container_in.content);
    check_emails(&container_in, container_in.content);

    output_container_in(&container_in);
}

// catches the source url of the copied place if the copied part is from html
void check_html(char *clipboard_html_content)
{

    char *urlStart = strstr(clipboard_html_content, "SourceURL:");
    char *urlEnd = strstr(clipboard_html_content, "<html>");

    // this is a hardcoded function that can strip out the sourceurl likely will change
    if (urlStart != NULL && urlEnd != NULL)
    {
        // Advance the pointer to skip "SourceURL:"
        urlStart += strlen("SourceURL:");

        int urlLength = urlEnd - urlStart;
        char *url = (char *)malloc(urlLength + 1);
        strncpy(url, urlStart, urlLength);
        url[urlLength] = '\0';

        snprintf(container_in.source, sizeof(container_in.source), "%.*s", 256, url);

        free(url);
    }

    // Output the HTML content
    // printf("\nHTML Content > %s", clipboard_html_content);
}
