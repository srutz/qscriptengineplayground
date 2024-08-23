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
    QScriptEngine *m_scriptEngine;   // scriptengine
    PropertyGrid *m_propertyGrid;    // widget
    QTextEdit *m_editor;             // widget for source code
    QTextEdit *m_output;             // widget for script output

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /* run the script that is currently in the editor */
    void runScript();

    /* insert code that opens a message box */
    void insertMessageBoxCode();

    /* this method is called from the script */
    QScriptValue messageBox(QScriptContext *context, QScriptEngine *engine);

    static MainWindow *activeInstance; // access the mainwindow statically via this method
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
