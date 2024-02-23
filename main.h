
#ifndef MAIN_H
#define MAIN_H

#define MAX_EMAIL_LENGTH 256

#define MAX_URLS 35

// clipboard item structure 
struct Cargo {
    char title[31];
    char type[20];
    char source[256];
    char *content;
    char urls[MAX_URLS][256];
    int num_urls;
    char emails[MAX_URLS][256];
    int num_emails;

};


void search_urls(struct Cargo *cargo, const char *text);

void search_emails(struct Cargo *cargo, const char *text);

void search_content(char *clipboardText);

void search_html(char *htmlText);

void cargo_out();

void write_cargo_to_yaml(const char *filename, const struct Cargo *cargo);

void reset_cargo();

BOOL open_clipboard(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif