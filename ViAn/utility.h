#ifndef UTILITY_H
#define UTILITY_H

#include <QSize>

namespace Utility{
     std::pair<double, double> size_ratio(QSize s1, QSize s2);
     double min_size_ratio(QSize s1, QSize s2);
     int number_of_digits(int n);
     std::string zfill(std::string number, int length);
     std::string name_from_path(const std::string full_path);
}

#endif // UTILITY_H

