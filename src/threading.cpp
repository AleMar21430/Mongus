#include "threading.h"

void Async_Thread::run() {
	if (work.type == Async_Type::LISTINGS) {
		if (work.query.at("url") != "Not available.") {
			emit logMsg("Requesting thumbnail: " + QString::fromStdString(work.query.at("url")));

			cpr::Response response = cpr::Get(cpr::Url{ work.query.at("url") }, work.headers);
			if (response.status_code == 200) {
				QPixmap pixmap;
				pixmap.loadFromData(QByteArray::fromStdString(response.text));
				emit iconResult(work, QIcon(pixmap));
			}
			else {
				stringstream msg;
				msg << "Error loading: " << work.query.at("url") << "  [" << to_string(response.status_code) + "] " << response.text;
				emit logMsg(QString::fromStdString(msg.str()));
				emit iconResult(work, QIcon("./res/Error.png"));
			}
		}
		else {
			emit iconResult(work, QIcon("./res/Error.png"));
		}
	}
}

void Mongo_Thread::run() {
	while (true) {
		semaphore.lock();
		while (!work_queue.isEmpty()) {
			const Mongo_Query& queuedWork = work_queue.dequeue();
			semaphore.unlock();
			processWork(queuedWork);
			semaphore.lock();
		}
		if (work_queue.isEmpty())
			condition.wait(&semaphore);
		semaphore.unlock();
	}
}

void Mongo_Thread::processWork(const Mongo_Query& work) {
	emit logMsg("Mongo Thread Processing");
	if (work.type == Mongo_Type::LISTINGS) {
		work.query.at("collection");
		mongo::collection collection = database["movies"];
		mongo::cursor cursor = collection.find({});
		
		json json_data;
		for (const bsoncxx::v_noabi::document::view& doc : cursor) {
			json_data.push_back(json::parse(bsoncxx::to_json(doc)));
		}
		
		emit result(work, json_data);
	}
}

void Mongo_Thread::queueWork(const Mongo_Query& work) {
	QMutexLocker locker(&semaphore);
	work_queue.enqueue(work);
	condition.wakeOne();
	emit logMsg("Mongo Thread Work Queued");
}

void Mongo_Thread::cancelWork() {
	QMutexLocker locker(&semaphore);
	work_queue.clear();
	condition.wakeOne();
	emit logMsg("Mongo Thread Work Cancelled");
}