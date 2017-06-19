#ifndef PROJECT_H
#define PROJECT_H
#include <map>
#include <string>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>

#include "Filehandler/filehandler.h"
#include "videoproject.h"
#include "video.h"
#include "Filehandler/saveable.h"
#include "Project/Analysis/analysismeta.h"
#include "projectmanager.h"
#include "projecttestsuite.h"
typedef int ID;
class FileHandler;
class ProjectManager;
/**
 * @brief The Project class
 * incomplete class, will be added on
 * along with parser functionality
 */
class Project : public Saveable{
    friend class ProjectTestsuite;

    bool changes_made = true;
    std::map<ID,VideoProject*> videos;
    std::vector<Report*> reports;
    ID video_counter;
public:  
    Q_DECL_DEPRECATED int id;
    const std::string name;
    std::string dir = "";
    std::string dir_bookmarks = "";
    std::string dir_videos = "";
public:
    Project();
    Project(const int& id, const string &name);
    ~Project();

    void add_report(Report* report);
    ID add_video(const Video *vid);
    ID add_video_project(const VideoProject* vid_proj);
    ID add_bookmark(const int& vid_id, const Bookmark *bookmark);
    ID add_analysis(const int& vid_id, const AnalysisMeta& analysis);
    void add_report(const string &file_path);
    void remove_video_project(const int &id);


    // read and write operator for Projects
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    void delete_artifacts();

    bool is_saved() const;
    void save_project() const;
    void load_project();
    std::map<ID, VideoProject *>& get_videos();
    VideoProject* get_video(ID id);
    bool operator==(const Project& other);
// TODO

//    void add_drawing();      
};


#endif // PROJECT_H
