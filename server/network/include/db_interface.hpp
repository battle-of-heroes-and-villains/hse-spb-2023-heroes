#ifndef DB_INTERFACE_HPP_
#define DB_INTERFACE_HPP_

#include <sqlite3.h>
#include <string>

class user {
public:
    grpc::string password;
    int rating{0};
};

static user callback_data;

class db {
    sqlite3 *db{};
    char *zErrMsg = nullptr;
    int rc{};

public:
    static int callback(void *data, int argc, char **argv, char **azColName) {
        int i;
        fprintf(stderr, "%s: ", (const char *)data);

        for (i = 0; i < argc; i++) {
            printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        }
        printf("\n");
        return 0;
    }

    static int
    callback_int(void *count, int argc, char **argv, char **azColName) {
        int *c = static_cast<int *>(count);
        *c = atoi(argv[0]);
        return 0;
    }

    static int
    callback_args(void *data, int argc, char **argv, char **azColName) {
        callback_data.password = argv[1];
        callback_data.rating = std::stoi(std::string{*argv[2]});
        return 0;
    }

    void open() {
        rc = sqlite3_open("DatabaseName.db", &db);

        if (rc) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        } else {
            fprintf(stderr, "Opened database successfully\n");
        }
    }

    void create() {
        char *command =
            "CREATE TABLE IF NOT EXISTS users (\n"
            "\tname TEXT PRIMARY KEY UNIQUE,\n"
            "\tpassword TEXT NOT NULL,\n"
            "\trating INTEGER DEFAULT 0\n"
            ")";
        rc = sqlite3_exec(db, command, callback, nullptr, &zErrMsg);

        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            fprintf(stdout, "Table created successfully\n");
        }
    }

    void get_user(const std::string &name) {
        std::string sql = "SELECT * FROM users WHERE name = '" + name + "';";
        zErrMsg = nullptr;
        rc = sqlite3_exec(db, sql.c_str(), callback_args, nullptr, &zErrMsg);

        if (rc != SQLITE_OK) {
            fprintf(stderr, "Can't open database: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
    }

    void insert(const char *name, const char *password, int rating) {
        char *command = new char[100];
        sprintf(
            command,
            "INSERT INTO users (name, password, rating)\n"
            "VALUES\n"
            "\t(\"%s\", \"%s\", %d);",
            name, password, rating
        );
        rc = sqlite3_exec(db, command, callback, 0, &zErrMsg);
        delete[] command;
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            fprintf(stdout, "Records created successfully\n");
        }
    }

    void delete_user(const char *name) {
        char *command = new char[100];
        sprintf(command, "DELETE FROM users where name=\"%s\"", name);
        rc = sqlite3_exec(db, command, callback, nullptr, &zErrMsg);
        delete[] command;
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            fprintf(stdout, "Records deleted successfully\n");
        }
    }

    void update_rate(const char *name, int rate) {
        char *command = new char[100];
        sprintf(
            command, "UPDATE users set rating = %d where name=\"%s\"", rate,
            name
        );
        const char *data = "Callback function called";
        rc = sqlite3_exec(db, command, callback, (void *)data, &zErrMsg);

        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            fprintf(stdout, "Operation done successfully\n");
        }
    }

    int size() {
        char *command = "select count(*) from users";
        int count = 0;
        rc = sqlite3_exec(db, command, callback_int, &count, &zErrMsg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            printf("count: %d\n", count);
        }
        return count;
    }

    bool has_table(){
        const char* name = "users";
        int rc = sqlite3_open(name, &db);
        return rc == SQLITE_OK;
    }

    void close() {
        sqlite3_close(db);
        fprintf(stdout, "closed database\n");
    }
};

#endif  // DB_INTERFACE_HPP_
