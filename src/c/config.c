#include "config.h"
#include "internal.h"

#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *trim_in_place(char *text)
{
    if (text == NULL) {
        return NULL;
    }

    while (*text != '\0' && isspace((unsigned char)*text)) {
        ++text;
    }

    char *end = text + strlen(text);
    while (end > text && isspace((unsigned char)*(end - 1))) {
        --end;
    }
    *end = '\0';
    return text;
}

static void join_path(char *buffer, size_t buffer_size, const char *directory, const char *filename)
{
    if (buffer == NULL || buffer_size == 0) {
        return;
    }

    if (directory == NULL || *directory == '\0') {
        snprintf(buffer, buffer_size, "%s", filename);
        return;
    }

    size_t length = strlen(directory);
    if (directory[length - 1] == '/') {
        snprintf(buffer, buffer_size, "%s%s", directory, filename);
    } else {
        snprintf(buffer, buffer_size, "%s/%s", directory, filename);
    }
}

static constitution_constituency_kind_t parse_constituency_kind(const char *text)
{
    if (text == NULL) {
        return CONSTITUTION_CONSTITUENCY_KIND_OTHER;
    }
    if (strcmp(text, "共和国") == 0) return CONSTITUTION_CONSTITUENCY_KIND_NATION;
    if (strcmp(text, "大区") == 0) return CONSTITUTION_CONSTITUENCY_KIND_REGION;
    if (strcmp(text, "省") == 0) return CONSTITUTION_CONSTITUENCY_KIND_PROVINCE;
    if (strcmp(text, "广域市") == 0) return CONSTITUTION_CONSTITUENCY_KIND_METROPOLITAN_CITY;
    if (strcmp(text, "省区") == 0) return CONSTITUTION_CONSTITUENCY_KIND_SUB_PREFECTURE;
    if (strcmp(text, "公社") == 0) return CONSTITUTION_CONSTITUENCY_KIND_COMMUNE;
    return CONSTITUTION_CONSTITUENCY_KIND_OTHER;
}

static constitution_status_t load_regions_csv(constitution_t *constitution, const char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return CONSTITUTION_STATUS_NOT_FOUND;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL) {
        char *cursor = trim_in_place(line);
        if (cursor == NULL || *cursor == '\0' || *cursor == '#') {
            continue;
        }

        char *fields[6] = {0};
        size_t field_count = 0;
        char *token = strtok(cursor, ",");
        while (token != NULL && field_count < 6) {
            fields[field_count++] = trim_in_place(token);
            token = strtok(NULL, ",");
        }

        if (field_count < 4) {
            continue;
        }

        const char *label = NULL;
        const char *seat_name = NULL;
        const char *population_text = NULL;
        const char *parent_text = NULL;

        if (field_count >= 6) {
            label = (fields[2] == NULL || fields[2][0] == '\0' || strcmp(fields[2], "-") == 0) ? NULL : fields[2];
            seat_name = (fields[3] == NULL || fields[3][0] == '\0' || strcmp(fields[3], "-") == 0) ? NULL : fields[3];
            population_text = fields[4];
            parent_text = fields[5];
        } else if (field_count == 5) {
            label = (fields[2] == NULL || fields[2][0] == '\0' || strcmp(fields[2], "-") == 0) ? NULL : fields[2];
            population_text = fields[3];
            parent_text = fields[4];
        } else {
            population_text = fields[2];
            parent_text = fields[3];
        }

        unsigned long long population = strtoull(population_text, NULL, 10);
        constitution_add_constituency(constitution,
                                      fields[0],
                                      label,
                                      seat_name,
                                      parse_constituency_kind(fields[1]),
                                      population,
                                      (strcmp(parent_text, "-") == 0 || parent_text[0] == '\0') ? NULL : parent_text);
    }

    fclose(file);
    return CONSTITUTION_STATUS_OK;
}

static constitution_status_t load_people_csv(constitution_t *constitution, const char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return CONSTITUTION_STATUS_NOT_FOUND;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL) {
        char *cursor = trim_in_place(line);
        if (cursor == NULL || *cursor == '\0' || *cursor == '#') {
            continue;
        }

        char *fields[4] = {0};
        size_t field_count = 0;
        char *token = strtok(cursor, ",");
        while (token != NULL && field_count < 4) {
            fields[field_count++] = trim_in_place(token);
            token = strtok(NULL, ",");
        }

        if (field_count < 4) {
            continue;
        }

        member_t *member = constitution_add_member(constitution, fields[0], fields[1], (unsigned)strtoul(fields[2], NULL, 10), fields[3]);
        if (member != NULL && (strcmp(fields[3], "-") == 0 || fields[3][0] == '\0')) {
            member_assign_constituency(member, NULL);
        }
    }

    fclose(file);
    return CONSTITUTION_STATUS_OK;
}

constitution_status_t constitution_load_config_directory(constitution_t *constitution, const char *directory)
{
    if (constitution == NULL || directory == NULL) {
        return CONSTITUTION_STATUS_INVALID_ARGUMENT;
    }

    char path[512];
    FILE *probe = NULL;

    join_path(path, sizeof(path), directory, "regions.csv");
    probe = fopen(path, "r");
    if (probe != NULL) {
        fclose(probe);
        constitution_status_t status = load_regions_csv(constitution, path);
        if (status != CONSTITUTION_STATUS_OK && status != CONSTITUTION_STATUS_NOT_FOUND) {
            return status;
        }
    }

    join_path(path, sizeof(path), directory, "people.csv");
    probe = fopen(path, "r");
    if (probe != NULL) {
        fclose(probe);
        constitution_status_t status = load_people_csv(constitution, path);
        if (status != CONSTITUTION_STATUS_OK && status != CONSTITUTION_STATUS_NOT_FOUND) {
            return status;
        }
    }

    return CONSTITUTION_STATUS_OK;
}
