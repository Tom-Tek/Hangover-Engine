#include "EditorTheme.h"

#include <QApplication>
#include <QFont>
#include <QPainter>
#include <QPalette>
#include <QPolygonF>
#include <QProxyStyle>
#include <QStyleFactory>
#include <QStyleOption>

#include <algorithm>
#include <utility>
#include <vector>

namespace
{
	// Fusion is fully palette driven, so it renders whatever colors the theme gives it.
	// The proxy lets the theme also control sizes and small behaviours.
	class EditorStyle : public QProxyStyle
	{
	public:
		explicit EditorStyle(const EditorTheme& theme)
			: QProxyStyle(QStyleFactory::create("Fusion"))
			, m_theme(theme)
		{
		}

		int pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const override
		{
			switch (metric)
			{
			case PM_SmallIconSize:
			case PM_ButtonIconSize:
				return m_theme.iconSize;
			case PM_ToolBarIconSize:
				return m_theme.toolbarIconSize;
			case PM_LayoutHorizontalSpacing:
			case PM_LayoutVerticalSpacing:
				return m_theme.layoutSpacing;
			case PM_LayoutLeftMargin:
			case PM_LayoutTopMargin:
			case PM_LayoutRightMargin:
			case PM_LayoutBottomMargin:
				return m_theme.layoutMargin;
			case PM_SplitterWidth:
			case PM_DockWidgetSeparatorExtent:
				return m_theme.splitterWidth;
			case PM_TreeViewIndentation:
				return m_theme.treeIndent;
			case PM_SliderThickness:
				return m_theme.sliderThickness;
			case PM_SliderLength:
				return m_theme.sliderHandleLength;
			default:
				return QProxyStyle::pixelMetric(metric, option, widget);
			}
		}

		int styleHint(StyleHint hint, const QStyleOption* option, const QWidget* widget, QStyleHintReturn* returnData) const override
		{
			// Highlight the whole row in trees, not just the text
			if (hint == SH_ItemView_ShowDecorationSelected)
				return 1;

			return QProxyStyle::styleHint(hint, option, widget, returnData);
		}

		void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const override
		{
			// No dotted focus rectangles
			if (element == PE_FrameFocusRect)
				return;

			// Thin chevrons instead of Fusion's filled triangles
			if (element == PE_IndicatorBranch)
			{
				if (option->state & State_Children)
					DrawChevron(painter, option->rect, option->state & State_Open, option->state & State_Selected);
				return;
			}

			QProxyStyle::drawPrimitive(element, option, painter, widget);
		}

