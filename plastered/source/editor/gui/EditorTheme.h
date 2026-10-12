#pragma once

#include <QColor>
#include <QString>

class QApplication;

// Every visual property of the editor lives here, like a stylesheet.
// Change a value once and every widget picks it up through ApplyEditorTheme.
struct EditorTheme
{
	// Surfaces
	QColor window        { 42, 42, 44 };    // panels, toolbars, dock backgrounds
	QColor base          { 30, 30, 32 };    // trees, lists, input fields
	QColor alternateBase { 35, 35, 37 };    // alternating rows
	QColor raised        { 58, 58, 62 };    // buttons, tabs, hovered controls
	QColor titleBar      { 34, 34, 36 };    // menu bar, status bar
	QColor dockTitle     { 56, 56, 60 };    // dock title bars, column headers
	QColor sectionHeader { 64, 64, 68 };    // collapsible section bars
	QColor groupBox      { 38, 38, 40 };    // inset boxes inside sections
	QColor border        { 22, 22, 24 };
	QColor borderHover   { 92, 92, 98 };
	QColor hover         { 255, 255, 255, 12 }; // translucent wash over hovered rows

	// Text
	QColor text          { 204, 204, 204 };
	QColor textDim       { 140, 140, 145 };
	QColor textDisabled  { 95, 95, 98 };
	QColor textBright    { 245, 245, 245 };

	// Accents
	QColor accent        { 51, 102, 165 };  // selection, focus
	QColor accentText    { 255, 255, 255 };
	QColor link          { 95, 200, 85 };   // asset/decl references
	QColor iconGreen     { 120, 200, 95 };
	QColor iconFolder    { 225, 190, 80 };
	QColor iconNeutral   { 210, 210, 210 };
	QColor record        { 215, 40, 40 };
	QColor closeHover    { 196, 43, 28 };  // title bar close button
	QColor axisX         { 220, 75, 65 };
	QColor axisY         { 105, 200, 85 };
	QColor axisZ         { 85, 135, 230 };

	// Viewport
	QColor viewportBackdropTop    { 118, 82, 50 };
	QColor viewportBackdropBottom { 52, 37, 26 };
	QColor viewportGround         { 40, 30, 22 };
	QColor viewportGrid           { 205, 125, 55 };
	QColor viewportFloorFar       { 112, 112, 110 };
	QColor viewportFloorNear      { 172, 172, 168 };
	QColor viewportModel          { 150, 140, 146 };

	// Timeline and property states
	QColor timelineRuler    { 48, 48, 50 };
	QColor timelineTick     { 110, 110, 110 };
	QColor timelinePlayhead { 210, 210, 210 };
	QColor valueHighlight   { 46, 112, 46 };   // live numeric fields, e.g. Tolerance
	QColor propertyModified { 225, 150, 70 };  // properties that differ from their parent decl

	// Typography
	QString fontFamily     = "Segoe UI";
	QString monoFontFamily = "Consolas";
	int fontPointSize      = 8;

	// Metrics
	int iconSize            = 16;
	int toolbarIconSize     = 16;
	int modeIconSize        = 26;
	int layoutSpacing       = 4;
	int layoutMargin        = 4;
	int splitterWidth       = 3;
	int rowHeight           = 18;
	int treeIndent          = 14;
	int sectionHeaderHeight = 20;
	int propertyLabelWidth  = 100;
	int cornerRadius        = 0;
	int sliderThickness     = 14;
	int sliderHandleLength  = 9;
};

void ApplyEditorTheme(QApplication& app, const EditorTheme& theme);
