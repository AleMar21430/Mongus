
```python
ID[str, str] = id_ , name #ID siempre es el id de referencia y el nombre/titulo del objeto

MOVIE_VIEW params=(_id) {
	"_id": id
	"title": str
	"premiere_date": str
	"duration": int
	"score": float    # Se calcula en base a los scores de los reviews que tiene
	"budget": int
	"box_office": int
	"producer": Producer_ID
	"director": Actor_ID

	"genres": List[str]
	"cast": List[Actor_ID]
	"reviews": List[Tuple[User_ID, str, int]] #(User_ID, Review Text, Score)
}
PRODUCER_VIEW params=(_id) {
	"_id": id
	"name": str
	"movies": List[Movie_ID]
}
ACTOR_VIEW params=(_id) {
	"_id": id
	"name": str
	"age": int
	"movies": List[Movie_ID]
}
USER_VIEW params=(_id) {
	"_id": id
	"username": str
	"reviews": List[Tuple[Movie_ID, str, int]] #(Movie_ID, Review Text, Score)
}
SHOWING_VIEW params=(_id) {
	"_id": id
	"room_name": str
	"seats_sold": int
	"amount_sold": int # in $
	"movie": Movie_ID
	"show_time": date
}

MOVIE_LISTINGS: List[ID]
PRODUCER_LIST: List[ID]
ACTOR_LIST: List[ID]
USER_LIST: List[ID]
SHOWINGS_LIST: List[ID]
```