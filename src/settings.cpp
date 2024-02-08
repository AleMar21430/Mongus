#include "settings.h"

#include "main window.h"

Settings::Settings(App* i_app) :
	Form(),
	app(i_app)
{
	Search_Bar* db_url = new Search_Bar();
	db_url->setPlaceholderText("db_url");
	db_url->setText(app->settings["db_url"]);

	Search_Bar* username = new Search_Bar();
	username->setPlaceholderText("username");
	username->setText(app->settings["username"]);

	Search_Bar* password = new Search_Bar();
	password->setPlaceholderText("password");
	password->setText(app->settings["password"]);

	layout->addRow("Database Url:", db_url);
	layout->addRow("Username:", username);
	layout->addRow("Password:", password);

	connect(db_url, &Search_Bar::textChanged, [this, db_url]() {
		app->settings["db_url"] = db_url->text();
	});
	connect(username, &Search_Bar::textChanged, [this, username]() {
		app->settings["username"] = username->text();
	});
	connect(password, &Search_Bar::textChanged, [this, password]() {
		app->settings["password"] = password->text();
	});
}