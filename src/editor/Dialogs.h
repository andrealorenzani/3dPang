#pragma once
// Editor dialogs and the small widgets they share.
// (In 2005 these were Qt 3 Designer forms; version 2 builds them in code.)

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QSpinBox>

namespace pang {

class EditorView;

// Three red/green/blue sliders (0-255)
class ColorSliders : public QWidget {
    Q_OBJECT
public:
    explicit ColorSliders(QWidget* parent = nullptr);
    float red() const { return float(r->value()) / 255.0f; }
    float green() const { return float(g->value()) / 255.0f; }
    float blue() const { return float(b->value()) / 255.0f; }
    void reset();

    QSlider* r;
    QSlider* g;
    QSlider* b;
};

// "Enable texture" check box, file name and browse button. File names are
// kept relative to the data directory when possible.
class TexturePicker : public QWidget {
    Q_OBJECT
public:
    explicit TexturePicker(const QString& label, QWidget* parent = nullptr);
    bool isTextured() const { return enable->isChecked(); }
    QString file() const { return edit->text(); }
    void reset();

    QCheckBox* enable;
    QLineEdit* edit;
    QPushButton* browse;

signals:
    void changed();

private slots:
    void chooseFile();
};

// Asked at start-up and for "New": the size of the main floor
class BaseFloorDialog : public QDialog {
    Q_OBJECT
public:
    explicit BaseFloorDialog(EditorView* view, QWidget* parent = nullptr);
    void accept() override;

private:
    EditorView* view;
    QSpinBox* cellsX;
    QSpinBox* cellsZ;
    QSpinBox* thickness;
};

class BallsDialog : public QDialog {
    Q_OBJECT
public:
    explicit BallsDialog(EditorView* view, QWidget* parent = nullptr);
    void accept() override;

private:
    EditorView* view;
    QSpinBox* radius;
    QSpinBox* maxHeight;
    QSpinBox* startHeight;
    QComboBox* gravity;
    QSpinBox* customGravity;
    QSpinBox* children;
    QSlider* dirX;
    QSlider* dirZ;
    ColorSliders* color;
    TexturePicker* texture;
};

class FloorDialog : public QDialog {
    Q_OBJECT
public:
    explicit FloorDialog(EditorView* view, QWidget* parent = nullptr);
    void accept() override;

private:
    EditorView* view;
    QSpinBox* cellsX;
    QSpinBox* cellsZ;
    QSpinBox* thickness;
    QSpinBox* height;
    QRadioButton* destructible;
    ColorSliders* color;
    TexturePicker* texture;
};

class WallDialog : public QDialog {
    Q_OBJECT
public:
    explicit WallDialog(EditorView* view, QWidget* parent = nullptr);
    void accept() override;

private:
    EditorView* view;
    QSpinBox* length;
    QSpinBox* width;
    QSpinBox* wallHeight;
    QSpinBox* elevation;
    QRadioButton* destructible;
    ColorSliders* color;
    TexturePicker* texture;
};

class AboutDialog : public QDialog {
    Q_OBJECT
public:
    explicit AboutDialog(QWidget* parent = nullptr);
};

// Header with the 3dPang logo, a caption and an icon, used by every dialog
QWidget* dialogHeader(const QString& html, const QString& icon, QWidget* parent);

// Asks for an image in data/textures; returns a data-relative path (or empty)
QString chooseImage(QWidget* parent, const QString& title, const QString& startDir = QStringLiteral("textures"));

} // namespace pang
