#include "reportgenerator.h"

/**
 * @brief ReportGenerator::ReportGenerator
 * The constructor will assign parameters to the class members.
 * @param proj, the current project that we are creating document for.
 * @param file_handler, the file_handler that is used to get path information for saving.
 */
ReportGenerator::ReportGenerator(Project* proj) {
    this->proj = proj;
}

/**
 * @brief ReportGenerator::~ReportGenerator
 * destructor to remove allocated memory.
 */
ReportGenerator::~ReportGenerator() {
    delete word;
}

/**
 * @brief ReportGenerator::create_report
 * This method will do 5 steps to create a word document, load it with bookmarks and save it.
 */
void ReportGenerator::create_report() {
    //1. START APPLICATION
    word = new QAxObject("Word.Application");
    if(!word->isNull()){
        //2.OPEN THE DOCUMENT
        QAxObject* doc = word->querySubObject("Documents");
        doc->dynamicCall("Add()");
        word->setProperty("Visible",false); // second bool to hide application when opened.

        //3.GET TO THE CONTENTS
        QAxObject* active_document = word->querySubObject("ActiveDocument");
        QAxObject* active_window = active_document->querySubObject( "ActiveWindow" );
        QAxObject* selection = active_window->querySubObject( "Selection" );

        // Save all for project bookmarks in a list.
        create_list_of_names();

        // Make sure there is bookmarks to put in report.
        if(!all_bookmarks.empty()){
            //4. ADD IMAGES FROM BOOKMARK FOLDER
            add_bookmarks(selection);

            //5. SAVE AND CLOSE FILE
            QString file_path = save_report(active_document);
            this->proj->add_report(new Report(file_path.toStdString()));
        }
        close_report(doc, word);
    }else{
        qWarning("could not find Word instance");
    }
}

/**
 * @brief ReportGenerator::create_list_of_names
 * This method will take all bookmarks for all videos in a
 * project and put their path and description in a list.
 * Change in this method if reports should only be based on the current
 * video that is selected and not the entire project. Changes will be needed
 * on other places aswell.
 */
void ReportGenerator::create_list_of_names() {
    std::vector<VideoProject*> videos = proj->get_videos();

    // get all bookmarks for a project by iterating over each videos bookmarks.
    for(auto it = videos.begin(); it != videos.end(); it++) {
        std::map<ID, Bookmark *> bookmark_list = (*it)->get_bookmarks();
        for(std::pair<ID,Bookmark*> vid_bm_pair : bookmark_list) {
            Bookmark* video_bookmark = vid_bm_pair.second;
            all_bookmarks.push_back(video_bookmark);
        }
    }
}

/**
 * @brief ReportGenerator::resize_picture
 * This method will make all images to be of the same size with a width
 * that is based on the constant IMAGE_WIDTH_REFERENCE. All images will keep
 * its aspect ratio.
 * @param pic_path, path to the bookmark that is to be resized.
 * @param inline_shape, A word specific object that is a shape where its
 * layout is "inline" with the rest of the document.
 */
void ReportGenerator::resize_picture(QString pic_path, QAxObject* inline_shape) {
    QImage image = QImage(pic_path);
    int const original_height = image.height();
    int const original_width = image.width();

    //Scale all images to have same width but keep aspect ratio
    double multiply_factor = IMAGE_WIDTH_REFERENCE/original_width;
    inline_shape->dynamicCall( "Height", (multiply_factor*original_height));
    inline_shape->dynamicCall( "Width", IMAGE_WIDTH_REFERENCE);
}

/**
 * @brief ReportGenerator::add_paragraph
 * This adds paragraphs (spaces) between the bookmarks in the
 * document to make it more readable. To increase or decrease the number
 * of paragraphs change the number of times the loop is executed.
 * @param selection, the selector in the active document.
 */
