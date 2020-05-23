#include "neweventdialog.h"
#include "tagpicker.h"

NewEventDialog::NewEventDialog(QDialog *parent) : QDialog(parent) {
    mainLayout = new QVBoxLayout();
    selLayout = new QVBoxLayout();
    eventLayout = new QVBoxLayout();

    mainLayout->setAlignment(Qt::AlignTop);
    initComboBox();

    resize(450, height());
    setLayout(mainLayout);
}

void NewEventDialog::deleteLayout(QLayout* layout) {
    QLayoutItem* child;

    while(layout->count() > 0) {
        child = eventLayout->takeAt(0);

        if (child->layout() != 0)
            deleteLayout(child->layout());
        else if (child->widget() != 0)
            delete child->widget();

        delete child;
    }
}

void NewEventDialog::initComboBox() {
    selEvento = new QComboBox();
    selEvento->addItem("Allenamento");
    selEvento->addItem("Compleanno");
    selEvento->addItem("Meeting");
    selEvento->addItem("Promemoria");

    connect(selEvento, SIGNAL(currentIndexChanged(int)), this , SLOT(changedSel(int)) );

    selLayout->setAlignment(Qt::AlignTop);
    selLayout->addWidget(selEvento);
    mainLayout->addLayout(selLayout);
    mainLayout->addLayout(eventLayout);

    selEvento->setCurrentIndex(0);
    changedSel(0);
}

void NewEventDialog::changedSel(int index) {
    deleteLayout(eventLayout);

    // Possibile memory leak DA RISOLVERE
    switch(index){
        case 0:
            view = new ViewAllenamento(this);
            break;
        case 1:
            view = new ViewCompleanno(this);
            break;
        case 2:
            view = new ViewMeeting(this);
            break;
        case 3:
            view = new ViewPromemoria(this);
            break;
        // default:
            // ERRORE
    }

    eventLayout->addWidget(view);

    QPushButton* addEvent= new QPushButton(tr("Aggiungi Evento"));
    connect(addEvent,SIGNAL(clicked(bool)),this, SLOT(addPushed()));
    eventLayout->addWidget(addEvent);
}

void NewEventDialog::addPushed() {
    /* try {
     *  emit newEventCreated(view->createEvent())
     * } catch(SyntaxException se) {
     *  QMessageBox::critical(this, QString("Error"), se.getMessage()));
     * }
    */

    std::cout<< "Bottone Premuto" << std::endl;
}