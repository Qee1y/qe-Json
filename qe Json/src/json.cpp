#include "json.h"
#include "parser.h"

#include <iostream>
#include <sstream>

using namespace Qee1y::QeJson;

/*重载运算符*************************************************************************************************/
Json::operator bool()
{
	if (m_type != json_bool)
		throw new std::logic_error("类型错误，非bool类型!");
	return m_value.m_bool;
}

Json::operator int()
{
	if(m_type != json_int)
		throw new std::logic_error("类型错误，非int类型!");
	return m_value.m_int;
}

Json::operator double()
{
	if (m_type != json_double)
		throw new std::logic_error("类型错误，非double类型!");
	return m_value.m_double;
}

Json::operator std::string()
{
	if (m_type != json_string)
		throw new std::logic_error("类型错误，非str类型!");
	return *m_value.m_string;
 }

Json& Json::operator[](int index)
{
	if (m_type != json_array)
	{
		m_type = json_array;
		m_value.m_array = new std::vector<Json>();
	}
	if(index < 0)
		throw new std::logic_error("类型错误，非array类型!");

	int size = m_value.m_array->size();
	if (index >= size)
		for (int i = size; i <= index; i++)
			m_value.m_array->push_back(Json());
	return m_value.m_array->at(index);
}

Json& Json::operator[](const char* str)
{
	string name(str);
 	return (*(this))[name];
}

Json& Json::operator[](const string& str)
{
	if (m_type != json_object)
	{
		m_type = json_object;
		m_value.m_object = new std::map<string, Json>();
	}
	return (*(m_value.m_object))[str];
}

/*******************************************************************************************************************/
Json::Json() : m_type(json_null){}

Json::~Json() 
{ 
	switch (m_type)
	{
	case Qee1y::QeJson::Json::json_null:
		break;
	case Qee1y::QeJson::Json::json_bool:
		m_value.m_bool = false;
		break;
	case Qee1y::QeJson::Json::json_int:
		m_value.m_int = 0;
		break;
	case Qee1y::QeJson::Json::json_double:
		m_value.m_double = 0.0;
		break;
	case Qee1y::QeJson::Json::json_array:
		m_value.m_array = new std::vector<Json>();
		delete m_value.m_array;
		break;
	case Qee1y::QeJson::Json::json_object:
		//delete m_value.m_object;
		break;
	case Qee1y::QeJson::Json::json_string:
		m_value.m_string = new string("");
		delete m_value.m_string;
		break;
	default:
		break;
	}
	m_type = json_null;
}

Json::Json(bool value) : m_type(json_bool)
{
	m_value.m_bool = value;
}

Json::Json(int value) : m_type(json_int)
{
	m_value.m_int = value;
}

Json::Json(double value) : m_type(json_double)
{
	m_value.m_double = value;
}

Json::Json(const char* value) : m_type(json_string)
{
	m_value.m_string = new string(value);
}

Json::Json(const string& value) : m_type(json_string)
{
	m_value.m_string = new string(value);
}

Json::Json(Type type) : m_type(type)
{
	switch (m_type)
	{
	case json_null:
		break;
	case json_bool:
		m_value.m_bool = false;
		break;
	case json_int:
		m_value.m_int = 0;
		break;
	case json_double:
		m_value.m_double = 0.0;
		break;
	case json_array:
		m_value.m_array = new std::vector<Json>();
		break;
	case json_object:
		m_value.m_object = new std::map<string, Json>();
		break;
	case json_string:
		m_value.m_string = new string("");
		break;
	default:
		break;
	}
}

Json::Json(const Json& othen)
{
	m_type = othen.m_type;
	switch (m_type)
	{
	case json_null:
		break;
	case json_bool:
		m_value.m_bool = othen.m_value.m_bool;
		break;
	case json_int:
		m_value.m_int = othen.m_value.m_int;
		break;
	case json_double:
		m_value.m_double = othen.m_value.m_double;
		break;
	case json_array:
		m_value.m_array = othen.m_value.m_array;
		break;
	case json_object:
		m_value.m_object = new std::map<string, Json>();
		break;
	case json_string:
		m_value.m_string = new string(*othen.m_value.m_string);  //深拷贝 (深拷贝会造成性能慢问题)
		break;
	default:
		break;
	}
}

void Json::append(const Json& other)
{
	if (m_type != json_array)
	{
		m_type = json_array;
		m_value.m_array = new std::vector<Json>();
	}
	m_value.m_array->push_back(other);
}

void Json::parse(const string& str)
{
	Parser p;
	p.load(str);
	*this = p.parse();
}

string Json::str() const
{
	std::stringstream ss;
	switch (m_type)
	{
	case json_null:
		ss << "null";
		break;
	case json_bool:
		if (m_value.m_bool)
			ss << "true";
		else
			ss << "fasle";
		break;
	case json_int:
		ss << m_value.m_int;
		break;
	case json_double:
		ss << m_value.m_double;
		break;
	case json_string:
		ss << '\"' << *m_value.m_string << '\"';
		break;
	case json_array:
		ss << '[';
		for (auto it = m_value.m_array->begin(); it != m_value.m_array->end(); it++)
		{
			if (it != m_value.m_array->begin())
				ss << ',';
			ss << it->str();
		}
		ss << ']';
		break;
	case json_object:
		ss << '{';
		for (auto it = m_value.m_object->begin(); it != m_value.m_object->end(); it++)
		{
			if (it != m_value.m_object->begin())
				ss << ',';
			ss << '\"' << it->first << '\"' << ':' << it->second.str();
		}
		ss << '}';
	}
	return ss.str();
}