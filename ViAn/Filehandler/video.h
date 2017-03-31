#ifndef VIDEO_H
#define VIDEO_H
#include <iostream>
#include <fstream>
#include <sstream>
typedef int ID;
class Video
{
public:
    Video();
    Video(std::string file_path);
    std::string file_path;
    friend std::stringstream& operator>>(std::stringstream& is, Video& vid);
    friend std::stringstream& operator<<(std::stringstream& os, const Video& vid);
    friend bool operator==(Video v1, Video v2);
};

#endif // VIDEO_H
