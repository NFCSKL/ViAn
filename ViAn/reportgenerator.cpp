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
        //make_doc(word);
        qDebug() << "startat word";
        //2.OPEN THE DOCUMENT        
        QAxObject* doc = word->querySubObject("Documents");
        doc->dynamicCall("Add()");
        word->setProperty("Visible",false); // second bool to hide application when opened.

        //3.GET TO THE CONTENTS
        QAxObject* active_document = word->querySubObject("ActiveDocument");
        QAxObject* active_window = active_document->querySubObject( "ActiveWindow" );
        QAxObject* selection = active_window->querySubObject( "Selection" );

        // Make sure there is bookmarks to put in report.
        //4. ADD IMAGES FROM BOOKMARK FOLDER        
        QAxObject* fig_table = active_document->querySubObject("TablesOfFigures");
        make_doc(fig_table, "fig_table");
        auto img_ref = add_bookmarks(active_document,selection, m_ref_disp);
        QAxObject* tables = add_table(active_document,selection, img_ref.size()+1,2);
        QAxObject* table = tables->querySubObject("Item(int)",1);
        add_pic_ref(table,img_ref);
        //5. SAVE AND CLOSE FILE
        QString file_path = save_report(active_document);
        close_report(doc, word);
    }else{
        qWarning("could not find Word instance");
    }
    qDebug() << "report finished!";
}

QString ReportGenerator::get_bookmark_descr(Bookmark *bm)
{
    QString frame_nr = QString("Frame number: %1").arg(bm->get_frame_number());
    QString time = QString("Time: %1").arg(calculate_time(bm->get_time()));
    QString bm_description = QString::fromStdString(bm->get_description());
    QString description = "";
    if (!bm_description.isEmpty()) {
        description = QString("Description: %1").arg(bm_description);
    }
//    selection->dynamicCall( "InsertParagraphAfter()" );
//    selection->dynamicCall("InsertAfter(const QString&)", time);
//    selection->dynamicCall( "InsertParagraphAfter()" );
//    selection->dynamicCall("InsertAfter(const QString&)", frame_nr);
//    selection->dynamicCall( "InsertParagraphAfter()" );
//    selection->dynamicCall("InsertAfter(const QString&)", description);
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
std::vector<std::pair<QString, QString> > ReportGenerator::add_bookmarks(QAxObject* active_document,QAxObject* selection, RefDisp bookmark_list) {
    std::vector<std::pair<QString,QString>> image_refs;
    int fig_num = 1;
    for (int i = 0; i != bookmark_list.size(); i++) { // for each category, make a paragraph of bookmarks
        qDebug() << "beg loop "<< i;
        std::vector<BookmarkItem*> bm_ref = bookmark_list.at(i).first;
        std::vector<BookmarkItem*> bm_disp = bookmark_list.at(i).second;
        int rows = std::min(bm_ref.size(),bm_disp.size());
        qDebug() << "rows" << rows;
        for(size_t j = 0; j != rows; ++j){ // while images on both sides
            BookmarkItem* bm_left = bm_ref.at(j);
            BookmarkItem* bm_right = bm_disp.at(j);;

            QString ref = get_bookmark_fig_txt(bm_left,fig_num++);
            QString disp = get_bookmark_fig_txt(bm_right,fig_num++);
            image_refs.push_back(std::make_pair(ref,disp));

            add_bookmark_pair(selection, bm_left, bm_right);
        }

        // Add paragraphs between images
        add_paragraph(selection);
    }
    qDebug() << "add_bookmarks_done";
    return image_refs;
}

void ReportGenerator::add_bookmark_pair(QAxObject *selection, BookmarkItem *bm_ref, BookmarkItem* bm_disp)
{
    add_img(selection, bm_ref->get_bookmark(), 2);
    add_img(selection, bm_disp->get_bookmark(), 2);
}

void ReportGenerator::add_img(QAxObject *selection, Bookmark *bm, int alignment)
{
    QAxObject* shapes = selection->querySubObject( "InlineShapes" );
    QString img_path = QString::fromStdString(bm->m_file);
    //Fix to make path work with windows word
    //application when spaces are involved
    img_path.replace("/", "\\\\");
    QAxObject* inline_shape = shapes->querySubObject(
                "AddPicture(const QString&,bool,bool,QVariant)",
                 img_path, false, true);
    resize_picture(img_path, inline_shape);
}

void ReportGenerator::add_entry_to_cell(QAxObject *table, QString entry,int row, int col)
{
    QAxObject* cell_text = table->querySubObject("Cell(int,int)", row, col)->querySubObject("Range");
    cell_text->dynamicCall("SetText(QString Text)", entry);
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
QAxObject* ReportGenerator::add_table(QAxObject *active_document, QAxObject *selection, int rows, int cols)
{
    QAxObject* rng = selection->querySubObject("Range");
    QAxObject* tables = active_document->querySubObject("Tables");
    //make_doc(table);
    QAxObject* table = tables->querySubObject("Add(QVariant,int,int)",rng->asVariant(), rows,cols,1,1);
    QVariant style(36);
    table->dynamicCall("AutoFormat(QVariant)", style);
    return tables;
}

void ReportGenerator::add_pic_ref(QAxObject* table, std::vector<std::pair<QString,QString>> table_contents)
{
    add_entry_to_cell(table, QString::fromStdString("Referens"), 1,1);
    add_entry_to_cell(table, QString::fromStdString("Omstritt"), 1,2);
    int row = 2;
    int col = 1;
    auto str_it = table_contents.begin();
    while(str_it != table_contents.end()){
        add_entry_to_cell(table,str_it->first,row,col++);
        add_entry_to_cell(table,str_it->second,row++,col);
        col = 1;
        str_it++;
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
    std::string proj_name = m_path.substr(0, m_path.find_last_of("/")); // Strip away last "/"
    proj_name = proj_name.substr(proj_name.find_last_of("/"), proj_name.size());
    qDebug() << proj_name.c_str();
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
