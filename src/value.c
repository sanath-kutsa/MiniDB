#include "minidb.h"

#include <stdlib.h>
#include <string.h>

MDBValue mdb_value_integer(long long value) {
    MDBValue result;
    result.type = MDB_TYPE_INTEGER;
    result.as.integer = value;
    return result;
}

MDBValue mdb_value_float(double value) {
    MDBValue result;
    result.type = MDB_TYPE_FLOAT;
    result.as.floating = value;
    return result;
}

MDBValue mdb_value_text(const char *value) {
    MDBValue result;
    result.type = MDB_TYPE_TEXT;
    result.as.text = NULL;
    if (value != NULL) {
        size_t length = strlen(value) + 1;
        result.as.text = (char *)malloc(length);
        if (result.as.text != NULL) {
            memcpy(result.as.text, value, length);
        }
    }
    return result;
}

MDBValue mdb_value_boolean(bool value) {
    MDBValue result;
    result.type = MDB_TYPE_BOOLEAN;
    result.as.boolean = value;
    return result;
}

void mdb_value_destroy(MDBValue *value) {
    if (value == NULL) return;
    if (value->type == MDB_TYPE_TEXT) {
        free(value->as.text);
        value->as.text = NULL;
    }
}

void mdb_value_print(const MDBValue *value, FILE *out) {
    if (value == NULL || out == NULL) return;
    switch (value->type) {
        case MDB_TYPE_INTEGER: fprintf(out, "%lld", value->as.integer); break;
        case MDB_TYPE_FLOAT: fprintf(out, "%.6g", value->as.floating); break;
        case MDB_TYPE_TEXT: fprintf(out, "%s", value->as.text ? value->as.text : "(null)"); break;
        case MDB_TYPE_BOOLEAN: fprintf(out, "%s", value->as.boolean ? "TRUE" : "FALSE"); break;
        default: fprintf(out, "(invalid)"); break;
    }
}

const char *mdb_type_name(MDBType type) {
    switch (type) {
        case MDB_TYPE_INTEGER: return "INTEGER";
        case MDB_TYPE_FLOAT: return "FLOAT";
        case MDB_TYPE_TEXT: return "TEXT";
        case MDB_TYPE_BOOLEAN: return "BOOLEAN";
        default: return "UNKNOWN";
    }
}
