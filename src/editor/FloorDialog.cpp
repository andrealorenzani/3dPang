#include "Dialogs.h"
#include "EditorView.h"

#include <QButtonGroup>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace pang {

FloorDialog::FloorDialog(EditorView* v, QWidget* parent) : QDialog(parent), view(v)
{
    setWindowTitle(tr("3dPang - Floors"));
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(dialogHeader(tr("<font size=\"+1\"><b>Properties of the<br><u>floor</u> to create</b></font>"),
                                   QStringLiteral(":/icons/floors.png"), this));

    auto* sizeBox = new QGroupBox(tr("Size and position"), this);
    auto* form = new QFormLayout(sizeBox);
    cellsX = new QSpinBox(sizeBox);
    cellsX->setRange(1, 9);
    cellsX->setValue(3);
    cellsZ = new QSpinBox(sizeBox);
    cellsZ->setRange(1, 9);
    cellsZ->setValue(3);
    thickness = new QSpinBox(sizeBox);
    thickness->setRange(1, 10);
    height = new QSpinBox(sizeBox);
    // Not below the main floor
    height->setRange(std::max(2, view->scene->floorThickness), 20);
    height->setValue(height->minimum());
    form->addRow(tr("Cells along X"), cellsX);
    form->addRow(tr("Cells along Z"), cellsZ);
    form->addRow(tr("Thickness"), thickness);
    form->addRow(tr("Height"), height);
    auto* kind = new QHBoxLayout();
    auto* fixed = new QRadioButton(tr("&Fixed"), sizeBox);
    fixed->setChecked(true);
    destructible = new QRadioButton(tr("&Destructible"), sizeBox);
    destructible->setToolTip(tr("Destroyed when hit by a shot"));
    auto* group = new QButtonGroup(this);
    group->addButton(fixed);
    group->addButton(destructible);
    kind->addWidget(fixed);
    kind->addWidget(destructible);
    form->addRow(kind);
    layout->addWidget(sizeBox);

    auto* colorBox = new QGroupBox(tr("Colour"), this);
    auto* cl = new QVBoxLayout(colorBox);
    color = new ColorSliders(colorBox);
    cl->addWidget(color);
    layout->addWidget(colorBox);

    auto* texBox = new QGroupBox(this);
    auto* tl = new QVBoxLayout(texBox);
    texture = new TexturePicker(tr("Enable floor texture"), texBox);
    tl->addWidget(texture);
    layout->addWidget(texBox);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);
}

void FloorDialog::accept()
{
    view->newFloor(destructible->isChecked(), false, cellsX->value(), cellsZ->value(), height->value(),
                   thickness->value(), color->red(), color->green(), color->blue(), texture->isTextured(),
                   texture->file());
    QDialog::accept();
}

} // namespace pang
