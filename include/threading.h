#pragma once

#include "include.h"
#include "qt core.h"

struct Async_Query;
class Async_Thread;
enum struct Async_Type {
	UNKNOWN
};
struct Mongo_Query;
class Mongo_Thread;

struct Async_Query {
	QString query;
	Async_Type type;
	uint32_t request_id;

	Async_Query(
		const QString& query,
		const Async_Type& type,
		const uint32_t& request_id
	) :
		query(query),
		type(type),
		request_id(request_id)
	{}
};

struct Mongo_Query {
	QString query;
	uint32_t request_id;

	string collection;

	Mongo_Query(
		const QString& query,
		const uint32_t& request_id
	) :
		query(query),
		request_id(request_id)
	{}
};

class Async_Thread : public QThread {
	Q_OBJECT
public:
	Async_Query work;

	Async_Thread(const Async_Query& i_work) :
		QThread(),
		work(i_work)
	{};
	~Async_Thread() {};
	void run() override;
signals:
	void result(const uint32_t& i_request_id, const string& i_data);
	void logMsg(const QString& i_message);
};

class Mongo_Thread : public QThread {
	Q_OBJECT
public:
	QQueue<Mongo_Query> work_queue;
	QWaitCondition condition;
	QMutex semaphore;
	// Uniforms
	unordered_map<QString, QString>* settings;

	mongo::client mongo_connection;
	mongo::database database;

	Mongo_Thread(unordered_map<QString, QString>* i_settings = nullptr) :
		QThread(),
		settings(i_settings)
	{
		mongo_connection = mongo::client(mongo::uri((*settings)["db_url"].toStdString()));
		database = mongo_connection["Proyecto_1"];
	};
	~Mongo_Thread() {
	};

	void run() override;

	void processWork(const Mongo_Query& i_work);
	void queueWork(const Mongo_Query& i_work);
	void cancelWork();
signals:
	void result(const uint32_t& i_request_id, const json& i_data);
	void logMsg(const QString& i_message);
};