void ReportGenerator::add_paragraph(QAxObject* selection) {
    selection->dynamicCall( "Collapse(int)", 0 );
    for (int i = 0; i < 2; ++i) {
         selection->dynamicCall( "InsertParagraphAfter()" );
    }
    selection->dynamicCall( "Collapse(int)", 0 );
}
/**
 * @brief ReportGenerator::calculate_time
 * This method will convert milliseconds into a QString with format
 * "Hours:Minutes:Seconds"
 * @param ms, the time in milliseconds that are to be converted
 * @return a QString with format as specified above in @brief
 */
QString ReportGenerator::calculate_time(int ms) {
    int seconds = (int) (ms / 1000) % 60 ;
    int minutes = (int) ((ms / (1000*60)) % 60);
    int hours   = (int) ((ms / (1000*60*60)) % 24);
    return QString("%1:%2:%3").arg(hours, 2, 10, QChar('0'))\
            .arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
}

/**
 * @brief ReportGenerator::add_bookmarks
 * This method will add all bookmarks for the current project
 * to the document.
 * @param selection, the selector in the active document.
 */
void ReportGenerator::add_bookmarks(QAxObject* selection) {
    QAxObject* shapes = selection->querySubObject( "InlineShapes" );
    for (Bookmark* bookmark : all_bookmarks) {
        QString pic_path = QString::fromStdString(bookmark);
        //Fix to make path work with windows word
        //application when spaces are involved
        pic_path.replace("/", "\\\\");

        QAxObject* inline_shape = shapes->querySubObject(
                    "AddPicture(const QString&,bool,bool,QVariant)",
                     pic_path, false, true);

        resize_picture(pic_path, inline_shape);

        // Center image
        selection->querySubObject( "ParagraphFormat" )->setProperty( "Alignment", 1 );

        //adds description beneath image
        QString frame_nr = QString("Frame number: %1").arg(bookmark->get_frame_number());
        QString time = QString("Time: %1").arg(calculate_time(bookmark->get_time()));
        QString bm_description = QString::fromStdString(bookmark->get_description());
        QString description = "";

        if (!bm_description.isEmpty()) {
            description = QString("Description: %1").arg(bm_description);
        }
        selection->dynamicCall( "InsertParagraphAfter()" );
        selection->dynamicCall("InsertAfter(const QString&)", time);
        selection->dynamicCall( "InsertParagraphAfter()" );
        selection->dynamicCall("InsertAfter(const QString&)", frame_nr);
        selection->dynamicCall( "InsertParagraphAfter()" );
        selection->dynamicCall("InsertAfter(const QString&)", description);

        // Add paragraphs between images
        add_paragraph(selection);
    }
}

/**
 * @brief ReportGenerator::date_time_generator
 * calculates the current time and date and changes the format
 * to use "_" instead of ":" since it is not allowed in word application
 * to save file names that have a colon in the name.
 * @return string, the current time and date.
 */
std::string ReportGenerator::date_time_generator() {
    time_t now = time(0);
    std::string dt = ctime(&now);
    std::replace( dt.begin(), dt.end(), ':', '-');
    std::replace( dt.begin(), dt.end(), ' ' , '_');
    dt.erase(std::remove(dt.begin(), dt.end(), '\n'), dt.end());
    return dt;
}

/**
 * @brief ReportGenerator::save_report
 * This method will save the word document in the project folder.
 * The name will be based on the project and current date and time.
 * @param active_document, the document that is selected
 */
QString ReportGenerator::save_report(QAxObject* active_document) {
    std::string dt = date_time_generator();
    std::string proj_path = proj->getDir();
    std::string path = proj_path.append("/").append(proj->getName()).append("_").append(dt).append(".docx");
    active_document->dynamicCall("SaveAs (const QString&)", QString::fromStdString(path));
    return QString::fromStdString(path);
}

/**
 * @brief ReportGenerator::close_report
 * This method will close the document and quit the the word application that are sent in
 * as a parameter.
 * @param doc, document instance
 * @param word, word application
 */
void ReportGenerator::close_report(QAxObject* doc, QAxObject*  word) {
    doc->dynamicCall("Close()");
    word->dynamicCall("Quit()");
}
