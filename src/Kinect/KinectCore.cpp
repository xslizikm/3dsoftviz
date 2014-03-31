#include "Kinect/KinectCore.h"
#include "QDebug"

#include "Kinect/KinectThread.h"
#include "Kinect/KinectWindow.h"

using namespace Kinect;
Q_DECLARE_METATYPE(cv::Mat)



Kinect::KinectCore * Kinect::KinectCore::mKinectCore;

Kinect::KinectCore::KinectCore( QApplication* app,QWidget *parent)
{
	mKinectCore = this;
	mParent=parent;
	this->app=app;

	mKinectDialog=NULL;

	mThrsCreated=false;
	mThrKinect=NULL;

	qRegisterMetaType<cv::Mat>("Mat");
}

Kinect::KinectCore::~KinectCore()
{
	if(mThrsCreated)
	{
		mThrKinect->setCancel(true);
		mThrKinect->setImageSend(false);

		mKinectDialog->disconnect();
		mKinectDialog->deleteLater();

		mThrKinect->wait();
		mThrKinect->deleteLater();
	}
}

void Kinect::KinectCore::kinectRecognition()
{

	if(!mKinectDialog)
	{
		if(!mThrsCreated)
		{
			qDebug() << "Kinect Thread";
			mThrsCreated=true;
			mThrKinect = new Kinect::KinectThread();
		}
		qDebug()<< "create Kinect Window";
		mKinectDialog= new Kinect::KinectWindow(mParent,app,mThrKinect);
	}
	mKinectDialog->show();
	createConnectionKinect();
}


Kinect::KinectCore * Kinect::KinectCore::getInstance( QApplication* app,QWidget *parent)
{
	// if Kinect exists
	if(mKinectCore == NULL)
	{
		mKinectCore = new Kinect::KinectCore(app,parent);
	}
	return mKinectCore;
}

void Kinect::KinectCore::createConnectionKinect()
{

	QObject::connect(mKinectDialog,
					 SIGNAL(sendImageKinect(bool)),
					 mThrKinect,
					 SLOT(setImageSend(bool)));
	QObject::connect(mThrKinect,
					 SIGNAL(pushImage(cv::Mat)),
					 mKinectDialog,
					 SLOT(setLabel(cv::Mat)));

	QObject::connect(mKinectDialog,
					 SIGNAL(startKinect()),
					 mThrKinect,
					 SLOT(start()));

	QObject::connect(mKinectDialog,
					 SIGNAL(stopKinect(bool)),
					 mThrKinect,
					 SLOT(setCancel(bool)));


}
