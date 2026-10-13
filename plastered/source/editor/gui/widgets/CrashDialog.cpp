#include "CrashDialog.h"

#include <QApplication>
#include <QClipboard>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

CrashDialog::CrashDialog(const QString& message, QWidget* parent) :
	QDialog(parent)
{
	setObjectName("editor_crashDialog");
	setWindowTitle("Plastered crashed");
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	resize(520, 280);

	auto* layout = new QVBoxLayout(this);

	auto* header = new QLabel("The editor hit an unhandled exception and has to close.", this);
	header->setWordWrap(true);
	layout->addWidget(header);

	auto* details = new QPlainTextEdit(message, this);
	details->setReadOnly(true);
	layout->addWidget(details);

	auto* buttons = new QDialogButtonBox(this);
	QPushButton* copy = buttons->addButton("Copy", QDialogButtonBox::ActionRole);
	QPushButton* close = buttons->addButton("Close", QDialogButtonBox::AcceptRole);
	close->setDefault(true);
	layout->addWidget(buttons);

	connect(copy, &QPushButton::clicked, this, [message] { QApplication::clipboard()->setText(message); });
	connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
}
