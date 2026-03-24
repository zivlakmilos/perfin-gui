#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <QMainWindow>

class DWRibbon;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  virtual ~MainWindow(void);

private:
  void setupUi(void);

  DWRibbon *m_dwRibbon;
};

#endif // _MAIN_WINDOW_H_
