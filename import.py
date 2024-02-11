import pandas as pd
from pymongo import MongoClient

# Conectar a MongoDB Atlas
client = MongoClient("tmongodb+srv://est20853:<BqzE2nF1pib6pc8z>@proyect1.eolzon9.mongodb.net/")
db = client["Lab03"]
collection = db["movies"]

# Cargar datos desde el archivo .csv
data = pd.read_csv("movies.csv")

# Convertir el DataFrame a formato JSON
data_json = data.to_dict(orient='records')

# Insertar los datos en MongoDB Atlas
collection.insert_many(data_json)

# Cerrar la conexión
client.close()
