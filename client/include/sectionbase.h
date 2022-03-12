#ifndef SECTIONBASE_H
#define SECTIONBASE_H

#include <QMainWindow>
#include "library_window.h"
#include "readnow.h"
#include "db_manager.h"
#include "dictionary.h"

namespace Ui {
class SectionBase;
}

class SectionBase : public QMainWindow {
    Q_OBJECT

private:
    LibraryWindow libraryWindow;
    ReadNow readNow;
    Dictionary dictionary;

    DBManager& dbManager;

public:

    SectionBase(DBManager& m, QWidget *parent = nullptr);
    ~SectionBase();

    //кнопки на меню
    //   QAction* collection;
    QAction *library;
    QAction *reading_now;
    QAction* dictionary_action;
   /*  QAction* cards;
    QAction* settings;
    QAction* entrance_exit;
*/
private:
     Ui::SectionBase *ui;  // TODO: better scoped_ptr
};

#endif  // SECTIONBASE_H