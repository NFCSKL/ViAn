#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

// ViAn files
#include "Project/project.h"
#include "Filehandler/saveable.h"

class Project;
class ProjectManager
{
    std::mutex proj_map_lock;   // lock for handling project write/read
    std::map<ID,Project*> projects;
    ID project_id;              //counter for project ids
    bool proj_equals(Project& proj, Project& proj2);
    friend bool operator==(Project& proj, Project& proj2);
public:
    std::vector<ID> open_projects;
public:
    ProjectManager();
    //  Project methods
    //  Project* open_project(ID id); // To be added
    //  Project* close_project(ID id);
    Project* get_project(ID id);
    Project* create_project(const string &proj_name, const string &dir_path, const std::string& vid_path);
    bool delete_project(ID proj_id);
    Project* load_project(const string &full_project_path);
    void save_project(ID id);
    void open_project(ID id);
    void close_project(ID id);

    //  Video methods
    void remove_video_from_project(ID proj_id, ID vid_id);
    ID add_video(Project* proj, std::string file_path);

private:
    // Project methods
    ID add_project(Project* proj);
    void add_project(ID id, Project *proj);

    Project* load_project(std::string proj_name, std::string dir_path);
    void save_project(Project* proj);
};

#endif // PROJECTMANAGER_H
