#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "nativecontext.h"
#include <QDebug>
#include <QToolBar>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Setup the gui
     * we have 2 areas for scripting and output
     * and a simple grid showing the properties
     * of the NativeContext object
     */

    connect(ui->actionExit, &QAction::triggered, this, [=] {
        this->close();
    });

    auto context = new NativeContext(this);

    m_editor = new QTextEdit(this);
    m_editor->setPlaceholderText("Enter script here");
    m_editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_editor->setText(
        "context.counter = context.counter + 1;\n"
        "context.flag = !context.flag;\n"
        "context.title = new Date().toString();\n");

    m_output = new QTextEdit(this);
    m_output->setPlaceholderText("Script Results");
    m_output->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_propertyGrid = new PropertyGrid(this);
    m_propertyGrid->setWatchedObject(context);
    m_propertyGrid->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    m_scriptEngine = new QScriptEngine(this);
    auto jsObject = m_scriptEngine->newQObject(context);
    m_scriptEngine->globalObject().setProperty("context", jsObject);

    auto contentLayout = new QVBoxLayout(ui->content);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    ui->content->setLayout(contentLayout);

    auto splitter2 = new QSplitter(Qt::Vertical, ui->content);
    splitter2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    splitter2->addWidget(m_editor);
    splitter2->addWidget(m_output);
    splitter2->setStretchFactor(0, 3);
    splitter2->setStretchFactor(1, 2);

    auto splitter1 = new QSplitter(Qt::Horizontal, ui->content);
    splitter1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    splitter1->addWidget(m_propertyGrid);
    splitter1->addWidget(splitter2);
    splitter1->setStretchFactor(0, 2);
    splitter1->setStretchFactor(1, 3);
    contentLayout->addWidget(splitter1);

    auto toolbar = new QToolBar(this);
    this->addToolBar(toolbar);
    auto runScriptAction = new QAction(this);
    runScriptAction->setText("Run Script");
    runScriptAction->setShortcut(QKeySequence::fromString("CTRL+R"));
    connect(runScriptAction, &QAction::triggered, this, &MainWindow::runScript);
    connect(ui->actionRunScript, &QAction::triggered, this, &MainWindow::runScript);
    toolbar->addAction(runScriptAction);
}

MainWindow::~MainWindow()
{
    delete ui;
}


/* we end up here when the user runs the script
 * we take the m_editor contents, run them and
 * append the output to the m_output buffer.
 */
void MainWindow::runScript()
{
    auto script = this->m_editor->toPlainText();
    auto result = m_scriptEngine->evaluate(script);

    m_output->append(result.toString());
    m_propertyGrid->updateGui();
}
