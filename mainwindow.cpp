#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "nativecontext.h"
#include <QDebug>
#include <QToolBar>
#include <QSplitter>
#include <QMessageBox>

MainWindow *MainWindow::activeInstance = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    MainWindow::activeInstance = this;
    ui->setupUi(this);

    /* configure script engine */
    m_scriptEngine = new QScriptEngine(this);

    // context is a QObject we can interact with from the scriptengine
    auto context = new NativeContext(this);
    auto jsObject = m_scriptEngine->newQObject(context);
    m_scriptEngine->globalObject().setProperty("context", jsObject);

    // showMessageBox is a function implemented in c++ that we can call from the scriptengine
    auto l = [] (QScriptContext *context, QScriptEngine *engine) {
        auto mainWindow = MainWindow::activeInstance;
        return mainWindow->messageBox(context, engine);
    };
    QScriptValue func = m_scriptEngine->newFunction(l, 1);
    m_scriptEngine->globalObject().setProperty("messageBox", func);



    /* Setup the gui
     * we have 2 areas for scripting and output
     * and a simple grid showing the properties
     * of the NativeContext object
     */

    connect(ui->actionExit, &QAction::triggered, this, [=] {
        this->close();
    });

    /* configure gui and actions */
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
    toolbar->addAction(runScriptAction);
    connect(runScriptAction, &QAction::triggered, this, &MainWindow::runScript);
    connect(ui->actionRunScript, &QAction::triggered, this, &MainWindow::runScript);

    auto messageBoxCodeAction = new QAction(this);
    messageBoxCodeAction->setText("Insert MessageBox-Code");
    messageBoxCodeAction->setShortcut(QKeySequence::fromString("CTRL+M"));
    toolbar->addAction(messageBoxCodeAction);
    connect(messageBoxCodeAction, &QAction::triggered, this, &MainWindow::insertMessageBoxCode);
    connect(ui->actionRunMessageBoxCode, &QAction::triggered, this, &MainWindow::insertMessageBoxCode);
}

MainWindow::~MainWindow()
{
    delete ui;
    MainWindow::activeInstance = nullptr;
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

/* we end up here when the user runs shows a
 * message box...
 * this happens directly via the script
 */
void MainWindow::insertMessageBoxCode()
{
    this->m_editor->setText(
        "/* invoking a c++ function via the scriptengine */\n"
        "messageBox(\"Hello Script \" + new Date().toString());");
}


QScriptValue MainWindow::messageBox(QScriptContext *context, QScriptEngine *engine) {
    if (context->argumentCount() != 1) {
        return context->throwError(QScriptContext::SyntaxError, "Function requires exactly 1 argument.");
    }
    QString message = context->argument(0).toString();

    QMessageBox messageBox(QApplication::activeWindow());
    messageBox.setWindowTitle("Message");
    messageBox.setText(message);
    messageBox.setIcon(QMessageBox::Information);
    messageBox.setStandardButtons(QMessageBox::Ok);
    messageBox.setDefaultButton(QMessageBox::Ok);
    auto result = messageBox.exec();

    return QScriptValue(result);
}

