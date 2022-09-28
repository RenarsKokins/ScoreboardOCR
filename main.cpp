#include "scoreboardocr.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScoreboardOCR w;
    w.show();
    return a.exec();
}
