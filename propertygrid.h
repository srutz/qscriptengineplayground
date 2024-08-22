#ifndef PROPERTYGRID_H
#define PROPERTYGRID_H

#include <QTableWidget>
#include <QWidget>

/*
 * A simple property grid showing the
 * QProperties of the watchObject
 */
class PropertyGrid : public QWidget
{
    Q_OBJECT

    QTableWidget *m_tableWidget;
    QObject *m_watchedObject = nullptr;

public:
    explicit PropertyGrid(QWidget *parent = nullptr);
    void setWatchedObject(QObject *m_watchedObject);

signals:

public slots:
    void updateGui();
};

#endif // PROPERTYGRID_H
