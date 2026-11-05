#include "Dialogs.h"
#include "EditorView.h"

#include "DataPaths.h"

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLCDNumber>
#include <QVBoxLayout>

namespace pang {

// ---------------------------------------------------------------- shared widgets

QWidget* dialogHeader(const QString& html, const QString& icon, QWidget* parent)
{
    auto* box = new QGroupBox(parent);
    auto* layout = new QHBoxLayout(box);
    auto* logo = new QLabel(box);
    logo->setPixmap(QPixmap(QStringLiteral(":/icons/logo.png")));
    auto* text = new QLabel(html, box);
    text->setAlignment(Qt::AlignCenter);
    auto* picture = new QLabel(box);
    picture->setPixmap(QPixmap(icon));
    layout->addWidget(logo);
    layout->addWidget(text, 1);
    layout->addWidget(picture);
    return box;
}

QString chooseImage(QWidget* parent, const QString& title, const QString& startDir)
{
    const QString file = QFileDialog::getOpenFileName(parent, title, paths::resolve(startDir),
                                                      QObject::tr("Images (*.png *.jpg *.jpeg *.bmp)"));
    return file.isEmpty() ? QString() : paths::relativize(file);
}

ColorSliders::ColorSliders(QWidget* parent) : QWidget(parent)
{
    auto* layout = new QFormLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    const auto make = [this]() {
        auto* s = new QSlider(Qt::Horizontal, this);
        s->setRange(0, 255);
        s->setValue(255);
        s->setPageStep(10);
        return s;
    };
    r = make();
    g = make();
    b = make();
    layout->addRow(QStringLiteral("<font color=\"#ff0000\"><b>Red</b></font>"), r);
    layout->addRow(QStringLiteral("<font color=\"#00aa00\"><b>Green</b></font>"), g);
    layout->addRow(QStringLiteral("<font color=\"#0000ff\"><b>Blue</b></font>"), b);
}

void ColorSliders::reset()
{
    r->setValue(255);
    g->setValue(255);
    b->setValue(255);
}

TexturePicker::TexturePicker(const QString& label, QWidget* parent) : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    enable = new QCheckBox(label, this);
    auto* row = new QHBoxLayout();
    edit = new QLineEdit(this);
    edit->setEnabled(false);
    edit->setPlaceholderText(tr("textures/..."));
    browse = new QPushButton(QStringLiteral("..."), this);
    browse->setEnabled(false);
    browse->setToolTip(tr("Choose a texture"));
    row->addWidget(edit, 1);
    row->addWidget(browse);
    layout->addWidget(enable);
    layout->addLayout(row);
    connect(enable, &QCheckBox::toggled, edit, &QWidget::setEnabled);
    connect(enable, &QCheckBox::toggled, browse, &QWidget::setEnabled);
    connect(enable, &QCheckBox::toggled, this, [this](bool on) {
        if (!on)
            edit->clear();
        emit changed();
    });
    connect(edit, &QLineEdit::editingFinished, this, &TexturePicker::changed);
    connect(browse, &QPushButton::clicked, this, &TexturePicker::chooseFile);
}

void TexturePicker::chooseFile()
{
    const QString file = chooseImage(this, tr("Choose a texture"));
    if (!file.isEmpty()) {
        edit->setText(file);
        emit changed();
    }
}

void TexturePicker::reset()
{
    enable->setChecked(false);
    edit->clear();
}

// ---------------------------------------------------------------- BallsDialog

BallsDialog::BallsDialog(EditorView* v, QWidget* parent) : QDialog(parent), view(v)
{
    setWindowTitle(tr("3dPang - Balls"));
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(dialogHeader(tr("<font size=\"+1\"><b>Properties of the <u>balls</u><br>to create on the "
                                      "selected cells</b></font>"),
                                   QStringLiteral(":/icons/balls.png"), this));

    auto* props = new QGroupBox(tr("Ball"), this);
    auto* form = new QFormLayout(props);
    radius = new QSpinBox(props);
    radius->setRange(1, 5);
    radius->setValue(2);
    maxHeight = new QSpinBox(props);
    maxHeight->setRange(7, 22);
    maxHeight->setValue(15);
    startHeight = new QSpinBox(props);
    startHeight->setRange(2, 20);
    startHeight->setValue(10);
    gravity = new QComboBox(props);
    gravity->addItems({tr("Earth (9.81)"), tr("Moon (1.6)"), tr("Jupiter (26)"), tr("Mars (3.72)"),
                       tr("Pluto (0.4)"), tr("Custom...")});
    customGravity = new QSpinBox(props);
    customGravity->setRange(1, 30);
    customGravity->setEnabled(false);
    connect(gravity, qOverload<int>(&QComboBox::currentIndexChanged), this,
            [this](int i) { customGravity->setEnabled(i == 5); });
    children = new QSpinBox(props);
    children->setRange(1, 4);
    children->setValue(1);
    form->addRow(tr("Size"), radius);
    form->addRow(tr("Maximum height"), maxHeight);
    form->addRow(tr("Start height"), startHeight);
    auto* g = new QHBoxLayout();
    g->addWidget(gravity, 1);
    g->addWidget(customGravity);
    form->addRow(tr("Gravity"), g);
    form->addRow(tr("Balls after a hit"), children);
    layout->addWidget(props);

    auto* colorBox = new QGroupBox(tr("Colour"), this);
    auto* cl = new QVBoxLayout(colorBox);
    color = new ColorSliders(colorBox);
    cl->addWidget(color);
    layout->addWidget(colorBox);

    auto* texBox = new QGroupBox(this);
    auto* tl = new QVBoxLayout(texBox);
    texture = new TexturePicker(tr("Enable ball texture"), texBox);
    tl->addWidget(texture);
    layout->addWidget(texBox);

    auto* dirBox = new QGroupBox(tr("Direction"), this);
    auto* dl = new QFormLayout(dirBox);
    const auto direction = [dirBox](QSlider*& slider) {
        auto* row = new QHBoxLayout();
        slider = new QSlider(Qt::Horizontal, dirBox);
        slider->setRange(-20, 20);
        slider->setPageStep(5);
        auto* lcd = new QLCDNumber(3, dirBox);
        lcd->setSegmentStyle(QLCDNumber::Flat);
        lcd->display(0);
        QObject::connect(slider, &QSlider::valueChanged, lcd, qOverload<int>(&QLCDNumber::display));
        row->addWidget(slider, 1);
        row->addWidget(lcd);
        return row;
    };
    dl->addRow(QStringLiteral("X:"), direction(dirX));
    dl->addRow(QStringLiteral("Z:"), direction(dirZ));
    layout->addWidget(dirBox);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);
}

void BallsDialog::accept()
{
    static const float gravities[] = {9.81f, 1.60f, 26.00f, 3.72f, 0.40f};
    const int index = gravity->currentIndex();
    const float g = index < 5 ? gravities[index] : float(customGravity->value());
    int hmax = maxHeight->value();
    // High enough to jump over the player
    if (hmax < 5 + radius->value())
        hmax = 5 + radius->value();
    view->newBalls(radius->value(), children->value(), startHeight->value(), hmax, g, dirX->value() * 0.5f,
                   dirZ->value() * 0.5f, color->red(), color->green(), color->blue(), texture->isTextured(),
                   texture->file());
    QDialog::accept();
}

} // namespace pang
