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
	setContentsMargins(0, 0, 0, 0);
	layout = new Linear_Layout(Vertical, Margins);
	setLayout(layout);
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

Tree_Item::Tree_Item(const QString& i_label, const QString& i_color, const QString& i_tooltip, const QString& i_metadata, Tree* i_parent) :
	QTreeWidgetItem(i_parent)
{
	const QStringList colorValues = i_color.split(" ", Qt::SplitBehaviorFlags::SkipEmptyParts);
	setForeground(0, QColor(colorValues[0].toInt(), colorValues[1].toInt(), colorValues[2].toInt()));
	setToolTip(0, i_tooltip);
	setText(0, i_label);
	setWhatsThis(0, "Parent");

	setForeground(1, QColor(140, 140, 140));
	setText(1, i_metadata);
}

Tree_Item::Tree_Item(const QString& i_label, const QString& i_color, const QString& i_tooltip, const uint32_t& i_metadata, Tree* i_parent) :
	QTreeWidgetItem(i_parent)
{
	const QStringList colorValues = i_color.split(" ", Qt::SplitBehaviorFlags::SkipEmptyParts);
	setForeground(0, QColor(colorValues[0].toInt(), colorValues[1].toInt(), colorValues[2].toInt()));
	setToolTip(0, i_tooltip);
	setText(0, i_label);
	setWhatsThis(0, "Parent");

	setForeground(1, QColor(140, 140, 140));
	setText(1, QString::fromStdString(to_string(i_metadata)));
}

Tree_Item::Tree_Item(const QString& i_label, const QString& i_color, const QString& i_tooltip, const QString& i_metadata, Tree_Item* i_parent) :
	QTreeWidgetItem(i_parent)
{
	const QStringList colorValues = i_color.split(" ", Qt::SplitBehaviorFlags::SkipEmptyParts);
	setForeground(0, QColor(colorValues[0].toInt(), colorValues[1].toInt(), colorValues[2].toInt()));
	setToolTip(0, i_tooltip);
	setText(0, i_label);
	setWhatsThis(0, "Parent");

	setForeground(1, QColor(140, 140, 140));
	setText(1, i_metadata);
}

Tree_Item::Tree_Item(const QString& i_label, const QString& i_color, const QString& i_tooltip, const uint32_t& i_metadata, Tree_Item* i_parent) :
	QTreeWidgetItem(i_parent)
{
	const QStringList colorValues = i_color.split(" ", Qt::SplitBehaviorFlags::SkipEmptyParts);
	setForeground(0, QColor(colorValues[0].toInt(), colorValues[1].toInt(), colorValues[2].toInt()));
	setToolTip(0, i_tooltip);
	setText(0, i_label);
	setWhatsThis(0, "Parent");

	setForeground(1, QColor(140, 140, 140));
	setText(1, QString::fromStdString(to_string(i_metadata)));
}

void Tree_Item::clear() {
	while (childCount() > 0)
		takeChild(0);
}

Tree_Filter_Item::Tree_Filter_Item(const QString& i_label, const QString& i_color, const Qt::CheckState& i_state, const QString& i_tooltip, const QString& i_metadata, Tree* i_parent) :
	Tree_Item(i_label, i_color, i_tooltip, i_metadata, i_parent)
{
	setFlags(flags() | Qt::ItemFlag::ItemIsUserCheckable | Qt::ItemFlag::ItemIsUserTristate);
	setCheckState(0, i_state);
	setWhatsThis(0, "Filter");
}

Tree_Filter_Item::Tree_Filter_Item(const QString& i_label, const QString& i_color, const Qt::CheckState& i_state, const QString& i_tooltip, const uint32_t& i_metadata, Tree* i_parent) :
	Tree_Item(i_label, i_color, i_tooltip, i_metadata, i_parent)
{
	setFlags(flags() | Qt::ItemFlag::ItemIsUserCheckable | Qt::ItemFlag::ItemIsUserTristate);
	setCheckState(0, i_state);
	setWhatsThis(0, "Filter");
}

