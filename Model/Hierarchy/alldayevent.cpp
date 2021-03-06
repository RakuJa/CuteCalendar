#include "alldayevent.h"

AllDayEvent::AllDayEvent(const std::string& nome,const std::string& descr,
        const std::string& luogo,const Date& data,
        const std::vector<std::string>& tags)
    : Event(nome,descr,luogo,data,tags) {}

Time AllDayEvent::getDuration() const {
    return Time(23,59,59);
}

bool AllDayEvent::isCompleted() const {
    return Event::getDate()<Date();
}

void AllDayEvent::serialize(QJsonObject &json) const {
    //Chiamata alla base
    Event::serialize(json);
}

