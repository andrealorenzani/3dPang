#pragma once
// Main window of the editor: the editing tabs on the left, the 3D view on
// the right, toolbar and menus on top.

#include "Dialogs.h"

#include <QAction>
#include <QComboBox>
#include <QLCDNumber>
#include <QLineEdit>
#include <QMainWindow>
#include <QRadioButton>
#include <QSlider>
#include <QSpinBox>

namespace pang {

class EditorView;
class ViewPreview;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();
    EditorView* view() const { return editorView; }
    // Shows the main floor dialog; false if cancelled
    bool askMainFloor();
    bool openLevel(const QString& file);

protected:
    void closeEvent(QCloseEvent* e) override;

private slots:
    void fileNew();
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void addToCampaign();
    void editBalls();
    void editFloor();
    void editWall();
    void applyBallTexture();
    void applyFloorTexture();
    void applyWallTexture();
    void playerChanged();
    void choosePlayerModel();
    void choosePlayerSkin();
    void viewChanged();
    void saveView();
    void snapshot();
    void chooseSkybox();
    void saveProbabilities();
    void helpGuide();
    void helpAbout();

private:
    QWidget* buildBallsTab();
    QWidget* buildFloorsTab();
    QWidget* buildWallsTab();
    QWidget* buildPlayersTab();
    QWidget* buildViewsTab();
    QWidget* buildPowerUpsTab();
    void buildActions();
    void resetControls();
    void resetPowerUps();
    void updateTitle();
    int selectedView() const;

    EditorView* editorView;
    ViewPreview* preview;
    QAction* pauseAction = nullptr;

    ColorSliders* ballColor;
    ColorSliders* floorColor;
    ColorSliders* wallColor;
    TexturePicker* ballTexture;
    TexturePicker* floorTexture;
    TexturePicker* wallTexture;

    QRadioButton* playerOneRadio;
    QRadioButton* playerTwoRadio;
    QLineEdit* playerModel;
    QLineEdit* playerSkin;
    QComboBox* playerShow;

    QRadioButton* viewRadio[6];
    QSpinBox* viewX;
    QSpinBox* viewY;
    QSpinBox* viewZ;

    QSlider* powerUpSlider;
    QSlider* bonusSlider;
    QLCDNumber* malusLcd;
    QComboBox* weaponsCombo;
    QComboBox* fruitCombo;
    QComboBox* livesCombo;
    QComboBox* bombsCombo;
    QComboBox* tornadoCombo;

    int snapshotCount = 0;
};

} // namespace pang
