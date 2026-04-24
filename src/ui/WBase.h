#ifndef _WBASE_H_
#define _WBASE_H_

#include <QWidget>

class WBase : public QWidget
{
  Q_OBJECT;

public:
  WBase(QWidget *parent = nullptr);
  virtual ~WBase(void);
};

#endif // _WBASE_H_
