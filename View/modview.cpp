#include "modview.h"

ModView::ModView(QWidget *parent)
        : QWidget(parent), enabled(true){

    //init grafica
    mainLayout = new QVBoxLayout(this);

    lTag = new QLabel("Tag",this);
    lNome= new QLabel("Nome",this);
    lDesc= new QLabel("Descrizione",this);
    lLuogo = new QLabel("Luogo",this);
    txtDesc = new QTextEdit(this);
    txtNome = new QLineEdit(this);
    txtLuogo = new QLineEdit(this);
    checkTag= new TagPicker(this);

    mainLayout->addWidget(lNome);
    mainLayout->addWidget(txtNome);
    mainLayout->addWidget(lLuogo);
    mainLayout->addWidget(txtLuogo);
    mainLayout->addWidget(lDesc);
    mainLayout->addWidget(txtDesc);
    mainLayout->addWidget(lTag);
    mainLayout->addWidget(checkTag);
}

//Set delle componenti della view a enabled o disabled
void ModView::setEnabled(bool e) {
    enabled = e;

    txtDesc->setReadOnly(!e);
    txtNome->setReadOnly(!e);
    txtLuogo->setReadOnly(!e);
    checkTag->setEnabled(e);
}

bool ModView::isEnabled() const {
    return enabled;
}

//Passaggio del contenuto della view al Model per il salvataggio delle modifiche.
void ModView::pushSaves(const Model::It& it) const {
    QString error = "";
    if(checkPushable(error)){
        (*it)->setDesc((txtDesc->toPlainText()).toStdString());
        (*it)->setName((txtNome->text()).toStdString());
        (*it)->setPlace((txtLuogo->text()).toStdString());
        (*it)->clearTags();
        for (auto tag: checkTag->getTags()){
            (*it)->addTag(tag);
        }
    } else
        throw std::logic_error(error.toStdString());
}

//Caricamento del contenuto delle evento nella view
void ModView::fillView(const Model::It& it) {
    txtDesc->setText(QString::fromStdString((*it)->getDesc()));
    txtNome->setText(QString::fromStdString((*it)->getName()));
    txtLuogo->setText(QString::fromStdString((*it)->getPlace()));
    for (auto tag: (*it)->getTags()){
        checkTag->addTag(tag);
    }
}

//Controllo errori nella view prima del salvataggio
bool ModView::checkPushable(QString& err) const {

    bool ritorno = true;

    if(txtNome->text().trimmed().isEmpty()){
        ritorno=false;
        err +=  + "Il campo Nome non può essere vuoto.\n";
    }

    if(txtLuogo->text().trimmed().isEmpty()){
        ritorno=false;
        err +=  + "Il campo Luogo non può essere vuoto.\n";
    }

    if(txtDesc->toPlainText().trimmed().isEmpty()){
        ritorno=false;
        err +=  + "Il campo Descrizione non può essere vuoto.\n";
    }

    return ritorno;
}
