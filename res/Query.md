Convert the follwoing mongo aggregation pipeline to c++ mongocxx code utilizing the .lookup(), .match(), .group(), etc.   and using make_document() and kvp()


# MOVIE_VIEW
```json
[
	{
		"$lookup": {
			"from": "actores",
			"localField": "actores",
			"foreignField": "_id",
			"as": "actores_detalle"
		}
	},
	{
		"$lookup": {
			"from": "staff_produccion",
			"localField": "staff_produccion",
			"foreignField": "_id",
			"as": "staff_produccion_detalle"
		}
	},
	{
		"$project": {
			"titulo": 1,
			"anio_lanzamiento": 1,
			"actores_detalle.nombre": 1,
			"actores_detalle.fecha_nacimiento": 1,
			"actores_detalle.nacionalidad": 1,
			"staff_produccion_detalle.nombre": 1,
			"staff_produccion_detalle.cargo": 1
		}
	}
]
```
```cpp
mongocxx::pipeline pipeline;

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

pipeline.project(make_document(
	kvp("titulo", 1),
	kvp("anio_lanzamiento", 1),
	kvp("actores_detalle.nombre", 1),
	kvp("actores_detalle.fecha_nacimiento", 1),
	kvp("actores_detalle.nacionalidad", 1),
	kvp("staff_produccion_detalle.nombre", 1),
	kvp("staff_produccion_detalle.cargo", 1)
));
```

# MOVIE_LISTINGS
```json
[
	{
		"$match": {
			"en_cartelera": true
		}
	},
	{
		"$lookup": {
			"from": "actores",
			"localField": "actores",
			"foreignField": "_id",
			"as": "actores_detalle"
		}
	},
	{
		"$lookup": {
			"from": "premios",
			"localField": "premios",
			"foreignField": "_id",
			"as": "premios_detalle"
		}
	},
	{
		"$lookup": {
			"from": "resenas",
			"localField": "resenas",
			"foreignField": "_id",
			"as": "resenas_detalle"
		}
	},
	{
		"$lookup": {
			"from": "staff_produccion",
			"localField": "staff_produccion",
			"foreignField": "_id",
			"as": "staff_produccion_detalle"
		}
	},
	{
		"$project": {
			"titulo": 1,
			"anio_lanzamiento": 1,
			"actores_detalle": {
				"nombre": 1,
				"fecha_nacimiento": 1,
				"nacionalidad": 1
			},
			"premios_detalle": {
				"nombre_premio": 1,
				"anno": 1,
				"categoria": 1
			},
			"resenas_detalle": {
				"comentario": 1,
				"calificacion": 1,
				"fecha_resena": 1
			},
			"staff_produccion_detalle": {
				"nombre": 1,
				"cargo": 1
			}
		}
	}
]
```
```cpp
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
```

# PRODUCER_VIEW
```json
[
	{
		"$lookup": {
			"from": "peliculas",
			"localField": "_id",
			"foreignField": "casa_productora",
			"as": "peliculas_detalle"
		}
	},
	{
		"$project": {
			"nombre": 1,
			"pais": 1,
			"peliculas_detalle.titulo": 1,
			"peliculas_detalle.anio_lanzamiento": 1
		}
	}
]
```
```cpp
mongocxx::pipeline pipeline;

pipeline.lookup({
	make_document(
		kvp("from", "peliculas"),
		kvp("localField", "_id"),
		kvp("foreignField", "casa_productora"),
		kvp("as", "peliculas_detalle")
	)
});

pipeline.project({
	make_document(
		kvp("nombre", 1),
		kvp("pais", 1),
		kvp("peliculas_detalle.titulo", 1),
		kvp("peliculas_detalle.anio_lanzamiento", 1)
	)
});
```

