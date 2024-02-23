#include <yaml.h>
#include <stdio.h>
#include <string.h>

#define MAX_URLS 10

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

struct Cargo CargoClip;

void write_cargo_to_yaml(const char *filename, const struct Cargo *cargo) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        return;
    }

    yaml_emitter_t emitter;
    yaml_emitter_initialize(&emitter);

    yaml_emitter_set_output_file(&emitter, file);

    yaml_event_t event;

    // Start YAML document
    yaml_stream_start_event_initialize(&event, YAML_UTF8_ENCODING);
    yaml_emitter_emit(&emitter, &event);

    // Start document
    yaml_document_start_event_initialize(&event, NULL, NULL, NULL, 0);
    yaml_emitter_emit(&emitter, &event);

    // Write Cargo struct fields
    yaml_mapping_start_event_initialize(&event, NULL, NULL, 1, YAML_BLOCK_MAPPING_STYLE);
    yaml_emitter_emit(&emitter, &event);

    // Title
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)"title", -1, 1, 0, YAML_PLAIN_SCALAR_STYLE);
    yaml_emitter_emit(&emitter, &event);

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)cargo->title, strlen(cargo->title), 1, 0, YAML_PLAIN_SCALAR_STYLE);
    yaml_emitter_emit(&emitter, &event);

    // Type
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)"type", -1, 1, 0, YAML_PLAIN_SCALAR_STYLE);
    yaml_emitter_emit(&emitter, &event);

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)cargo->type, strlen(cargo->type), 1, 0, YAML_PLAIN_SCALAR_STYLE);
    yaml_emitter_emit(&emitter, &event);

    // Source
    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)"source", -1, 1, 0, YAML_PLAIN_SCALAR_STYLE);
    yaml_emitter_emit(&emitter, &event);

    yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)cargo->source, strlen(cargo->source), 1, 0, YAML_PLAIN_SCALAR_STYLE);
    yaml_emitter_emit(&emitter, &event);

    // Content (assuming it is a string)
    if (cargo->content) {
        yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)"content", -1, 1, 0, YAML_PLAIN_SCALAR_STYLE);
        yaml_emitter_emit(&emitter, &event);

        yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)cargo->content, strlen(cargo->content), 1, 0, YAML_PLAIN_SCALAR_STYLE);
        yaml_emitter_emit(&emitter, &event);
    }

    // URLs
    if (cargo->num_urls > 0) {
        yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)"urls", -1, 1, 0, YAML_PLAIN_SCALAR_STYLE);
        yaml_emitter_emit(&emitter, &event);

        yaml_sequence_start_event_initialize(&event, NULL, NULL, 1, YAML_BLOCK_SEQUENCE_STYLE);
        yaml_emitter_emit(&emitter, &event);

        for (int i = 0; i < cargo->num_urls; i++) {
            yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)cargo->urls[i], strlen(cargo->urls[i]), 1, 0, YAML_PLAIN_SCALAR_STYLE);
            yaml_emitter_emit(&emitter, &event);
        }

        yaml_sequence_end_event_initialize(&event);
        yaml_emitter_emit(&emitter, &event);
    }

    // Emails
    if (cargo->num_emails > 0) {
        yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)"emails", -1, 1, 0, YAML_PLAIN_SCALAR_STYLE);
        yaml_emitter_emit(&emitter, &event);

        yaml_sequence_start_event_initialize(&event, NULL, NULL, 1, YAML_BLOCK_SEQUENCE_STYLE);
        yaml_emitter_emit(&emitter, &event);

        for (int i = 0; i < cargo->num_emails; i++) {
            yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t *)cargo->emails[i], strlen(cargo->emails[i]), 1, 0, YAML_PLAIN_SCALAR_STYLE);
            yaml_emitter_emit(&emitter, &event);
        }

        yaml_sequence_end_event_initialize(&event);
        yaml_emitter_emit(&emitter, &event);
    }

    // End mapping
    yaml_mapping_end_event_initialize(&event);
    yaml_emitter_emit(&emitter, &event);

    // End document
    yaml_document_end_event_initialize(&event, 0);
    yaml_emitter_emit(&emitter, &event);

    // End stream
    yaml_stream_end_event_initialize(&event);
    yaml_emitter_emit(&emitter, &event);

    // Clean up
    yaml_emitter_delete(&emitter);
    fclose(file);
}