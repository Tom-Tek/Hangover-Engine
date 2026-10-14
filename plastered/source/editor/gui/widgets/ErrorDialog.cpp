#include "ErrorDialog.h"

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStyle>
#include <QVBoxLayout>

ErrorDialog::ErrorDialog(const QString& message, const QString& title, QWidget* parent) :
	QDialog(parent)
{
	setObjectName("editor_errorDialog");
	setWindowTitle(title);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setMinimumWidth(360);

	auto* layout = new QVBoxLayout(this);

	auto* body = new QHBoxLayout();
	body->setSpacing(12);

	auto* icon = new QLabel(this);
	icon->setPixmap(style()->standardIcon(QStyle::SP_MessageBoxCritical).pixmap(32, 32));
	icon->setAlignment(Qt::AlignTop);
	body->addWidget(icon);

	auto* text = new QLabel(message, this);
	text->setWordWrap(true);
	text->setTextInteractionFlags(Qt::TextSelectableByMouse);
	body->addWidget(text, 1);

	layout->addLayout(body);

	auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok, this);
	buttons->button(QDialogButtonBox::Ok)->setDefault(true);
	layout->addWidget(buttons);

	connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
}
