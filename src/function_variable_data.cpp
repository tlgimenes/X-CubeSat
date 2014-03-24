#include "function_variable_data.hpp"

FunctionVariableData::FunctionVariableData():Function()
{
    /*
     * Nothing to do here;
     */
}

FunctionVariableData::FunctionVariableData(FunctionVariableData* d): Function()
{
    this->received_data = *d->get_value_received();
    this->format        = *d->get_value_format();
    this->date          =  d->get_value_date();
}

FunctionVariableData::FunctionVariableData(Glib::ustring str, Glib::ustring format, unsigned int date):Function()
{
    this->received_data = str;
    this->format = format;
    this->date   = date;
}

void FunctionVariableData::set_value(FunctionVariableData* d)
{
    this->received_data = *d->get_value_received();
    this->format        = *d->get_value_format();
    this->date          =  d->get_value_date();
}

void FunctionVariableData::set_value(Glib::ustring str, Glib::ustring format, unsigned int date)
{
    this->received_data = str;
    this->format = format;
    this->date   = date;
}

Glib::ustring *FunctionVariableData::get_value_received()
{
    return &(this->received_data);
}

Glib::ustring *FunctionVariableData::get_value_format()
{
    return &(this->format);
}

unsigned int FunctionVariableData::get_value_date()
{
    return this->date;
}

Function *FunctionVariableData::run(std::vector<Function*> *runQueue, Glib::ustring *satName) throw(std::bad_typeid*)
{
    std::cout << "VariableData";

    return this;
}
