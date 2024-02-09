import pandas as pd
import json

data = pd.read_csv("anime.csv")

data = data[data["type"] == "Movie"]
print(data.head(1))

data = data.drop(columns = ["anime_id","title_synonyms","airing","aired_string","aired","episodes","status","related"])
data = data.fillna("Not available.")
#[anime_id, title, title_english, title_japanese, title_synonyms, image_url, type, source, episodes, status, airing, aired_string, aired, duration, rating, score, scored_by, rank, popularity, members, favorites, background, premiered, broadcast, related, producer, licensor, studio, genre, opening_theme, ending_theme]
json_data = []
for index, row in data.iterrows():
	json_data.append(
		{
			"title": row.title,
			"image_url" : row.image_url.replace("myanimelist.cdn-dena.com", "cdn.myanimelist.net"),
			"title_english" : row.title_english,
			"title_japanese" : row.title_japanese,
			"premiere": row.premiered,
			"duration": row.duration,
			"studio": row.studio,
			"producer": row.producer,
			"licensor": row.licensor,
			"rating": row.rating,
			"genre": row.genre,

			"members": row.members,
			"score": row.score,
			"scored_by": row.scored_by,
			"popularity": row.popularity,
			"favorites": row.favorites,
		}
	)


open('movies.json', 'w', -1, "utf-8").write(json.dumps(json_data))