#include "propertygrid.h"

#include <QVBoxLayout>
#include <QHeaderView>
#include <QMetaMethod>

PropertyGrid::PropertyGrid(QWidget *parent)
    : QWidget{parent}
{
    m_tableWidget = new QTableWidget(this);
    m_tableWidget->setColumnCount(2);
    m_tableWidget->setHorizontalHeaderLabels(QStringList() << "Property" << "Value");
    m_tableWidget->horizontalHeader()->setStretchLastSection(true);
    m_tableWidget->verticalHeader()->setVisible(false);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_tableWidget);
    setLayout(layout);
}

void PropertyGrid::setWatchedObject(QObject *watchedObject) {
    if (watchedObject == m_watchedObject) {
        return;
    }

    if (m_watchedObject != nullptr) {
        // disconnect old object
        disconnect(m_watchedObject, nullptr, this, nullptr);
    }

    m_watchedObject = watchedObject;
    updateGui();
}

void PropertyGrid::updateGui() {
    if (m_watchedObject == nullptr) {
        m_tableWidget->setRowCount(0);
    } else {
        auto metaObject = m_watchedObject->metaObject();
        int propertyCount = metaObject->propertyCount();
        m_tableWidget->setRowCount(propertyCount);

        for (int i = 0; i < propertyCount; ++i) {
            auto metaProperty = metaObject->property(i);
            auto propertyName = QString(metaProperty.name());
            auto propertyValue = m_watchedObject->property(metaProperty.name());

            auto nameItem = new QTableWidgetItem(propertyName);
            auto valueItem = new QTableWidgetItem(propertyValue.toString());
            m_tableWidget->setItem(i, 0, nameItem);
            m_tableWidget->setItem(i, 1, valueItem);
        }
        //m_tableWidget->resizeColumnsToContents();
    }
}
