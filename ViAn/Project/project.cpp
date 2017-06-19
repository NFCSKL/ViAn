#include "project.h"

/**
 * @brief Project::Project
 * @param file_handler
 * @param id
 * @param name
 */
Project* Project::fromFile(const string &full_path)
{
    Project* proj = new Project();
    proj->load_saveable(full_path);
    proj->m_changes = false;
    return proj;
}

/**
 * @brief Project::Project
 * @param name     := name of the project
 * @param dir_path := directory to save project in
 * @param vid_path := path for videos folder
 */
Project::Project(const std::string& name, const std::string& dir_path, const std::string& vid_path){
    m_name = name;
    m_full_path = dir_path + name;
    m_dir_bookmarks = dir_path + "/Boomarks";
    m_dir_videos = vid_path;
}


/**
 * @brief Project::~Project
 * Clears contents of video map
 */
Project::~Project(){
    for (auto vid_it = m_videos.begin(); vid_it != m_videos.end(); ++vid_it) {
        delete vid_it->second;
    }
    for (auto rep_it = m_reports.begin(); rep_it != m_reports.end(); ++rep_it) {
        delete *rep_it;
    }
}

/**
 * @brief Project::add_video
 * @return Video ID to be used for identifying the video.
 */
ID Project::add_video_project(VideoProject *vid_proj){
    vid_proj->id = m_vid_count;
    m_videos.insert(std::make_pair(m_vid_count, vid_proj));
    m_changes = true;
    return m_vid_count++;
}

/**
 * @brief Project::remove_video
 * @param id
 * Remove video from videos and delete its contents.
 */
void Project::remove_video_project(const int& id){
    VideoProject* temp = m_videos.at(id);
    delete temp;
    m_videos.erase(id);
    m_changes = true;
}


/**
 * @brief Project::add_report
 * @param report
 * Required for load, object locally allocated
 */
ID Project::add_report(Report *report){
    m_reports.push_back(report);
    return m_reports.size();
}

/**
 * @brief Project::remove_report
 * @param id
 */
void Project::remove_report(const int &id)
{
    Report* temp = m_reports.at(id);
    delete temp;
    m_videos.erase(id);
    m_changes = true;
}


/**
 * @brief Project::delete_artifacts
 * Delete all projects files.
 */
void Project::delete_artifacts(){
    // Delete files in all videoprojects
    for(auto it = m_videos.begin(); it != m_videos.end(); it++){
        VideoProject* vp = it->second;
        vp->delete_artifacts();
    }
    // Delete all reports.
    for(auto it = m_reports.begin(); it != m_reports.end(); it++){
        Report* temp = *it;
        QFile file (QString::fromStdString(temp->get_file_path()));
        file.remove();
    }
    // Delete directories
    delete_saveable();
    QDir directory;
    QString q_dir = QString::fromStdString(m_dir);
    QString q_dir_bookmarks = QString::fromStdString(m_dir_bookmarks);
    directory.rmdir(q_dir_bookmarks);
    directory.rmdir(q_dir);
}

/**
 * @brief Project::read
 * @param json
 * Read project parameters from json object.
 */
void Project::read(const QJsonObject& json){
    m_name = json["name"].toString().toStdString();
    m_dir = json["root_dir"].toString().toStdString();
    m_dir_bookmarks = json["bookmark_dir"].toString().toStdString();
    m_dir_videos = json["video_dir"].toString().toStdString();
    m_full_path = m_dir +"/"+ m_name;
    // Read videos from json
    QJsonArray json_vid_projs = json["videos"].toArray();
    for (int i = 0; i < json_vid_projs.size(); ++i) {
        QJsonObject json_vid_proj = json_vid_projs[i].toObject();
        VideoProject* v = new VideoProject();
        v->read(json_vid_proj);
        add_video_project(v);
    }    
    // Read reports from json
    QJsonArray json_reports = json["reports"].toArray();
    for (int i = 0; i < json_reports.size(); ++i) {
        QJsonObject json_report = json_reports[i].toObject();
        Report* report = new Report();
        report->read(json_report);
        add_report(report);
    }
}

/**
 * @brief Project::write
 * @param json
 * Write project parameters to json object.
 */
void Project::write(QJsonObject& json){
    json["name"] = QString::fromStdString(m_name);
    json["root_dir"] =  QString::fromStdString(m_dir);
    json["bookmark_dir"] = QString::fromStdString(m_dir_bookmarks);
    json["video_dir"] = QString::fromStdString(m_dir_videos);
    json["full_path"] = QString::fromStdString(m_full_path);
    QJsonArray json_proj;
    // Write Videos to json
    for(auto it = m_videos.begin(); it != m_videos.end(); it++){
        QJsonObject json_vid_proj;
        VideoProject* v = it->second;
        v->write(json_vid_proj);
        json_proj.append(json_vid_proj);
    }
    json["videos"] = json_proj;
    // Write reports to json
    QJsonArray json_reports;
    for(auto it = m_reports.begin(); it != m_reports.end(); it++){
        QJsonObject json_report;
        Report* report = *it;
        report->write(json_report);
        json_reports.append(json_report);
    }
    json["reports"] = json_reports;
}

/**
 * @brief Project::add_analysis
 * @param v_id id of video to add analysis to
 * @param analysis
 */
ID Project::add_analysis(const int &vid_id, AnalysisMeta &analysis){
    m_changes = true;
    return m_videos.at(vid_id)->add_analysis(analysis);
}

/**
 * @brief Project::add_bookmark
 * @param v_id the id of the video
 * @param bookmark
 * Add new bookmark to Videoproj corresponding to id.
 */
ID Project::add_bookmark(const int &vid_id, Bookmark *bookmark){
    VideoProject* v = m_videos.at(vid_id);
    m_changes = true;
    return v->add_bookmark(bookmark);
}

/**
 * @brief Project::is_saved
 * @return true if saved
 */
bool Project::is_saved() const{
    return !m_changes;
}

/**
 * @brief Project::save_project
 * @return sets saved =true
 */
bool Project::save_project(){
    m_changes = false;
    QDir directory;
    directory.mkpath(QString::fromStdString(m_dir));
    directory.mkpath(QString::fromStdString(m_dir_bookmarks));
    return save_saveable(m_name, m_dir);
}

/**
 * @brief Project::get_videos
 * @return videos&
 */
std::map<ID, VideoProject*> &Project::get_videos(){
    return m_videos;
}

/**
 * @brief Project::get_video
 * @param id
 * @return Returns the video with the specified id.
 */
VideoProject* Project::get_video(ID id) {
    return m_videos[id];
}

/**
 * @brief Project::Project
 */
std::string Project::getDir_bookmarks() const
{
    return m_dir_bookmarks;
}

std::string Project::getDir_videos() const
{
    return m_dir_videos;
}

std::string Project::getDir() const
{
    return m_dir;
}

std::string Project::getName() const
{
    return m_name;
}
