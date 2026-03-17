#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <direct.h>


#include "core.h"
#include "db.h"
#include "record.h"
#include "table.h"
#include "error.h"

static Database *g_db = NULL;
static Table *g_active_table = NULL;


int core_is_initialized(void) {
    return g_db != NULL;
}

/*---------------------------------------------*/

int core_init(const char *path)
{
    if (g_db) 
        return 0; 

    g_db = db_create();
    
    if (!g_db)
        return -1;

    #ifdef _WIN32
        _mkdir(path);
    #else
        mkdir(path, 0777);
    #endif

    if (db_open(g_db, path) != 0) {
        fprintf(stderr, "%s\n", error_get());
        db_destroy(g_db);
        g_db = NULL;
        return -1;
    }

    return 0;
}

/*---------------------------------------------*/

void core_shutdown(void)
{
    if (!g_db)
        return;

    db_close(g_db);
    db_destroy(g_db);
    g_db = NULL;
}

/*---------------------------------------------*/

int core_create_table(const char *name)
{
    if (!g_db)
        return -1;

    if (db_create_table(g_db, name) != 0) {
        fprintf(stderr, "%s\n", error_get());
        return -1;
    }

    return 0;
}

/*---------------------------------------------*/
int core_drop_table(const char *name)
{
    if (!g_db) return -1;

    if (db_drop_table(g_db, name) != 0) {
        fprintf(stderr, "%s\n", error_get()); 
        return -1;
    }
    return 0;
}

/*---------------------------------------------*/

void core_list_tables(void) {
    if (!g_db) return;

    size_t count = db_table_count(g_db);

    for (size_t i = 0; i < count; i++) {
        Table *table = db_table_at(g_db, i);
        if (table)
            printf("%s\n", table_get_name(table));
    }
}

/*---------------------------------------------*/

int core_use_table(const char *name)
{
    if (!g_db)
        return -1;

    Table *table = db_get_table(g_db, name);
    if (!table) {
        return -1;
    }

    g_active_table = table;
    return 0;
}

/*---------------------------------------------*/

int core_insert_record(char **fields, int field_count)
{
    if (!g_active_table) {
        return -1;
    }

    if (field_count > NUM_FIELDS) {
        return -1;
    }

    Record *rec = record_create();
    if (!rec)
        return -1;

    for (int i = 0; i < field_count; i++)
        record_set_field(rec, i, fields[i]);

    int result = table_insert(g_active_table, rec);
    record_destroy(rec);

    if (result != 0)
        printf("%s\n", error_get());

    return result;
}

/*---------------------------------------------*/

int core_read_record(int index)
{
    if (!g_active_table) {
        return -1;
    }

    Record *rec = record_create();
    if (!rec)
        return -1;

    if (table_read(g_active_table, index, rec) != 0) {
        record_destroy(rec);
        return -1;
    }

    for (int i = 0; i < NUM_FIELDS; i++) {
        const char *value = record_get_field(rec, i);
        if (value)
            printf("[%d] %s\n", i, value);
    }

    record_destroy(rec);
    return 0;
}

/*---------------------------------------------*/

int core_update_record(int index, char **fields, int field_count)
{
    if (!g_active_table) {
        return -1;
    }

    if (field_count > NUM_FIELDS) {
        return -1;
    }

    Record *rec = record_create();
    if (!rec)
        return -1;

    for (int i = 0; i < field_count; i++)
        record_set_field(rec, i, fields[i]);

    int result = table_update(g_active_table, index, rec);
    record_destroy(rec);

    if (result != 0)
        printf("%s\n", error_get());

    return result;
}

/*---------------------------------------------*/

int core_delete_record(int index)
{
    if (!g_active_table) {
        return -1;
    }

    if (table_delete(g_active_table, index) != 0) {
        return -1;
    }

    return 0;
}

/*---------------------------------------------*/

void core_list_records(void)
{
    if (!g_active_table) {
        return;
    }

    int count = table_record_count(g_active_table);

    for (int i = 0; i < count; i++) {

        Record *rec = record_create();
        if (!rec)
            return;

        if (table_read(g_active_table, i, rec) == 0) {

            for (int j = 0; j < NUM_FIELDS; j++) {
                const char *value = record_get_field(rec, j);
                if (value)
                    printf("  %s\n", value);
            }
        }

        record_destroy(rec);
    }
}