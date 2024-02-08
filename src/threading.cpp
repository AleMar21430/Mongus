#include "threading.h"

void Async_Thread::run() {
	emit result(work, "");
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
		//work.query.at("collection")
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