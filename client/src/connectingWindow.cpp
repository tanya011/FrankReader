#include "include/connectingWindow.h"
#include <QMenuBar>
#include "include/collectionWindow.h"
#include "include/dictionaryWindow.h"
#include "include/learnWindow.h"
#include "include/readNowWindow.h"
#include "include/libraryWindow.h"

ConnectingWindow::ConnectingWindow(QWidget *parent)
    : allWindows(new QStackedWidget), QMainWindow(parent) {
    addMenu();
}
void ConnectingWindow::setUser(User *u) {
    user = u;
}

void ConnectingWindow::addMenu() {
    // инициализация кнопок меню
    collectionAction_ = new QAction("Коллекция", this);
    readNowAction_ = new QAction("Читаю сейчас", this);
    dictionaryAction_ = new QAction("Словарь", this);
    authorizationAction_ = new QAction("Войти", this);
    cardsAction_ = new QAction("Карточки", this);
    libraryAction_ = new QAction("Библиотека", this);

    menuBar()->addAction(libraryAction_);
    menuBar()->addAction(collectionAction_);
    menuBar()->addAction(readNowAction_);
    menuBar()->addAction(dictionaryAction_);
    menuBar()->addAction(cardsAction_);
    menuBar()->addAction(authorizationAction_);

    setCentralWidget(&allWindows);

    QObject::connect(readNowAction_, &QAction::triggered, this, [=]() {
        if (!user->isAuthorized())
            return;
        if (this->windowTitle() == "") {
            this->setWindowTitle("Книга не выбрана");
        } else {
            this->setWindowTitle(dynamic_cast<ReadNowWindow *>(
                                     allWindows.widget(windowIndexes.readNow))
                                     ->getAuthorTitle());
        }
        showReadNow();
    });

    QObject::connect(collectionAction_, &QAction::triggered, this, [=]() {
        if (!user->isAuthorized())
            return;
        this->setWindowTitle("Коллекция");
        showCollection();
    });

    QObject::connect(libraryAction_, &QAction::triggered, this, [=]() {
        if (!user->isAuthorized())
            return;
        this->setWindowTitle("Библиотека");
        updateLibrary();
        showLibrary();
    });

    QObject::connect(dictionaryAction_, &QAction::triggered, this, [=]() {
        if (!user->isAuthorized())
            return;
        this->setWindowTitle("Словарь");
        updateDictionary();  // not implemented yet
        showDictionary();
        dynamic_cast<DictionaryWindow *>(
            allWindows.widget(windowIndexes.dictionary))
            ->showWordSet(1);
    });

    QObject::connect(authorizationAction_, &QAction::triggered, this, [=]() {
        this->setWindowTitle("Авторизация");
        showAuth();
    });

    QObject::connect(cardsAction_, &QAction::triggered, this, [=]() {
        if (!user->isAuthorized())
            return;
        this->setWindowTitle("Карточки");
        updateCards();
        showCards();
    });
}

void ConnectingWindow::showLibrary() {
    allWindows.setCurrentIndex(windowIndexes.library);
}

void ConnectingWindow::showCollection() {
    allWindows.setCurrentIndex(windowIndexes.collection);
}
void ConnectingWindow::showReadNow() {
    allWindows.setCurrentIndex(windowIndexes.readNow);
}

void ConnectingWindow::showDictionary() {
    allWindows.setCurrentIndex(windowIndexes.dictionary);
}

void ConnectingWindow::showCards() {
    allWindows.setCurrentIndex(windowIndexes.learn);
}

void ConnectingWindow::showAuth() {
    allWindows.setCurrentIndex(windowIndexes.auth);
}

std::vector<WordSet> ConnectingWindow::getWordSets() {
    return dynamic_cast<DictionaryWindow *>(
               allWindows.widget(windowIndexes.dictionary))
        ->getWordSets();
}

void ConnectingWindow::updateLibrary() {
    dynamic_cast<LibraryWindow *>(
            allWindows.widget(windowIndexes.library))->updateWindow();
}

void ConnectingWindow::showServerProblems() {
    allWindows.setCurrentIndex(windowIndexes.serverProblems);
}


void ConnectingWindow::updateReadNow(const std::string &title,
                                     const std::string &author,
                                     const std::string &text) {
    auto readNow =
        dynamic_cast<ReadNowWindow *>(allWindows.widget(windowIndexes.readNow));

    readNow->printBook(QString::fromStdString(text),
                       QString::fromStdString(author),
                       QString::fromStdString(title));
    if (readNow->emptyTranslatedTextDisplay()) {
        readNow->cleanTranslatedTextDisplay();
    }
    readNow->updateToolBar();
}

void ConnectingWindow::updateDictionary() {
    auto dictionary = dynamic_cast<DictionaryWindow *>(
            allWindows.widget(windowIndexes.dictionary));
    dictionary->removeIcons();
    dictionary->makeIcons();
}

void ConnectingWindow::updateCards() {
    dynamic_cast<LearnWindow *>(allWindows.widget(windowIndexes.learn))
        ->setWordSets(getWordSets());
}
void ConnectingWindow::updateCollection() {
    auto collection = dynamic_cast<CollectionWindow *>(
        allWindows.widget(windowIndexes.collection));
    collection->updateWindow();
}