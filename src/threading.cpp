#include "threading.h"

void Async_Thread::run() {
	emit result(work.request_id, "");
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

void Mongo_Thread::processWork(const Mongo_Query& i_work) {
	mongo::collection collection = database[i_work.collection];
	mongo::cursor cursor = collection.find({});

	json json_data;
	for (const bsoncxx::v_noabi::document::view& doc : cursor) {
		json_data.push_back(json::parse(bsoncxx::to_json(doc)));
	}
	emit result(i_work.request_id, json_data);
}

void Mongo_Thread::queueWork(const Mongo_Query& i_work) {
	QMutexLocker locker(&semaphore);
	work_queue.enqueue(i_work);
	condition.wakeOne();
}

void Mongo_Thread::cancelWork() {
	QMutexLocker locker(&semaphore);
	work_queue.clear();
	condition.wakeOne();
}