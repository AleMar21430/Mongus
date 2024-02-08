#pragma once

#include <unordered_map>
#include <filesystem>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <memory>
#include <cerrno>
#include <vector>
#include <map>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/options/find.hpp>
#include <bsoncxx/json.hpp>

#include <cpr/cpr.h>
#include <json.h>

#include <QtGui>
#include <QtCore>
#include <QtCharts>
#include <QtWidgets>
#include <QtDataVisualization>

using namespace std;
namespace fs = filesystem;
namespace mongo = mongocxx;

using json = nlohmann::json;

enum struct Remote_Request_Type {
	UNKNOWN
};

enum struct Local_Request_Type {
	UNKNOWN
};