#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "propertygrid.h"

#include <QMainWindow>
#include <QScriptEngine>
#include <QTextEdit>

/*
 * The main Window class, all gui setup takes place here
 * in the constructor of MainWindow.
 *
 * The MainWindow contains the QScriptEngine and also
 * the pointers to the main widgets.
 */

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QScriptEngine *m_scriptEngine;
    PropertyGrid *m_propertyGrid;
    QTextEdit *m_editor;
    QTextEdit *m_output;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void runScript();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
