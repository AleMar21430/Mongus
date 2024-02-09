#pragma once

#include "include.h"
#include "qt core.h"

struct Async_Query;
class Async_Thread;
enum struct Async_Type {
	UNKNOWN,
	LISTINGS
};
struct Mongo_Query;
class Mongo_Thread;
enum struct Mongo_Type {
	UNKNOWN,
	LISTINGS
};

struct Async_Query {
	unordered_map<string, string> query;
	Async_Type type;
	uint32_t request_id;
	int thread_id;

	Async_Query(
		const unordered_map<string, string>& query,
		const Async_Type& type,
		const uint32_t& request_id,
		const int& thread_id = 0
	) :
		query(query),
		type(type),
		request_id(request_id),
		thread_id(thread_id)
	{}
};

struct Mongo_Query {
	unordered_map<string, string> query;
	Mongo_Type type;
	uint32_t request_id;
	int thread_id;

	Mongo_Query(
		const unordered_map<string, string>& query,
		const Mongo_Type& type,
		const uint32_t& request_id,
		const int& thread_id = 0
	) :
		query(query),
		type(type),
		request_id(request_id),
		thread_id(thread_id)
	{}
};

class Async_Thread : public QThread {
	Q_OBJECT
public:
	Async_Query work;

	Async_Thread(const Async_Query& work) :
		QThread(),
		work(work)
	{};
	~Async_Thread() {};

	void run() override;
signals:
	void result(const Async_Query& query, const string& data);
	void iconResult(const Async_Query& query, const QIcon& icon);
	void logMsg(const QString& message);
};

class Mongo_Thread : public QThread {
	Q_OBJECT
public:
	QQueue<Mongo_Query> work_queue;
	QWaitCondition condition;
	QMutex semaphore;

	unordered_map<QString, QString>* settings;
	mongo::client mongo_connection;
	mongo::database database;

	Mongo_Thread(unordered_map<QString, QString>* settings = nullptr) :
		QThread(),
		settings(settings)
	{
		mongo_connection = mongo::client(mongo::uri((*settings)["db_url"].toStdString()));
		database = mongo_connection["Proyecto_1"];
	};
	~Mongo_Thread() {};

	void run() override;
	void processWork(const Mongo_Query& work);
	void queueWork(const Mongo_Query& work);
	void cancelWork();
signals:
	void result(const Mongo_Query& query, const json& data);
	void logMsg(const QString&_message);
};