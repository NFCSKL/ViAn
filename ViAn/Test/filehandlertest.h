#ifndef FILEHANDLERTEST_H
#define FILEHANDLERTEST_H
#include "Filehandler/filehandler.h"
#include "Filehandler/project.h"
#include <QTest>
#include <QObject>


class filehandlertest : public QObject
{
    Q_OBJECT

public:
    explicit filehandlertest(QObject *parent = 0);
private:
    FileHandler* file_handler;
    ID dir_id;
    ID file_id;
    Project* proj;
private slots:
    // Directory test
    void directory_create_test();
    void directory_delete_test();
    // Workspace test
    void workspace_set_test();
    // File tests
    void file_test();

    // File create and deletion tests
    void file_test_init(); // Create directory
    void file_create_test(); // Create file
    void file_delete_test(); // Delete file

    void file_create_delete_multiple(); // Create a number of files, check ids and then delete them
    // File read and write tests
    void file_read_write_init();
    void file_read_write_test();
    void file_read_write_option_test();
    void file_read_lines_test();

    // Project tests
    void project_handling_test();

    void project_init_tests();
    void project_create_test();
    void project_delete_test();
    void project_load_save_test();

};

#endif // FILEHANDLER_TESTSUITE_H
