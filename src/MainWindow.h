#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <QMainWindow>

class DWRibbon;
class QTabWidget;
class QLabel;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  virtual ~MainWindow(void);

private:
  void setupUi(void);

  DWRibbon *m_dwRibbon;
  QTabWidget *m_twCentralWidget;
  QLabel *m_lblStatus;
};

#endif // _MAIN_WINDOW_H_
