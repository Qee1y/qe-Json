#include <iostream>
#include <fstream>
#include <sstream>

#include "json.h"


int main()
{
	using Qee1y::QeJson::Json;

	std::ifstream in("C:/Users/16493/Downloads/sojson.com.json");
	std::stringstream ss;
	ss << in.rdbuf();
	Json v;
	v.parse(ss.str());
	std::cout << v.str();

	bool islogin = v["data"]["isLogin"];
	int email_verified = v["data"]["email_verified"];
	int mid = v["data"]["mid"];
	return 0;
}