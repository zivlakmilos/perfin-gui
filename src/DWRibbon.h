#ifndef _WRIBBON_H_
#define _WRIBBON_H_

#include <QDockWidget>

namespace Ui
{
class DWRibbon;
}

class DWRibbon : public QDockWidget
{
  Q_OBJECT

public:
  DWRibbon(QWidget *parent = nullptr);
  virtual ~DWRibbon(void);

private:
  Ui::DWRibbon *m_ui;
};

#endif // _WRIBBON_H_
