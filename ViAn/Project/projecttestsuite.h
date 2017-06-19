#ifndef PROJECTTESTSUITE_H
#define PROJECTTESTSUITE_H
#include <QJsonObject>
#include <QObject>
#include <QTest>
#include "project.h"

class ProjectTestsuite
{
    Project* m_proj;
public:
    explicit ProjectTestsuite(QObject *parent = 0);
private slots:
    void init_project_test();

    void add_proj_test();
    void remove_from_proj_test();

    void changes_made_test();
    void read_write_test();
    void save_load_test();
    void delete_files_test();



};

#endif // PROJECTTESTSUITE_H
