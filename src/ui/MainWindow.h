#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <QMainWindow>
#include <QMap>
#include <QString>

class DWRibbon;
class QTabWidget;
class QLabel;

class WBase;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  virtual ~MainWindow(void);

private:
  struct Tab
  {
    int index;
    WBase *widget;
  };

  void setupUi(void);
  void setupHandlers(void);

  DWRibbon *m_dwRibbon;
  QTabWidget *m_twCentralWidget;
  QLabel *m_lblStatus;

  QMap<QString, Tab> m_tabs;
};

#endif // _MAIN_WINDOW_H_
