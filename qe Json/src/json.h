#pragma once

#include <string>
#include <vector>
#include <map>

namespace Qee1y
{
	namespace QeJson
	{
		using std::string;

		class Json
		{
		public:
			enum Type {
				json_null = 0,
				json_bool,
				json_int,
				json_double,
				json_array,
				json_object,
				json_string
			};

			Json();
			~Json();
			Json(bool value);
			Json(int vlaue);
			Json(double value);
			Json(const char* value);
			Json(const string& value);
			Json(Type type);
			Json(const Json& other);

			void append(const Json& other);
			void parse(const string& str);

			string str() const;

			operator bool();
			operator int();
			operator double();
			operator string();

			Json& operator[](int index);
			Json& operator[](const char* str);
			Json& operator[](const string& str);

		private:
			union Value
			{
				bool m_bool;
				int m_int;
				double m_double;
				string* m_string;
				std::vector<Json>* m_array;
				std::map<string, Json>* m_object;

			};

			Type m_type;
			Value m_value;
		};
	}
}