	private:
		void DrawChevron(QPainter* painter, const QRect& rect, bool open, bool selected) const
		{
			const QPointF c = QRectF(rect).center();
			const qreal s = 3.0;
			const QPolygonF chevron = open
				? QPolygonF({ c + QPointF(-s, -s * 0.5), c + QPointF(0, s * 0.5), c + QPointF(s, -s * 0.5) })
				: QPolygonF({ c + QPointF(-s * 0.5, -s), c + QPointF(s * 0.5, 0), c + QPointF(-s * 0.5, s) });

			painter->save();
			painter->setRenderHint(QPainter::Antialiasing);
			painter->setPen(QPen(selected ? m_theme.accentText : m_theme.textDim, 1.3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
			painter->drawPolyline(chevron);
			painter->restore();
		}

		EditorTheme m_theme;
	};

	QPalette MakePalette(const EditorTheme& theme)
	{
		QPalette palette;
		palette.setColor(QPalette::Window, theme.window);
		palette.setColor(QPalette::WindowText, theme.text);
		palette.setColor(QPalette::Base, theme.base);
		palette.setColor(QPalette::AlternateBase, theme.alternateBase);
		palette.setColor(QPalette::Text, theme.text);
		palette.setColor(QPalette::PlaceholderText, theme.textDim);
		palette.setColor(QPalette::Button, theme.raised);
		palette.setColor(QPalette::ButtonText, theme.text);
		palette.setColor(QPalette::BrightText, theme.textBright);
		palette.setColor(QPalette::ToolTipBase, theme.raised);
		palette.setColor(QPalette::ToolTipText, theme.text);
		palette.setColor(QPalette::Highlight, theme.accent);
		palette.setColor(QPalette::HighlightedText, theme.accentText);
		palette.setColor(QPalette::Link, theme.link);
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
		palette.setColor(QPalette::Accent, theme.accent);
#endif

		// Bevel shades Fusion uses for frames and separators
		palette.setColor(QPalette::Light, theme.raised.lighter(125));
		palette.setColor(QPalette::Midlight, theme.raised);
		palette.setColor(QPalette::Mid, theme.window.darker(130));
		palette.setColor(QPalette::Dark, theme.border);
		palette.setColor(QPalette::Shadow, theme.border.darker(150));

		for (QPalette::ColorRole role : { QPalette::WindowText, QPalette::Text, QPalette::ButtonText })
			palette.setColor(QPalette::Disabled, role, theme.textDisabled);

		return palette;
	}

	QString Css(const QColor& color)
	{
		return QString("rgba(%1, %2, %3, %4)").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
	}

	QString Px(int value)
	{
		return QString::number(value) + "px";
	}

	// The palette covers colors; the stylesheet covers shapes the palette can't express
	// (rounded inputs, flat tabs, thin scrollbars). $tokens are filled from the theme.
	const char* k_StyleSheetTemplate = R"qss(
QMainWindow::separator {
	background: $border;
	width: $splitter;
	height: $splitter;
}
QMainWindow::separator:hover {
	background: $accent;
}

QDockWidget::title {
	background: $dockTitle;
	padding: 3px 6px;
	text-align: left;
	border-bottom: 1px solid $border;
}

QTabBar::tab {
	background: $titleBar;
	color: $textDim;
	padding: 3px 10px;
	border: 1px solid $border;
	border-bottom: none;
	margin-right: -1px;
}
QTabBar::tab:selected {
	background: $dockTitle;
	color: $textBright;
}
QTabBar::tab:hover:!selected {
	background: $raised;
	color: $text;
}

QMenuBar {
	background: $titleBar;
	padding: 2px;
}
QMenuBar::item {
	background: transparent;
	padding: 4px 8px;
	border-radius: $radius;
}
QMenuBar::item:selected {
	background: $raised;
}
QMenu {
	background: $raised;
	border: 1px solid $border;
	padding: 4px;
}
QMenu::item {
	padding: 3px 24px 3px 22px;
	border-radius: $radius;
}
QMenu::item:selected {
	background: $accent;
	color: $accentText;
}
QMenu::item:disabled {
	color: $textDisabled;
}
QMenu::separator {
	height: 1px;
	background: $border;
	margin: 4px 8px;
}

QToolBar {
	background: $window;
	border: none;
	border-bottom: 1px solid $border;
	padding: 2px;
	spacing: 1px;
}
QToolBar::separator {
	background: $border;
	width: 1px;
	margin: 4px 6px;
}
QToolButton {
	background: transparent;
	border: 1px solid transparent;
	border-radius: $radius;
	padding: 2px;
}
QToolButton:hover {
	background: $raised;
}
QToolButton:pressed {
	background: $base;
}
QToolButton:checked {
	background: $base;
	border-color: $borderHover;
}
QToolButton::menu-indicator {
	image: none;
	width: 0;
}

QPushButton {
	background: $raised;
	border: 1px solid $border;
	border-radius: $radius;
	padding: 3px 12px;
}
QPushButton:hover {
	border-color: $borderHover;
}
QPushButton:pressed {
	background: $base;
}
QPushButton:default {
	border-color: $accent;
}

QLineEdit, QAbstractSpinBox {
	background: $base;
	border: 1px solid $border;
	border-radius: $radius;
	padding: 1px 3px;
	selection-background-color: $accent;
	selection-color: $accentText;
}
QLineEdit:hover, QAbstractSpinBox:hover {
	border-color: $borderHover;
}
QLineEdit:focus, QAbstractSpinBox:focus {
	border-color: $accent;
}
QAbstractSpinBox::up-button, QAbstractSpinBox::down-button {
	width: 0;
	border: none;
}
QLineEdit#EntityName {
	font-weight: 600;
}

QTreeView, QListView, QPlainTextEdit {
	background: $base;
	border: none;
	outline: 0;
	selection-background-color: $accent;
}
QTreeView::item {
	height: $rowHeight;
}
QHeaderView::section {
	background: $dockTitle;
	color: $textDim;
	border: none;
	border-right: 1px solid $border;
	border-bottom: 1px solid $border;
	padding: 2px 6px;
}
QTreeView::item:hover {
	background: $hover;
}
QTreeView::item:selected {
	background: $accent;
	color: $accentText;
}

QScrollBar:vertical {
	background: $base;
	width: 12px;
	margin: 0;
}
QScrollBar:horizontal {
	background: $base;
	height: 12px;
	margin: 0;
}
QScrollBar::handle:vertical {
	background: $raised;
	min-height: 24px;
	border: 1px solid $border;
	margin: 1px;
}
QScrollBar::handle:horizontal {
	background: $raised;
	min-width: 24px;
	border: 1px solid $border;
	margin: 1px;
}
QScrollBar::handle:hover {
	background: $borderHover;
}
QScrollBar::add-line, QScrollBar::sub-line {
	width: 0;
	height: 0;
	border: none;
	background: none;
}
QScrollBar::add-page, QScrollBar::sub-page {
	background: none;
}

QToolTip {
	background: $raised;
	color: $text;
	border: 1px solid $border;
	padding: 4px;
}

QStatusBar {
	background: $titleBar;
	color: $textDim;
	border-top: 1px solid $border;
}
QStatusBar::item {
	border: none;
}
QStatusBar QLabel {
	color: $textDim;
	padding: 0 6px;
}

QLabel#PropertyLabel, QLabel#KindLabel, QLabel#EmptyHint, QLabel#ToolBarLabel {
	color: $textDim;
}
)qss";

	QString MakeStyleSheet(const EditorTheme& theme)
	{
		std::vector<std::pair<QString, QString>> tokens = {
			{ "$window",       Css(theme.window) },
			{ "$base",         Css(theme.base) },
			{ "$raised",       Css(theme.raised) },
			{ "$titleBar",     Css(theme.titleBar) },
			{ "$dockTitle",    Css(theme.dockTitle) },
			{ "$border",       Css(theme.border) },
			{ "$borderHover",  Css(theme.borderHover) },
			{ "$hover",        Css(theme.hover) },
			{ "$text",         Css(theme.text) },
			{ "$textDim",      Css(theme.textDim) },
			{ "$textDisabled", Css(theme.textDisabled) },
			{ "$textBright",   Css(theme.textBright) },
			{ "$accent",       Css(theme.accent) },
			{ "$accentText",   Css(theme.accentText) },
			{ "$radius",       Px(theme.cornerRadius) },
			{ "$rowHeight",    Px(theme.rowHeight) },
			{ "$splitter",     Px(theme.splitterWidth) },
		};

		// Longest first, so $textDim is replaced before $text can eat its prefix
		std::sort(tokens.begin(), tokens.end(), [](const auto& a, const auto& b) { return a.first.size() > b.first.size(); });

		QString sheet = k_StyleSheetTemplate;
		for (const auto& [token, value] : tokens)
			sheet.replace(token, value);

		return sheet;
	}
}

void ApplyEditorTheme(QApplication& app, const EditorTheme& theme)
{
	app.setStyle(new EditorStyle(theme)); // QApplication takes ownership
	app.setPalette(MakePalette(theme));
	app.setFont(QFont(theme.fontFamily, theme.fontPointSize));
	app.setStyleSheet(MakeStyleSheet(theme));
}
