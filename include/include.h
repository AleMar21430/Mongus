#pragma once

#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <windows.h>
#include <Shlwapi.h>
#include <stdexcept>
#include <iostream>
#include <optional>
#include <direct.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <variant>
#include <cstdlib>
#include <cstring>
#include <numeric>
#include <string>
#include <memory>
#include <cerrno>
#include <vector>
#include <chrono>
#include <thread>
#include <random>
#include <future>
#include <math.h>
#include <array>
#include <tuple>
#include <any>
#include <map>
#include <set>

//#include <mongocxx/client.hpp>
//#include <mongocxx/instance.hpp>
//#include <mongocxx/options/find.hpp>
#include <cpr/cpr.h>
#include <json.h>

#include <QtGui>
#include <QtCore>
#include <QtCharts>
#include <QtWidgets>
#include <QtDataVisualization>

using namespace std;
namespace fs = filesystem;
namespace json = nlohmann;
//namespace mongo = mongocxx;

enum struct Remote_Request_Type {
	UNKNOWN
};

enum struct Local_Request_Type {
	UNKNOWN
};