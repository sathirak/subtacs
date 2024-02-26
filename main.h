
#ifndef MAIN_H
#define MAIN_H

#define MAX_EMAIL_LENGTH 256

#define MAX_URLS 35

#define FILE_PATH_MAX 1024

// clipboard item structure 
struct clipboard_container {
    char title[31];
    char type[20];
    char source[256];
    char *content;
    char urls[MAX_URLS][256];
    int num_urls;
    char emails[MAX_URLS][256];
    int num_emails;
    char *date_time;
};

void handle_configuration();

void check_urls(struct clipboard_container *cargo, const char *text);

void check_emails(struct clipboard_container *cargo, const char *text);

int get_filenumber(const char* folder_path);

void convert_to_json(struct clipboard_container *cargo);

void process_container_in(char *clipboard_text);

void check_html(char *clipboard_html_content);

void output_container_in();

void reset_container();

void output_window(HWND hwnd, const char *text);

BOOL open_clipboard(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

extern struct clipboard_container container_in;



#endif