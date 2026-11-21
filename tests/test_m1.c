#include "minidb.h"

#include <assert.h>
#include <stdio.h>

static void test_table_creation_and_insert(void) {
    MDBColumn columns[] = {
        {"id", MDB_TYPE_INTEGER},
        {"name", MDB_TYPE_TEXT}
    };
    MDBValue values[] = {
        mdb_value_integer(7),
        mdb_value_text("Test row")
    };
    MDBTable *table = mdb_table_create("records", columns, 2);

    assert(table != NULL);
    assert(table->row_count == 0);
    assert(mdb_table_insert(table, values, 2));
    assert(table->row_count == 1);
    assert(table->rows[0].values[0].as.integer == 7);
    assert(table->rows[0].values[1].type == MDB_TYPE_TEXT);
    assert(table->rows[0].values[1].as.text != values[1].as.text);
    assert(!mdb_table_insert(table, values, 1));

    mdb_table_destroy(table);
    mdb_value_destroy(&values[1]);
}

static void test_type_validation(void) {
    MDBColumn columns[] = {{"count", MDB_TYPE_INTEGER}};
    MDBValue wrong[] = {mdb_value_text("not an integer")};
    MDBTable *table = mdb_table_create("typed", columns, 1);

    assert(table != NULL);
    assert(!mdb_table_insert(table, wrong, 1));
    assert(table->row_count == 0);

    mdb_table_destroy(table);
    mdb_value_destroy(&wrong[0]);
}

static void test_invalid_schema(void) {
    MDBColumn columns[] = {{NULL, MDB_TYPE_INTEGER}};
    assert(mdb_table_create("invalid", columns, 1) == NULL);
    assert(mdb_table_create("empty", columns, 0) == NULL);
}

int main(void) {
    test_table_creation_and_insert();
    test_type_validation();
    test_invalid_schema();
    puts("M1 tests passed.");
    return 0;
}