Tree_Filter_Item::Tree_Filter_Item(const QString& i_label, const QString& i_color, const Qt::CheckState& i_state, const QString& i_tooltip, const QString& i_metadata, Tree_Item* i_parent) :
	Tree_Item(i_label, i_color, i_tooltip, i_metadata, i_parent)
{
	setFlags(flags() | Qt::ItemFlag::ItemIsUserCheckable | Qt::ItemFlag::ItemIsUserTristate);
	setCheckState(0, i_state);
	setWhatsThis(0, "Filter");
}

Tree_Filter_Item::Tree_Filter_Item(const QString& i_label, const QString& i_color, const Qt::CheckState& i_state, const QString& i_tooltip, const uint32_t& i_metadata, Tree_Item* i_parent) :
	Tree_Item(i_label, i_color, i_tooltip, i_metadata, i_parent)
{
	setFlags(flags() | Qt::ItemFlag::ItemIsUserCheckable | Qt::ItemFlag::ItemIsUserTristate);
	setCheckState(0, i_state);
	setWhatsThis(0, "Filter");
}

Tree::Tree() :
	QTreeWidget()
{
	setContentsMargins(0, 0, 0, 0);
	setSelectionMode(QTreeWidget::SelectionMode::SingleSelection);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setItemDelegate(new BranchDelegate(this));
	verticalScrollBar()->setSingleStep(10);
	setAllColumnsShowFocus(true);
	setHeaderHidden(true);
	setIndentation(0);
	setColumnCount(1);
	
	header()->setStretchLastSection(false);
	header()->setSectionResizeMode(0, QHeaderView::ResizeMode::Stretch);
	header()->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);

	filter_active = false;
	expand_active = false;
	item_state = nullptr;
}

vector<QString> Tree::getCheckedStates() const {
	vector<QString> labels;
	for (int i = 0; i < topLevelItemCount(); ++i) {
		for (int j = 0; j < topLevelItem(i)->childCount(); ++j) {
			QTreeWidgetItem* item = topLevelItem(i)->child(j);
			if (item && item->whatsThis(0) == "Filter" && item->checkState(0) == Qt::CheckState::Checked) {
				labels.push_back(item->text(0));
			}
		}
	}
	return labels;
}

vector<QString> Tree::getUnCheckedStates() const {
	vector<QString> labels;
	for (int i = 0; i < topLevelItemCount(); ++i) {
		for (int j = 0; j < topLevelItem(i)->childCount(); ++j) {
			QTreeWidgetItem* item = topLevelItem(i)->child(j);
			if (item && item->whatsThis(0) == "Filter" && item->checkState(0) == Qt::CheckState::PartiallyChecked) {
				labels.push_back(item->text(0));
			}
		}
	}
	return labels;
}

int Tree::count() const {
	int count = topLevelItemCount();
	for (int i = 0; i < topLevelItemCount(); ++i) {
		count += topLevelItem(i)->childCount();
	}
	return count;
}

vector<QTreeWidgetItem*> Tree::topLevelItems() const {
	vector<QTreeWidgetItem*> items;
	for (int i = 0; i < topLevelItemCount(); ++i) {
		items.push_back(topLevelItem(i));
	}
	return items;
}

void Tree::modifyFilters(const bool& i_inverted, QTreeWidgetItem* i_item) {
	if (i_inverted) {
		if (i_item->checkState(0) == Qt::Unchecked)
			i_item->setCheckState(0, Qt::PartiallyChecked);
		else if (i_item->checkState(0) == Qt::Checked)
			i_item->setCheckState(0, Qt::Unchecked);
		else
			i_item->setCheckState(0, Qt::Checked);
	}
	else {
		if (i_item->checkState(0) == Qt::Unchecked)
			i_item->setCheckState(0, Qt::Checked);
		else if (i_item->checkState(0) == Qt::Checked)
			i_item->setCheckState(0, Qt::PartiallyChecked);
		else
			i_item->setCheckState(0, Qt::Unchecked);
	}
}

