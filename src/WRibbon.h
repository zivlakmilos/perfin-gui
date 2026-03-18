#ifndef _WRIBBON_H_
#define _WRIBBON_H_

#include <QDockWidget>

class WRibbon : public QDockWidget {
  Q_OBJECT

public:
  WRibbon(QWidget *parent = nullptr);
  virtual ~WRibbon(void);

private:
};

#endif // _WRIBBON_H_
