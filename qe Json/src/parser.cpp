#include <iostream>

#include "parser.h"

using namespace Qee1y::QeJson;

void Parser::load(const string& str)
{
	m_str = str;
	m_index = 0;
}

Json Parser::parse()
{
	skip_white_space();
	char ch = get_next_token();
	switch (ch)
	{
	case 'n':
		m_index--;
		return parse_null();
	case 'f':
	case 't':
		m_index--;
		return parse_bool();
	case '-':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		m_index--;
		return parse_number();
	case '"':
		return Json(parse_str());
	case '[':
		return parse_array();
	case '{':
		return parse_object();
	default:
		break;
	}
	return Json();
}

void Parser::skip_white_space()
{

	while (m_str[m_index] == ' ' || m_str[m_index] == '\n' || 
		m_str[m_index] == '\r' || m_str[m_index] == '\t')
	{
		m_index++;
	}
}

char Parser::get_next_token()
{
	return m_str[m_index++];
}

Json Parser::parse_null()
{
	if (m_str.compare(m_index, 4, "null") == 0)
	{
		m_index += 4;
		return Json();
	}
}

Json Parser::parse_bool()
{
	if (m_str.compare(m_index, 4, "true") == 0)
	{
		m_index += 4;
		return Json(true);
	}
	else if (m_str.compare(m_index, 5, "false") == 0)
	{
		m_index += 5;
		return Json(false);
	}
}

Json Parser::parse_number()
{
	int pos = m_index;
	if (m_str[m_index] == '-')
		m_index++;
	if (m_str[m_index] < '0' || m_str[m_index] > '9')
	{

	}
	while (m_str[m_index] >= '0' && m_str[m_index] <= '9')
		m_index++;
	if (m_str[m_index] != '.')
	{
		int i = std::atoi(m_str.c_str() + pos);
		return Json(i);
	}
	m_index++;
	while (m_str[m_index] <= '9' && m_str[m_index] >= '0')
		m_index++;
	double f = std::atof(m_str.c_str() + pos);
	return  Json(f);

}

Json Parser::parse_array()
{
	Json arr(Json::json_array);
	skip_white_space();
	char ch = get_next_token();
	
	if (ch == ']')
		return arr;
	m_index--;
	while (true)
	{
 		arr.append(parse());
		skip_white_space();
		ch = get_next_token();
		if (ch == ']')
			break;
		if (ch != ',')
		{

		}
	}
	return arr;
}

Json Parser::parse_object()
{
	Json obj(Json::json_object);
	skip_white_space();
	char ch = get_next_token();
	if (ch == '}')
		return obj;
	m_index--;
	while (true)
	{
		skip_white_space();
		ch = get_next_token();
		if (ch != '"')
		{
			throw new std::logic_error("error");
		}
		string key = parse_str();
		skip_white_space();
		ch = get_next_token();
		if (ch != ':')
		{
			throw new std::logic_error("error");
		}
		obj[key] = parse();
		skip_white_space();
		ch = get_next_token();
		if (ch == '}')
			break;
		m_index++;
	}
	return obj;
}

string Parser::parse_str()
{
	string temp;
	while (true)
	{
		char ch = get_next_token();
		if (ch == '"')
			break;
		if (ch == '\\')
		{
			ch = get_next_token();
			switch (ch)
			{
			case '\n':
				temp += '\n';
				break;
			case '\r':
				temp += '\r';
				break;
			case '\t':
				temp += '\t';
				break;
			case '\b':
				temp += '\b';
				break;
			case '\f':
				temp += '\f';
				break;
			case '"':
				temp += "\\\"";
				break;
			case '\\':
				temp += "\\\\";
				break;
			case'\\u':
				temp += '\\u';
				for (int i = 0; i < 4; i++)
				{
					temp += m_str[m_index++];
				}
			default:
				break;
			}
		}
		else
			temp += ch;
	}
	return temp;
}
 