# Query de ordenamiento:
### Ordenamiento ascendente
```json
[
	{
		"$lookup": {
			"from": "actores",
			"localField": "actores",
			"foreignField": "_id",
			"as": "actores_detalle"
		}
	},
	{
		"$lookup": {
			"from": "staff_produccion",
			"localField": "staff_produccion",
			"foreignField": "_id",
			"as": "staff_produccion_detalle"
		}
	},
	{
		"$project": {
			"titulo": 1,
			"anio_lanzamiento": 1,
			"actores_detalle.nombre": 1,
			"actores_detalle.fecha_nacimiento": 1,
			"actores_detalle.nacionalidad": 1,
			"staff_produccion_detalle.nombre": 1,
			"staff_produccion_detalle.cargo": 1
		}
	},
	{
		"$sort": {
			"anio_lanzamiento": 1
		}
	}
]
```
```cpp
mongocxx::pipeline pipeline;

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

pipeline.project(make_document(
	kvp("titulo", 1),
	kvp("anio_lanzamiento", 1),
	kvp("actores_detalle.nombre", 1),
	kvp("actores_detalle.fecha_nacimiento", 1),
	kvp("actores_detalle.nacionalidad", 1),
	kvp("staff_produccion_detalle.nombre", 1),
	kvp("staff_produccion_detalle.cargo", 1)
));

pipeline.sort(make_document(
	kvp("anio_lanzamiento", 1)
));
```

### Ordenamiento Descendente
```json
[
	{
		"$lookup": {
			"from": "actores",
			"localField": "actores",
			"foreignField": "_id",
			"as": "actores_detalle"
		}
	},
	{
		"$lookup": {
			"from": "staff_produccion",
			"localField": "staff_produccion",
			"foreignField": "_id",
			"as": "staff_produccion_detalle"
		}
	},
	{
		"$project": {
			"titulo": 1,
			"anio_lanzamiento": 1,
			"actores_detalle.nombre": 1,
			"actores_detalle.fecha_nacimiento": 1,
			"actores_detalle.nacionalidad": 1,
			"staff_produccion_detalle.nombre": 1,
			"staff_produccion_detalle.cargo": 1
		}
	},
	{
		"$sort": {
			"anio_lanzamiento": -1
		}
	}
]
```
```cpp
mongocxx::pipeline pipeline;

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

pipeline.project(make_document(
	kvp("titulo", 1),
	kvp("anio_lanzamiento", 1),
	kvp("actores_detalle.nombre", 1),
	kvp("actores_detalle.fecha_nacimiento", 1),
	kvp("actores_detalle.nacionalidad", 1),
	kvp("staff_produccion_detalle.nombre", 1),
	kvp("staff_produccion_detalle.cargo", 1)
));

pipeline.sort(make_document(
	kvp("anio_lanzamiento", -1)
));
```

# AVG_SCORE_BY_AGE_GROUPS
```json
[
	{ $group: { _id: "$clasificacion_edad", promedio_calificacion: { $avg: "$resenas.calificacion"
			}
		}
	}
]
```
```cpp
mongocxx::pipeline pipeline;

pipeline.group(make_document(
	kvp("_id", "$clasificacion_edad"),
	kvp("promedio_calificacion", make_document(kvp("$avg", "$resenas.calificacion")))
));
```

# GENRE_FILTER_MOVIE_COUNT
```json
[
	{ $lookup: { from: "generos_cinematograficos", localField: "genero", foreignField: "_id", as: "genero_info"
		}
	},
	{ $unwind: "$genero_info"
	},
	{ $group: { _id: "$genero_info.nombre_genero", cantidad: { $sum: 1
			}
		}
	},
	{ $sort: { cantidad: -1
		}
	}
]
```
```cpp
mongocxx::pipeline pipeline;

pipeline.lookup(
	"generos_cinematograficos",
	"$genero",
	"_id",
	"genero_info"
);

pipeline.unwind("$genero_info");

pipeline.group(make_document(
	kvp("_id", "$genero_info.nombre_genero"),
	kvp("cantidad", make_document(kvp("$sum", 1)))
));

pipeline.sort(make_document(kvp("cantidad", -1)));
```

# ACTOR_VIEW
```json
[
	{
		"$lookup": {
			"from": "peliculas",
			"localField": "peliculas_participadas",
			"foreignField": "_id",
			"as": "peliculas_participadas"
		}
	},
	{
		"$project": {
			"nombre": 1,
			"peliculas_participadas.titulo": 1
		}
	}
]
```
```cpp
mongocxx::pipeline pipeline;

pipeline.lookup(
	"peliculas",
	"peliculas_participadas",
	"_id",
	"peliculas_participadas"
);

pipeline.project(make_document(
		kvp("nombre", 1),
		kvp("peliculas_participadas.titulo", 1)
));
```