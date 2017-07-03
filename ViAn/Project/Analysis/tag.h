#ifndef TAG_H
#define TAG_H
#include "Filehandler/saveable.h"
#include <set>

class Tag : public Saveable {


public:
    Tag(QString name);
    ~Tag();
    std::set<int> frames;
    std::string m_name;
    void add_frame(int frame);

    void read(const QJsonObject& json);
    void write(QJsonObject& json);
};

#endif // TAG_H
