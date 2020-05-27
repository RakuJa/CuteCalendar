#include "factory.h"


void Factory::firstParse() {
    //Ricavo dei campi dati di evento
    nome = json["NAME"].toString().toStdString();
    //conversione stringa -> data
    temp= std::istringstream(json["DATA"].toString().toStdString());
    temp>>date;
    descr = json["DESCRIPTION"].toString().toStdString();
    place = json["PLACE"].toString().toStdString();
    //conversione jsonarray -> array stringhe
    tmp = json["TAGS"].toArray();
    for (QJsonArray::const_iterator it = tmp.begin();it!=tmp.end();++it) {
        std::string tag = it->toString().toStdString();
        //Check se tag sono empty(formato sbagliato es)
        if (!tag.empty())
            tags.push_back(it->toString().toStdString());
    }
}

void Factory::firstCheck() const {
    if (nome.empty() || descr.empty() || place.empty())
        throw std::runtime_error("Salvataggio corrotto : campi fondamentali vuoti o di tipo errato");
}

void Factory::secondParse() {
    if (id!=BirthDay::ID && id!=ToDoList::ID) {
        //Check autonomamente in time o date
        temp= std::istringstream(json["START_TIME"].toString().toStdString());
        temp>>start;
        temp= std::istringstream(json["END_TIME"].toString().toStdString());
        temp>>end;
        if (id!=Workout::ID) {
            rep = json["REPEAT"].toBool();
            //automatico false in caso di formato errato
            //conversione stringa -> time
            temp= std::istringstream(json["ALERT_TIME"].toString().toStdString());
            temp>>alert;
        }
    }
}

Factory::Factory(QJsonObject &js) : json(js) {
    tags = std::vector<std::string>();
    vectPart = std::vector<std::string>();
}

bool Factory::isEmail(const std::string& email) const{
   // define a regular expression
   const std::regex pattern
      ("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");

   // try to match the string with the regular expression
   return std::regex_match(email, pattern);
}

Event* Factory::parse() {

    id = json["ID"].toInt();

    if (id == 0) throw std::runtime_error("Salvataggio corrotto : ID INVALIDO");
    firstParse();
    firstCheck();
    secondParse();

    switch (id) {
    case Reminder::ID:
        //REMINDER = 1
        urg = json["URGENCY"].toInt();
        if (urg==0) {
            throw std::runtime_error("Salvataggio corrotto : campo priotità vuoto o di tipo errato");
        }
        return new Reminder(urg,start,end,alert,rep,nome,descr,place,date,tags);
    case Workout::ID:
        //WORKOUT int x,Time start,Time end
        act = json["ACTIVITY"].toInt();
        if (act==0) {
            throw std::runtime_error("Salvataggio corrotto : campo attività vuoto o di tipo errato");
        }
        return new Workout(act,start,end,nome,descr,place,date,tags);
    case Meeting::ID:
        //MEETING
        tmp = json["PARTECIPANTS"].toArray();
        for (QJsonArray::const_iterator it = tmp.begin();it!=tmp.end();++it) {
            std::string mail = it->toString().toStdString();
            //manca un check di validità di mail
            //Check se email sono empty(formato sbagliato es)
            if (!mail.empty() && isEmail(mail))
                vectPart.push_back(mail);
        }
        return new Meeting(vectPart,start,end,alert,rep,nome,descr,place,date,tags);
    case BirthDay::ID:
        //BIRTHDAY
        birth = (json["ANNO_NASCITA"].toInt());
        if (birth==0) {
            throw std::runtime_error("Salvataggio corrotto : campo anno nascita vuoto o di tipo errato");
        }
        return new BirthDay(birth,nome,descr,place,date,tags);
    case ToDoList::ID:
        ToDoList* temp = new ToDoList(nome,descr,place,date,tags);
        tmp = json["TODOLIST"].toArray();
        for (QJsonArray::const_iterator it = tmp.begin();it!=tmp.end();++it) {
            QJsonObject itemObject = (*it).toObject();
            std::string item_desc = itemObject["item_description"].toString().toStdString();
            if (!item_desc.empty())
                temp->addItem(item_desc,itemObject["item_isdone"].toBool());
        }
        return temp;
    }

    return nullptr;
}

