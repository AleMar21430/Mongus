#include "threading.h"

void Async_Thread::run() {
	if (work.type == Async_Type::UNKNOWN) {
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

using namespace bsoncxx::builder::basic;

void Mongo_Thread::processWork(const Mongo_Query& work) {
	emit logMsg("Mongo Thread Processing");
	switch (work.type) {
	case Mongo_Type::PRODUCERS:
		PRODUCERS(work);
		break;
	case Mongo_Type::SHOWINGS:
		SHOWINGS(work);
		break;
	case Mongo_Type::MOVIES:
		MOVIES(work);
		break;
	case Mongo_Type::ACTORS:
		ACTORS(work);
		break;
	case Mongo_Type::STAFFS:
		STAFFS(work);
		break;
	case Mongo_Type::USERS:
		USERS(work);
		break;
	case Mongo_Type::PRODUCER:
		PRODUCER(work);
		break;
	case Mongo_Type::SHOWING:
		SHOWING(work);
		break;
	case Mongo_Type::MOVIE:
		MOVIE(work);
		break;
	case Mongo_Type::ACTOR:
		ACTOR(work);
		break;
	case Mongo_Type::STAFF:
		STAFF(work);
		break;
	case Mongo_Type::USER:
		USER(work);
		break;
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

void Mongo_Thread::PRODUCERS(const Mongo_Query& work) {
	mongo::collection collection = database["casas_productoras"];
	mongocxx::pipeline pipeline;
	mongocxx::cursor mongo_result = collection.aggregate(pipeline);

	json json_data;
	for (const bsoncxx::v_noabi::document::view& doc : mongo_result) {
		json_data.push_back(json::parse(bsoncxx::to_json(doc)));
	}
	emit result(work, json_data);
}
void Mongo_Thread::SHOWINGS(const Mongo_Query& work) {
	mongo::collection collection = database["proyecciones"];
	mongocxx::pipeline pipeline;
	mongocxx::cursor mongo_result = collection.aggregate(pipeline);

	json json_data;
	for (const bsoncxx::v_noabi::document::view& doc : mongo_result) {
		json_data.push_back(json::parse(bsoncxx::to_json(doc)));
	}
	emit result(work, json_data);
}
void Mongo_Thread::MOVIES(const Mongo_Query& work) {
	mongo::collection collection = database["peliculas"];
	mongocxx::pipeline pipeline;

	mongocxx::cursor mongo_result = collection.aggregate(pipeline);

	json json_data;
	for (const bsoncxx::v_noabi::document::view& doc : mongo_result) {
		json_data.push_back(json::parse(bsoncxx::to_json(doc)));
	}
	emit result(work, json_data);
}
void Mongo_Thread::ACTORS(const Mongo_Query& work) {
	mongo::collection collection = database["actores"];
	mongocxx::pipeline pipeline;
	mongocxx::cursor mongo_result = collection.aggregate(pipeline);

	json json_data;
	for (const bsoncxx::v_noabi::document::view& doc : mongo_result) {
		json_data.push_back(json::parse(bsoncxx::to_json(doc)));
	}
	emit result(work, json_data);
}
void Mongo_Thread::STAFFS(const Mongo_Query& work) {
	mongo::collection collection = database["staff_produccion"];
	mongocxx::pipeline pipeline;
	mongocxx::cursor mongo_result = collection.aggregate(pipeline);

	json json_data;
	for (const bsoncxx::v_noabi::document::view& doc : mongo_result) {
		json_data.push_back(json::parse(bsoncxx::to_json(doc)));
	}
	emit result(work, json_data);
}
void Mongo_Thread::USERS(const Mongo_Query& work) {
	mongo::collection collection = database["usuarios"];
	mongocxx::pipeline pipeline;
	mongocxx::cursor mongo_result = collection.aggregate(pipeline);

	json json_data;
	for (const bsoncxx::v_noabi::document::view& doc : mongo_result) {
		json_data.push_back(json::parse(bsoncxx::to_json(doc)));
	}
	emit result(work, json_data);
}

void Mongo_Thread::PRODUCER(const Mongo_Query& work) {
	mongo::collection collection = database["casas_productoras"];
	mongocxx::pipeline pipeline;

	pipeline.match(make_document(kvp("nombre", work.query.at("nombre"))));

	pipeline.lookup(make_document(
		kvp("from", "peliculas"),
		kvp("localField", "_id"),
		kvp("foreignField", "casa_productora"),
		kvp("as", "peliculas_detalle")
	));

	pipeline.project(make_document(
		kvp("nombre", 1),
		kvp("pais", 1),
		kvp("peliculas_detalle.titulo", 1),
		kvp("peliculas_detalle.anio_lanzamiento", 1),
		kvp("_id", 0)
	));

	mongocxx::cursor mongo_result = collection.aggregate(pipeline);

	json json_data;
	for (const bsoncxx::v_noabi::document::view& doc : mongo_result) {
		json_data.push_back(json::parse(bsoncxx::to_json(doc)));
	}
	emit result(work, json_data);
}
void Mongo_Thread::SHOWING(const Mongo_Query& work) {
	mongo::collection collection = database["peliculas"];
	mongocxx::pipeline pipeline;
	mongocxx::cursor mongo_result = collection.aggregate(pipeline);

	json json_data;
	for (const bsoncxx::v_noabi::document::view& doc : mongo_result) {
		json_data.push_back(json::parse(bsoncxx::to_json(doc)));
	}
	emit result(work, json_data);
}
void Mongo_Thread::MOVIE(const Mongo_Query& work) {
	mongo::collection collection = database["peliculas"];
	mongocxx::pipeline pipeline;

	pipeline.match(make_document(kvp("titulo", work.query.at("titulo"))));

	pipeline.lookup(make_document(
		kvp("from", "actores"),
		kvp("localField", "actores"),
		kvp("foreignField", "_id"),
		kvp("as", "actores_detalle")
	));

	pipeline.lookup(make_document(
		kvp("from", "staff_produccion"),
		kvp("localField", "staff_produccion"),
		kvp("foreignField", "_id"),
		kvp("as", "staff_produccion_detalle")
	));

	pipeline.lookup(make_document(
		kvp("from", "generos_cinematograficos"),
		kvp("localField", "genero"),
		kvp("foreignField", "_id"),
		kvp("as", "genero_detalle")
	));

	pipeline.lookup(make_document(
		kvp("from", "taquilla"),
		kvp("localField", "taquilla"),
		kvp("foreignField", "_id"),
		kvp("as", "taquilla_detalle")
	));

	pipeline.lookup(make_document(
		kvp("from", "resenas"),
		kvp("localField", "resenas"),
		kvp("foreignField", "_id"),
		kvp("as", "resenas_detalle")
	));

	pipeline.lookup(make_document(
		kvp("from", "premios"),
		kvp("localField", "premios"),
		kvp("foreignField", "_id"),
		kvp("as", "premios_detalle")
	));

	pipeline.lookup(make_document(
		kvp("from", "casas_productoras"),
		kvp("localField", "casa_productora"),
		kvp("foreignField", "_id"),
		kvp("as", "casa_productora_detalle")
	));

	pipeline.project(make_document(
		kvp("_id", 1),
		kvp("titulo", 1),
		kvp("genero_detalle", 1),
		kvp("director", 1),
		kvp("anio_lanzamiento", 1),
		kvp("sinopsis", 1),
		kvp("clasificacion_edad", 1),
		kvp("actores_detalle", 1),
		kvp("taquilla_detalle", 1),
		kvp("resenas_detalle", 1),
		kvp("premios_detalle", 1),
		kvp("staff_produccion_detalle", 1),
		kvp("casa_productora_detalle", 1),
		kvp("en_cartelera", 1)
	));

	mongocxx::cursor mongo_result = collection.aggregate(pipeline);

	json json_data;
	for (const bsoncxx::v_noabi::document::view& doc : mongo_result) {
		json_data.push_back(json::parse(bsoncxx::to_json(doc)));
	}
	emit result(work, json_data);
}
void Mongo_Thread::ACTOR(const Mongo_Query& work) {
	mongo::collection collection = database["peliculas"];
	mongocxx::pipeline pipeline;
	mongocxx::cursor mongo_result = collection.aggregate(pipeline);

	json json_data;
	for (const bsoncxx::v_noabi::document::view& doc : mongo_result) {
		json_data.push_back(json::parse(bsoncxx::to_json(doc)));
	}
	emit result(work, json_data);
}
void Mongo_Thread::STAFF(const Mongo_Query & work) {
	mongo::collection collection = database["peliculas"];
	mongocxx::pipeline pipeline;
	mongocxx::cursor mongo_result = collection.aggregate(pipeline);

	json json_data;
	for (const bsoncxx::v_noabi::document::view& doc : mongo_result) {
		json_data.push_back(json::parse(bsoncxx::to_json(doc)));
	}
	emit result(work, json_data);
}
void Mongo_Thread::USER(const Mongo_Query& work) {
	mongo::collection collection = database["peliculas"];
	mongocxx::pipeline pipeline;
	mongocxx::cursor mongo_result = collection.aggregate(pipeline);

	json json_data;
	for (const bsoncxx::v_noabi::document::view& doc : mongo_result) {
		json_data.push_back(json::parse(bsoncxx::to_json(doc)));
	}
	emit result(work, json_data);
}