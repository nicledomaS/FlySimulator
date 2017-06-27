#ifndef STATISTICS_H
#define STATISTICS_H

#include <QDialog>

namespace Ui {
class Statistics;
}

class Statistics : public QDialog
{
    Q_OBJECT

public:
    explicit Statistics(QWidget *parent = 0);
    ~Statistics();
    void setText(const QString & text);

private:
    Ui::Statistics *ui;
};

#endif // STATISTICS_H
