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
};

class List : public QListWidget {
	Q_OBJECT
public:
	List(const bool& i_vertical = true);

	bool itemExists(const QString& i_label) const;
	int itemRow(const QString& i_label) const;
	vector<int> visibleRange(const int& Padding = 14) const;
};

class BranchDelegate : public QStyledItemDelegate {
	Q_OBJECT
public:
	BranchDelegate(QObject* parent = nullptr);

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
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

struct Tree_Item : QTreeWidgetItem {
	Tree_Item() {
		setWhatsThis(0, "Parent");
	};
	Tree_Item(const QString& i_label, const QString& i_color, const QString& i_tooltip, const QString& i_metadata = "", Tree * i_parent = nullptr);
	Tree_Item(const QString& i_label, const QString& i_color, const QString& i_tooltip, const QString& i_metadata, Tree_Item* i_parent);
	Tree_Item(const QString& i_label, const QString& i_color, const QString& i_tooltip, const uint32_t& i_metadata, Tree* i_parent = nullptr);
	Tree_Item(const QString& i_label, const QString& i_color, const QString& i_tooltip, const uint32_t& i_metadata, Tree_Item* i_parent);


	void clear();
};

struct Tree_Filter_Item : Tree_Item {
	Tree_Filter_Item() {
		setWhatsThis(0, "Filter");
	};
	Tree_Filter_Item(const QString& i_label, const QString& i_color, const Qt::CheckState& i_state, const QString& i_tooltip, const QString& i_metadata = "", Tree* i_parent = nullptr);
	Tree_Filter_Item(const QString& i_label, const QString& i_color, const Qt::CheckState& i_state, const QString& i_tooltip, const QString& i_metadata, Tree_Item* i_parent);
	Tree_Filter_Item(const QString& i_label, const QString& i_color, const Qt::CheckState& i_state, const QString& i_tooltip, const uint32_t& i_metadata, Tree* i_parent = nullptr);
	Tree_Filter_Item(const QString& i_label, const QString& i_color, const Qt::CheckState& i_state, const QString& i_tooltip, const uint32_t& i_metadata, Tree_Item* i_parent);
};

class Tree : public QTreeWidget {
	Q_OBJECT
public:
	bool expand_active;
	bool filter_active;
	QTreeWidgetItem* item_state;

	QList<QTreeWidgetItem*> selected_list;

	Tree();

	void clearChildren();

	int count() const;

	vector<QString> getCheckedStates() const;
	vector<QString> getUnCheckedStates() const;

	vector<QTreeWidgetItem*> topLevelItems() const;
	void modifyFilters(const bool& i_inverted, QTreeWidgetItem* i_item);

	void mousePressEvent  (QMouseEvent* i_event) override;
	void mouseMoveEvent   (QMouseEvent* i_event) override;
	void mouseReleaseEvent(QMouseEvent* i_event) override;

signals:
	void onMouseRelease();
};

class FullscreenImageViewer : public QGraphicsView {
	Q_OBJECT
public:
	QGraphicsScene* image_scene;
	QString current_path;
	bool panning;
	QPointF last_pos;
	qreal currentScale;
	bool& fullscreen;

	FullscreenImageViewer(bool& i_make_fullscreen) :
		QGraphicsView(),
		fullscreen(i_make_fullscreen)
	{
		fullscreen = true;
		setViewportUpdateMode(QGraphicsView::ViewportUpdateMode::FullViewportUpdate);
		setAttribute(Qt::WidgetAttribute::WA_NativeWindow);
		setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setResizeAnchor(QGraphicsView::AnchorUnderMouse);

		setRenderHint(QPainter::RenderHint::LosslessImageRendering);
		setRenderHint(QPainter::RenderHint::SmoothPixmapTransform);
		setRenderHint(QPainter::RenderHint::Antialiasing);

		setContentsMargins(0, 0, 0, 0);

		image_scene = new QGraphicsScene(this);
		setScene(image_scene);
		setMouseTracking(true);

		panning = false;
		currentScale = 1;
	}

	void quit() {
		fullscreen = false;
		hide();
		delete this;
	}

	void wheelEvent(QWheelEvent* event) override {
		qreal zoomFactor = 1.25;

		QPointF oldPos = mapToScene(event->position().toPoint());
		if (event->angleDelta().y() > 0) {
			scale(zoomFactor, zoomFactor);
			currentScale *= zoomFactor;
		}
		else if (currentScale > 0.1) {
			scale(1 / zoomFactor, 1 / zoomFactor);
			currentScale /= zoomFactor;
		}

		QPointF newPos = mapToScene(event->position().toPoint());
		QPointF delta = newPos - oldPos;
		translate(delta.x(), delta.y());
	}

	void mousePressEvent(QMouseEvent* event) override {
		panning = true;
		last_pos = event->pos();
	}

	void mouseDoubleClickEvent(QMouseEvent* event) override {
		quit();
	}

	void mouseReleaseEvent(QMouseEvent* event) override {
		panning = false;
	}

	void mouseMoveEvent(QMouseEvent* event) override {
		if (panning) {
			QPointF delta = event->pos() - last_pos;
			horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
			verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
			last_pos = event->pos();
		}
		else {
			QGraphicsView::mouseMoveEvent(event);
		}
	}

	void keyPressEvent(QKeyEvent* i_event) override {
		if (i_event->key() == Qt::Key_F11 || i_event->key() == Qt::Key_Escape) {
			quit();
		}
		else if (i_event->key() == Qt::Key_F) {
			fit();
		}
		QGraphicsView::keyPressEvent(i_event);
	}

	void fit() {
		fitInView(scene()->items()[1]->boundingRect(), Qt::KeepAspectRatio);
		centerOn(scene()->items()[1]->boundingRect().center());
		currentScale = 1;
	}

	void setImageOrVideo(const QString& path) {
		current_path = path;
		image_scene->clear();

		QGraphicsPixmapItem* graphicsImage;

		if (!current_path.endsWith(".mp4")) {
			QImageReader reader(current_path);
			QPixmap image = QPixmap::fromImageReader(&reader);
			graphicsImage = new QGraphicsPixmapItem(image);
			graphicsImage->setTransformationMode(Qt::SmoothTransformation);
			image_scene->addItem(graphicsImage);
		}
		QGraphicsRectItem* bg = new QGraphicsRectItem(-80000, -45000, 160000, 90000);
		bg->setBrush(Qt::NoBrush);
		image_scene->addItem(bg);
		if (!current_path.endsWith(".mp4"))
			fit();
	}

	void setImageOrVideo(const QPixmap& image) {
		image_scene->clear();

		QGraphicsPixmapItem* graphicsImage;

		if (!current_path.endsWith(".mp4")) {
			graphicsImage = new QGraphicsPixmapItem(image);
			graphicsImage->setTransformationMode(Qt::SmoothTransformation);
			image_scene->addItem(graphicsImage);
		}
		QGraphicsRectItem* bg = new QGraphicsRectItem(-80000, -45000, 160000, 90000);
		bg->setBrush(Qt::NoBrush);
		image_scene->addItem(bg);
		if (!current_path.endsWith(".mp4"))
			fit();
	}
};