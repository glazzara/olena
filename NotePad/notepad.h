#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>

namespace Ui
{
    class NotePad;
}

class NotePad : public QMainWindow
{
    Q_OBJECT
    public:
        NotePad(QWidget *parent = 0);
        ~NotePad();

    protected:
        void changeEvent(QEvent *e);

    private:
        Ui::NotePad *ui;
        void createDockWindows();
};

#endif // NOTEPAD_H
