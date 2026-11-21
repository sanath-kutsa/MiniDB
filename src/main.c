#include "minidb.h"

int main(void) {
    MDBColumn columns[] = {
        {"id", MDB_TYPE_INTEGER},
        {"name", MDB_TYPE_TEXT},
        {"score", MDB_TYPE_FLOAT},
        {"active", MDB_TYPE_BOOLEAN}
    };
    MDBTable *students = mdb_table_create("students", columns, 4);
    MDBValue ada[] = {
        mdb_value_integer(1), mdb_value_text("Ada Lovelace"),
        mdb_value_float(98.5), mdb_value_boolean(true)
    };
    MDBValue grace[] = {
        mdb_value_integer(2), mdb_value_text("Grace Hopper"),
        mdb_value_float(96.0), mdb_value_boolean(true)
    };
    size_t i;

    if (students == NULL) {
        fprintf(stderr, "MiniDB: failed to create demo table.\n");
        for (i = 0; i < 4; ++i) {
            mdb_value_destroy(&ada[i]);
            mdb_value_destroy(&grace[i]);
        }
        return 1;
    }

    if (!mdb_table_insert(students, ada, 4) ||
        !mdb_table_insert(students, grace, 4)) {
        fprintf(stderr, "MiniDB: failed to insert demo rows.\n");
        mdb_table_destroy(students);
        for (i = 0; i < 4; ++i) {
            mdb_value_destroy(&ada[i]);
            mdb_value_destroy(&grace[i]);
        }
        return 1;
    }

    mdb_table_print(students, stdout);
    mdb_table_destroy(students);
    for (i = 0; i < 4; ++i) {
        mdb_value_destroy(&ada[i]);
        mdb_value_destroy(&grace[i]);
    }
    return 0;
}
