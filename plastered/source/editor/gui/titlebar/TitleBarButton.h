#pragma once

#include <QPushButton>

// Minimize / maximize / close button for the custom title bar.
// The glyph is painted rather than taken from a font, so it looks the same on every platform;
// colours come from the editor stylesheet.
class TitleBarButton : public QPushButton
{
	Q_OBJECT
public:
	enum class Kind
	{
		Minimize,
		Maximize,
		Close,
	};

	explicit TitleBarButton(Kind kind);

	// Maximize buttons switch to the "restore" glyph while the window is maximized
	void SetMaximized(bool maximized);

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	Kind m_kind;
	bool m_maximized = false;
};
