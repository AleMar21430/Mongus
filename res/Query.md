# MOVIE_VIEW
```json
[{"$lookup": {"from": "actores", "localField": "actores", "foreignField": "_id", "as": "actores_detalle"}}, {"$lookup": {"from": "staff_produccion", "localField": "staff_produccion", "foreignField": "_id", "as": "staff_produccion_detalle"}}, {"$project": {"titulo": 1, "anio_lanzamiento": 1, "actores_detalle.nombre": 1, "actores_detalle.fecha_nacimiento": 1, "actores_detalle.nacionalidad": 1, "staff_produccion_detalle.nombre": 1, "staff_produccion_detalle.cargo": 1}}]
```
# MOVIE_LISTINGS
```json
[{"$match": {"en_cartelera": true}}, {"$lookup": {"from": "actores", "localField": "actores", "foreignField": "_id", "as": "actores_detalle"}}, {"$lookup": {"from": "premios", "localField": "premios", "foreignField": "_id", "as": "premios_detalle"}}, {"$lookup": {"from": "resenas", "localField": "resenas", "foreignField": "_id", "as": "resenas_detalle"}}, {"$lookup": {"from": "staff_produccion", "localField": "staff_produccion", "foreignField": "_id", "as": "staff_produccion_detalle"}}, {"$project": {"titulo": 1, "anio_lanzamiento": 1, "actores_detalle": {"nombre": 1, "fecha_nacimiento": 1, "nacionalidad": 1}, "premios_detalle": {"nombre_premio": 1, "anno": 1, "categoria": 1}, "resenas_detalle": {"comentario": 1, "calificacion": 1, "fecha_resena": 1}, "staff_produccion_detalle": {"nombre": 1, "cargo": 1}}}]
```
# PRODUCER_VIEW
```json
[{"$lookup": {"from": "peliculas", "localField": "_id", "foreignField": "casa_productora", "as": "peliculas_detalle"}}, {"$project": {"nombre": 1, "pais": 1, "peliculas_detalle.titulo": 1, "peliculas_detalle.anio_lanzamiento": 1}}]
```


# Query de ordenamiento:
Ordenamiento ascendente
db.peliculas.aggregate([{"$lookup": {"from": "actores", "localField": "actores", "foreignField": "_id", "as": "actores_detalle"}}, {"$lookup": {"from": "staff_produccion", "localField": "staff_produccion", "foreignField": "_id", "as": "staff_produccion_detalle"}}, {"$project": {"titulo": 1, "anio_lanzamiento": 1, "actores_detalle.nombre": 1, "actores_detalle.fecha_nacimiento": 1, "actores_detalle.nacionalidad": 1, "staff_produccion_detalle.nombre": 1, "staff_produccion_detalle.cargo": 1}}, {"$sort": {"anio_lanzamiento": 1}}])
Ordenamiento Descendente
db.peliculas.aggregate([{"$lookup": {"from": "actores", "localField": "actores", "foreignField": "_id", "as": "actores_detalle"}}, {"$lookup": {"from": "staff_produccion", "localField": "staff_produccion", "foreignField": "_id", "as": "staff_produccion_detalle"}}, {"$project": {"titulo": 1, "anio_lanzamiento": 1, "actores_detalle.nombre": 1, "actores_detalle.fecha_nacimiento": 1, "actores_detalle.nacionalidad": 1, "staff_produccion_detalle.nombre": 1, "staff_produccion_detalle.cargo": 1}}, {"$sort": {"anio_lanzamiento": -1}}])


Obtener el promedio de calificación de las películas por clasificación de edad:
db.peliculas.aggregate([{ $group: { _id: "$clasificacion_edad", promedio_calificacion: { $avg: "$resenas.calificacion" } } }])





Cantidad de peliculas por genero
db.peliculas.aggregate([{ $lookup: { from: "generos_cinematograficos", localField: "genero", foreignField: "_id", as: "genero_info" } }, { $unwind: "$genero_info" }, { $group: { _id: "$genero_info.nombre_genero", cantidad: { $sum: 1 } } }, { $sort: { cantidad: -1 } }])


actores con las peliculas que han tenido
db.actores.aggregate([{"$lookup": {"from": "peliculas", "localField": "peliculas_participadas", "foreignField": "_id", "as": "peliculas_participadas"}}, {"$project": {"nombre": 1, "peliculas_participadas.titulo": 1}}])

