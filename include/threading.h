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
enum struct Mongo_Type {
	PRODUCERS,
	SHOWINGS,
	MOVIES,
	ACTORS,
	STAFFS,
	USERS,
	PRODUCER,
	SHOWING,
	MOVIE,
	ACTOR,
	STAFF,
	USER,
	REVIEW_ADD
};

struct Async_Query {
	unordered_map<string, string> query;
	Async_Type type;
	uint32_t request_id;
	int thread_id;
	cpr::Header headers;

	Async_Query(
		const unordered_map<string, string>& query,
		const Async_Type& type,
		const uint32_t& request_id,
		const cpr::Header& headers,
		const int& thread_id = 0
	) :
		query(query),
		type(type),
		request_id(request_id),
		headers(headers),
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

	void PRODUCERS(const Mongo_Query& work);
	void SHOWINGS(const Mongo_Query& work);
	void MOVIES(const Mongo_Query& work);
	void ACTORS(const Mongo_Query& work);
	void STAFFS(const Mongo_Query& work);
	void USERS(const Mongo_Query& work);

	void PRODUCER(const Mongo_Query& work);
	void SHOWING(const Mongo_Query& work);
	void MOVIE(const Mongo_Query& work);
	void ACTOR(const Mongo_Query& work);
	void STAFF(const Mongo_Query& work);
	void USER(const Mongo_Query& work);

signals:
	void result(const Mongo_Query& query, const json& data);
	void logMsg(const QString&_message);
};