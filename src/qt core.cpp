#include "qt core.h"

Linear_Layout::Linear_Layout(const bool& Vertical, const int& Margins) :
	QBoxLayout(Vertical ? QBoxLayout::TopToBottom : QBoxLayout::LeftToRight)
{
	setAlignment(Vertical ? Qt::AlignTop : Qt::AlignLeft);
	setContentsMargins(Margins, Margins, Margins, Margins);
	setSpacing(1);
}

void Linear_Layout::clear() {
	while (count() > 0) {
		itemAt(0)->widget()->hide();
		itemAt(0)->widget()->deleteLater();
	}
}

Linear_Contents::Linear_Contents(const bool& Vertical, const int& Margins) :
	QWidget()
{
	setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
	setContentsMargins(0, 0, 0, 0);
	layout = new Linear_Layout(Vertical, Margins);
	setLayout(layout);
}

void Linear_Contents::addWidget(QWidget* widget) {
	layout->addWidget(widget);
}

void Linear_Contents::clear() {
	layout->clear();
}

List::List(const bool& i_icon_mode) :
	QListWidget()
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setMovement(QListView::Movement::Static);
	verticalScrollBar()->setSingleStep(50);
	setResizeMode(QListView::Adjust);
	setContentsMargins(0, 0, 0, 0);
	setUniformItemSizes(true);
	setDragEnabled(false);
	setSpacing(0);

	if (i_icon_mode) {
		setIconSize(QSize(256, 256));
		setViewMode(QListWidget::ViewMode::IconMode);
		setSelectionMode(QListWidget::SelectionMode::ExtendedSelection);
	}
}

bool List::itemExists(const QString& i_label) const {
	for (int i = 0; i < count(); ++i) {
		QListWidgetItem* item = this->item(i);
		if (item->text() == i_label) {
			return true;
		}
	}
	return false;
}

int List::itemRow(const QString& i_label) const {
	for (int i = 0; i < count(); ++i) {
		QListWidgetItem* item = this->item(i);
		if (item->text() == i_label) {
			return row(item);
		}
	}
	return -1;
}

vector<int> List::visibleRange(const int& i_padding) const {
	int a = indexAt(viewport()->rect().topLeft()    + QPoint(10,  10)).row();
	int b = indexAt(viewport()->rect().bottomLeft() + QPoint(10, -10)).row();
	if (a < 0) a = 0;
	if (b < 0) b = count();

	if (b + i_padding > count()) b = count();
	else b += i_padding;
	if (a < i_padding) a = 0;
	else a -= i_padding;

	vector<int> range;
	for (int i = a; i < b; ++i)
		range.push_back(i);

	return range;
}

Splitter::Splitter(const bool& i_vertical) {
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
	i_vertical ? setOrientation(Qt::Orientation::Vertical) : setOrientation(Qt::Orientation::Horizontal);
}

Tabs::Tabs() :
	QTabWidget()
{
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
}

Search_Bar::Search_Bar() :
	QLineEdit()
{
	setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
}

Label::Label(const QString& i_label) :
	QLabel(i_label)
{
	setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
	setAlignment(Qt::AlignmentFlag::AlignCenter);
}

void Label::setFontSize(const int& size) {
	setStyleSheet(styleSheet() + "font-size: " + QString::fromStdString(to_string(size) + "px"));
}

Form::Form() {
	layout = new QFormLayout();
	setContentsMargins(0, 0, 0, 0);
	setLayout(layout);
}

Button::Button() {
	setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
}

Button::Button(const QString& i_label) :
	QPushButton(i_label)
{
	setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
}

void Button::setFontSize(const int& size) {
	setStyleSheet(styleSheet() + "font-size: " + QString::fromStdString(to_string(size) + "px"));
}

Text_Edit::Text_Edit() {
	setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
}

Text_Stream::Text_Stream(QWidget* i_parent) : QTextBrowser(i_parent) {
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setTabStopDistance(40);
}

void Text_Stream::append(const QString& text) {
	cerr << text.toStdString() << endl;
	QTextBrowser::append(text);
}

Widget_List::Widget_List(const QString& title):
	Linear_Contents()
{
	contents = new Linear_Contents();

	scroll_area = new QScrollArea();
	scroll_area->setWidget(contents);
	scroll_area->setWidgetResizable(true);
	scroll_area->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
	scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);

	label = new Label(title);
	label->setStyleSheet(styleSheet() + "font-size: 18px; background: rgb(15,15,15); ");

	Linear_Contents::addWidget(label);
	Linear_Contents::addWidget(scroll_area);
}

void Widget_List::addWidget(QWidget* widget) {
	contents->addWidget(widget);
}