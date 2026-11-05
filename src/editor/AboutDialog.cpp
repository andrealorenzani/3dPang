#include "Dialogs.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>

namespace pang {

AboutDialog::AboutDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle(tr("About 3dPang"));
    auto* layout = new QVBoxLayout(this);
    auto* logo = new QLabel(this);
    logo->setPixmap(QPixmap(QStringLiteral(":/icons/logo.png")));
    logo->setAlignment(Qt::AlignCenter);
    layout->addWidget(logo);
    auto* text = new QLabel(this);
    text->setTextFormat(Qt::RichText);
    text->setAlignment(Qt::AlignCenter);
    text->setOpenExternalLinks(true);
    text->setText(tr("<h2>3dPang Editor %1</h2>"
                     "<p><b>Andrea Lorenzani</b> and <b>Eva Tamburini</b></p>"
                     "<p>Project for the course <i>Costruzione di Interfacce</i>, a.y. 2003/2004<br>"
                     "University of Pisa - teacher: Paolo Cignoni</p>"
                     "<p>Inspired by <i>Pang</i> (C) Mitchell, 1989</p>"
                     "<p>Version 2 (2026): ported to Qt 6, SDL3 and the current vcglib.<br>"
                     "The 2005 version is the <code>original_with_Eva</code> tag.</p>"
                     "<p><a href=\"https://github.com/andrealorenzani/3dPang\">"
                     "github.com/andrealorenzani/3dPang</a></p>")
                      .arg(QStringLiteral(PANG3D_VERSION)));
    layout->addWidget(text);
    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    layout->addWidget(buttons);
}

} // namespace pang
