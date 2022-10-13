#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H

#include <QObject>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

class FilterManager : public QObject
{
    Q_OBJECT
public:
    FilterManager();
    ~FilterManager();
    cv::Mat *getImageWithFilters();                         // Returns a pointer to image with filters
    void createImageWithFilters(cv::Mat *, QList<QPoint>);  // Creates a image with filters and transformation applied

public slots:
    void setItalic(short);
    void setBlur(unsigned short);
    void setErode(unsigned short);
    void setDialate(unsigned short);
    void setFillGaps(unsigned short);
    void setThreshold(unsigned short);

private:
    cv::Mat img;            // Image with applied filters and transform
    cv::Point2f src[4];     // Source quadangle (used for perspective transform)
    cv::Point2f dst[4];     // Destination quadangle (used for perspective transform)
    short italic = 0;
    unsigned short blur = 0;
    unsigned short erode = 0;
    unsigned short dialate = 0;
    unsigned short fillGaps = 0;
    unsigned short threshold = 127;
};

#endif // FILTERMANAGER_H
