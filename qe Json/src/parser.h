#pragma once

#include <string>
#include "json.h"

namespace Qee1y 
{
	namespace QeJson
	{
		using std::string;

		class Parser
		{
		public:
			Parser() : m_str(""), m_index(0) {};
			Json parse();
			char get_next_token();       
			void skip_white_space();
			void load(const std::string& str);
		private:
			Json parse_null();
			Json parse_bool();
			Json parse_number();
			Json parse_array();
			Json parse_object();
			string parse_str();
		private:
			string m_str;
			int m_index;
		};
	}
}