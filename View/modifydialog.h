#ifndef MODIFYDIALOG_H
#define MODIFYDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include "Model/model.h"
#include "modview.h"
#include "viewworkout.h"
#include "viewbirthday.h"
#include "viewmeeting.h"
#include "viewreminder.h"
#include "viewtodolist.h"

class ModifyDialog : public QDialog {
    Q_OBJECT

public:
    explicit ModifyDialog(QDate date,const Model::It&, QDialog *parent = nullptr);
    virtual ~ModifyDialog();

signals:
    void deleteEvent(Model::It);
    void modifiedEvent(Model::It);

protected slots:
    virtual void modifyPushed();
    virtual void deletePushed();

private:
    Model::It it;
    ModView* view;
    bool modifyEnabled;

    QVBoxLayout* layout;
    QVBoxLayout* viewLayout;
    QHBoxLayout* buttomLayout;
    QLabel* completato;
    QDate date;

    QPushButton* btnModify;
    QPushButton* btnDelete;
};

#endif // MODIFYDIALOG_H
