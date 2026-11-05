#include "Dialogs.h"
#include "EditorView.h"

#include <QButtonGroup>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace pang {

WallDialog::WallDialog(EditorView* v, QWidget* parent) : QDialog(parent), view(v)
{
    setWindowTitle(tr("3dPang - Walls"));
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(dialogHeader(tr("<font size=\"+1\"><b>Properties of the<br><u>wall</u> to create</b></font>"),
                                   QStringLiteral(":/icons/floors.png"), this));

    auto* sizeBox = new QGroupBox(tr("Size and position"), this);
    auto* form = new QFormLayout(sizeBox);
    length = new QSpinBox(sizeBox);
    length->setRange(1, 9);
    length->setValue(2);
    width = new QSpinBox(sizeBox);
    width->setRange(1, 9);
    width->setValue(2);
    wallHeight = new QSpinBox(sizeBox);
    wallHeight->setRange(1, 99);
    wallHeight->setValue(5);
    elevation = new QSpinBox(sizeBox);
    elevation->setRange(std::max(1, view->scene->floorThickness), 99);
    elevation->setValue(std::max(4, elevation->minimum()));
    form->addRow(tr("Length (cells along Z)"), length);
    form->addRow(tr("Width (cells along X)"), width);
    form->addRow(tr("Wall height"), wallHeight);
    form->addRow(tr("Elevation"), elevation);
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
    texture = new TexturePicker(tr("Enable wall texture"), texBox);
    tl->addWidget(texture);
    layout->addWidget(texBox);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);
}

void WallDialog::accept()
{
    view->newWall(destructible->isChecked(), false, length->value(), width->value(), wallHeight->value(),
                  elevation->value(), color->red(), color->green(), color->blue(), texture->isTextured(),
                  texture->file());
    QDialog::accept();
}

} // namespace pang
