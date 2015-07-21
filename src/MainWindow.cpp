#include "MainWindow.h"
#include "GLWindow.h"

//----------------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow(
                       QWidget *_parent
                      ):
                        QMainWindow(_parent)
{
  // set the window size
  this->resize(QSize(1024,720));
  // create our GLWindow
  m_gl = new GLWindow(this);
  // set the tite bar
  this->setWindowTitle(QString("Ribbons"));
}

//----------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
}


//----------------------------------------------------------------------------------------------------------------------
void MainWindow::keyPressEvent(
                               QKeyEvent *_event
                              )
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : QApplication::exit(EXIT_SUCCESS); break;
  // turn on wireframe rendering
  case Qt::Key_W : m_gl->toggleWireframe(); break;
  // show full screen
  case Qt::Key_F10 : showFullScreen(); break;
  // show windowed
  case Qt::Key_F11 : showNormal(); break;

  case Qt::Key_R : m_gl->reset(); break;

  //case Qt::Key_Space : m_gl->; ; break;

  default : break;
  }
  // finally update the GLWindow and re-draw
  //m_gl->updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::resizeEvent ( QResizeEvent * _event )
{
  m_gl->resize(_event->size());
}

