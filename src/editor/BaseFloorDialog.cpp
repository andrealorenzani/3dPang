#include "Dialogs.h"
#include "EditorView.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>

namespace pang {

BaseFloorDialog::BaseFloorDialog(EditorView* v, QWidget* parent) : QDialog(parent), view(v)
{
    setWindowTitle(tr("3dPang - Main floor"));
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(dialogHeader(tr("<font size=\"+1\"><b>Size of the<br><u>main floor</u></b></font>"),
                                   QStringLiteral(":/icons/floors.png"), this));
    auto* form = new QFormLayout();
    cellsX = new QSpinBox(this);
    cellsX->setRange(2, 30);
    cellsX->setValue(15);
    cellsZ = new QSpinBox(this);
    cellsZ->setRange(2, 30);
    cellsZ->setValue(15);
    thickness = new QSpinBox(this);
    thickness->setRange(1, 10);
    thickness->setValue(1);
    form->addRow(tr("Cells along X"), cellsX);
    form->addRow(tr("Cells along Z"), cellsZ);
    form->addRow(tr("Thickness"), thickness);
    layout->addLayout(form);
    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);
}

void BaseFloorDialog::accept()
{
    view->newLevel(cellsX->value(), cellsZ->value(), thickness->value());
    QDialog::accept();
}

} // namespace pang
