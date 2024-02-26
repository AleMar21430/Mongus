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

	if (work.type == Mongo_Type::PRODUCER_VIEW) {
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

	if (work.type == Mongo_Type::MOVIE_VIEW) {
		mongo::collection collection = database["peliculas"];
		mongocxx::pipeline pipeline;

		cerr << work.query.at("titulo") << endl;

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

	if (work.type == Mongo_Type::MOVIE_LISTINGS) {
		mongo::collection collection = database["peliculas"];
		mongocxx::pipeline pipeline;

		pipeline.match(make_document(kvp("en_cartelera", true)));

		pipeline.lookup(make_document(
			kvp("from", "actores"),
			kvp("localField", "actores"),
			kvp("foreignField", "_id"),
			kvp("as", "actores_detalle")
		));

		pipeline.lookup(make_document(
			kvp("from", "premios"),
			kvp("localField", "premios"),
			kvp("foreignField", "_id"),
			kvp("as", "premios_detalle")
		));

		pipeline.lookup(make_document(
			kvp("from", "resenas"),
			kvp("localField", "resenas"),
			kvp("foreignField", "_id"),
			kvp("as", "resenas_detalle")
		));

		pipeline.lookup(make_document(
			kvp("from", "staff_produccion"),
			kvp("localField", "staff_produccion"),
			kvp("foreignField", "_id"),
			kvp("as", "staff_produccion_detalle")
		));

		pipeline.project(make_document(
			kvp("titulo", 1),
			kvp("anio_lanzamiento", 1),
			kvp("actores_detalle", make_document(
				kvp("nombre", 1),
				kvp("fecha_nacimiento", 1),
				kvp("nacionalidad", 1)
			)),
			kvp("premios_detalle", make_document(
				kvp("nombre_premio", 1),
				kvp("anno", 1),
				kvp("categoria", 1)
			)),
			kvp("resenas_detalle", make_document(
				kvp("comentario", 1),
				kvp("calificacion", 1),
				kvp("fecha_resena", 1)
			)),
			kvp("staff_produccion_detalle", make_document(
				kvp("nombre", 1),
				kvp("cargo", 1)
			))
		));

		mongocxx::cursor mongo_result = collection.aggregate(pipeline);

		json json_data;
		for (const bsoncxx::v_noabi::document::view& doc : mongo_result) {
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