#include "reportgenerator.h"

/**
 * @brief ReportGenerator::ReportGenerator
 * The constructor will assign parameters to the class members.
 * @param proj, the current project that we are creating document for.
 * @param file_handler, the file_handler that is used to get path information for saving.
 */
ReportGenerator::ReportGenerator(std::string proj_path, RefDisp ref_disp) {
    m_path = proj_path;
    m_ref_disp = ref_disp;
    word = new QAxObject("Word.Application");
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
    qDebug() << "startat rapport";
    if(!word->isNull()){
        qDebug() << "startat word";
        //2.OPEN THE DOCUMENT;
        QAxObject* doc = word->querySubObject("Documents");
        doc->dynamicCall("Add()");
        QAxObject* active_doc = word->querySubObject("ActiveDocument");
//        QAxObject* range = para->querySubObject("Range");

        //3.GET TO THE CONTENTS
        //QAxObject* active_document = word->querySubObject("ActiveDocument");
//        QAxObject* active_window = active_document->querySubObject( "ActiveWindow" );
//        QAxObject* selection = active_window->querySubObject( "Selection" );

        // Make sure there is bookmarks to put in report.
        //4. ADD IMAGES FROM BOOKMARK FOLDER
        create_bookmark_table(active_doc, m_ref_disp);
        //5. SAVE AND CLOSE FILE
        QString file_path = save_report(active_doc);
        close_report(doc, word);
    }else{
        qWarning("could not find Word instance");
    }
    qDebug() << "report finished!";
}

QString ReportGenerator::get_bookmark_descr(BookmarkItem *bm)
{
    QString frame_nr = QString("Frame number: %1").arg(bm->get_frame_number());
    QString time = QString("Time: %1").arg(calculate_time(bm->get_bookmark()->get_time()));
    QString bm_description = QString::fromStdString(bm->get_bookmark()->get_description());
    QString description = "";
    if (!bm_description.isEmpty()) {
        description = QString("Description: %1").arg(bm_description);
    }
    return description;
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
void ReportGenerator::create_bookmark_table(QAxObject* para, RefDisp bookmark_list) {
    QAxObject* range = para->querySubObject("Range(int,int)",0,0);
    QAxObject* table = add_table(range,bookmark_list.size()*2+1,2,BORDER);
//    range->dynamicCall("MoveEnd(int)",-2);
//    range->dynamicCall("Collapse(int)",1);
    cell_add_text(table, QString::fromStdString("Referens"), 1,1);
    cell_add_text(table, QString::fromStdString("Omstritt"), 1,2);    
    int cell_row = 2;
    for (int i = 0; i != bookmark_list.size(); i++) { // for each category, make a paragraph of bookmarks        
        std::vector<BookmarkItem*> bm_ref = bookmark_list.at(i).first;
        std::vector<BookmarkItem*> bm_disp = bookmark_list.at(i).second;

        cell_add_text(table, QString::fromStdString("Category"), cell_row,1);
        cell_add_text(table, QString::fromStdString("Category"), cell_row,2);
        cell_row++;
        QAxObject* cell_ref = table->querySubObject("Cell(int,int)", cell_row, 1);
        QAxObject* cell_disp = table->querySubObject("Cell(int,int)", cell_row, 2);

        cell_insert_category(cell_ref, bm_ref);
        cell_insert_category(cell_disp, bm_disp);

        cell_row++;
        qDebug() << cell_row;
    }
    qDebug() << "add_bookmarks_done";
}

void ReportGenerator::cell_insert_category(QAxObject* cell, std::vector<BookmarkItem *> bm_list)
{
    QAxObject* cell_range = cell->querySubObject("Range");
    int cell_row = 1;
    int max_row = bm_list.size();
    //QAxObject* range = cell->querySubObject("Range");
    QAxObject* table = add_table(cell_range, max_row, 1);
    qDebug() << "tabl inserted";
    for(size_t j = 0; j != bm_list.size(); ++j){ // while images on both sides
        BookmarkItem* bm = bm_list.at(j);
        cell_add_img(table, bm->get_file_path(), cell_row, 1);
        cell_add_text(table, bm->getDescription(),cell_row,1);
        cell_row++;
    }
    qDebug() << "exit insert cat";
}

void ReportGenerator::cell_add_img(QAxObject *table, QString file_name, int row, int col)
{
    QAxObject* cell = table->querySubObject("Cell(int,int)",row,col);
    //QAxObject* shapes = selection->querySubObject( "InlineShapes" );
    QAxObject* range = cell->querySubObject("Range");
    QAxObject* shapes = range->querySubObject("InlineShapes");
    //Fix to make path work with windows word
    //application when spaces are involved
    file_name.replace("/", "\\\\");
    QAxObject* inline_shape = shapes->querySubObject(
                "AddPicture(const QString&,bool,bool,QVariant)",
                 file_name, false, true, range->asVariant());
    resize_picture(file_name, inline_shape);
}

void ReportGenerator::cell_add_text(QAxObject* table,QString entry,int row, int col)
{
    QAxObject* range = table->querySubObject("Cell(int,int)", row, col)->querySubObject("Range");
    range->dynamicCall("InsertAfter(QString Text)", entry);
}

void ReportGenerator::make_doc(QAxObject *obj, QString file_name)
{
    QDir dir;
    dir.mkpath("C:/Documents/WORD_COM/");
    QFile file1("C:/Documents/WORD_COM/"+ file_name + ".html");
       file1.open(QIODevice::WriteOnly | QIODevice::Text);
       QTextStream out(&file1);
       out << obj->generateDocumentation();
       file1.close();
}

QString ReportGenerator::get_bookmark_fig_txt(BookmarkItem *bm, int fig_num)
{
    return QString::fromStdString("Figur" + std::to_string(fig_num) + ": ") + bm->getDescription();
}

QAxObject* ReportGenerator::add_table(QAxObject *range, int rows, int cols, TABLE_STYLE style)
{
    range->dynamicCall("Collapse(int)",1); // Don't touch this, magically works
    QAxObject* tables = range->querySubObject("Tables");
    make_doc(tables, "tables");
    QAxObject* table = tables->querySubObject("Add(QVariant,int,int)",range->asVariant(), rows,cols,1,1);
    table->dynamicCall("AutoFormat(QVariant)", QVariant(style));
    table->dynamicCall("SetTitle(QString)", "Title");
    return table;
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
    std::string proj_name = m_path.substr(0, m_path.find_last_of("/")); // Strip away last "/"
    proj_name = proj_name.substr(proj_name.find_last_of("/"), proj_name.size());
    std::string path = m_path.append(proj_name).append("_").append(dt).append(".docx");
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
