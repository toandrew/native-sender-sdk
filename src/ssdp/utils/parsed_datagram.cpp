#include "parsed_datagram.h"

static void remove_space(string& str){
	string buff(str); 
	char space = ' '; 
	if (str == " ") {
		str = "";
		return;
	}
	str.assign(buff.begin() + buff.find_first_not_of(space),
	buff.begin() + buff.find_last_not_of(space) + 1); 
}

ParsedDatagram::ParsedDatagram(DatagramPacket *packet) {
	dp = packet;

	string messages = packet->getData();

	string delim("\r\n");

	fillMaps(messages, delim);
};

ParsedDatagram::~ParsedDatagram() {
}

map<string, string> ParsedDatagram::getData() {
	return data;
}

string ParsedDatagram::getType() {
	return type;
}

string ParsedDatagram::asciiUpper(string text) {
	return text;
}

int ParsedDatagram::fillMaps(const string& s, const string& delim) {
	printf("fillMaps!!!![%s]\n", s.c_str());
	size_t last = 0;
	size_t index = s.find_first_of(delim, last);
	while (index != std::string::npos) {
		string line = s.substr(last, index - last);
		if (last == 0) {
			type = line;
			printf("type:[%s]\n", type.c_str());
		}else {
			//printf("line:[%s]\n", line.c_str());
			size_t pos = line.find_first_of(":", 0);
			if (pos == std::string::npos) {
				printf("No lines![%d][%d][%d]\n", pos, index, last);
				break;
			}
		
			string key = asciiUpper(line.substr(0, pos));
			remove_space(key);
			string value = line.substr(pos + 1);
 			remove_space(value);
			printf("key[%s]value[%s]\n",key.c_str(), value.c_str());
			//data.insert(pair(key, value));
			data[key]=value;
		}

		last = index + 2; 
		index = s.find_first_of(delim, last);
		//printf("index[%d][%d]\n", last, index);
	}

	std::map<string, string>::iterator it;
	for (it = data.begin(); it != data.end(); ++it) {
		printf("[%s][%s]\n", ((string)it->first).c_str(), ((string)it->second).c_str());
	}
	

	/*if (index-last > 0) {
		ret->push_back(s.substr(last, index - last));
	}*/

	return 0;
}
