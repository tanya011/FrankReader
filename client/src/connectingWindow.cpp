#include "include/connectingWindow.h"
#include <QMenuBar>
#include "include/dictionaryWindow.h"
#include "include/learnWindow.h"
#include "include/readNowWindow.h"
#include "include/collectionWindow.h"

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
    // TODO: checking for new books in database, now book are loaded only on
    // creation
}

void ConnectingWindow::updateReadNow(const std::string &title,
                                     const std::string &author,
                                     const std::string &text) {
    auto readNow =
            dynamic_cast<ReadNowWindow *>(allWindows.widget(windowIndexes.readNow));



    readNow->printBook(QString::fromStdString(text),
                       QString::fromStdString(author),
                       QString::fromStdString(title));
    if (readNow->translatedTextDisplay_ != nullptr) {
        readNow->translatedTextDisplay_->clear();
    }

    removeToolBar(readNow->toolBar_);
    readNow->toolBar_->clear();
    readNow->createActions();
    readNow->createToolBars();

}

void ConnectingWindow::updateDictionary() {
    // TODO: same as in updateLibrary
}

void ConnectingWindow::updateCards() {
    dynamic_cast<LearnWindow *>(allWindows.widget(windowIndexes.learn))
            ->setWordSets(getWordSets());
}
void ConnectingWindow::updateCollection() {

    auto Collection =
            dynamic_cast<CollectionWindow *>(allWindows.widget(windowIndexes.collection));

    for (int i = 0; i < Collection->books_.size(); i++) {
        QLayoutItem *item;
        while ((item = Collection->layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item->layout();
            delete item;
        }
    }
    Collection->titleLabels_.clear();
    Collection->readBtns_.clear();

    //Collection->books_ = Collection->parent_->user->getLibraryBooks();
    Collection->books_ = Collection->parent_->user->getCollectionBooks();

    Collection->titleLabels_ = std::vector<QLabel *>(Collection->books_.size());
    Collection->readBtns_ = std::vector<QPushButton *>(Collection->books_.size());

    for (int i = 0; i < Collection->books_.size(); i++) {
        Collection->titleLabels_[i] = new QLabel(
                QString("Name: %1,   Author: %2")
                        .arg(Collection->books_[i].getName().c_str(), Collection->books_[i].getAuthor().c_str()));
        Collection->layout->addWidget(Collection->titleLabels_[i], i, 0);

        Collection->readBtns_[i] = new QPushButton(tr("Read"));
        Collection->layout->addWidget(Collection->readBtns_[i], i, 1);

        Collection->readBtns_[i]->setFixedWidth(100);
        Collection->readBtns_[i]->setFixedHeight(50);
    }

    Collection->scrollArea->setWidget(Collection->box);

    for (int i = 0; i < Collection->books_.size(); i++) {
        QObject::connect(Collection->readBtns_[i], &QPushButton::clicked, this,
                         [=]() { Collection->connectWithReader(Collection->books_[i].getId()); });
    }
}