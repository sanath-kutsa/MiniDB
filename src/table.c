#include "minidb.h"

#include <stdlib.h>
#include <string.h>

static char *mdb_copy_string(const char *source) {
    size_t length;
    char *copy;
    if (source == NULL) return NULL;
    length = strlen(source) + 1;
    copy = (char *)malloc(length);
    if (copy != NULL) memcpy(copy, source, length);
    return copy;
}

static void mdb_row_destroy(MDBRow *row) {
    size_t i;
    if (row == NULL) return;
    for (i = 0; i < row->value_count; ++i) {
        mdb_value_destroy(&row->values[i]);
    }
    free(row->values);
    row->values = NULL;
    row->value_count = 0;
}

MDBTable *mdb_table_create(const char *name, const MDBColumn *columns,
                           size_t column_count) {
    MDBTable *table;
    size_t i;
    if (name == NULL || columns == NULL || column_count == 0) return NULL;

    table = (MDBTable *)calloc(1, sizeof(*table));
    if (table == NULL) return NULL;
    table->name = mdb_copy_string(name);
    table->columns = (MDBColumn *)calloc(column_count, sizeof(*table->columns));
    if (table->name == NULL || table->columns == NULL) {
        mdb_table_destroy(table);
        return NULL;
    }

    table->column_count = column_count;
    for (i = 0; i < column_count; ++i) {
        if (columns[i].name == NULL || columns[i].name[0] == '\0') {
            mdb_table_destroy(table);
            return NULL;
        }
        table->columns[i].name = mdb_copy_string(columns[i].name);
        table->columns[i].type = columns[i].type;
        if (table->columns[i].name == NULL) {
            mdb_table_destroy(table);
            return NULL;
        }
    }
    return table;
}

bool mdb_table_insert(MDBTable *table, const MDBValue *values,
                      size_t value_count) {
    MDBValue *copies;
    size_t i;
    if (table == NULL || values == NULL || value_count != table->column_count)
        return false;

    for (i = 0; i < value_count; ++i) {
        if (values[i].type != table->columns[i].type) return false;
        if (values[i].type == MDB_TYPE_TEXT && values[i].as.text == NULL)
            return false;
    }

    copies = (MDBValue *)calloc(value_count, sizeof(*copies));
    if (copies == NULL) return false;

    for (i = 0; i < value_count; ++i) {
        copies[i] = values[i];
        if (values[i].type == MDB_TYPE_TEXT) {
            copies[i].as.text = mdb_copy_string(values[i].as.text);
            if (copies[i].as.text == NULL) {
                size_t j;
                for (j = 0; j < i; ++j) mdb_value_destroy(&copies[j]);
                free(copies);
                return false;
            }
        }
    }

    if (table->row_count == table->row_capacity) {
        size_t new_capacity = table->row_capacity == 0 ? 4 : table->row_capacity * 2;
        MDBRow *new_rows = (MDBRow *)realloc(table->rows,
                                             new_capacity * sizeof(*new_rows));
        if (new_rows == NULL) {
            for (i = 0; i < value_count; ++i) mdb_value_destroy(&copies[i]);
            free(copies);
            return false;
        }
        table->rows = new_rows;
        table->row_capacity = new_capacity;
    }

    table->rows[table->row_count].values = copies;
    table->rows[table->row_count].value_count = value_count;
    table->row_count++;
    return true;
}

void mdb_table_print(const MDBTable *table, FILE *out) {
    size_t r, c;
    if (table == NULL || out == NULL) return;
    fprintf(out, "Table: %s (%zu rows)\n", table->name, table->row_count);
    for (c = 0; c < table->column_count; ++c) {
        fprintf(out, "%s%s", table->columns[c].name,
                c + 1 == table->column_count ? "\n" : " | ");
    }
    for (r = 0; r < table->row_count; ++r) {
        for (c = 0; c < table->rows[r].value_count; ++c) {
            mdb_value_print(&table->rows[r].values[c], out);
            fprintf(out, "%s", c + 1 == table->rows[r].value_count ? "\n" : " | ");
        }
    }
}

void mdb_table_destroy(MDBTable *table) {
    size_t i;
    if (table == NULL) return;
    for (i = 0; i < table->column_count; ++i) free(table->columns[i].name);
    free(table->columns);
    for (i = 0; i < table->row_count; ++i) mdb_row_destroy(&table->rows[i]);
    free(table->rows);
    free(table->name);
    free(table);
}
