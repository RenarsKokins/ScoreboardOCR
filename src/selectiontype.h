#ifndef SELECTIONTYPE_H
#define SELECTIONTYPE_H

#include <vector>
#include <QString>
#include <QStringList>

class SelectionType
{
public:
    enum Type
    {
        time,                   // Time (mm:ss)
        timeMilliseconds,       // Time (mm:ss.ms)
        period,                 // Period (1st, 2nd, 3rd, 4th, OT)
        longPeriod,             // Custom period (1st, 2nd, ..., 9th)
        seconds,                // 24 second clock (ss.ms)
        secondsMilliseconds,    // 24, 30 or 45 second clock (ss)
        anyNumber,              // Outputs raw number without postprocessing
    };

    SelectionType();
    Type getType();
    void setType(Type);
    void setType(short);
    QString getDescription();
    QStringList getTypeDescriptions();

private:
    Type type;
    QStringList descriptions = {
        "Time (mm:ss)",
        "Time (mm:ss.ms)",
        "Period (1st, 2nd, 3rd, 4th, OT)",
        "Custom period (1st, 2nd, ..., 9th)",
        "24 second clock with milliseconds (ss.ms)",
        "24, 30 or 45 second clock (ss)",
        "Unformatted number",
    };
};

#endif // SELECTIONTYPE_H
