#include "MainWindow.h"

#include "DataPaths.h"
#include "EditorView.h"
#include "ViewPreview.h"

#include <QButtonGroup>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSplitter>
#include <QStandardPaths>
#include <QStatusBar>
#include <QTabWidget>
#include <QTextStream>
#include <QToolBar>
#include <QUrl>
#include <QVBoxLayout>

namespace pang {

namespace {
const char* const kGuideUrl = "https://github.com/andrealorenzani/3dPang/blob/master/docs/EDITOR.md";

QLabel* tabTitle(const QString& html, QWidget* parent)
{
    auto* label = new QLabel(QStringLiteral("<p align=\"center\"><font size=\"+1\"><b>") + html +
                                 QStringLiteral("</b></font></p>"),
                             parent);
    label->setWordWrap(true);
    return label;
}

QComboBox* frequencyCombo(QWidget* parent)
{
    auto* c = new QComboBox(parent);
    c->addItems({QObject::tr("Very low"), QObject::tr("Low"), QObject::tr("Medium"), QObject::tr("High"),
                 QObject::tr("Very high")});
    c->setCurrentIndex(2);
    return c;
}
} // namespace

MainWindow::MainWindow()
{
    setWindowIcon(QIcon(QStringLiteral(":/icons/logo.png")));
    editorView = new EditorView(this);
    editorView->setMinimumSize(480, 400);

    auto* tabs = new QTabWidget(this);
    tabs->addTab(buildBallsTab(), QIcon(QStringLiteral(":/icons/balls.png")), tr("Balls"));
    tabs->addTab(buildFloorsTab(), QIcon(QStringLiteral(":/icons/floors.png")), tr("Floors"));
    tabs->addTab(buildWallsTab(), tr("Walls"));
    tabs->addTab(buildPlayersTab(), tr("Players"));
    tabs->addTab(buildViewsTab(), tr("Views"));
    tabs->addTab(buildPowerUpsTab(), tr("Power-ups"));
    auto* scroll = new QScrollArea(this);
    scroll->setWidget(tabs);
    scroll->setWidgetResizable(true);
    scroll->setMinimumWidth(380);

    auto* splitter = new QSplitter(this);
    splitter->addWidget(scroll);
    splitter->addWidget(editorView);
    splitter->setStretchFactor(1, 1);
    setCentralWidget(splitter);
    buildActions();
    resize(1200, 760);
    updateTitle();
    statusBar()->showMessage(tr("Data directory: %1").arg(paths::dataDir()), 8000);
}

// ---------------------------------------------------------------- tabs

QWidget* MainWindow::buildBallsTab()
{
    auto* tab = new QWidget();
    auto* layout = new QVBoxLayout(tab);
    layout->addWidget(tabTitle(tr("Properties of the<br><u>selected balls</u>"), tab));
    ballColor = new ColorSliders(tab);
    connect(ballColor->r, &QSlider::valueChanged, editorView, &EditorView::setBallRed);
    connect(ballColor->g, &QSlider::valueChanged, editorView, &EditorView::setBallGreen);
    connect(ballColor->b, &QSlider::valueChanged, editorView, &EditorView::setBallBlue);
    layout->addWidget(ballColor);
    ballTexture = new TexturePicker(tr("&Enable ball texture"), tab);
    connect(ballTexture, &TexturePicker::changed, this, &MainWindow::applyBallTexture);
    layout->addWidget(ballTexture);
    auto* add = new QPushButton(tr("&Add balls..."), tab);
    add->setToolTip(tr("Select one or more floor cells first"));
    auto* del = new QPushButton(tr("&Delete balls"), tab);
    connect(add, &QPushButton::clicked, this, &MainWindow::editBalls);
    connect(del, &QPushButton::clicked, editorView, &EditorView::deleteBalls);
    layout->addWidget(add);
    layout->addWidget(del);
    layout->addStretch();
    return tab;
}

QWidget* MainWindow::buildFloorsTab()
{
    auto* tab = new QWidget();
    auto* layout = new QVBoxLayout(tab);
    layout->addWidget(tabTitle(tr("Properties of the<br><u>selected floors</u>"), tab));
    floorColor = new ColorSliders(tab);
    connect(floorColor->r, &QSlider::valueChanged, editorView, &EditorView::setFloorRed);
    connect(floorColor->g, &QSlider::valueChanged, editorView, &EditorView::setFloorGreen);
    connect(floorColor->b, &QSlider::valueChanged, editorView, &EditorView::setFloorBlue);
    layout->addWidget(floorColor);
    floorTexture = new TexturePicker(tr("&Enable floor texture"), tab);
    connect(floorTexture, &TexturePicker::changed, this, &MainWindow::applyFloorTexture);
    layout->addWidget(floorTexture);
    auto* add = new QPushButton(tr("&Add floor..."), tab);
    auto* del = new QPushButton(tr("&Delete floors"), tab);
    connect(add, &QPushButton::clicked, this, &MainWindow::editFloor);
    connect(del, &QPushButton::clicked, editorView, &EditorView::deleteFloors);
    layout->addWidget(add);
    layout->addWidget(del);
    auto* line = new QFrame(tab);
    line->setFrameShape(QFrame::HLine);
    layout->addWidget(line);
    auto* addLadder = new QPushButton(tr("Add &ladder"), tab);
    addLadder->setToolTip(tr("Then click where it goes. Wheel: rungs, right click: turn"));
    auto* delLadder = new QPushButton(tr("Delete l&adders"), tab);
    connect(addLadder, &QPushButton::clicked, editorView, &EditorView::addLadder);
    connect(delLadder, &QPushButton::clicked, editorView, &EditorView::deleteLadders);
    layout->addWidget(addLadder);
    layout->addWidget(delLadder);
    layout->addStretch();
    return tab;
}

QWidget* MainWindow::buildWallsTab()
{
    auto* tab = new QWidget();
    auto* layout = new QVBoxLayout(tab);
    layout->addWidget(tabTitle(tr("Properties of the<br><u>selected walls</u>"), tab));
    wallColor = new ColorSliders(tab);
    connect(wallColor->r, &QSlider::valueChanged, editorView, &EditorView::setWallRed);
    connect(wallColor->g, &QSlider::valueChanged, editorView, &EditorView::setWallGreen);
    connect(wallColor->b, &QSlider::valueChanged, editorView, &EditorView::setWallBlue);
    layout->addWidget(wallColor);
    wallTexture = new TexturePicker(tr("&Enable wall texture"), tab);
    connect(wallTexture, &TexturePicker::changed, this, &MainWindow::applyWallTexture);
    layout->addWidget(wallTexture);
    auto* add = new QPushButton(tr("&Add wall..."), tab);
    auto* del = new QPushButton(tr("&Delete walls"), tab);
    connect(add, &QPushButton::clicked, this, &MainWindow::editWall);
    connect(del, &QPushButton::clicked, editorView, &EditorView::deleteWalls);
    layout->addWidget(add);
    layout->addWidget(del);
    layout->addStretch();
    return tab;
}

QWidget* MainWindow::buildPlayersTab()
{
    auto* tab = new QWidget();
    auto* layout = new QVBoxLayout(tab);
    layout->addWidget(tabTitle(tr("Properties of the<br><u>players</u>"), tab));
    auto* which = new QGroupBox(tab);
    auto* wl = new QHBoxLayout(which);
    playerOneRadio = new QRadioButton(tr("Player &1"), which);
    playerOneRadio->setChecked(true);
    playerTwoRadio = new QRadioButton(tr("Player &2"), which);
    wl->addWidget(playerOneRadio);
    wl->addWidget(playerTwoRadio);
    connect(playerOneRadio, &QRadioButton::toggled, this, &MainWindow::playerChanged);
    layout->addWidget(which);

    auto* form = new QFormLayout();
    const auto fileRow = [tab](QLineEdit*& edit, const QString& placeholder) {
        auto* row = new QHBoxLayout();
        edit = new QLineEdit(tab);
        edit->setReadOnly(true);
        edit->setPlaceholderText(placeholder);
        auto* browse = new QPushButton(QStringLiteral("..."), tab);
        row->addWidget(edit, 1);
        row->addWidget(browse);
        return std::make_pair(row, browse);
    };
    auto [modelRow, modelBrowse] = fileRow(playerModel, tr("(default)"));
    auto [skinRow, skinBrowse] = fileRow(playerSkin, tr("(default)"));
    connect(modelBrowse, &QPushButton::clicked, this, &MainWindow::choosePlayerModel);
    connect(skinBrowse, &QPushButton::clicked, this, &MainWindow::choosePlayerSkin);
    form->addRow(tr("&Model"), modelRow);
    form->addRow(tr("&Skin"), skinRow);
    playerShow = new QComboBox(tab);
    playerShow->addItems({tr("Number"), tr("Model")});
    connect(playerShow, qOverload<int>(&QComboBox::activated), editorView, &EditorView::setPlayerModelVisible);
    form->addRow(tr("Show"), playerShow);
    layout->addLayout(form);
    auto* place = new QPushButton(tr("&Place"), tab);
    place->setToolTip(tr("Then click on the start position"));
    connect(place, &QPushButton::clicked, this, [this] {
        editorView->playerOne = playerOneRadio->isChecked();
        editorView->startPlacingPlayer();
    });
    layout->addWidget(place);
    layout->addStretch();
    return tab;
}

QWidget* MainWindow::buildViewsTab()
{
    auto* tab = new QWidget();
    auto* layout = new QVBoxLayout(tab);
    layout->addWidget(tabTitle(tr("Settings of the<br><u>game views</u>"), tab));
    auto* views = new QGroupBox(tr("Views"), tab);
    auto* grid = new QGridLayout(views);
    auto* group = new QButtonGroup(this);
    for (int i = 0; i < 6; ++i) {
        viewRadio[i] = new QRadioButton(tr("F%1 (player %2)").arg(i + 3).arg(i < 3 ? 1 : 2), views);
        group->addButton(viewRadio[i], i);
        grid->addWidget(viewRadio[i], i % 3, i / 3);
    }
    viewRadio[0]->setChecked(true);
    connect(group, &QButtonGroup::idClicked, this, &MainWindow::viewChanged);
    layout->addWidget(views);

    layout->addWidget(new QLabel(tr("Camera position relative to the player (X, Y, Z)"), tab));
    auto* row = new QHBoxLayout();
    const auto spin = [tab, row](int min, int max, int value) {
        auto* s = new QSpinBox(tab);
        s->setRange(min, max);
        s->setValue(value);
        row->addWidget(s);
        return s;
    };
    viewX = spin(-50, 50, 25);
    viewY = spin(0, 50, 20);
    viewZ = spin(-50, 50, 50);
    layout->addLayout(row);
    preview = new ViewPreview(tab);
    connect(viewX, qOverload<int>(&QSpinBox::valueChanged), preview, &ViewPreview::setViewX);
    connect(viewY, qOverload<int>(&QSpinBox::valueChanged), preview, &ViewPreview::setViewY);
    connect(viewZ, qOverload<int>(&QSpinBox::valueChanged), preview, &ViewPreview::setViewZ);
    layout->addWidget(preview, 1);
    auto* apply = new QPushButton(tr("&Save view"), tab);
    connect(apply, &QPushButton::clicked, this, &MainWindow::saveView);
    layout->addWidget(apply);
    return tab;
}

QWidget* MainWindow::buildPowerUpsTab()
{
    auto* tab = new QWidget();
    auto* layout = new QVBoxLayout(tab);
    layout->addWidget(tabTitle(tr("<u>Bonuses</u> and <u>maluses</u>: how often they appear"), tab));

    auto* chance = new QGroupBox(tr("Probability"), tab);
    auto* form = new QFormLayout(chance);
    const auto sliderRow = [chance](QSlider*& slider, QLCDNumber*& lcd) {
        auto* row = new QHBoxLayout();
        slider = new QSlider(Qt::Horizontal, chance);
        slider->setRange(0, 100);
        lcd = new QLCDNumber(3, chance);
        lcd->setSegmentStyle(QLCDNumber::Flat);
        QObject::connect(slider, &QSlider::valueChanged, lcd, qOverload<int>(&QLCDNumber::display));
        row->addWidget(slider, 1);
        row->addWidget(lcd);
        return row;
    };
    QLCDNumber* powerUpLcd = nullptr;
    QLCDNumber* bonusLcd = nullptr;
    form->addRow(tr("&Power-up"), sliderRow(powerUpSlider, powerUpLcd));
    form->addRow(tr("&Bonus"), sliderRow(bonusSlider, bonusLcd));
    malusLcd = new QLCDNumber(3, chance);
    malusLcd->setSegmentStyle(QLCDNumber::Flat);
    malusLcd->display(100);
    connect(bonusSlider, &QSlider::valueChanged, this, [this](int b) { malusLcd->display(100 - b); });
    form->addRow(tr("Malus"), malusLcd);
    layout->addWidget(chance);

    auto* bonuses = new QGroupBox(tr("Bonus frequency"), tab);
    auto* bf = new QFormLayout(bonuses);
    weaponsCombo = frequencyCombo(bonuses);
    fruitCombo = frequencyCombo(bonuses);
    livesCombo = frequencyCombo(bonuses);
    bf->addRow(tr("&Weapons"), weaponsCombo);
    bf->addRow(tr("&Fruit"), fruitCombo);
    bf->addRow(tr("&Lives"), livesCombo);
    layout->addWidget(bonuses);

    auto* maluses = new QGroupBox(tr("Malus frequency"), tab);
    auto* mf = new QFormLayout(maluses);
    bombsCombo = frequencyCombo(maluses);
    tornadoCombo = frequencyCombo(maluses);
    mf->addRow(tr("B&ombs"), bombsCombo);
    mf->addRow(tr("&Tornado"), tornadoCombo);
    layout->addWidget(maluses);

    auto* save = new QPushButton(tr("&Save probabilities"), tab);
    connect(save, &QPushButton::clicked, this, &MainWindow::saveProbabilities);
    layout->addWidget(save);
    layout->addStretch();
    return tab;
}

void MainWindow::buildActions()
{
    const auto action = [this](const QString& icon, const QString& text, auto slot, QKeySequence key = {}) {
        auto* a = new QAction(icon.isEmpty() ? QIcon() : QIcon(QStringLiteral(":/icons/") + icon), text, this);
        if (!key.isEmpty())
            a->setShortcut(key);
        connect(a, &QAction::triggered, this, slot);
        return a;
    };
    auto* newAct = action(QStringLiteral("filenew.png"), tr("&New"), &MainWindow::fileNew, QKeySequence::New);
    auto* openAct = action(QStringLiteral("fileopen.png"), tr("&Open..."), &MainWindow::fileOpen, QKeySequence::Open);
    auto* saveAct = action(QStringLiteral("filesave.png"), tr("&Save"), &MainWindow::fileSave, QKeySequence::Save);
    auto* saveAsAct = action(QStringLiteral("filesave.png"), tr("Save &As..."), &MainWindow::fileSaveAs,
                             QKeySequence::SaveAs);
    auto* campaignAct = action(QStringLiteral("filesave.png"), tr("Add to &Campaign..."), &MainWindow::addToCampaign,
                               QKeySequence(tr("Ctrl+E")));
    auto* exitAct = action(QStringLiteral("exit.png"), tr("E&xit"), &QWidget::close, QKeySequence::Quit);
    auto* aboutAct = action(QStringLiteral("about.png"), tr("&About"), &MainWindow::helpAbout);
    auto* guideAct = action(QStringLiteral("help.png"), tr("&Guide"), &MainWindow::helpGuide, QKeySequence::HelpContents);
    auto* snapAct = action(QStringLiteral("camera.png"), tr("Snapshot"), &MainWindow::snapshot);
    auto* skyAct = action(QStringLiteral("cubemap.png"), tr("Background (sky box)..."), &MainWindow::chooseSkybox);

    pauseAction = new QAction(QIcon(QStringLiteral(":/icons/pause.png")), tr("Pause"), this);
    pauseAction->setCheckable(true);
    pauseAction->setChecked(true);
    connect(pauseAction, &QAction::toggled, editorView, &EditorView::pause);
    auto* rewindAct = new QAction(QIcon(QStringLiteral(":/icons/rewind.png")), tr("Rewind"), this);
    connect(rewindAct, &QAction::triggered, editorView, &EditorView::rewind);
    auto* trackAct = new QAction(QIcon(QStringLiteral(":/icons/trackball.png")), tr("Trackball"), this);
    trackAct->setCheckable(true);
    connect(trackAct, &QAction::toggled, editorView, &EditorView::setTrackball);
    auto* shadowAct = new QAction(QIcon(QStringLiteral(":/icons/shadows.png")), tr("Shadows"), this);
    shadowAct->setCheckable(true);
    shadowAct->setChecked(true);
    connect(shadowAct, &QAction::toggled, editorView, &EditorView::setShadows);
    auto* zoomInAct = new QAction(QIcon(QStringLiteral(":/icons/zoomin.png")), tr("Zoom in"), this);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    connect(zoomInAct, &QAction::triggered, editorView, &EditorView::zoomIn);
    auto* zoomOutAct = new QAction(QIcon(QStringLiteral(":/icons/zoomout.png")), tr("Zoom out"), this);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    connect(zoomOutAct, &QAction::triggered, editorView, &EditorView::zoomOut);

    QMenu* file = menuBar()->addMenu(tr("&File"));
    file->addActions({newAct, openAct, saveAct, saveAsAct});
    file->addSeparator();
    file->addAction(campaignAct);
    file->addSeparator();
    file->addAction(exitAct);
    QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addActions({pauseAction, rewindAct, trackAct, shadowAct, zoomInAct, zoomOutAct});
    viewMenu->addSeparator();
    viewMenu->addActions({snapAct, skyAct});
    QMenu* help = menuBar()->addMenu(tr("&Help"));
    help->addAction(aboutAct);
    help->addSeparator();
    help->addAction(guideAct);

    QToolBar* bar = addToolBar(tr("Toolbar"));
    bar->setObjectName(QStringLiteral("toolbar"));
    bar->addActions({newAct, openAct, saveAct});
    bar->addSeparator();
    bar->addActions({pauseAction, rewindAct});
    bar->addSeparator();
    bar->addActions({trackAct, shadowAct});
    bar->addSeparator();
    bar->addActions({zoomInAct, zoomOutAct});
    bar->addSeparator();
    bar->addActions({snapAct, skyAct});
    bar->addSeparator();
    bar->addActions({aboutAct, guideAct});
    bar->addSeparator();
    bar->addAction(exitAct);
}

// ---------------------------------------------------------------- helpers

void MainWindow::resetControls()
{
    for (ColorSliders* c : {ballColor, floorColor, wallColor}) {
        const QSignalBlocker b1(c->r), b2(c->g), b3(c->b);
        c->reset();
    }
    for (TexturePicker* t : {ballTexture, floorTexture, wallTexture}) {
        const QSignalBlocker b(t);
        t->reset();
    }
}

void MainWindow::resetPowerUps()
{
    const SceneGraph* s = editorView->scene;
    powerUpSlider->setValue(s->powerUpChance);
    bonusSlider->setValue(s->bonusChance);
    for (QComboBox* c : {weaponsCombo, fruitCombo, livesCombo, bombsCombo, tornadoCombo})
        c->setCurrentIndex(2);
    playerChanged();
}

void MainWindow::updateTitle()
{
    const QString name = editorView->fileName.isEmpty() ? tr("untitled") : QFileInfo(editorView->fileName).fileName();
    setWindowTitle(tr("3dPang Editor - %1").arg(name));
}

int MainWindow::selectedView() const
{
    for (int i = 0; i < 6; ++i)
        if (viewRadio[i]->isChecked())
            return i;
    return 0;
}

bool MainWindow::askMainFloor()
{
    BaseFloorDialog dialog(editorView, this);
    const bool ok = dialog.exec() == QDialog::Accepted;
    if (ok) {
        editorView->resetCamera();
        resetControls();
        resetPowerUps();
        updateTitle();
    }
    return ok;
}

bool MainWindow::openLevel(const QString& file)
{
    if (!editorView->load(file)) {
        QMessageBox::warning(this, tr("3dPang Editor"), tr("Cannot open the level\n%1").arg(file));
        return false;
    }
    editorView->resetCamera();
    resetControls();
    resetPowerUps();
    updateTitle();
    return true;
}

// ---------------------------------------------------------------- file

void MainWindow::fileNew()
{
    if (QMessageBox::question(this, tr("3dPang Editor"),
                              tr("Unsaved changes will be lost. Do you want to continue?")) != QMessageBox::Yes)
        return;
    askMainFloor();
}

void MainWindow::fileOpen()
{
    if (QMessageBox::question(this, tr("3dPang Editor"),
                              tr("Opening a level discards the changes to this one. Do you want to continue?")) !=
        QMessageBox::Yes)
        return;
    const QString file = QFileDialog::getOpenFileName(this, tr("Open a level"), paths::resolve(QStringLiteral("levels")),
                                                      tr("Levels (*.xml)"));
    if (!file.isEmpty())
        openLevel(file);
}

void MainWindow::fileSave()
{
    if (editorView->fileName.isEmpty()) {
        fileSaveAs();
        return;
    }
    if (!editorView->save(editorView->fileName))
        QMessageBox::warning(this, tr("3dPang Editor"), tr("Cannot save\n%1").arg(editorView->fileName));
    else
        statusBar()->showMessage(tr("Saved %1").arg(editorView->fileName), 4000);
}

void MainWindow::fileSaveAs()
{
    QString file = QFileDialog::getSaveFileName(this, tr("Save the level"), paths::resolve(QStringLiteral("levels")),
                                                tr("Levels (*.xml)"));
    if (file.isEmpty())
        return;
    if (!file.endsWith(QLatin1String(".xml"), Qt::CaseInsensitive))
        file += QStringLiteral(".xml");
    editorView->fileName = file;
    fileSave();
    updateTitle();
}

void MainWindow::addToCampaign()
{
    if (editorView->fileName.isEmpty()) {
        QMessageBox::information(this, tr("3dPang Editor"), tr("Save the level first."));
        return;
    }
    // Campaigns refer to levels by name: they must be in data/levels
    const QString levelsDir = paths::resolve(QStringLiteral("levels"));
    if (QFileInfo(editorView->fileName).absolutePath() != QFileInfo(levelsDir).absoluteFilePath())
        QMessageBox::warning(this, tr("3dPang Editor"),
                             tr("Campaigns can only play levels saved in\n%1").arg(levelsDir));
    QString file = QFileDialog::getSaveFileName(this, tr("Add the level to a campaign"),
                                                paths::resolve(QStringLiteral("campaigns")),
                                                tr("Campaigns (*.campaign)"), nullptr,
                                                QFileDialog::DontConfirmOverwrite);
    if (file.isEmpty())
        return;
    if (!file.endsWith(QLatin1String(".campaign")))
        file += QStringLiteral(".campaign");
    QFile out(file);
    const bool isNew = !out.exists();
    if (!out.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QMessageBox::warning(this, tr("3dPang Editor"), tr("Cannot write\n%1").arg(file));
        return;
    }
    QTextStream stream(&out);
    if (isNew)
        stream << "# 3dPang campaign: one level file per line, relative to data/levels\n";
    stream << QFileInfo(editorView->fileName).fileName() << '\n';
    statusBar()->showMessage(tr("Added to %1").arg(QFileInfo(file).fileName()), 4000);
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    if (!isVisible() || QCoreApplication::closingDown()) {
        e->accept();
        return;
    }
    if (QMessageBox::question(this, tr("3dPang Editor"), tr("Quit the editor? Unsaved changes will be lost.")) ==
        QMessageBox::Yes)
        e->accept();
    else
        e->ignore();
}

// ---------------------------------------------------------------- editing

void MainWindow::editBalls()
{
    pauseAction->setChecked(true);
    BallsDialog dialog(editorView, this);
    dialog.exec();
    editorView->rewind();
}

void MainWindow::editFloor()
{
    pauseAction->setChecked(true);
    FloorDialog dialog(editorView, this);
    dialog.exec();
    editorView->rewind();
}

void MainWindow::editWall()
{
    pauseAction->setChecked(true);
    WallDialog dialog(editorView, this);
    dialog.exec();
    editorView->rewind();
}

void MainWindow::applyBallTexture()
{
    editorView->setBallTexture(ballTexture->isTextured(), ballTexture->file());
}

void MainWindow::applyFloorTexture()
{
    editorView->setFloorTexture(floorTexture->isTextured(), floorTexture->file());
}

void MainWindow::applyWallTexture()
{
    editorView->setWallTexture(wallTexture->isTextured(), wallTexture->file());
}

void MainWindow::playerChanged()
{
    const bool one = playerOneRadio->isChecked();
    editorView->playerOne = one;
    const PlayerGroup* g = one ? editorView->scene->player1 : editorView->scene->player2;
    playerModel->setText(g != nullptr ? g->player->modelFile : QString());
    playerSkin->setText(g != nullptr ? g->player->skinFile : QString());
    playerShow->setCurrentIndex(0);
    editorView->setPlayerModelVisible(0);
}

void MainWindow::choosePlayerModel()
{
    const QString file = QFileDialog::getOpenFileName(this, tr("Choose a player model"),
                                                      paths::resolve(QStringLiteral("models/md2")),
                                                      tr("Quake II models (*.md2)"));
    if (file.isEmpty())
        return;
    PlayerGroup* g = playerOneRadio->isChecked() ? editorView->scene->player1 : editorView->scene->player2;
    if (g == nullptr)
        return;
    const QString rel = paths::relativize(file);
    // A new model: use the first skin found next to it
    QString skin;
    const QStringList skins = QFileInfo(file).dir().entryList({QStringLiteral("*.png")}, QDir::Files, QDir::Name);
    if (!skins.isEmpty())
        skin = paths::relativize(QFileInfo(file).dir().filePath(skins.first()));
    editorView->makeCurrent();
    g->player->setModel(skin, rel);
    editorView->doneCurrent();
    playerModel->setText(rel);
    playerSkin->setText(skin);
}

void MainWindow::choosePlayerSkin()
{
    PlayerGroup* g = playerOneRadio->isChecked() ? editorView->scene->player1 : editorView->scene->player2;
    if (g == nullptr)
        return;
    const QString model = g->player->modelFile.isEmpty() ? DEFAULT_PLAYER_MODEL : g->player->modelFile;
    const QString file = chooseImage(this, tr("Choose a player skin"), QFileInfo(model).path());
    if (file.isEmpty())
        return;
    editorView->makeCurrent();
    g->player->setModel(file);
    editorView->doneCurrent();
    playerSkin->setText(file);
}

void MainWindow::viewChanged()
{
    // Show the current settings of the selected view
    const Point3i e = editorView->scene->views[selectedView()].eye;
    viewX->setValue(e[0]);
    viewY->setValue(e[1]);
    viewZ->setValue(e[2]);
}

void MainWindow::saveView()
{
    editorView->setView(selectedView(), viewX->value(), viewY->value(), viewZ->value());
    statusBar()->showMessage(tr("View F%1 saved").arg(selectedView() + 3), 3000);
}

void MainWindow::snapshot()
{
    const QImage image = editorView->snapshot();
    QString dir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    if (dir.isEmpty())
        dir = QDir::homePath();
    QString name;
    do {
        name = QDir(dir).filePath(QStringLiteral("3dPangSnapshot_%1.png").arg(snapshotCount++));
    } while (QFileInfo::exists(name));
    if (image.save(name))
        QMessageBox::information(this, tr("Snapshot"), tr("Scene saved as\n%1").arg(name));
    else
        QMessageBox::warning(this, tr("Snapshot"), tr("Cannot save\n%1").arg(name));
}

void MainWindow::chooseSkybox()
{
    const QString file = QFileDialog::getOpenFileName(
        this, tr("Choose a face of the sky box"), paths::resolve(QStringLiteral("skyboxes")),
        tr("Sky box faces (*_posx.* *_negx.* *_posy.* *_negy.* *_posz.* *_negz.*)"));
    if (file.isEmpty())
        return;
    // "mount_posx.jpg" -> "mount.jpg"
    const QFileInfo info(file);
    const QString base = info.completeBaseName();
    const int underscore = base.lastIndexOf(QLatin1Char('_'));
    if (underscore < 0)
        return;
    const QString baseName = info.dir().filePath(base.left(underscore) + QLatin1Char('.') + info.suffix());
    editorView->setSkybox(paths::relativize(baseName));
}

void MainWindow::saveProbabilities()
{
    // Each frequency becomes a weight; weights are normalised to 100 within
    // bonuses (weapons, fruit, lives) and within maluses (bombs, tornado)
    int weapons = 3 + 3 * weaponsCombo->currentIndex();
    int fruit = 3 + 3 * fruitCombo->currentIndex();
    int lives = 1 + livesCombo->currentIndex();
    int bombs = 3 + 3 * bombsCombo->currentIndex();
    int tornado = 3 + 3 * tornadoCombo->currentIndex();
    const int bonusScale = 100 / (weapons + fruit + lives);
    const int malusScale = 100 / (bombs + tornado);
    weapons *= bonusScale;
    fruit *= bonusScale;
    lives *= bonusScale;
    bombs *= malusScale;
    tornado *= malusScale;
    editorView->setProbabilities(powerUpSlider->value(), bonusSlider->value(), weapons, fruit, lives, tornado, bombs);
    statusBar()->showMessage(tr("Power-up probabilities saved"), 3000);
}

void MainWindow::helpGuide()
{
    QDesktopServices::openUrl(QUrl(QString::fromLatin1(kGuideUrl)));
}

void MainWindow::helpAbout()
{
    AboutDialog dialog(this);
    dialog.exec();
}

} // namespace pang
