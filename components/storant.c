#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/cJSON.h"
#include "../lib/sqlite/sqlite3.h"

#include "../main.h"

// this function lets you insert the cargo to the database
void insert_to_db(struct clipboard_container *cargo) {

    // prepare 2 JSON arrays to put the found urls and emails

    cJSON *urls_arr = cJSON_CreateArray();
    for (int i = 0; i < cargo->num_urls; ++i) {
        cJSON_AddItemToArray(urls_arr, cJSON_CreateString(cargo->urls[i]));
    }

    cJSON *email_arr = cJSON_CreateArray();
    for (int i = 0; i < cargo->num_emails; ++i) {
        cJSON_AddItemToArray(email_arr, cJSON_CreateString(cargo->emails[i]));
    }

    // add the two jsons into two strings
    char *url_json = cJSON_Print(urls_arr);
    char *email_json = cJSON_Print(email_arr);

    // now initialise the db and open it
    sqlite3 *db;
    char *errMsg = 0;
    int rc;

    // open the cargo database, error if cannot open the db
    rc = sqlite3_open("cargo.db", &db);
    if (rc) {
        fprintf(stderr, "   [ configuring ] cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

    }

    sqlite3_stmt *stmt;

    // insert query to into the table
    const char *sql = "INSERT INTO clipboard_container (title, type, source, content, urls, emails, num_urls, num_emails, date_time, owner_class) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

    // prepare a statement for the insert query
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "   [ configuring ] failed to prepare statement: %s\n", sqlite3_errmsg(db));

    }

    sqlite3_bind_text(stmt, 1, cargo->title, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, cargo->type, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, cargo->source, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, cargo->content, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, url_json, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, email_json, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 7, cargo->num_urls);
    sqlite3_bind_int(stmt, 8, cargo->num_emails);
    sqlite3_bind_text(stmt, 9, cargo->date_time, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 10, cargo->owner_class, -1, SQLITE_STATIC);

    // execute prepared statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "   [ configuring ] failed to execute statement: %s\n", sqlite3_errmsg(db));

    }

    // free the JSON and other strings
    cJSON_Delete(urls_arr);
    cJSON_Delete(email_arr);
    free(url_json);
    free(email_json);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

}
