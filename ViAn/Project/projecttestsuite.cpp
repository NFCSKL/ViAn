#include "projecttestsuite.h"

ProjectTestsuite::ProjectTestsuite(QObject *parent)
{


}

void ProjectTestsuite::init_project_test(){
    m_proj = new Project(0, "TEST_PROJ");
}

void ProjectTestsuite::add_proj_test(){
//    m_proj->add_report();
//    m_proj->add_video();
//    m_proj->add_video_project();

}

void ProjectTestsuite::remove_from_proj_test(){

}

void ProjectTestsuite::changes_made_test(){

}

void ProjectTestsuite::read_write_test(){

}

void ProjectTestsuite::save_load_test(){

}

void ProjectTestsuite::delete_files_test(){

}
