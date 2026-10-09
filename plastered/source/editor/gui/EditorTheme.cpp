#include "EditorTheme.h"

#include <QApplication>
#include <QFont>
#include <QPalette>
#include <QProxyStyle>
#include <QStyleFactory>

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

			QProxyStyle::drawPrimitive(element, option, painter, widget);
		}

	private:
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
}

void ApplyEditorTheme(QApplication& app, const EditorTheme& theme)
{
	app.setStyle(new EditorStyle(theme)); // QApplication takes ownership
	app.setPalette(MakePalette(theme));
	app.setFont(QFont(theme.fontFamily, theme.fontPointSize));
}