void Tree::mousePressEvent(QMouseEvent* i_event) {
	QTreeWidget::mousePressEvent(i_event);
	QTreeWidgetItem* item = itemAt(i_event->pos());

	if (item) {
		item_state = currentItem();
		if (item_state->whatsThis(0) == "Filter") {
			setSelectionMode(QTreeWidget::SelectionMode::ExtendedSelection);
			if (i_event->modifiers() == Qt::KeyboardModifier::AltModifier || i_event->modifiers() == (Qt::KeyboardModifier::ShiftModifier | Qt::KeyboardModifier::AltModifier))
				modifyFilters(true, item_state);
			else
				modifyFilters(false, item_state);
			selected_list.append(item);
			filter_active = true;
		}
		else if (item_state->whatsThis(0) == "Parent") {
			setSelectionMode(QTreeWidget::SelectionMode::SingleSelection);
			item_state->setExpanded(!item_state->isExpanded());
			selected_list.append(item);
			expand_active = true;
		}
	}
}

void Tree::mouseMoveEvent(QMouseEvent* i_event) {
	QTreeWidget::mouseMoveEvent(i_event);
	QTreeWidgetItem* item = itemAt(i_event->pos());

	if (item && item_state && filter_active && !selected_list.contains(item) && item->whatsThis(0) == "Filter") {
		if (i_event->modifiers() == (Qt::KeyboardModifier::ShiftModifier | Qt::KeyboardModifier::AltModifier))
			modifyFilters(true, item);
		else if (i_event->modifiers() == Qt::KeyboardModifier::ShiftModifier)
			modifyFilters(false, item);
		else
			item->setCheckState(0, item_state->checkState(0));
		selected_list.append(item);
	}
	else if (item && item_state && expand_active && !selected_list.contains(item) && item->whatsThis(0) == "Parent") {
		item->setExpanded(item_state->isExpanded());
		selected_list.append(item);
	}
}

void Tree::mouseReleaseEvent(QMouseEvent* i_event) {
	QTreeWidget::mouseReleaseEvent(i_event);
	setSelectionMode(QTreeWidget::SelectionMode::SingleSelection);
	clearSelection();
	selected_list = QList<QTreeWidgetItem*>();
	filter_active = false;
	expand_active = false;
	emit onMouseRelease();
}

void Tree::clearChildren() {
	for (int i = topLevelItemCount() - 1; i >= 0; --i) {
		topLevelItem(i)->takeChildren();
	}
}

BranchDelegate::BranchDelegate(QObject* parent) :
	QStyledItemDelegate(parent)
{}

void BranchDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
	QStyleOptionViewItem opt = option;
	if (index.column() == 0) {
		opt.rect.adjust(opt.rect.height(), 0, 0, 0);
	}

	QStyledItemDelegate::paint(painter, opt, index);

	if (index.column() == 0) {
		QStyleOptionViewItem branch;
		branch.rect = QRect(0, opt.rect.y(), opt.rect.height(), opt.rect.height());
		branch.state = option.state;
		const QWidget* widget = option.widget;
		const QStyle* style = widget ? widget->style() : qApp->style();
		style->drawPrimitive(QStyle::PE_IndicatorBranch, &branch, painter, widget);
	}
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
{}

Form::Form() {
	layout = new QFormLayout();
	setContentsMargins(0, 0, 0, 0);
	setLayout(layout);
}

Button::Button() {
	setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
}

Text_Edit::Text_Edit() {
	setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
}

Text_Stream::Text_Stream(QWidget* i_parent) : QTextBrowser(i_parent) {
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setTabStopDistance(38);
}

void Text_Stream::append(const QString& text) {
	cerr << text.toStdString() << endl;
	QTextBrowser::append(text);
}