#pragma once

#include "include.h"
#include "qt core.h"

struct Remote_Query;
class Remote_Thread;
struct Local_Query;
class Local_Thread;

struct Remote_Query {
	QString query;
	Remote_Request_Type type;
	uint32_t request_id;

	Remote_Query(
		const QString& query,
		const Remote_Request_Type& type,
		const uint32_t& request_id
	) :
		query(query),
		type(type),
		request_id(request_id)
	{}
};

struct Local_Query {
	QString query;
	Local_Request_Type type;
	uint32_t request_id;

	Local_Query(
		const QString& query,
		const Local_Request_Type& type,
		const uint32_t& request_id
	) :
		query(query),
		type(type),
		request_id(request_id)
	{}
};

class Remote_Thread : public QThread {
	Q_OBJECT
public:
	Remote_Query work;

	Remote_Thread(const Remote_Query& i_work) :
		QThread(),
		work(i_work)
	{};
	~Remote_Thread() {};
	void run() override;
signals:
	void result(const uint32_t& i_request_id, const QString& i_data, const int& i_index);
	void logMsg(const QString& i_message);
};

class Local_Thread : public QThread {
	Q_OBJECT
public:
	Local_Query work;

	Local_Thread(const Local_Query& i_work) :
		QThread(),
		work(i_work)
	{};
	~Local_Thread() {};
	void run() override;
signals:
	void result(const uint32_t& i_request_id, const QString& i_data, const int& i_index);
	void logMsg(const QString& i_message);
};