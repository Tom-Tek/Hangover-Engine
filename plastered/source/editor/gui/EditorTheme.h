#pragma once

#include <QColor>
#include <QString>

class QApplication;

// Every visual property of the editor lives here, like a stylesheet.
// Change a value once and every widget picks it up through ApplyEditorTheme.
struct EditorTheme
{
	// Surfaces
	QColor window        { 37, 37, 38 };    // panels, toolbars, dock backgrounds
	QColor base          { 27, 27, 28 };    // trees, lists, input fields
	QColor alternateBase { 32, 32, 33 };    // alternating rows
	QColor raised        { 55, 55, 57 };    // buttons, tabs
	QColor sectionHeader { 74, 74, 76 };    // collapsible section bars
	QColor groupBox      { 31, 31, 32 };    // inset boxes inside sections
	QColor border        { 18, 18, 18 };

	// Text
	QColor text          { 200, 200, 200 };
	QColor textDim       { 140, 140, 140 };
	QColor textDisabled  { 95, 95, 95 };
	QColor textBright    { 245, 245, 245 };

	// Accents
	QColor accent        { 46, 112, 178 };  // selection, focus
	QColor accentText    { 255, 255, 255 };
	QColor link          { 95, 200, 85 };   // asset/decl references
	QColor iconGreen     { 120, 200, 95 };
	QColor iconFolder    { 225, 190, 80 };
	QColor iconNeutral   { 210, 210, 210 };
	QColor record        { 215, 40, 40 };
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
	int toolbarIconSize     = 18;
	int modeIconSize        = 26;
	int layoutSpacing       = 4;
	int layoutMargin        = 4;
	int splitterWidth       = 3;
	int rowHeight           = 20;
	int treeIndent          = 14;
	int sectionHeaderHeight = 20;
	int sliderThickness     = 14;
	int sliderHandleLength  = 9;
};

void ApplyEditorTheme(QApplication& app, const EditorTheme& theme);
