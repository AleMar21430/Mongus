#pragma once

#include "include.h"

struct Tree_Item;
struct Tree_Filter_Item;
class Tree;

class Application : public QApplication {
	Q_OBJECT
public:
};

class Button : public QPushButton {
	Q_OBJECT
public:
	Button();
	Button(const QString& i_label);

	void setFontSize(const int& size);
};

class Form : public QWidget {
	Q_OBJECT
public:
	QFormLayout* layout;

	Form();
};

class Label : public QLabel {
	Q_OBJECT
public:
	Label(const QString& i_label);

	void setFontSize(const int& size);
};

class Linear_Layout : public QBoxLayout {
	Q_OBJECT
public:
	Linear_Layout(const bool& Vertical = true, const int& Margins = 0);

	void clear();
};

class Linear_Contents : public QWidget {
	Q_OBJECT
public:
	Linear_Layout* layout;

	Linear_Contents(const bool& Vertical = true, const int& Margins = 0);

	void addWidget(QWidget* widget);
	void clear();
};

class List : public QListWidget {
	Q_OBJECT
public:
	List(const bool& i_icon_mode);

	bool itemExists(const QString& i_label) const;
	int itemRow(const QString& i_label) const;
	vector<int> visibleRange(const int& Padding = 14) const;
};

class Search_Bar : public QLineEdit {
	Q_OBJECT
public:
	Search_Bar();
};

class Text_Edit : public QPlainTextEdit {
	Q_OBJECT
public:
	Text_Edit();
};

class Widget_List : public Linear_Contents {
	Q_OBJECT
public:
	QScrollArea* scroll_area;
	Label* label;
	Linear_Contents* contents;

	Widget_List(const QString& title = "");

	void addWidget(QWidget* widget);
};

class Splitter : public QSplitter {
	Q_OBJECT
public:
	Splitter(const bool& i_vertical = false);
};

class Tabs : public QTabWidget {
	Q_OBJECT
public:
	Tabs();
};

class Text_Stream : public QTextBrowser {
	Q_OBJECT
public:
	Text_Stream(QWidget* i_parent = nullptr);

	void append(const QString& text);
};