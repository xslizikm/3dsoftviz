﻿#pragma once

#include <QApplication>

#include "Mouse3d/LibMouse3d/Mouse3d.h"

namespace App {

class ApplicationX11 : public QApplication {
	Q_OBJECT

public:
   ApplicationX11( int &argc, char **argv );

  ~ApplicationX11();

   bool x11EventFilter( XEvent *event );

   void sendX11Event( XEvent *event );

   void initConnection( LibMouse3d::Mouse3dDevice *mouse3d );

   void closeConnection();

signals:
   void passX11Event( XEvent *event );
   void passDummy();

private:
   bool isConnected;

};
} //App
