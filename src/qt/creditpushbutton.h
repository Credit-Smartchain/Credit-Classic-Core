#ifndef CREDITPUSHBUTTON_H
#define CREDITPUSHBUTTON_H
#include <QPushButton>
#include <QStyleOptionButton>
#include <QIcon>

class CreditPushButton : public QPushButton
{
public:
    explicit CreditPushButton(QWidget * parent = Q_NULLPTR);
    explicit CreditPushButton(const QString &text, QWidget *parent = Q_NULLPTR);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    void updateIcon(QStyleOptionButton &pushbutton);

private:
    bool m_iconCached;
    QIcon m_downIcon;
};

#endif // CREDITPUSHBUTTON_H
