#include <QDebug>
#include <QPoint>
#include <opencv2/imgproc.hpp>
#include "filtermanager.h"

FilterManager::FilterManager()
{
    qDebug() << "Initializing filter manager...";
}

FilterManager::~FilterManager()
{
    qDebug() << "Destroying filter manager...";
}

cv::Mat *FilterManager::getImageWithFilters()
{
    return &img;
}

void FilterManager::createImageWithFilters(cv::Mat *image, QList<QPoint> points)
{
    if(image->empty())
    {
        qWarning() << "BLANK frame in filter manager, cannot apply filters";
        return;
    }

    for (int i = 0; i < 4; i++)
    {
        src[i] = cv::Point2f(points[i].x(), points[i].y());
    }

    // Italic skew is based proportionally based on image width
    int italicAdjusted = (italic * image->cols) / 100;

    // italic transforamtion is made here
    dst[0] = cv::Point2f(0 + italicAdjusted, 0);
    dst[1] = cv::Point2f(image->cols + italicAdjusted, 0);
    dst[2] = cv::Point2f(0 - italicAdjusted, image->rows);
    dst[3] = cv::Point2f(image->cols - italicAdjusted, image->rows);

    cv::Mat M = cv::getPerspectiveTransform(src, dst);
    cv::Size dsize = cv::Size(image->cols, image->rows);
    cv::warpPerspective(*image, img, M, dsize);  // Warp image

    if(img.channels() > 1)
        cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);  // Convert image to grayscale

    if(blur != 0)
        cv::blur(img, img, cv::Size(blur, blur));   // Blur image
    cv::threshold(img, img, threshold, 255, 0);     // Apply threshold

    // Invert image colors if specified
    if(invert)
        cv::bitwise_not(img, img);

    // Apply fill vetical gaps (it is just dialate and erode together with vertical kernel)
    if(fillGaps != 0)
    {
        cv::Mat element = cv::getStructuringElement( cv::MORPH_ELLIPSE,
                             cv::Size(1, 2 * fillGaps + 1 ));
        cv::dilate(img, img, element, cv::Point(-1, -1));
        cv::erode(img, img, element, cv::Point(-1, -1));
    }

    // Apply dialation, if necessary
    if(dialate != 0)
    {
        cv::Mat element = cv::getStructuringElement( cv::MORPH_ELLIPSE,
                             cv::Size( 2 * dialate + 1, 2 * dialate + 1 ),
                             cv::Point( dialate, dialate ));
        cv::dilate(img, img, element);
    }

    // Apply erode, if necessary
    if(erode != 0)
    {
        cv::Mat element = cv::getStructuringElement( cv::MORPH_ELLIPSE,
                             cv::Size( 2 * erode + 1, 2 * erode + 1 ),
                             cv::Point( erode, erode ));
        cv::erode(img, img, element);
    }
}

void FilterManager::setInvert(bool i)
{
    invert = i;
}

void FilterManager::setItalic(short i)
{
    italic = i;
}

void FilterManager::setBlur(unsigned short i)
{
    blur = i;
}

void FilterManager::setErode(unsigned short i)
{
    erode = i;
}

void FilterManager::setDialate(unsigned short i)
{
    dialate = i;
}

void FilterManager::setFillGaps(unsigned short i)
{
    fillGaps = i;
}

void FilterManager::setThreshold(unsigned short i)
{
    threshold = i;
}
