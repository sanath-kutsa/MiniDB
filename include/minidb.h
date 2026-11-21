#ifndef MINIDB_H
#define MINIDB_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef enum {
    MDB_TYPE_INTEGER,
    MDB_TYPE_FLOAT,
    MDB_TYPE_TEXT,
    MDB_TYPE_BOOLEAN
} MDBType;

typedef struct {
    MDBType type;
    union {
        long long integer;
        double floating;
        char *text;
        bool boolean;
    } as;
} MDBValue;

typedef struct {
    char *name;
    MDBType type;
} MDBColumn;

typedef struct {
    MDBValue *values;
    size_t value_count;
} MDBRow;

typedef struct {
    char *name;
    MDBColumn *columns;
    size_t column_count;
    MDBRow *rows;
    size_t row_count;
    size_t row_capacity;
} MDBTable;

MDBValue mdb_value_integer(long long value);
MDBValue mdb_value_float(double value);
MDBValue mdb_value_text(const char *value);
MDBValue mdb_value_boolean(bool value);
void mdb_value_destroy(MDBValue *value);
void mdb_value_print(const MDBValue *value, FILE *out);

MDBTable *mdb_table_create(const char *name, const MDBColumn *columns,
                           size_t column_count);
bool mdb_table_insert(MDBTable *table, const MDBValue *values,
                      size_t value_count);
void mdb_table_print(const MDBTable *table, FILE *out);
void mdb_table_destroy(MDBTable *table);
const char *mdb_type_name(MDBType type);

#endif
