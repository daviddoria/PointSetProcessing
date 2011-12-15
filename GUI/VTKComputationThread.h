#ifndef VTKComputationThread_H
#define VTKComputationThread_H

#include <QObject>

#include <vtkObject.h>

class FilterComputationThreadNonTemplated : public QObject
{
Q_OBJECT
public slots:
  virtual void start() = 0;

signals:
  void finished();
  void progressUpdate(int);
};

template <typename TFilter>
class VTKComputationThread : public FilterComputationThreadNonTemplated
{
public:

  VTKComputationThread();

  void IterateCallbackFunction(vtkObject* caller, long unsigned int eventId, void* callData);

  // This function is called when the thread is started
  void start();

  void SetFilter(TFilter* filter);

private:
  TFilter* Filter;

};

#include "VTKComputationThread.hxx"

#endif
