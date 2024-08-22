#ifndef NATIVECONTEXT_H
#define NATIVECONTEXT_H

#include <QObject>

/*
 * An Object with QProperties that we will modify from
 * inside the QScriptEngine
 *
 * The object will be registered under the global name "context"
 *
 */
class NativeContext : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
    Q_PROPERTY(int counter READ counter WRITE setCounter NOTIFY counterChanged FINAL)
    Q_PROPERTY(bool flag READ flag WRITE setFlag NOTIFY flagChanged FINAL)

    QString m_title = "";
    int m_counter = 0;
    bool m_flag = false;

public:
    explicit NativeContext(QObject *parent = nullptr);

    QString title() const { return m_title; }
    void setTitle(const QString &title) {
        if (m_title != title) {
            m_title = title;
            emit titleChanged();
        }
    }

    int counter() const { return m_counter; }
    void setCounter(int counter) {
        if (m_counter != counter) {
            m_counter = counter;
            emit counterChanged();
        }
    }

    bool flag() const { return m_flag; }
    void setFlag(int flag) {
        if (m_flag != flag) {
            m_flag = flag;
            emit flagChanged();
        }
    }
signals:
    void titleChanged();
    void counterChanged();
    void flagChanged();
};

#endif // NATIVECONTEXT_H
